# Press R to Remove

透過自訂快捷鍵快速移除 Windows 開始功能表與 Jump List 項目的 Windhawk 模組。

🌐 Language / 語言

- [English](README.md)
- **繁體中文**

---

## 功能展示

<p align="center">
  <img src="https://raw.githubusercontent.com/jaxon0523/press-r-to-remove/main/screenshots/demo.gif" alt="Demo">
</p>

<p align="center">
  <em>按下 R，即可快速移除開始功能表建議項目與 Jump List 項目。</em>
</p>

---

## 功能特色

- 在開始功能表按下 **R** 快速執行「從清單移除」。
- 在 Jump List 按下 **R** 快速執行「從此清單移除」。
- 支援自訂快捷鍵。
- 使用 Windows UI Automation 技術。
- 不安裝全域鍵盤 Hook。
- 輕量且低資源占用。

---

## 開始功能表支援

<p align="center">
  <img src="https://raw.githubusercontent.com/jaxon0523/press-r-to-remove/main/screenshots/開始選單-從清單移除.png" alt="Start ">
  <img src="https://raw.githubusercontent.com/jaxon0523/press-r-to-remove/main/screenshots/工具列-最近-從此清單移除.png"lign="center">
  <img src="https://raw.githubusercontent.com/jaxon0523/press-r-to-remove/main/screenshots/settings.png" alt="Settings">

### 從 Windhawk 安裝

1. 安裝 Windhawk。
2. 安裝 Press R to Remove。
3. 啟用模組。
4. 開啟支援的右鍵選單。
5. 按下設定的快捷鍵。

---

## 系統需求

- Windows 11
- Windhawk

---

## 已測試環境

- Windows 11 24H2
- Windows 11 25H2

---

## 支援的選單項目

### 英文

- Remove from list
- Remove from this list

### 繁體中文

- 從清單移除
- 從此清單移除

---

## 運作原理

本模組會偵測支援的內容選單，並透過 Microsoft UI Automation（UIA）尋找對應命令並執行。

僅會處理目前可見且支援的彈出式選單，因此對系統效能影響極低。

---

## 設定選項

預設快捷鍵：

```text
R
```

可在 Windhawk 設定中修改。

例如：

```text
R
X
Delete
```

---

## 已知限制

- 目前僅支援列出的選單項目。
- 不同 Windows 版本與語言的選單文字可能有所差異。
- 未來 Windows 更新可能需要相容性調整。

---

## 更新紀錄

### v1.0.0

- 首次公開發布
- 支援開始功能表建議項目
- 支援 Jump List 項目
- 支援自訂快捷鍵
- 採用 UI Automation 執行命令

---

## 授權條款

MIT License

Copyright (c) 2026 jaxon0523

詳細內容請參閱 LICENSE 檔案。

---

## 致謝

本專案使用：

- Windhawk
- Microsoft UI Automation API
- Windows SDK

感謝 Windhawk 社群提供優秀的 Windows 客製化平台。
`