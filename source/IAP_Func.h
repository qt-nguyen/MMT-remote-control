#pragma once

#include "DataObj/DataObj.h"
#include "DataObj/IAP_Obj.h"

// containing functions for Installed AaP

class IAP_Func
{
private:

public:

	DataObj* listApps();

    // Start an app
	DataObj* startApp(std::string appName);

    // Stop an app
	DataObj* stopApp(std::string appName);

};