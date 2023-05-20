#pragma once
#include <string>
#include "DataObj/DataObj.h"
#include <vector>
#include <filesystem>
#include <iostream>
#include <string>


class DIR_Func
{
public:
    void print_directory_tree(const fs::path& path = fs::current_path(), int max_level = -1, bool include_files = true, int level = 0) {
        try {
            if (!fs::exists(path) || !fs::is_directory(path)) return;
            for (const auto& entry : fs::directory_iterator(path)) {
                const auto filename = entry.path().filename().string();
                if (entry.is_directory()) {
                    std::cout << std::string(level * 2, ' ') << level << " [+] " << filename << '\n';
                    if (max_level <= 0 || level + 1 < max_level) {
                        print_directory_tree(entry, max_level, include_files, level + 1);
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

    json directory_tree_to_json(const fs::path& path = fs::current_path(), int max_level = -1, bool include_files = true, int level = 0) {
        json result;
        try {
            if (!fs::exists(path) || !fs::is_directory(path)) return result;
            result["name"] = path.filename().string();
            result["level"] = level;
            result["type"] = "directory";
            for (const auto& entry : fs::directory_iterator(path)) {
                if (entry.is_directory()) {
                    if (max_level <= 0 || level + 1 < max_level) {
                        result["children"].push_back(directory_tree_to_json(entry, max_level, include_files, level + 1));
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




};


