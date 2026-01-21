<<<<<<< Updated upstream
//
// Created by 21272 on 2025/12/17.
//

#ifndef NOJ_SOLUTION_FILEMANAGER_H
#define NOJ_SOLUTION_FILEMANAGER_H

// 包含所有 FileManager 命名空间下的类
#include "Beatmap.h"
#include "Config.h"
#include "User.h"
#include "Screenshot.h"
#include "Logger.h"

#endif //NOJ_SOLUTION_FILEMANAGER_H
=======
//
// Created by 21272 on 2025/12/17.
//

#ifndef NOJ_SOLUTION_FILEMANAGER_H
#define NOJ_SOLUTION_FILEMANAGER_H

#include <string>

// 包含所有 FileManager 命名空间下的类
#include "Beatmap.h"
#include "Config.h"
#include "LocalScore.h"
#include "Logger.h"
#include "Menu.h"

namespace FileManager {
    // 全局变量：储存成品路径
    extern std::string product_path;

    // 初始化 FileManager 模块
    // 如果 product_path 为空，则使用当前工作目录
    void init(const std::string& product_path = "");

    // 获取成品路径
    std::string getProductPath();

    // 设置成品路径
    void setProductPath(const std::string& path);
} // namespace FileManager

#endif //NOJ_SOLUTION_FILEMANAGER_H
>>>>>>> Stashed changes
