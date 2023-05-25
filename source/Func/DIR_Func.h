#pragma once
#include "../DataObj/DataObj.h"
#include "../Utils/utils.h"

#include <nlohmann/json.hpp>
#include <filesystem>
#include <vector>
#include <iostream>
#include <string>

using json = nlohmann::json;
namespace fs = std::filesystem;

class DIR_Func
{
public:
	json DirectoryTreeToJson(const fs::path& path = fs::current_path(), int max_level = -1, bool include_files = true, int level = 0);

	void PrintDirectoryTree(const fs::path& path = fs::current_path(), int max_level = -1, bool include_files = true, int level = 0);

	void DirectoryTreeToString_utils(std::stringstream& ss, const fs::path& path = fs::current_path(), int max_level = -1, bool include_files = true, int level = 0);

	std::string DirectoryTreeToString(const fs::path& path = fs::current_path(), int max_level = -1, bool include_files = true);

	void PrintDirectoryTree(const json& tree, int indent = 0);

	void ParseCommand(const std::string& command, int& maxDepth, bool& includeFiles, std::string& path);

	//Handle a DataObj as request and execute the corresponding function
	std::shared_ptr<DataObj> HandleRequest(DataObj request);
};


