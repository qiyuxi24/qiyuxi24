//
// Created by 21272 on 2025/12/17.
//

#ifndef NOJ_SOLUTION_SCREENSHOT_H
#define NOJ_SOLUTION_SCREENSHOT_H

#include <string>
#include <stdexcept>
#include <SFML/Graphics.hpp>

namespace FileManager {
    // 异常类定义
    class ScreenshotException : public std::runtime_error {
    public:
        explicit ScreenshotException(const std::string& message) : std::runtime_error(message) {}
    };

    class Screenshot {
    public:
        // 从 RenderTexture 保存截图
        static void save(const sf::RenderTexture& render_texture, const std::string& filename);

        // 从窗口保存截图
        static void save(const sf::RenderWindow& window, const std::string& filename);

        // 获取截图保存路径
        static std::string getScreenshotPath(const std::string& filename);
    };
} // namespace FileManager

#endif //NOJ_SOLUTION_SCREENSHOT_H
