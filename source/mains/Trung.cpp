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

using json = nlohmann::json;

using  namespace std;


#include <chrono>
#include <ctime>
#include <iomanip>

int main() {
	auto now = std::chrono::system_clock::now();
	std::time_t now_time = std::chrono::system_clock::to_time_t(now);
	std::tm* now_tm = std::localtime(&now_time);
	std::cout << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S") << '\n';


	system("pause");
	return 0;
}
