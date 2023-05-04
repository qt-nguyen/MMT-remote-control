#include "DIR_Obj.h"
#include <string>
#include <filesystem>
namespace fs = std::filesystem;







void DIR_Obj::buildTree(const fs::path& path, int max_depth, DirectoryTreeNode& node, int current_depth)
{
    // If current depth exceeds max depth, exits program
    if (current_depth > max_depth && max_depth) return;

    // If the current path is a folder / directory
    if (fs::is_directory(path)) {
        // Save to DirectoryTreeNode object
        node._name = path.filename().string();
        if (current_depth < max_depth) {
            try {
                for (const auto& entry : fs::directory_iterator(path)) {
                    DirectoryTreeNode child_node;

                    // Recursively adds its sub-directories to the tree
                    buildTree(entry.path(), max_depth, child_node, current_depth + 1);
                    node._children.push_back(child_node);
                }
            }
            catch (const fs::filesystem_error& err) {
                // Handle error...
            }
        }
    }


}

std::string DIR_Obj::toString()
{
	return 
}

std::string DIR_Obj::toFile(std::string filename)
{
	return std::string();
}


