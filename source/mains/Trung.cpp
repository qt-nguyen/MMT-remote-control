/*
This is Trung's demo to test functionalities and dependencies
Please exclude this file from project if you want to run your own main demo
Don't change this file  without informing the author
*/
#include <nlohmann/json.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "../DataObj/DataObj.h"
#include "../utils.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <filesystem>
#include "../DIR_Func.h"
#include "../IAP_Func.h"
#include "../RPC_Func.h"
namespace fs = std::filesystem;
using json = nlohmann::json;

using  namespace std;




int main() {
    DIR_Func dir;
    auto req = DataObj("", REQUEST, DIR, SHOW, "D:\\Desktop");
    auto res = dir.HandleRequest(req);

    auto  data = json::parse(res->getData_String());
    dir.PrintDirectoryTree(data, 4);
    

    system("pause");
    return 0;
}
