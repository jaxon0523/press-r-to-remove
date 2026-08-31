// ==WindhawkMod==
// @id              press-r-to-remove
// @name            Press R to Remove
// @description     Quickly remove Start menu recommendations and Jump List items using a configurable hotkey
// @version         1.5
// @author          Jaxon
// @include         StartMenuExperienceHost.exe
// ==/WindhawkMod==

// Uncomment for debugging
// #define DEBUG_LOG

#ifdef DEBUG_LOG
#define LOG(...) Wh_Log(__VA_ARGS__)
#else
#define LOG(...)
#endif

// ==WindhawkModReadme==
/*
# Press R to Remove

Quickly remove Start menu recommendations and Jump List entries using a configurable hotkey.

## Supported commands

### Start Menu

- 從清單移除
- Remove from list

### Jump Lists

- 從此清單移除
- Remove from this list

## Hotkey

Default: R

You can change the hotkey in Settings.

Supported options:

- R
- Delete
- Backspace
- F8
- F9
- F10

### Compatibility

Tested on Windows 11 24H2.

Future Windows versions may require updates if Microsoft changes the Start menu or Jump List implementation.
*/
// ==/WindhawkModReadme==

// ==WindhawkModSettings==
/*
- Hotkey: R
  $name: Hotkey
  $description: Key used to invoke remove commands.

  $options:
  - R: R
  - DELETE: Delete
  - BACKSPACE: Backspace
  - F8: F8
  - F9: F9
  - F10: F10
*/
// ==/WindhawkModSettings==

#include <windows.h>
#include <objbase.h>
#include <uiautomation.h>
#include <oleauto.h>
#include <atomic>

typedef HRESULT (WINAPI* CoInitializeEx_t)(
    LPVOID,
    DWORD);

typedef void (WINAPI* CoUninitialize_t)();

typedef HRESULT (WINAPI* CoCreateInstance_t)(
    REFCLSID,
    LPUNKNOWN,
    DWORD,
    REFIID,
    LPVOID*);

typedef BSTR (WINAPI* SysAllocString_t)(
    const OLECHAR*);

typedef void (WINAPI* SysFreeString_t)(
    BSTR);

static CoInitializeEx_t g_CoInitializeEx = nullptr;
static CoCreateInstance_t g_CoCreateInstance = nullptr;
static CoUninitialize_t g_CoUninitialize = nullptr;

static SysAllocString_t g_SysAllocString = nullptr;
static SysFreeString_t g_SysFreeString = nullptr;

static const GUID MY_CLSID_CUIAutomation =
{
    0xff48dba4, 0x60ef, 0x4201,
    {0xaa,0x87,0x54,0x10,0x3e,0xef,0x59,0x4e}
};

static const GUID MY_IID_IUIAutomation =
{
    0x30cbe57d, 0xd9d0, 0x452a,
    {0xab,0x13,0x7a,0xc5,0xac,0x48,0x25,0xee}
};

static HANDLE g_thread = nullptr;
static std::atomic_bool g_running = true;

static std::atomic<ULONGLONG> g_lastInvokeTick{0};
static int g_hotkeyVk = 'R';

static HMODULE g_hOle32 = nullptr;
static HMODULE g_hOleAut32 = nullptr;

void LoadSettings()
{
    PCWSTR value =
        Wh_GetStringSetting(L"Hotkey");

    if (!value)
    {
        g_hotkeyVk = 'R';
        return;
    }

    if (wcscmp(value, L"DELETE") == 0)
        g_hotkeyVk = VK_DELETE;
    else if (wcscmp(value, L"BACKSPACE") == 0)
        g_hotkeyVk = VK_BACK;
    else if (wcscmp(value, L"F8") == 0)
        g_hotkeyVk = VK_F8;
    else if (wcscmp(value, L"F9") == 0)
        g_hotkeyVk = VK_F9;
    else if (wcscmp(value, L"F10") == 0)
        g_hotkeyVk = VK_F10;
    else
        g_hotkeyVk = 'R';

    Wh_FreeStringSetting(value);
}

HWND FindVisiblePopup()
{
    HWND hwnd = nullptr;

    while ((hwnd = FindWindowExW(
        nullptr,
        hwnd,
        L"Xaml_WindowedPopupClass",
        nullptr)) != nullptr)
    {
        if (IsWindowVisible(hwnd))
        {
            return hwnd;
        }
    }

    return nullptr;
}

