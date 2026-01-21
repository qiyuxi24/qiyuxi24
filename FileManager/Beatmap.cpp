<<<<<<< Updated upstream
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

        try {
            // 使用 JsonParser 解析文件
            beatmap_data = Tools::JsonParser::parseFromFile(chart_path);

            // 验证谱面格式
            validateFormat();
        } catch (const Tools::FileNotFoundException& e) {
            throw BeatmapException("File not found: " + chart_path);
        } catch (const Tools::ParseException& e) {
            throw BeatmapException("Parse error: " + std::string(e.what()));
        } catch (const Tools::FileReadException& e) {
            throw BeatmapException("File read error: " + std::string(e.what()));
        } catch (const BeatmapException&) {
            throw;
        } catch (const std::exception& e) {
            throw BeatmapException("Unexpected error: " + std::string(e.what()));
        }
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

        try {
            const auto& meta = beatmap_data.at("meta");
            std::string bg_filename = meta.value("background", "background.jpg");

            // 构建完整路径
            std::filesystem::path dir_path(beatmap_dir);
            std::filesystem::path bg_path = dir_path / bg_filename;

            return bg_path.string();
        } catch (const std::exception& e) {
            throw BeatmapException("Failed to get background image path: " + std::string(e.what()));
        }
    }

    // 获取音乐文件路径
    std::string Beatmap::getMusicPath(const std::string& beatmap_dir) const {
        if (!isLoaded()) {
            throw BeatmapException("No beatmap loaded");
        }

        try {
            const auto& meta = beatmap_data.at("meta");
            const auto& song = meta.at("song");
            std::string music_filename = song.at("file").get<std::string>();

            // 构建完整路径
            std::filesystem::path dir_path(beatmap_dir);
            std::filesystem::path music_path = dir_path / music_filename;

            return music_path.string();
        } catch (const std::exception& e) {
            throw BeatmapException("Failed to get music path: " + std::string(e.what()));
        }
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
=======
//
// Created by 21272 on 2025/12/17.
//

#include "Beatmap.h"
#include "FileManager.h"
#include "Logger.h"
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
        // 使用局部变量构建完整路径
        std::string full_path;
        std::filesystem::path path_obj(chart_path);
        
        // 如果是绝对路径，直接使用；否则基于 product_path 构建
        if (path_obj.is_absolute()) {
            full_path = chart_path;
            this->file_path = chart_path;
        } else {
            std::string product_path = getProductPath();
            std::filesystem::path product_path_obj(product_path);
            std::filesystem::path full_path_obj = product_path_obj / chart_path;
            full_path = full_path_obj.string();
            this->file_path = chart_path; // 保存相对路径
        }

        try {
            // 使用 JsonParser 解析文件
            beatmap_data = Tools::JsonParser::parseFromFile(full_path);

            // 验证谱面格式
            validateFormat();
        } catch (const Tools::FileNotFoundException& e) {
            Logger::error("Beatmap file not found: " + full_path + " - " + std::string(e.what()));
            throw BeatmapException("File not found: " + full_path);
        } catch (const Tools::ParseException& e) {
            Logger::error("Beatmap parse error: " + full_path + " - " + std::string(e.what()));
            throw BeatmapException("Parse error: " + std::string(e.what()));
        } catch (const Tools::FileReadException& e) {
            Logger::error("Beatmap file read error: " + full_path + " - " + std::string(e.what()));
            throw BeatmapException("File read error: " + std::string(e.what()));
        } catch (const BeatmapException&) {
            throw;
        } catch (const std::exception& e) {
            Logger::error("Unexpected error in Beatmap::load: " + full_path + " - " + std::string(e.what()));
            throw BeatmapException("Unexpected error: " + std::string(e.what()));
        }
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

    // 获取背景图片路径
    std::string Beatmap::getBackgroundImagePath() const {
        if (!isLoaded()) {
            throw BeatmapException("No beatmap loaded");
        }

        try {
            const auto& meta = beatmap_data.at("meta");
            std::string bg_filename = meta.value("background", "background.jpg");

            // 使用局部变量构建完整路径
            std::string product_path = getProductPath();
            std::filesystem::path file_path_obj(file_path);
            std::filesystem::path beatmap_dir = file_path_obj.parent_path();
            
            // 如果 file_path 是相对路径，基于 product_path 构建
            if (!file_path_obj.is_absolute()) {
                std::filesystem::path product_path_obj(product_path);
                beatmap_dir = product_path_obj / beatmap_dir;
            }
            
            std::filesystem::path bg_path = beatmap_dir / bg_filename;

            return bg_path.string();
        } catch (const std::exception& e) {
            Logger::error("Failed to get background image path in Beatmap: " + std::string(e.what()));
            throw BeatmapException("Failed to get background image path: " + std::string(e.what()));
        }
    }

    // 获取音乐文件路径
    std::string Beatmap::getMusicPath() const {
        if (!isLoaded()) {
            throw BeatmapException("No beatmap loaded");
        }

        try {
            std::string music_filename;
            
            // 首先尝试从 meta.song.file 获取（如果存在）
            const auto& meta = beatmap_data.at("meta");
            const auto& song = meta.at("song");
            if (song.contains("file") && song["file"].is_string()) {
                music_filename = song["file"].get<std::string>();
            } else {
                // 如果 meta.song.file 不存在，从 note 数组的 sound 字段获取
                const auto& note_data = beatmap_data.at("note");
                if (note_data.is_array() && !note_data.empty()) {
                    for (const auto& note : note_data) {
                        if (note.is_object() && note.contains("sound") && note["sound"].is_string()) {
                            music_filename = note["sound"].get<std::string>();
                            break;
                        }
                    }
                }
                
                // 如果仍然没有找到，使用默认值
                if (music_filename.empty()) {
                    music_filename = "music.ogg";
                }
            }

            // 使用局部变量构建完整路径
            std::string product_path = getProductPath();
            std::filesystem::path file_path_obj(file_path);
            std::filesystem::path beatmap_dir = file_path_obj.parent_path();
            
            // 如果 file_path 是相对路径，基于 product_path 构建
            if (!file_path_obj.is_absolute()) {
                std::filesystem::path product_path_obj(product_path);
                beatmap_dir = product_path_obj / beatmap_dir;
            }
            
            std::filesystem::path music_path = beatmap_dir / music_filename;

            return music_path.string();
        } catch (const std::exception& e) {
            Logger::error("Failed to get music path in Beatmap: " + std::string(e.what()));
            throw BeatmapException("Failed to get music path: " + std::string(e.what()));
        }
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
        // file 和 bpm 字段在 Malody 格式中不是必需的
        // file 可以从 note 数组的 sound 字段获取
        // bpm 可以从 time[0].bpm 获取
    }

    // 获取曲目列表
    std::vector<BeatmapInfo> Beatmap::getBeatmapList() {
        std::vector<BeatmapInfo> beatmap_list;
        
        try {
            // 获取成品路径
            std::string product_path = getProductPath();
            std::filesystem::path product_path_obj(product_path);
            std::filesystem::path beatmap_dir = product_path_obj / "beatmap";
            
            // 检查 beatmap 目录是否存在
            if (!std::filesystem::exists(beatmap_dir) || !std::filesystem::is_directory(beatmap_dir)) {
                return beatmap_list; // 返回空列表
            }
            
            // 遍历 beatmap 目录下的所有子目录
            for (const auto& entry : std::filesystem::directory_iterator(beatmap_dir)) {
                if (!entry.is_directory()) {
                    continue;
                }
                
                std::filesystem::path subdir = entry.path();
                std::filesystem::path chart_file = subdir / "chart.mc";
                
                // 检查是否存在 chart.mc 文件
                if (!std::filesystem::exists(chart_file)) {
                    continue;
                }
                
                try {
                    // 创建临时 Beatmap 对象加载谱面
                    Beatmap temp_beatmap;
                    std::string chart_relative_path = "beatmap/" + subdir.filename().string() + "/chart.mc";
                    temp_beatmap.load(chart_relative_path);
                    
                    if (!temp_beatmap.isLoaded()) {
                        continue;
                    }
                    
                    // 提取元数据
                    const auto& meta = temp_beatmap.getMetadata();
                    const auto& song = meta.at("song");
                    
                    BeatmapInfo info;
                    info.id = subdir.filename().string();
                    info.title = song.at("title").get<std::string>();
                    info.artist = song.at("artist").get<std::string>();
                    
                    // 获取 BPM：优先从 time[0].bpm 获取，如果不存在则从 meta.song.bpm 获取
                    const auto& time_data = temp_beatmap.getTimeData();
                    if (time_data.is_array() && !time_data.empty() && time_data[0].contains("bpm") && time_data[0]["bpm"].is_number()) {
                        info.bpm = time_data[0]["bpm"].get<double>();
                    } else if (song.contains("bpm") && song["bpm"].is_number()) {
                        info.bpm = song["bpm"].get<double>();
                    } else {
                        info.bpm = 120.0; // 默认 BPM
                    }
                    
                    info.chart_path = chart_relative_path;
                    
                    // 获取背景图片文件名
                    std::string bg_filename = meta.value("background", "background.jpg");
                    info.background_path = "beatmap/" + info.id + "/" + bg_filename;
                    
                    // 获取音乐文件路径：优先从 note 数组的 sound 字段获取，否则从 meta.song.file 获取
                    std::string music_filename;
                    const auto& note_data = temp_beatmap.getNoteData();
                    if (note_data.is_array() && !note_data.empty()) {
                        for (const auto& note : note_data) {
                            if (note.is_object() && note.contains("sound") && note["sound"].is_string()) {
                                music_filename = note["sound"].get<std::string>();
                                break;
                            }
                        }
                    }
                    if (music_filename.empty() && song.contains("file") && song["file"].is_string()) {
                        music_filename = song["file"].get<std::string>();
                    }
                    if (music_filename.empty()) {
                        music_filename = "music.ogg"; // 默认文件名
                    }
                    info.music_path = "beatmap/" + info.id + "/" + music_filename;
                    
                    beatmap_list.push_back(info);
                } catch (const std::exception& e) {
                    // 如果加载失败，跳过该曲目（不抛出异常）
                    // 记录日志，但为了不影响其他曲目，继续处理下一个
                    Logger::error("Failed to load beatmap in getBeatmapList: " + subdir.filename().string() + " - " + std::string(e.what()));
                    continue;
                }
            }
        } catch (const std::exception& e) {
            // 如果扫描目录失败，返回空列表
            return beatmap_list;
        }
        
        return beatmap_list;
    }
} // namespace FileManager
>>>>>>> Stashed changes
