//
// Created by 21272 on 2025/12/17.
//

#include "Screenshot.h"
#include <filesystem>

namespace FileManager {
    // 从 RenderTexture 保存截图
    void Screenshot::save(const sf::RenderTexture& render_texture, const std::string& filename) {
        // 确保 screenshot 目录存在
        std::filesystem::create_directories("screenshot");

        std::filesystem::path screenshot_path = std::filesystem::path("screenshot") / filename;

        // 从 RenderTexture 获取图像并保存
        sf::Image image = render_texture.getTexture().copyToImage();
        if (!image.saveToFile(screenshot_path.string())) {
            throw ScreenshotException("Failed to save screenshot: " + screenshot_path.string());
        }
    }

    // 从窗口保存截图
    void Screenshot::save(const sf::RenderWindow& window, const std::string& filename) {
        // 确保 screenshot 目录存在
        std::filesystem::create_directories("screenshot");

        std::filesystem::path screenshot_path = std::filesystem::path("screenshot") / filename;

        // 创建纹理并复制窗口内容
        sf::Vector2u window_size = window.getSize();
        sf::Texture texture;
        texture.create(window_size.x, window_size.y);
        texture.update(window);

        // 保存纹理到文件
        if (!texture.copyToImage().saveToFile(screenshot_path.string())) {
            throw ScreenshotException("Failed to save screenshot: " + screenshot_path.string());
        }
    }

    // 获取截图保存路径
    std::string Screenshot::getScreenshotPath(const std::string& filename) {
        std::filesystem::path screenshot_path = std::filesystem::path("screenshot") / filename;
        return screenshot_path.string();
    }
} // namespace FileManager
