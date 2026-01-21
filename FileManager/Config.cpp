<<<<<<< Updated upstream
//
// Created by 21272 on 2025/12/17.
//

#include "Config.h"
#include <filesystem>

namespace FileManager {
    // 构造函数实现
    Config::Config() : config_data(nlohmann::json::object()), config_path("config.data") {
    }

    Config::Config(const std::string& config_path) : config_data(nlohmann::json::object()), config_path(config_path) {
        load(config_path);
    }

    // 加载配置
    void Config::load(const std::string& config_path) {
        this->config_path = config_path;

        try {
            // 检查文件是否存在
            if (std::filesystem::exists(config_path)) {
                // 使用 JsonParser 解析文件
                config_data = Tools::JsonParser::parseFromFile(config_path);
            } else {
                // 文件不存在，创建默认配置
                config_data = nlohmann::json::object();
            }
        } catch (const Tools::ParseException& e) {
            throw ConfigException("Parse error: " + std::string(e.what()));
        } catch (const Tools::FileReadException& e) {
            throw ConfigException("File read error: " + std::string(e.what()));
        } catch (const std::exception& e) {
            throw ConfigException("Unexpected error: " + std::string(e.what()));
        }
    }

    // 保存配置
    void Config::save(const std::string& config_path) {
        std::string save_path = config_path.empty() ? this->config_path : config_path;

        if (save_path.empty()) {
            throw ConfigException("No config path specified");
        }

        try {
            // 确保目录存在
            std::filesystem::path path_obj(save_path);
            if (path_obj.has_parent_path()) {
                std::filesystem::create_directories(path_obj.parent_path());
            }

            // 使用 JsonParser 保存文件
            Tools::JsonParser::saveToFile(config_data, save_path);
            this->config_path = save_path;
        } catch (const Tools::FileWriteException& e) {
            throw ConfigException("File write error: " + std::string(e.what()));
        } catch (const std::exception& e) {
            throw ConfigException("Unexpected error: " + std::string(e.what()));
        }
    }

    // 获取配置值
    nlohmann::json Config::get(const std::string& key) const {
        if (!isLoaded()) {
            throw ConfigException("No config loaded");
        }

        if (!config_data.contains(key)) {
            throw ConfigException("Config key not found: " + key);
        }

        return config_data[key];
    }

    // 设置配置值
    void Config::set(const std::string& key, const nlohmann::json& value) {
        if (!isLoaded()) {
            throw ConfigException("No config loaded");
        }

        config_data[key] = value;
    }

    // 检查配置项是否存在
    bool Config::has(const std::string& key) const {
        if (!isLoaded()) {
            return false;
        }

        return config_data.contains(key);
    }

    // 检查是否已加载配置
    bool Config::isLoaded() const {
        return !config_data.is_null();
    }

    // 获取配置文件路径
    const std::string& Config::getConfigPath() const {
        return config_path;
    }
} // namespace FileManager
=======
//
// Created by 21272 on 2025/12/17.
//

#include "Config.h"
#include "FileManager.h"
#include "Logger.h"
#include <filesystem>

namespace FileManager {
    // 构造函数实现
    Config::Config() : config_data(nlohmann::json::object()), config_path("") {
    }

    // 加载配置
    void Config::load() {
        // 使用局部变量构建完整路径
        std::string product_path = getProductPath();
        std::filesystem::path product_path_obj(product_path);
        std::filesystem::path config_path_obj = product_path_obj / "config.data";
        std::string full_config_path = config_path_obj.string();
        
        this->config_path = full_config_path;

        try {
            // 检查文件是否存在
            if (std::filesystem::exists(full_config_path)) {
                // 使用 JsonParser 解析文件
                config_data = Tools::JsonParser::parseFromFile(full_config_path);
            } else {
                // 文件不存在，创建默认配置
                config_data = nlohmann::json::object();
            }
        } catch (const Tools::ParseException& e) {
            Logger::error("Config parse error: " + full_config_path + " - " + std::string(e.what()));
            throw ConfigException("Parse error: " + std::string(e.what()));
        } catch (const Tools::FileReadException& e) {
            Logger::error("Config file read error: " + full_config_path + " - " + std::string(e.what()));
            throw ConfigException("File read error: " + std::string(e.what()));
        } catch (const std::exception& e) {
            Logger::error("Unexpected error in Config::load: " + full_config_path + " - " + std::string(e.what()));
            throw ConfigException("Unexpected error: " + std::string(e.what()));
        }
    }

    // 保存配置
    void Config::save() {
        // 使用局部变量构建完整路径
        std::string save_path;
        if (!config_path.empty()) {
            save_path = config_path;
        } else {
            std::string product_path = getProductPath();
            std::filesystem::path product_path_obj(product_path);
            std::filesystem::path config_path_obj = product_path_obj / "config.data";
            save_path = config_path_obj.string();
        }

        try {
            // 确保目录存在
            std::filesystem::path path_obj(save_path);
            if (path_obj.has_parent_path()) {
                std::filesystem::create_directories(path_obj.parent_path());
            }

            // 使用 JsonParser 保存文件
            Tools::JsonParser::saveToFile(config_data, save_path);
            this->config_path = save_path;
        } catch (const Tools::FileWriteException& e) {
            Logger::error("Config file write error: " + save_path + " - " + std::string(e.what()));
            throw ConfigException("File write error: " + std::string(e.what()));
        } catch (const std::exception& e) {
            Logger::error("Unexpected error in Config::save: " + save_path + " - " + std::string(e.what()));
            throw ConfigException("Unexpected error: " + std::string(e.what()));
        }
    }

    // 获取配置值
    nlohmann::json Config::get(const std::string& key) const {
        if (!isLoaded()) {
            throw ConfigException("No config loaded");
        }

        if (!config_data.contains(key)) {
            throw ConfigException("Config key not found: " + key);
        }

        return config_data[key];
    }

    // 设置配置值
    void Config::set(const std::string& key, const nlohmann::json& value) {
        if (!isLoaded()) {
            throw ConfigException("No config loaded");
        }

        config_data[key] = value;
    }

    // 检查配置项是否存在
    bool Config::has(const std::string& key) const {
        if (!isLoaded()) {
            return false;
        }

        return config_data.contains(key);
    }

    // 检查是否已加载配置
    bool Config::isLoaded() const {
        return !config_data.is_null();
    }

    // 获取配置文件路径
    const std::string& Config::getConfigPath() const {
        return config_path;
    }
} // namespace FileManager
>>>>>>> Stashed changes
