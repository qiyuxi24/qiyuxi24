//
// Created by 21272 on 2025/12/17.
//

#ifndef NOJ_SOLUTION_IMAGE_H
#define NOJ_SOLUTION_IMAGE_H

#include <string>
#include <SFML/Graphics.hpp>

namespace image {
    // 加载背景图片
    // image_path: 图片文件路径
    // 返回加载的纹理对象
    // 如果加载失败，抛出 std::runtime_error 异常
    sf::Texture loadBackgroundImage(const std::string& image_path);
}

#endif //NOJ_SOLUTION_IMAGE_H
