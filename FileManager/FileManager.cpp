//
// Created by 21272 on 2025/12/17.
//

#include "FileManager.h"
#include <windows.h>
#include <filesystem>

namespace FileManager {
    // 全局变量定义
    std::string g_executablePath;

    // 初始化函数：获取可执行文件路径
    void init() {
        if (g_executablePath.empty()) {
            char path[MAX_PATH];
            // 获取当前可执行文件的完整路径
            DWORD length = GetModuleFileNameA(NULL, path, MAX_PATH);
            if (length > 0 && length < MAX_PATH) {
                std::filesystem::path exe_file(path);
                g_executablePath = exe_file.parent_path().string();
            } else {
                // 如果获取失败，使用当前工作目录
                g_executablePath = std::filesystem::current_path().string();
            }
        }
    }

    // 获取可执行文件的绝对路径
    const std::string& getExecutablePath() {
        return g_executablePath;
    }
} // namespace FileManager