bool InvokeMenuItemInPopup(
    IUIAutomation* automation,
    HWND popupHwnd,
    const wchar_t* menuName)
{
    LOG(
        L"Searching menu: %s",
        menuName);

    if (!automation)
    {
        return false;
    }

    IUIAutomationElement* popupElement = nullptr;

    HRESULT hr =
        automation->ElementFromHandle(
            popupHwnd,
            &popupElement);

    if (FAILED(hr))
    {
        LOG(
            L"ElementFromHandle failed: 0x%08X",
            hr);

        return false;
    }

    if (!popupElement)
    {
        LOG(
            L"ElementFromHandle returned nullptr");

        return false;
    }

    LOG(
        L"ElementFromHandle success");

    VARIANT v{};
    v.vt = VT_BSTR;
    v.bstrVal = g_SysAllocString(menuName);
    if (!v.bstrVal)
    {
        popupElement->Release();

        return false;
    }

    IUIAutomationCondition* condition =
        nullptr;

    hr =
        automation->CreatePropertyCondition(
            UIA_NamePropertyId,
            v,
            &condition);

    if (FAILED(hr))
    {
        LOG(
            L"CreatePropertyCondition failed: 0x%08X",
            hr);

        g_SysFreeString(v.bstrVal);
        popupElement->Release();

        return false;
    }

    bool success = false;

    if (SUCCEEDED(hr) && condition)
    {
        IUIAutomationElement* found =
            nullptr;

        hr =
            popupElement->FindFirst(
                TreeScope_Subtree,
                condition,
                &found);
        if (FAILED(hr))
        {
            LOG(
                L"FindFirst failed: 0x%08X",
                hr);
        }
        else if (!found)
        {
            LOG(
                L"Menu item not found: %s",
                menuName);
        }

        if (SUCCEEDED(hr) && found)
        {
            LOG(
                L"Menu item found: %s",
                menuName);

            IUIAutomationInvokePattern* invoke =
                nullptr;

            hr =
                found->GetCurrentPatternAs(
                    UIA_InvokePatternId,
                    IID_PPV_ARGS(&invoke));

            if (FAILED(hr))
            {
                LOG(
                    L"GetCurrentPatternAs failed: 0x%08X",
                    hr);
            }
            else if (!invoke)
            {
                LOG(
                    L"GetCurrentPatternAs returned nullptr");
            }

            if (SUCCEEDED(hr) && invoke)
            {
                hr = invoke->Invoke();

                if (SUCCEEDED(hr))
                {
                    LOG(
                        L"Invoked: %s",
                        menuName);

                    success = true;
                }
                else
                {
                    LOG(
                        L"Invoke failed: 0x%08X",
                        hr);
                }

                invoke->Release();
            }

            found->Release();
        }

        condition->Release();
    }

    g_SysFreeString(v.bstrVal);

    popupElement->Release();

    return success;
}

DWORD WINAPI ThreadProc(LPVOID)
{
    if (!g_CoInitializeEx ||
        !g_CoCreateInstance ||
        !g_CoUninitialize ||
        !g_SysAllocString ||
        !g_SysFreeString)
    {
        LOG(
            L"COM functions not initialized");

        return 0;
    }

    LOG(
        L"Thread started: %u",
        GetCurrentThreadId());

    HRESULT hr =
        g_CoInitializeEx(
            nullptr,
            COINIT_APARTMENTTHREADED);

    if (FAILED(hr))
    {
        LOG(
            L"Thread CoInitializeEx failed: 0x%08X",
            hr);

        return 0;
    }

    IUIAutomation* automation = nullptr;

    hr =
        g_CoCreateInstance(
            MY_CLSID_CUIAutomation,
            nullptr,
            CLSCTX_INPROC_SERVER,
            MY_IID_IUIAutomation,
            (void**)&automation);

    if (FAILED(hr) || !automation)
    {
        LOG(
            L"Thread CoCreateInstance failed: 0x%08X",
            hr);

        g_CoUninitialize();
        return 0;
    }

    LOG(
        L"UI Automation initialized");

    static const wchar_t* targets[] =
    {
        L"從清單移除",
        L"Remove from list",

        L"從此清單移除",
        L"Remove from this list"
    };

    while (g_running.load())
    {
        if (GetAsyncKeyState(g_hotkeyVk) & 1)
        {
            ULONGLONG now =
                GetTickCount64();

            ULONGLONG lastTick =
                g_lastInvokeTick.load();

            if (now - lastTick < 300)
            {
                Sleep(20);
                continue;
            }

            HWND popup =
                FindVisiblePopup();

            if (!popup)
            {
                Sleep(20);
                continue;
            }

            bool handled = false;

            for (size_t i = 0;
                 i < ARRAYSIZE(targets);
                 i++)
            {
                if (InvokeMenuItemInPopup(
                        automation,
                        popup,
                        targets[i]))
                {
                    g_lastInvokeTick.store(now);

                    handled = true;
                    break;
                }
            }

            if (!handled)
            {
                LOG(
                    L"No supported remove command found");
            }
        }

        Sleep(20);
    }

    if (automation)
    {
        LOG(
            L"UI Automation released");

        automation->Release();
    }

    g_CoUninitialize();

    LOG(
        L"Thread exited");

    return 0;
}

