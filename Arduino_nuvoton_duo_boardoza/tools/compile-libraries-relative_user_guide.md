# compile-libraries-relative.ps1 使用指南

## 概覽

`compile-libraries-relative.ps1` 是一個 PowerShell 批次編譯腳本，用於自動編譯位於 `../libraries` 路徑下的所有 Arduino 草稿碼（`.ino`）。  
腳本不會更動 Arduino15 已安裝的平台目錄，因此不會發生 `Move-Item` IOException 的問題。

---

## 腳本位置

```
Arduino_nuvoton_duo\
  tools\
    compile-libraries-relative.ps1   ← 腳本
  libraries\
    LibraryA\
      examples\
        example1\
          example1.ino
    LibraryB\
      ...
```

腳本預設以自身所在目錄的上一層（`..`）為 package root，再往下找 `libraries\`。

---

## 前置需求

- **Windows PowerShell 5.1** 或 **PowerShell 7+**
- **Arduino IDE** 已安裝（腳本會自動偵測其內建的 `arduino-cli.exe`），或 `arduino-cli` 已加入 PATH
- 目標板卡的 BSP 已在 Arduino CLI 中安裝（預設 FQBN：`nuvoton:nuvoton_duo:nuvoton_m467sd`）

---

## 執行語法

```powershell
.\Arduino_nuvoton_duo\tools\compile-libraries-relative.ps1 [參數...]
```

> 若執行時出現「執行原則」錯誤，可在前面加上：
> ```powershell
> powershell -ExecutionPolicy Bypass -File .\Arduino_nuvoton_duo\tools\compile-libraries-relative.ps1
> ```

---

## 參數說明

| 參數 | 型別 | 預設值 | 說明 |
|------|------|--------|------|
| `-Fqbn` | String | `nuvoton:nuvoton_duo:nuvoton_m467sd` | 目標板卡的 Fully Qualified Board Name |
| `-LibrariesPath` | String | `（腳本位置）\..\libraries` | 指定要掃描的 libraries 根目錄（絕對或相對路徑） |
| `-Filter` | String | `（空，全部編譯）` | 只編譯相對路徑中包含此關鍵字的草稿碼（支援萬用字元 `*`） |
| `-LogFile` | String | `（空，不存檔）` | 將編譯摘要報告儲存到指定路徑的文字檔 |
| `-ShowCompileLog` | Switch | 不啟用 | 啟用後，每個草稿碼的完整 arduino-cli 輸出會印在終端機上 |

---

## 使用範例

### 1. 編譯所有 libraries 下的草稿碼（預設）

```powershell
.\Arduino_nuvoton_duo\tools\compile-libraries-relative.ps1
```

### 2. 只編譯名稱包含 `CAN` 的草稿碼

```powershell
.\Arduino_nuvoton_duo\tools\compile-libraries-relative.ps1 -Filter "CAN"
```

### 3. 儲存編譯摘要報告

```powershell
.\Arduino_nuvoton_duo\tools\compile-libraries-relative.ps1 -LogFile ".\compile_results.txt"
```

### 4. 顯示每個草稿碼的完整編譯 log

```powershell
.\Arduino_nuvoton_duo\tools\compile-libraries-relative.ps1 -ShowCompileLog
```

### 5. 組合使用：篩選 + 顯示 log + 存檔

```powershell
.\Arduino_nuvoton_duo\tools\compile-libraries-relative.ps1 -Filter "SPI" -ShowCompileLog -LogFile ".\spi_compile.txt"
```

### 6. 指定不同的 libraries 路徑

```powershell
.\Arduino_nuvoton_duo\tools\compile-libraries-relative.ps1 -LibrariesPath "C:\path\to\other\libraries"
```

### 7. 指定不同的板卡 FQBN

```powershell
.\Arduino_nuvoton_duo\tools\compile-libraries-relative.ps1 -Fqbn "arduino:avr:uno"
```

---

## 輸出說明

### 終端機輸出範例

```
========================================
Compile sketches under ../libraries
========================================
Script location: C:\...\tools
Libraries path: C:\...\libraries
FQBN: nuvoton:nuvoton_duo:nuvoton_m467sd
Found 42 .ino files; 42 sketches after filtering

[1/42] Compiling nvtCAN\examples\CAN_Loopback... ✓ Success
[2/42] Compiling nvtEther\examples\WebClient...   ✗ Failed
...

========================================
Compilation summary
========================================
Sketch                               File              Status   Size          Error
------                               ----              ------   ----          -----
nvtCAN\examples\CAN_Loopback         CAN_Loopback.ino  Success  48320 bytes
nvtEther\examples\WebClient          WebClient.ino     Failed   N/A           error: ...

Total: 42 sketches
Success: 40
Failed:  2
```

### -LogFile 存檔格式

若指定 `-LogFile`，會在指定路徑產生 UTF-8 文字檔，內容包含：

- 時間戳記
- 腳本及 libraries 路徑
- FQBN
- 成功 / 失敗統計
- 詳細結果表格（與終端機相同）

---

## 常見問題

### Q: 執行後出現「arduino-cli.exe not found」
**A:** 確認 Arduino IDE 已安裝於預設路徑（`%LOCALAPPDATA%\Programs\Arduino IDE`），或將 `arduino-cli` 加入 PATH 環境變數。

### Q: 出現「Libraries path not found」
**A:** 腳本找不到 `../libraries` 目錄。請確認腳本放在正確位置（`tools\` 資料夾），或使用 `-LibrariesPath` 明確指定路徑。

### Q: 出現「No .ino files found」
**A:** 指定的 libraries 目錄下沒有 `.ino` 檔案。請確認目錄結構正確。

### Q: 想看編譯失敗的完整錯誤訊息
**A:** 加上 `-ShowCompileLog` 參數，即可看到每個草稿碼的完整 `arduino-cli` 輸出。

---

## 與 compile-sketch.ps1 的差異

| 項目 | compile-sketch.ps1 | compile-libraries-relative.ps1 |
|------|-------------------|-------------------------------|
| 目的 | 編譯單一或指定草稿碼 | 批次編譯 libraries 下所有草稿碼 |
| 平台切換 | 使用 Move-Item 暫時切換 Arduino15 平台目錄 | 不操作平台目錄 |
| 穩定性 | 若目錄被鎖定會失敗（IOException） | 無此問題 |
| 適用情境 | 開發中需要測試 workspace BSP 版本 | 批次驗證 libraries 相容性 |

---

*本文件最後更新：2026-06-01*
