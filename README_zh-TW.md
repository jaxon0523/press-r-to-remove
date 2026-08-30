# Press R to Remove

透過自訂快捷鍵快速移除 Windows 開始功能表與 Jump List 項目的 Windhawk 模組。

🌐 Language / 語言

- [English](README.md)
- **繁體中文**

---

## Demo

<p align="center">
  screenshots/demo.gif
</p>

<p align="center">
  <em>Press R to quickly remove items from Start Menu recommendations and Taskbar Jump Lists.</em>
</p>

---

## 功能特色

- 在 Windows 開始功能表中按下 **R** 快速執行 **「從清單移除」**。
- 在工作列 Jump List 中按下 **R** 快速執行 **「從此清單移除」**。
- 可於 Windhawk 設定中自訂快捷鍵。
- 使用 Windows UI Automation 技術執行選單命令。
- 不安裝全域鍵盤 Hook。
- 輕量、快速且低資源占用。

---

## 開始功能表支援

在 Windows 開始功能表的「建議項目」中開啟右鍵選單後，按下 **R** 即可執行 **「從清單移除」**。

<p align="center">
  <img src="screenshots/開始選單-從清單移除.png
</p>

---

## Jump List 支援

在工作列應用程式的 Jump List 最近項目中開啟右鍵選單後，按下 **R** 即可執行 **「從此清單移除」**。

<p align="center">
  screenshots/工具列-最近-從此清單移除.png
</p>

---

## 設定畫面

可透過 Windhawk 設定頁面自訂啟用快捷鍵。

<p align="center">
  screenshots/settings.png
</p>

---

## 安裝方式

### 從 Windhawk 安裝

1. 安裝 Windhawk。
2. 安裝 **Press R to Remove** 模組。
3. 啟用模組。
4. 開啟支援的右鍵選單。
5. 按下設定的快捷鍵即可執行。

---

## 系統需求

- Windows 11
- Windhawk

已測試環境：

- Windows 11 24H2

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

本模組會監控支援的內容選單，在使用者按下設定的快捷鍵後，透過 Windows UI Automation（UIA）搜尋對應的選單項目並自動執行。

為了降低對系統的影響，本模組僅搜尋目前可見且受支援的彈出式選單，不會掃描無關的應用程式視窗。

---

## 設定選項

預設快捷鍵：

```text
R
```

可於 Windhawk 設定頁面修改。

例如：

```text
R
X
Delete
```

---

## 已知限制

- 目前僅支援本文件列出的選單項目。
- 不同 Windows 版本或語言可能導致選單文字有所差異。
- 未來 Windows 更新可能需要調整相容性。

---

## 更新紀錄

### v1.0.0

- 首次公開發布
- 支援開始功能表「建議項目」
- 支援工作列 Jump List 最近項目
- 使用 UI Automation 執行選單命令
- 支援自訂快捷鍵

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