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

class Base {
public:
	void speak(std::string name) {
		std::cout << "hello my name is " << name;
	}
};

class Derived : public Base{
public:
	void speak(std::string name) {
		std::cout << "hello my name is " << name << " and I am a cat";
	}
};


int  main() {
	DataObj object;


	//object.setID("ID123");
	//object.setDataType(RESPONSE);
	//object.setFuncType(DIR);
	//object.setCmdType(SHOW);
	//object.setData("trung nguyen quoc trung nguyen quoc tran trong nghia tran ky thanh");
	std::cout << object.toString();


	system("pause");

	return 0;
}