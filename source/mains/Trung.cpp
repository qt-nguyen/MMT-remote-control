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
    RPC_Func rpc;
    std::shared_ptr<DataObj> res = rpc.listPrcs();
    std::string res_str = res->getData_String();
    std::cout << res->toJsonString();

    system("pause");
    return 0;
}
