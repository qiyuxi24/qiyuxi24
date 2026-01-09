//
// Created by 21272 on 2025/12/17.
//

#ifndef NOJ_SOLUTION_FILEMANAGER_H
#define NOJ_SOLUTION_FILEMANAGER_H

#include <string>

// 包含所有 FileManager 命名空间下的类
#include "Beatmap.h"
#include "Config.h"
#include "User.h"
#include "Screenshot.h"
#include "Logger.h"

namespace FileManager {
    // 全局变量：可执行文件的绝对路径（目录部分）
    extern std::string g_executablePath;

    // 初始化函数：获取可执行文件路径，必须在调用其他 FileManager 功能前调用
    void init();
    
    // 获取可执行文件的绝对路径
    const std::string& getExecutablePath();
} // namespace FileManager

#endif //NOJ_SOLUTION_FILEMANAGER_H
