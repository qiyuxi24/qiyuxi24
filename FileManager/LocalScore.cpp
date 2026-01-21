//
// Created by 21272 on 2025/12/17.
//

#include "LocalScore.h"
#include "FileManager.h"
#include "Logger.h"
#include <filesystem>
#include <ctime>

namespace FileManager {

    // 构造函数实现
    LocalScore::LocalScore() : score_data(nlohmann::json::object()) {
        load();
    }

    // 加载成绩数据
    void LocalScore::load() {
        // 使用局部变量构建完整路径
        std::string product_path = getProductPath();
        std::filesystem::path product_path_obj(product_path);
        std::filesystem::path score_path_obj = product_path_obj / "local_score.data";
        std::string score_path = score_path_obj.string();
        
        try {
            // 检查文件是否存在
            if (std::filesystem::exists(score_path)) {
                // 使用 JsonParser 解析文件
                score_data = Tools::JsonParser::parseFromFile(score_path);
            } else {
                // 文件不存在，创建默认成绩数据
                score_data = nlohmann::json::object();
                score_data["scores"] = nlohmann::json::object();
            }
        } catch (const Tools::ParseException& e) {
            Logger::error("LocalScore parse error: " + score_path + " - " + std::string(e.what()));
            throw ScoreException("Parse error: " + std::string(e.what()));
        } catch (const Tools::FileReadException& e) {
            Logger::error("LocalScore file read error: " + score_path + " - " + std::string(e.what()));
            throw ScoreException("File read error: " + std::string(e.what()));
        } catch (const std::exception& e) {
            Logger::error("Unexpected error in LocalScore::load: " + score_path + " - " + std::string(e.what()));
            throw ScoreException("Unexpected error: " + std::string(e.what()));
        }
    }

    // 保存成绩数据
    void LocalScore::save() {
        // 使用局部变量构建完整路径
        std::string product_path = getProductPath();
        std::filesystem::path product_path_obj(product_path);
        std::filesystem::path score_path_obj = product_path_obj / "local_score.data";
        std::string score_path = score_path_obj.string();
        
        try {
            // 使用 JsonParser 保存文件
            Tools::JsonParser::saveToFile(score_data, score_path);
        } catch (const Tools::FileWriteException& e) {
            Logger::error("LocalScore file write error: " + score_path + " - " + std::string(e.what()));
            throw ScoreException("File write error: " + std::string(e.what()));
        } catch (const std::exception& e) {
            Logger::error("Unexpected error in LocalScore::save: " + score_path + " - " + std::string(e.what()));
            throw ScoreException("Unexpected error: " + std::string(e.what()));
        }
    }

    // 添加成绩
    void LocalScore::addScore(const std::string& beatmap_id, const nlohmann::json& score_data) {
        if (!isLoaded()) {
            throw ScoreException("No score data loaded");
        }

        if (!this->score_data.contains("scores")) {
            this->score_data["scores"] = nlohmann::json::object();
        }

        if (!this->score_data["scores"].contains(beatmap_id)) {
            this->score_data["scores"][beatmap_id] = nlohmann::json::array();
        }

        // 添加时间戳
        nlohmann::json score_with_time = score_data;
        std::time_t now = std::time(nullptr);
        score_with_time["timestamp"] = now;

        // 添加到成绩列表
        this->score_data["scores"][beatmap_id].push_back(score_with_time);
    }

    // 获取成绩列表
    nlohmann::json LocalScore::getScores(const std::string& beatmap_id) const {
        if (!isLoaded()) {
            throw ScoreException("No score data loaded");
        }

        if (!score_data.contains("scores") || !score_data["scores"].contains(beatmap_id)) {
            return nlohmann::json::array();
        }

        return score_data["scores"][beatmap_id];
    }

    // 获取最佳成绩
    nlohmann::json LocalScore::getBestScore(const std::string& beatmap_id) const {
        nlohmann::json scores = getScores(beatmap_id);

        if (scores.empty() || !scores.is_array()) {
            return nlohmann::json();
        }

        // 查找最高分
        nlohmann::json best_score;
        double best_acc = -1.0;

        for (const auto& score : scores) {
            if (score.contains("acc") && score["acc"].is_number()) {
                double acc = score["acc"].get<double>();
                if (acc > best_acc) {
                    best_acc = acc;
                    best_score = score;
                }
            }
        }

        return best_score;
    }

    // 检查是否已加载数据
    bool LocalScore::isLoaded() const {
        return !score_data.is_null() && !score_data.empty();
    }
} // namespace FileManager
