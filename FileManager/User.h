//
// Created by 21272 on 2025/12/17.
//

#ifndef NOJ_SOLUTION_USER_H
#define NOJ_SOLUTION_USER_H

#include <string>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include "../Tools/JsonParser.h"

namespace FileManager {
    // 异常类定义
    class UserException : public std::runtime_error {
    public:
        explicit UserException(const std::string& message) : std::runtime_error(message) {}
    };

    class User {
    private:
        nlohmann::json user_data;
        std::string username;

        // 计算密码哈希（简单实现）
        static std::string hashPassword(const std::string& password);

    public:
        // 构造函数
        User();
        explicit User(const std::string& username);

        // 加载用户数据
        void load(const std::string& username);

        // 保存用户数据
        void save(const std::string& username = "");

        // 创建新用户
        void createUser(const std::string& username, const std::string& password);

        // 验证密码
        bool verifyPassword(const std::string& password) const;

        // 添加成绩
        void addScore(const std::string& beatmap_id, const nlohmann::json& score_data);

        // 获取成绩列表
        nlohmann::json getScores(const std::string& beatmap_id) const;

        // 获取最佳成绩
        nlohmann::json getBestScore(const std::string& beatmap_id) const;

        // 检查是否已加载用户
        bool isLoaded() const;

        // 获取用户名
        const std::string& getUsername() const;
    };
} // namespace FileManager

#endif //NOJ_SOLUTION_USER_H
