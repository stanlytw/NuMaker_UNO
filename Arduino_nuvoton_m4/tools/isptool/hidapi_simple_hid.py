import hid
import time

# --- 1. 定義裝置參數 ---
# 請將這裡替換成您的裝置實際的 VID/PID
DEVICE_VID = 0x0416  
DEVICE_PID = 0x3F00  

# 請設定您的「客製化裝置」介面號碼。
# 這是您在裝置配置描述元 (Configuration Descriptor) 中賦予的號碼。
# 範例使用 2，請替換成您客製化介面的實際號碼！
TARGET_INTERFACE_NUMBER = 2 
# ---

def open_custom_hid_interface(vid, pid, target_interface_num):
    """
    使用 hidapi 列舉所有介面，並精確開啟指定的客製化介面。
    此方法適用於 Windows 環境。
    """
    print(f"--- 1. 正在列舉 VID={hex(vid)} PID={hex(pid)} 的所有 HID 介面 ---")

    # 步驟 1: 列舉所有匹配的 HID 介面
    devices = hid.enumerate(vid, pid)

    if not devices:
        print("錯誤: 未找到任何匹配的 HID 介面。請檢查 VID/PID 或連線。")
        return None

    custom_device_info = None

    # 步驟 2: 遍歷清單，尋找目標介面
    for device_info in devices:
        
        interface_num = device_info.get('interface_number')
        print(f"  > 目前interface number: {interface_num}")
        # 由於 Windows 上 hidapi 在處理 interface_number 的型別可能有些許差異，
        # 這裡的判斷應盡量寬鬆，但直接比對 int 即可。
        # 如果介面是鍵盤/滑鼠，interface_number 可能為 -1 或 None，請根據您的裝置定義調整。
        
        # 顯示所有找到的介面資訊供用戶診斷
        print(f"  > 發現介面: Path={device_info['path']}, Interface Num={interface_num}, Product='{device_info.get('product_string')}'")

        # 關鍵篩選：尋找目標介面號碼
        if interface_num == target_interface_num:
            custom_device_info = device_info
            break

    if custom_device_info is None:
        print(f"\n錯誤: 未找到介面號碼 {target_interface_num} 的客製化介面。")
        print("請確認您的 TARGET_INTERFACE_NUMBER 設定是否正確。")
        return None

    # 步驟 3: 使用唯一路徑 (Path) 精確開啟目標介面
    device_path = custom_device_info['path']
    print(f"\n--- 2. 找到目標介面，嘗試開啟: {device_path} ---")
    
    try:
        device_handle = hid.device()
        # open_path() 讓 hidapi 直接與指定的介面通訊，不會受其他介面影響
        device_handle.open_path(device_path) 
        print("成功開啟裝置句柄 (Handle)。")
        return device_handle
        
    except Exception as e:
        # 在 Windows 上，即使是標準 HID 介面，hidapi 通常也能開啟，
        # 但如果開啟失敗，可能是因為權限問題或裝置問題
        print(f"嚴重錯誤: 無法開啟裝置句柄！")
        print(f"錯誤訊息: {e}")
        return None


def hid_write_and_read(device_handle):
    """
    對已開啟的介面句柄執行寫入和讀取操作。
    """
    if device_handle is None:
        return

    # 準備要傳送的 HID Report 數據
    data_to_send = [0xae, 0x00, 0x00, 0x00,
                    0x01, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00,
                   ] # 假設 Report ID 1，請根據您的客製化格式調整

    # --- 執行寫入操作 ---
    try:
        bytes_written = device_handle.write(data_to_send)
        print(f"\n--- 3. 執行通訊 ---")
        print(f"成功寫入 {bytes_written} 個位元組: {data_to_send}")

    except Exception as e:
        print(f"錯誤: 寫入資料失敗: {e}")
        device_handle.close()
        return

    # --- 執行讀取操作 ---
#    try:
#        # 讀取 Report。設定合理的超時時間 (例如 100 毫秒)
#        report = device_handle.read(64, timeout=100) 
#        
#        if report:
#            print(f"成功讀取到 HID Report (長度 {len(report)}): {report}")
#       else:
#            print("讀取操作超時或裝置未回傳數據。")

#    except Exception as e:
#        print(f"錯誤: 讀取資料失敗: {e}")

    finally:
        # --- 4. 清理 ---
        print("正在關閉裝置句柄並釋放資源...")
        device_handle.close()


if __name__ == '__main__':
    # 1. 嘗試開啟客製化介面
    handle = open_custom_hid_interface(DEVICE_VID, DEVICE_PID, TARGET_INTERFACE_NUMBER)

    # 2. 如果成功，執行讀寫操作
    if handle:
        hid_write_and_read(handle)