#pragma once
#include "DataObj/DataObj.h"
#include "utils.h"

#include <vector>
#include <string>
#include <windows.h>
#include <thread>


class SCR_Func {
private:
    static bool capturing;
    static std::vector<char> screenData;

    static void captureLoop();

public:
    std::shared_ptr<DataObj> startCapture();

    std::shared_ptr<DataObj> stopCapture();
};
