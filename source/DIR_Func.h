#pragma once
#include <string>
#include "DataObj/DataObj.h"
#include <vector>
#include <filesystem>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace fs = std::filesystem;

class DIR_Func
{
public:
	json DirectoryTreeToJson(const fs::path& path = fs::current_path(), int max_level = -1, bool include_files = true, int level = 0);


	void PrintDirectoryTree(const fs::path& path = fs::current_path(), int max_level = -1, bool include_files = true, int level = 0);



	std::shared_ptr<DataObj> HandleRequest(DataObj request);


	void PrintDirectoryTree(const json& tree, int indent = 0);


};


