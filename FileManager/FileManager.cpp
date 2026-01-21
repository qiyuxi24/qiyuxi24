//
// Created by 21272 on 2025/12/17.
//

#include "FileManager.h"
#include <filesystem>

namespace FileManager {
    // 全局变量定义：储存成品路径
    std::string product_path = "";

    // 初始化 FileManager 模块
    void init(const std::string& product_path_param) {
        std::string path_to_set;
        
        if (product_path_param.empty()) {
            // 如果参数为空，使用当前工作目录
            path_to_set = std::filesystem::current_path().string();
        } else {
            // 如果提供了参数，使用该参数
            path_to_set = product_path_param;
        }
        
        // 使用 std::filesystem::path 规范化路径
        std::filesystem::path path_obj(path_to_set);
        path_to_set = path_obj.string();
        
        // 设置 product_path
        product_path = path_to_set;
    }

    // 获取成品路径
    std::string getProductPath() {
        // 如果 product_path 为空，返回当前工作目录
        if (product_path.empty()) {
            return std::filesystem::current_path().string();
        }
        return product_path;
    }

    // 设置成品路径
    void setProductPath(const std::string& path) {
        product_path = path;
    }
} // namespace FileManager
