//
// Created by 21272 on 2025/12/17.
//

#include "Beatmap.h"
#include <filesystem>
#include <algorithm>

namespace FileManager {
    // 构造函数实现
    Beatmap::Beatmap() : beatmap_data(nlohmann::json::object()), file_path("") {
    }

    Beatmap::Beatmap(const std::string& chart_path) : beatmap_data(nlohmann::json::object()), file_path("") {
        load(chart_path);
    }

    // 加载谱面文件
    void Beatmap::load(const std::string& chart_path) {
        this->file_path = chart_path;

        // 使用 JsonParser 解析文件
        beatmap_data = Tools::JsonParser::parseFromFile(chart_path);

        // 验证谱面格式
        validateFormat();
    }

    // 获取原始JSON数据
    const nlohmann::json& Beatmap::getRawData() const {
        if (!isLoaded()) {
            throw BeatmapException("No beatmap loaded");
        }
        return beatmap_data;
    }

    // 获取元数据
    const nlohmann::json& Beatmap::getMetadata() const {
        if (!isLoaded()) {
            throw BeatmapException("No beatmap loaded");
        }
        return beatmap_data.at("meta");
    }

    // 获取时间数据
    const nlohmann::json& Beatmap::getTimeData() const {
        if (!isLoaded()) {
            throw BeatmapException("No beatmap loaded");
        }
        return beatmap_data.at("time");
    }

    // 获取音符数据
    const nlohmann::json& Beatmap::getNoteData() const {
        if (!isLoaded()) {
            throw BeatmapException("No beatmap loaded");
        }
        return beatmap_data.at("note");
    }

    // 获取谱面文件路径
    const std::string& Beatmap::getFilePath() const {
        return file_path;
    }

    // 检查是否已加载谱面
    bool Beatmap::isLoaded() const {
        return !beatmap_data.is_null() && !beatmap_data.empty();
    }

    // 加载背景图片
    sf::Texture Beatmap::loadBackgroundImage(const std::string& beatmap_dir) const {
        if (!isLoaded()) {
            throw BeatmapException("No beatmap loaded");
        }

        std::string image_path = getBackgroundImagePath(beatmap_dir);
        sf::Texture texture;

        if (!texture.loadFromFile(image_path)) {
            throw BeatmapException("Failed to load background image: " + image_path);
        }

        return texture;
    }

    // 获取背景图片路径
    std::string Beatmap::getBackgroundImagePath(const std::string& beatmap_dir) const {
        if (!isLoaded()) {
            throw BeatmapException("No beatmap loaded");
        }

        const auto& meta = beatmap_data.at("meta");
        std::string bg_filename = meta.value("background", "background.jpg");

        // 构建完整路径
        std::filesystem::path dir_path(beatmap_dir);
        std::filesystem::path bg_path = dir_path / bg_filename;

        return bg_path.string();
    }

    // 获取音乐文件路径
    std::string Beatmap::getMusicPath(const std::string& beatmap_dir) const {
        if (!isLoaded()) {
            throw BeatmapException("No beatmap loaded");
        }

        const auto& meta = beatmap_data.at("meta");
        const auto& song = meta.at("song");
        std::string music_filename = song.at("file").get<std::string>();

        // 构建完整路径
        std::filesystem::path dir_path(beatmap_dir);
        std::filesystem::path music_path = dir_path / music_filename;

        return music_path.string();
    }

    // 验证谱面格式
    void Beatmap::validateFormat() const {
        // 检查必要的顶层字段
        if (!beatmap_data.contains("meta") || !beatmap_data["meta"].is_object()) {
            throw InvalidFormatException("Missing or invalid 'meta' field");
        }
        if (!beatmap_data.contains("time") || !beatmap_data["time"].is_array()) {
            throw InvalidFormatException("Missing or invalid 'time' field");
        }
        if (!beatmap_data.contains("note") || !beatmap_data["note"].is_array()) {
            throw InvalidFormatException("Missing or invalid 'note' field");
        }

        // 检查meta字段中的必要子字段
        const auto& meta = beatmap_data["meta"];
        if (!meta.contains("song") || !meta["song"].is_object()) {
            throw InvalidFormatException("Missing or invalid 'meta.song' field");
        }
        const auto& song = meta["song"];
        if (!song.contains("title") || !song["title"].is_string()) {
            throw InvalidFormatException("Missing or invalid 'meta.song.title' field");
        }
        if (!song.contains("artist") || !song["artist"].is_string()) {
            throw InvalidFormatException("Missing or invalid 'meta.song.artist' field");
        }
        if (!song.contains("file") || !song["file"].is_string()) {
            throw InvalidFormatException("Missing or invalid 'meta.song.file' field");
        }
        if (!song.contains("bpm") || !song["bpm"].is_number()) {
            throw InvalidFormatException("Missing or invalid 'meta.song.bpm' field");
        }
    }
} // namespace FileManager
