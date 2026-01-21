//
// Created by 21272 on 2025/12/17.
//

#ifndef NOJ_SOLUTION_LOCALSCORE_H
#define NOJ_SOLUTION_LOCALSCORE_H

#include <string>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include "../Tools/JsonParser.h"

namespace FileManager {
    // 异常类定义
    class ScoreException : public std::runtime_error {
    public:
        explicit ScoreException(const std::string& message) : std::runtime_error(message) {}
    };

    class LocalScore {
    private:
        nlohmann::json score_data;

    public:
        // 构造函数
        LocalScore();

        // 加载成绩数据
        void load();

        // 保存成绩数据
        void save();

        // 添加成绩
        void addScore(const std::string& beatmap_id, const nlohmann::json& score_data);

        // 获取成绩列表
        nlohmann::json getScores(const std::string& beatmap_id) const;

        // 获取最佳成绩
        nlohmann::json getBestScore(const std::string& beatmap_id) const;

        // 检查是否已加载数据
        bool isLoaded() const;
    };
} // namespace FileManager

#endif //NOJ_SOLUTION_LOCALSCORE_H
