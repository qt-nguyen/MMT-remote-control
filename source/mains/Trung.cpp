#include <iostream>
#include <string>
#include <sstream>
#include "../DIR_Func.h"
#include "../DataObj/DataObj.h"
void ParseCommand(const std::string& command, int& maxDepth, bool& includeFiles, std::string& path) {
    std::istringstream iss(command);
    std::string depthStr, includeFilesStr, pathStr;

    // Extract the three parameters from the command string
    iss >> depthStr >> includeFilesStr >> pathStr;

    // Convert and assign the values to the corresponding variables
    maxDepth = std::stoi(depthStr);
    includeFiles = (std::stoi(includeFilesStr) != 0);
    path = (pathStr.empty() ? "." : pathStr);
}

int main() {
    std::string command = "3 0 ";
    DataObj message("bullshit", REQUEST, DIR, SHOW, command);
    DIR_Func dir;
    auto res = dir.HandleRequest(message);
    std::cout << res->getData_String();
    

    system("pause");
}