void Wh_ModSettingsChanged()
{
    LoadSettings();

    LOG(
        L"Settings changed");
}

BOOL Wh_ModInit()
{
    g_running.store(true);

    LoadSettings();

    Wh_Log(
        L"Loaded");

    g_hOle32 =
    LoadLibraryW(
        L"ole32.dll");

    g_hOleAut32 =
    LoadLibraryW(
        L"oleaut32.dll");

    if (!g_hOle32 || !g_hOleAut32)
    {
        if (g_hOle32)
        {
            FreeLibrary(g_hOle32);
            g_hOle32 = nullptr;
        }

        if (g_hOleAut32)
        {
            FreeLibrary(g_hOleAut32);
            g_hOleAut32 = nullptr;
        }

    Wh_Log(
        L"Failed to load OLE libraries");

    return FALSE;
    }

    g_CoInitializeEx =
        (CoInitializeEx_t)GetProcAddress(
            g_hOle32,
            "CoInitializeEx");

    g_CoCreateInstance =
        (CoCreateInstance_t)GetProcAddress(
            g_hOle32,
            "CoCreateInstance");

    g_CoUninitialize =
        (CoUninitialize_t)GetProcAddress(
            g_hOle32,
            "CoUninitialize");

    g_SysAllocString =
        (SysAllocString_t)GetProcAddress(
            g_hOleAut32,
            "SysAllocString");

    g_SysFreeString =
        (SysFreeString_t)GetProcAddress(
            g_hOleAut32,
            "SysFreeString");

    if (!g_CoInitializeEx ||
        !g_CoCreateInstance ||
        !g_CoUninitialize)
    {
    if (g_hOle32)
    {
        FreeLibrary(g_hOle32);
        g_hOle32 = nullptr;
    }

    if (g_hOleAut32)
    {
        FreeLibrary(g_hOleAut32);
        g_hOleAut32 = nullptr;
    }

    Wh_Log(
        L"Failed to resolve COM functions");

    return FALSE;
    }

    if (!g_SysAllocString ||
        !g_SysFreeString)
    {
    if (g_hOle32)
    {
        FreeLibrary(g_hOle32);
        g_hOle32 = nullptr;
    }

    if (g_hOleAut32)
    {
        FreeLibrary(g_hOleAut32);
        g_hOleAut32 = nullptr;
    }

    Wh_Log(
        L"Failed to resolve OLE Automation functions");

    return FALSE;
    }


    g_thread =
        CreateThread(
            nullptr,
            0,
            ThreadProc,
            nullptr,
            0,
            nullptr);

    if (!g_thread)
    {
        if (g_hOle32)
        {
            FreeLibrary(g_hOle32);
            g_hOle32 = nullptr;
        }

        if (g_hOleAut32)
        {
            FreeLibrary(g_hOleAut32);
            g_hOleAut32 = nullptr;
        }

    Wh_Log(
        L"CreateThread failed");

    return FALSE;
    }

    LOG(
        L"Worker thread created");

    return TRUE;
}

void Wh_ModUninit()
{
    g_running.store(false);

    if (g_thread)
    {
        DWORD waitResult =
            WaitForSingleObject(
                g_thread,
                1000);

        if (waitResult == WAIT_TIMEOUT)
        {
            Wh_Log(
                L"Thread shutdown timeout");
        }
        else if (waitResult == WAIT_FAILED)
        {
            Wh_Log(
                L"WaitForSingleObject failed");
        }

        CloseHandle(g_thread);
        g_thread = nullptr;
    }

    if (g_hOle32)
    {
        FreeLibrary(g_hOle32);
        g_hOle32 = nullptr;
    }

    if (g_hOleAut32)
    {
        FreeLibrary(g_hOleAut32);
        g_hOleAut32 = nullptr;
    }

    g_CoInitializeEx = nullptr;
    g_CoCreateInstance = nullptr;
    g_CoUninitialize = nullptr;

    g_SysAllocString = nullptr;
    g_SysFreeString = nullptr;

    Wh_Log(
        L"Unloaded");
}