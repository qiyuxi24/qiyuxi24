//
// Created by 21272 on 2025/12/17.
//

#include "JsonParser.h"
#include <fstream>
#include <sstream>

namespace Tools {
    // 从文件解析 JSON
    nlohmann::json JsonParser::parseFromFile(const std::string& file_path) {
        // 打开文件
        std::ifstream file(file_path);
        if (!file.is_open()) {
            throw FileNotFoundException(file_path);
        }

        try {
            // 读取文件内容
            std::stringstream buffer;
            buffer << file.rdbuf();
            file.close();

            // 调用 parseFromString 解析
            return parseFromString(buffer.str());
        } catch (const FileNotFoundException&) {
            // 重新抛出文件未找到异常
            throw;
        } catch (const ParseException&) {
            // 重新抛出解析异常
            throw;
        } catch (const std::ifstream::failure& e) {
            throw FileReadException(file_path + ": " + std::string(e.what()));
        } catch (const std::exception& e) {
            throw FileReadException("Unexpected error while reading file: " + std::string(e.what()));
        }
    }

    // 从字符串解析 JSON
    nlohmann::json JsonParser::parseFromString(const std::string& json_string) {
        try {
            return nlohmann::json::parse(json_string);
        } catch (const nlohmann::json::parse_error& e) {
            throw ParseException(e.what());
        } catch (const std::exception& e) {
            throw ParseException("Unexpected error: " + std::string(e.what()));
        }
    }

    // 保存 JSON 到文件
    void JsonParser::saveToFile(const nlohmann::json& json_data, const std::string& file_path, int indent) {
        try {
            std::ofstream file(file_path);
            if (!file.is_open()) {
                throw FileWriteException("Failed to open file for writing: " + file_path);
            }

            // 格式化输出 JSON
            file << json_data.dump(indent);
            file.close();

            // 检查写入是否成功
            if (file.fail()) {
                throw FileWriteException("Failed to write to file: " + file_path);
            }
        } catch (const FileWriteException&) {
            // 重新抛出写入异常
            throw;
        } catch (const std::ofstream::failure& e) {
            throw FileWriteException(file_path + ": " + std::string(e.what()));
        } catch (const std::exception& e) {
            throw FileWriteException("Unexpected error while writing file: " + std::string(e.what()));
        }
    }

    // 验证字符串是否为有效 JSON
    bool JsonParser::isValidJson(const std::string& json_string) {
        try {
            nlohmann::json::parse(json_string);
            return true;
        } catch (...) {
            return false;
        }
    }

    // 验证文件是否为有效 JSON
    bool JsonParser::isValidJsonFile(const std::string& file_path) {
        try {
            // 检查文件是否存在
            std::ifstream file(file_path);
            if (!file.is_open()) {
                return false;
            }

            // 读取文件内容
            std::stringstream buffer;
            buffer << file.rdbuf();
            file.close();

            // 尝试解析
            nlohmann::json::parse(buffer.str());
            return true;
        } catch (...) {
            return false;
        }
    }
} // namespace Tools
