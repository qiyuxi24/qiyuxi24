//
// Created by 21272 on 2025/12/17.
//

#ifndef NOJ_SOLUTION_BEATMAP_H
#define NOJ_SOLUTION_BEATMAP_H

#include <string>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>
#include "../Tools/JsonParser.h"

namespace FileManager {
    // 异常类定义
    class BeatmapException : public std::runtime_error {
    public:
        explicit BeatmapException(const std::string& message) : std::runtime_error(message) {}
    };

    class InvalidFormatException : public BeatmapException {
    public:
        explicit InvalidFormatException(const std::string& message)
            : BeatmapException("Invalid format: " + message) {}
    };

    class Beatmap {
    private:
        nlohmann::json beatmap_data;
        std::string file_path;

    public:
        // 构造函数
        Beatmap();
        explicit Beatmap(const std::string& chart_path);

        // 加载谱面文件
        void load(const std::string& chart_path);

        // 获取原始JSON数据
        const nlohmann::json& getRawData() const;

        // 获取元数据
        const nlohmann::json& getMetadata() const;

        // 获取时间数据
        const nlohmann::json& getTimeData() const;

        // 获取音符数据
        const nlohmann::json& getNoteData() const;

        // 获取谱面文件路径
        const std::string& getFilePath() const;

        // 检查是否已加载谱面
        bool isLoaded() const;

        // 加载背景图片
        sf::Texture loadBackgroundImage(const std::string& beatmap_dir) const;

        // 获取背景图片路径
        std::string getBackgroundImagePath(const std::string& beatmap_dir) const;

        // 获取音乐文件路径
        std::string getMusicPath(const std::string& beatmap_dir) const;

    private:
        // 验证谱面格式
        void validateFormat() const;
    };
} // namespace FileManager

#endif //NOJ_SOLUTION_BEATMAP_H
