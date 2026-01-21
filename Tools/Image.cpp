//
// Created by 21272 on 2025/12/17.
//

#include "Image.h"
#include <stdexcept>

namespace image {
    // 加载背景图片
    sf::Texture loadBackgroundImage(const std::string& image_path) {
        sf::Texture texture;
        
        if (!texture.loadFromFile(image_path)) {
            throw std::runtime_error("Failed to load background image: " + image_path);
        }
        
        return texture;
    }
}
