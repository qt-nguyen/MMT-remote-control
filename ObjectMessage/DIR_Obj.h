#pragma once
#include "DataTransferObj.h"
#include <string>
#include <vector>
#include <filesystem>

namespace  fs = std::filesystem;

struct DirectoryTreeNode 
{
public:
	std::string _name;
	std::vector<DirectoryTreeNode> _children;
};


class DIR_Obj: public DataTransferObj
{
public:
	void buildTree(const fs::path& path, int max_depth, DirectoryTreeNode& node, int current_depth = 0);


	// Convert data object to string type
	std::string toString();

	// Convert data object to a file
	std::string toFile(std::string filename);



};