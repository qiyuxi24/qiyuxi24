//
// Created by 21272 on 2025/12/17.
//

#include "User.h"
#include <filesystem>
#include <functional>
#include <sstream>
#include <iomanip>

namespace FileManager {
    // 计算密码哈希（简单实现，使用 std::hash）
    std::string User::hashPassword(const std::string& password) {
        std::hash<std::string> hasher;
        size_t hash_value = hasher(password);
        
        // 转换为十六进制字符串
        std::stringstream ss;
        ss << std::hex << hash_value;
        return ss.str();
    }

    // 构造函数实现
    User::User() : user_data(nlohmann::json::object()), username("") {
    }

    User::User(const std::string& username) : user_data(nlohmann::json::object()), username("") {
        load(username);
    }

    // 加载用户数据
    void User::load(const std::string& username) {
        this->username = username;

        std::filesystem::path user_dir = std::filesystem::path("users") / username;
        std::filesystem::path user_file = user_dir / "user.data";

        // 检查文件是否存在
        if (std::filesystem::exists(user_file)) {
            // 使用 JsonParser 解析文件
            user_data = Tools::JsonParser::parseFromFile(user_file.string());
        } else {
            // 文件不存在，创建默认用户数据
            user_data = nlohmann::json::object();
            user_data["username"] = username;
            user_data["scores"] = nlohmann::json::object();
        }
    }

    // 保存用户数据
    void User::save(const std::string& username) {
        std::string save_username = username.empty() ? this->username : username;

        if (save_username.empty()) {
            throw UserException("No username specified");
        }

        std::filesystem::path user_dir = std::filesystem::path("users") / save_username;
        std::filesystem::path user_file = user_dir / "user.data";

        // 确保目录存在
        std::filesystem::create_directories(user_dir);

        // 使用 JsonParser 保存文件
        Tools::JsonParser::saveToFile(user_data, user_file.string());
        this->username = save_username;
    }

    // 创建新用户
    void User::createUser(const std::string& username, const std::string& password) {
        if (username.empty()) {
            throw UserException("Username cannot be empty");
        }

        std::filesystem::path user_dir = std::filesystem::path("users") / username;
        std::filesystem::path user_file = user_dir / "user.data";

        // 检查用户是否已存在
        if (std::filesystem::exists(user_file)) {
            throw UserException("User already exists: " + username);
        }

        // 创建新用户数据
        user_data = nlohmann::json::object();
        user_data["username"] = username;
        user_data["password_hash"] = hashPassword(password);
        user_data["scores"] = nlohmann::json::object();

        this->username = username;
        save(username);
    }

    // 验证密码
    bool User::verifyPassword(const std::string& password) const {
        if (!isLoaded()) {
            throw UserException("No user loaded");
        }

        if (!user_data.contains("password_hash")) {
            return false;
        }

        std::string stored_hash = user_data["password_hash"].get<std::string>();
        std::string input_hash = hashPassword(password);

        return stored_hash == input_hash;
    }

    // 添加成绩
    void User::addScore(const std::string& beatmap_id, const nlohmann::json& score_data) {
        if (!isLoaded()) {
            throw UserException("No user loaded");
        }

        if (!user_data.contains("scores")) {
            user_data["scores"] = nlohmann::json::object();
        }

        if (!user_data["scores"].contains(beatmap_id)) {
            user_data["scores"][beatmap_id] = nlohmann::json::array();
        }

        // 添加时间戳
        nlohmann::json score_with_time = score_data;
        std::time_t now = std::time(nullptr);
        score_with_time["timestamp"] = now;

        // 添加到成绩列表
        user_data["scores"][beatmap_id].push_back(score_with_time);
    }

    // 获取成绩列表
    nlohmann::json User::getScores(const std::string& beatmap_id) const {
        if (!isLoaded()) {
            throw UserException("No user loaded");
        }

        if (!user_data.contains("scores") || !user_data["scores"].contains(beatmap_id)) {
            return nlohmann::json::array();
        }

        return user_data["scores"][beatmap_id];
    }

    // 获取最佳成绩
    nlohmann::json User::getBestScore(const std::string& beatmap_id) const {
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

    // 检查是否已加载用户
    bool User::isLoaded() const {
        return !user_data.is_null() && !username.empty();
    }

    // 获取用户名
    const std::string& User::getUsername() const {
        return username;
    }
} // namespace FileManager
