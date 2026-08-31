# Press R to Remove

A Windhawk mod that allows removing Start Menu and Jump List items using a customizable keyboard shortcut.

🌐 Language / 語言

- **English**
- [EADME_zh-TW.md

---

## Demo

<p align="center">
  <img src="https://raw.githubusercontent.com/jaxon0523/press-r-to-remove/main/screenshots/demo.gif" alt="Demo">
</p>

<p align="center">
  <em>Press R to quickly remove items from Start Menu recommendations and Taskbar Jump Lists.</em>
</p>

---

## Features

- Press **R** to invoke **Remove from list** in the Start Menu.
- Press **R** to invoke **Remove from this list** in Taskbar Jump Lists.
- Customizable hotkey through Windhawk settings.
- Uses Windows UI Automation for reliable menu invocation.
- No global keyboard hooks.
- Lightweight and responsive.

---

## Start Menu Support

Press **R** to invoke **Remove from list** from the Windows Start Menu Recommended section.

<p align="center">
  <img src="https://raw.githubusercontent.com/jaxon0523/press-r-to-remove/main/screenshots/startmenu-recommended-remove-from-list.png>
</p>

---

## Jump List Support

Press **R** to invoke **Remove from this list** from Taskbar Jump Lists.

<p align="center">
  <img src="https://raw.githubusercontent.com/jaxon0523/press-r-to-remove/main/screenshots/jumplist-remove-from-this-list.png" alt="Jump List Support">
</p>

---

## Windhawk settings.

<p align="center">
  <img src="https://raw.githubusercontent.com/jaxon0523/press-r-to-remove/main/screenshots/settings.png">
</p>

---

## Installation

### Install from Windhawk

1. Install Windhawk.
2. Install Press R to Remove.
3. Enable the mod.
4. Open a supported context menu.
5. Press the configured hotkey.

---

## Requirements

- Windows 11
- Windhawk

---

## Tested On

- Windows 11 24H2
- Windows 11 25H2

---

## Supported Menu Items

### English

- Remove from list
- Remove from this list

### Traditional Chinese

- 從清單移除
- 從此清單移除

---

## How It Works

The mod detects supported context menus and uses Microsoft UI Automation (UIA) to locate and invoke the corresponding command when the configured hotkey is pressed.

Only visible and supported popup menus are processed, minimizing impact on system performance.

---

## Configuration

Default hotkey:

```text
R
```

Can be changed in Windhawk settings.

Examples:

```text
R
X
Delete
```

---

## Known Limitations

- Only supported menu items can be invoked.
- Menu text may vary across Windows versions and languages.
- Future Windows updates may require compatibility updates.

---

## Changelog

### v1.0.0

- Initial public release
- Start Menu support
- Jump List support
- Customizable hotkey support
- UI Automation based invocation

---

## License

MIT License

Copyright (c) 2026 jaxon0523

See the LICENSE file for details.

---

## Acknowledgements

Built with:

- Windhawk
- Microsoft UI Automation API
- Windows SDK

Special thanks to the Windhawk community.