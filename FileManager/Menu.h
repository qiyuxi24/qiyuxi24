//
// Created by 21272 on 2025/12/17.
//

#ifndef NOJ_SOLUTION_MENU_H
#define NOJ_SOLUTION_MENU_H

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

namespace FileManager {
    namespace Menu {
        // 加载图片，支持多种路径格式
        // 先尝试直接路径，再尝试 product_path 前缀路径，最后尝试 "./resource/" 目录
        // texture: 要加载的纹理对象引用
        // imagePath: 图片文件路径（相对或绝对）
        // 返回是否加载成功
        bool loadImageWithPath(sf::Texture& texture, const std::string& imagePath);
        
        // 加载字体，支持多种字体回退
        // fontPaths: 字体文件路径列表，按优先级排序
        // 返回加载成功的字体对象
        // 如果所有字体都加载失败，抛出 std::runtime_error 异常
        sf::Font loadFontWithFallback(const std::vector<std::string>& fontPaths);
        
        // 加载默认字体（微软雅黑）
        sf::Font loadDefaultFont();
        
        // 加载标题字体（装饰性字体，按优先级：Algerian, Agency FB Bold, Impact, Cooper Black, 微软雅黑）
        sf::Font loadTitleFont();
        
        // 加载副标题字体（仿宋，如果失败则使用默认字体）
        sf::Font loadSubtitleFont();
    }
}

#endif //NOJ_SOLUTION_MENU_H
