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

using json = nlohmann::json;

using  namespace std;


#include <chrono>
#include <ctime>
#include <iomanip>

int main() {
	

std:shared_ptr<DataObj> obj(new DataObj(utils::CurrentTime(), REQUEST, KLG, STOP, "Never gonna give you up"));
	std::cout << obj->toString();





	system("pause");
	return 0;
}
