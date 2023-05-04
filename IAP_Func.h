#pragma once

#include "DataTransferObj.h"
#include "IAP_TransferObj.h"

class IAP_Func
{
private:

public:

	DataTransferObj* listApps();

    // Start an app
	DataTransferObj* startApp(std::string appName);

    // Stop an app
	DataTransferObj* stopApp(std::string appName);

};