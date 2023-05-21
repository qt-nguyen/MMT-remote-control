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

DataObj DIR_Func::HandleRequest(DataObj request)
{
    DataObj result(utils::CurrentTime(), RESPONSE, DIR, CMD_TYPE, "");
    if (request.getDataType() != REQUEST || request.getFuncType() != DIR || request.getCmdType() != SHOW)
        return result;


}
