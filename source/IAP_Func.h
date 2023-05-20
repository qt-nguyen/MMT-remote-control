#pragma once

#include "DataObj/DataObj.h"

// containing functions for Installed AaP

class IAP_Func
{
private:

public:

	std::shared_ptr<DataObj> listApps();

    // Start an app
	std::shared_ptr<DataObj> startApp(std::string appName);

    // Stop an app
	std::shared_ptr<DataObj> stopApp(std::string appName);

};