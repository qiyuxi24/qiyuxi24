//
// Created by 21272 on 2025/12/17.
//

#ifndef NOJ_SOLUTION_CONFIG_H
#define NOJ_SOLUTION_CONFIG_H

#include <string>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include "../Tools/JsonParser.h"

namespace FileManager {
    // 异常类定义
    class ConfigException : public std::runtime_error {
    public:
        explicit ConfigException(const std::string& message) : std::runtime_error(message) {}
    };

    class Config {
    private:
        nlohmann::json config_data;
        std::string config_path;

    public:
        // 构造函数
        Config();
        explicit Config(const std::string& config_path);

        // 加载配置
        void load(const std::string& config_path = "config.data");

        // 保存配置
        void save(const std::string& config_path = "");

        // 获取配置值
        nlohmann::json get(const std::string& key) const;

        // 设置配置值
        void set(const std::string& key, const nlohmann::json& value);

        // 检查配置项是否存在
        bool has(const std::string& key) const;

        // 检查是否已加载配置
        bool isLoaded() const;

        // 获取配置文件路径
        const std::string& getConfigPath() const;
    };
} // namespace FileManager

#endif //NOJ_SOLUTION_CONFIG_H
