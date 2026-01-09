//
// Created by 21272 on 2025/12/17.
//

#ifndef NOJ_SOLUTION_JSONPARSER_H
#define NOJ_SOLUTION_JSONPARSER_H

#include <string>
#include <stdexcept>
#include <nlohmann/json.hpp>

namespace Tools {
    // 异常类定义
    class JsonParserException : public std::runtime_error {
    public:
        explicit JsonParserException(const std::string& message) : std::runtime_error(message) {}
    };

    class FileNotFoundException : public JsonParserException {
    public:
        explicit FileNotFoundException(const std::string& file_path)
            : JsonParserException("File not found: " + file_path) {}
    };

    class ParseException : public JsonParserException {
    public:
        explicit ParseException(const std::string& message)
            : JsonParserException("Parse error: " + message) {}
    };

    class FileReadException : public JsonParserException {
    public:
        explicit FileReadException(const std::string& message)
            : JsonParserException("File read error: " + message) {}
    };

    class FileWriteException : public JsonParserException {
    public:
        explicit FileWriteException(const std::string& message)
            : JsonParserException("File write error: " + message) {}
    };

    class JsonParser {
    public:
        // 从文件解析 JSON
        static nlohmann::json parseFromFile(const std::string& file_path);

        // 从字符串解析 JSON
        static nlohmann::json parseFromString(const std::string& json_string);

        // 保存 JSON 到文件
        static void saveToFile(const nlohmann::json& json_data, const std::string& file_path, int indent = 4);

        // 验证字符串是否为有效 JSON
        static bool isValidJson(const std::string& json_string);

        // 验证文件是否为有效 JSON
        static bool isValidJsonFile(const std::string& file_path);
    };
} // namespace Tools

#endif //NOJ_SOLUTION_JSONPARSER_H
