//
// Created by 21272 on 2025/12/17.
//

#include "Menu.h"
#include "FileManager.h"
#include "Logger.h"
#include <filesystem>
#include <stdexcept>

namespace FileManager {
    namespace Menu {
        // 加载图片，支持多种路径格式
        bool loadImageWithPath(sf::Texture& texture, const std::string& imagePath) {
            // 先尝试直接使用路径
            if (texture.loadFromFile(imagePath)) {
                return true;
            }
            
            // 如果失败，尝试添加 product_path 前缀
            std::string product_path = getProductPath();
            std::filesystem::path product_path_obj(product_path);
            std::filesystem::path image_path_obj(imagePath);
            std::filesystem::path full_path = product_path_obj / image_path_obj;
            
            if (texture.loadFromFile(full_path.string())) {
                return true;
            }
            
            // 如果还是失败，尝试使用 resource 目录（向后兼容）
            std::filesystem::path resource_path = std::filesystem::path(".") / "resource" / image_path_obj;
            if (texture.loadFromFile(resource_path.string())) {
                return true;
            }
            
            return false;
        }
        
        // 加载字体，支持多种字体回退
        sf::Font loadFontWithFallback(const std::vector<std::string>& fontPaths) {
            sf::Font font;
            
            for (const auto& fontPath : fontPaths) {
                if (font.loadFromFile(fontPath)) {
                    return font;
                }
            }
            
            // 如果所有字体都加载失败，抛出异常
            throw std::runtime_error("Failed to load any font from the provided paths");
        }
        
        // 加载默认字体（微软雅黑）
        sf::Font loadDefaultFont() {
            std::vector<std::string> fontPaths = {
                "C:\\Windows\\Fonts\\msyh.ttc"
            };
            
            try {
                return loadFontWithFallback(fontPaths);
            } catch (const std::exception& e) {
                // 如果微软雅黑加载失败，尝试使用系统默认字体
                // 这里可以添加其他回退字体
                Logger::error("Failed to load default font in Menu::loadDefaultFont: " + std::string(e.what()));
                throw;
            }
        }
        
        // 加载标题字体（装饰性字体）
        sf::Font loadTitleFont() {
            std::vector<std::string> fontPaths = {
                "C:\\Windows\\Fonts\\ALGER.TTF",      // Algerian
                "C:\\Windows\\Fonts\\AGENCYB.TTF",   // Agency FB Bold
                "C:\\Windows\\Fonts\\IMPACT.TTF",    // Impact
                "C:\\Windows\\Fonts\\COOPBL.TTF",    // Cooper Black
                "C:\\Windows\\Fonts\\msyh.ttc"       // 微软雅黑（最后回退）
            };
            
            return loadFontWithFallback(fontPaths);
        }
        
        // 加载副标题字体（仿宋）
        sf::Font loadSubtitleFont() {
            std::vector<std::string> fontPaths = {
                "C:\\Windows\\Fonts\\simfang.ttf",   // 仿宋
                "C:\\Windows\\Fonts\\msyh.ttc"       // 微软雅黑（回退）
            };
            
            return loadFontWithFallback(fontPaths);
        }
    }
}
