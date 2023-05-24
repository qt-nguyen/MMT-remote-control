#include "DIR_Func.h"
#include <nlohmann/json.hpp>
#include <string>
#include <filesystem>
#include "utils.h"

using json = nlohmann::json;
namespace fs = std::filesystem;

json DIR_Func::DirectoryTreeToJson(const fs::path& path, int max_level, bool include_files, int level)
{
    // path: path to directory
    // max_level: maximum level of nodes to traverse
    // include_files: include files in the result or not
    // level: used for recursive calling

    json result;
    try {
        if (!fs::exists(path) || !fs::is_directory(path)) return result;
        result["name"] = path.filename().string();
        result["level"] = level;
        result["type"] = "directory";
        result["path"] = fs::absolute(path).string();
        for (const auto& entry : fs::directory_iterator(path)) {
            if (entry.is_directory()) {
                if (max_level <= 0 || level + 1 < max_level) {
                    result["children"].push_back(DirectoryTreeToJson(entry, max_level, include_files, level + 1));
                }
            }
            else if (include_files && entry.is_regular_file()) {
                json file;
                file["name"] = entry.path().filename().string();
                file["level"] = level + 1;
                file["type"] = "file";
                file["path"] = fs::absolute(entry.path()).string();
                result["children"].push_back(file);
            }
        }
    }
    catch (const std::exception& e) {
        //std::cerr << e.what() << '\n';
    }
    return result;
}



void DIR_Func::PrintDirectoryTree(const fs::path& path, int max_level, bool include_files, int level) {
    try {
        if (!fs::exists(path) || !fs::is_directory(path)) return;
        for (const auto& entry : fs::directory_iterator(path)) {
            const auto filename = entry.path().filename().string();
            if (entry.is_directory()) {
                std::cout << std::string(level * 2, ' ') << level << " [+] " << filename << '\n';
                if (max_level <= 0 || level + 1 < max_level) {
                    PrintDirectoryTree(entry, max_level, include_files, level + 1);
                }
            }
            else if (include_files && entry.is_regular_file()) {
                std::cout << std::string(level * 2, ' ') << level << " - " << filename << '\n';
            }
        }
    }
    catch (const std::exception& e) {
        //std::cerr << e.what() << '\n';
    }
}


void DIR_Func::DirectoryTreeToString_utils(std::stringstream& ss, const fs::path& path, int max_level, bool include_files, int level)
{
    try {
        if (!fs::exists(path) || !fs::is_directory(path)) return;
        for (const auto& entry : fs::directory_iterator(path)) {
            const auto filename = entry.path().filename().string();
            if (entry.is_directory()) {
                ss << std::string(level * 4, ' ') << " [+] " << filename << '\n';
                if (max_level <= 0 || level + 1 < max_level) {
                    DirectoryTreeToString_utils(ss, entry, max_level, include_files, level + 1);
                }
            }
            else if (include_files && entry.is_regular_file()) {
                ss << std::string(level * 4, ' ')  << " - " << filename << '\n';
            }
        }
    }
    catch (const std::exception& e) {
        //std::cerr << e.what() << '\n';
    }
}


std::string DIR_Func::DirectoryTreeToString(const fs::path& path, int max_level, bool include_files)
{
    std::stringstream ss;
    DirectoryTreeToString_utils(ss, path, max_level, include_files, 0);
    std::string result = ss.str();
    return result;
}






void DIR_Func::PrintDirectoryTree(const json& tree, int indent) {
    std::string indent_str(indent, ' ');
    std::cout << indent_str << tree["name"] << '\n';
    if (tree.contains("children")) {
        for (const auto& child : tree["children"]) {
            PrintDirectoryTree(child, indent + 4);
        }
    }
}


void DIR_Func::ParseCommand(const std::string& command, int& maxDepth, bool& includeFiles, std::string& path) {
    std::istringstream iss(command);
    std::string depthStr, includeFilesStr, pathStr;

    // Extract the three parameters from the command string
    iss >> depthStr >> includeFilesStr >> pathStr;

    // Convert and assign the values to the corresponding variables
    maxDepth = std::stoi(depthStr);
    if (maxDepth < 0) maxDepth = -1;

    includeFiles = (std::stoi(includeFilesStr) != 0);
    path = (pathStr.empty() ? "." : pathStr);

    std::cout << depthStr << " : " << includeFilesStr << std::endl;
}


std::shared_ptr<DataObj> DIR_Func::HandleRequest(DataObj request)
{
    std::shared_ptr<DataObj> result(new DataObj(utils::CurrentTime(), RESPONSE, DIR, CMD_TYPE, ""));
    // Help request object is faulty, exit with null
    if (request.getDataType() != REQUEST || request.getFuncType() != DIR || request.getCmdType() != SHOW)
    {
        result->setData("MESSAGE PARAMETER ERRORS");
        return result;
    }

    int max_depth;
    bool include_files;
    std::string path;
    ParseCommand(request.getData_String(), max_depth, include_files, path);
    if (!fs::exists(path) || !fs::is_directory(path))
    {
        result->setData("DIRECTORY DOES NOT EXIST");
        return result;
    }

    auto res = this->DirectoryTreeToString(path, max_depth, include_files);
    result->setData(res);
    result->setCmdType(DATA);
    return result;

}
