<<<<<<< Updated upstream
//
// Created by 21272 on 2025/12/17.
//

#include "Logger.h"
#include <filesystem>
#include <iostream>

namespace FileManager {
    // 静态成员初始化
    std::string Logger::log_file_path = "logs/app.log";
    std::mutex Logger::log_mutex;
    std::ofstream Logger::log_file;

    // 格式化时间戳
    std::string Logger::formatTimestamp() {
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);

        std::stringstream ss;
        ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    // 写入日志
    void Logger::writeLog(LogLevel level, const std::string& message) {
        std::lock_guard<std::mutex> lock(log_mutex);

        // 确保 logs 目录存在
        std::filesystem::path log_path_obj(log_file_path);
        if (log_path_obj.has_parent_path()) {
            std::filesystem::create_directories(log_path_obj.parent_path());
        }

        // 打开日志文件（追加模式）
        if (!log_file.is_open()) {
            log_file.open(log_file_path, std::ios::app);
            if (!log_file.is_open()) {
                // 如果无法打开文件，输出到标准错误
                std::cerr << "[ERROR] Failed to open log file: " << log_file_path << std::endl;
                return;
            }
        }

        // 格式化日志级别字符串
        std::string level_str = (level == LogLevel::INFO) ? "INFO" : "ERROR";

        // 写入日志
        std::string timestamp = formatTimestamp();
        log_file << "[" << level_str << "] [" << timestamp << "] " << message << std::endl;
        log_file.flush();

        // 同时输出到控制台
        std::cout << "[" << level_str << "] [" << timestamp << "] " << message << std::endl;
    }

    // 设置日志文件路径
    void Logger::setLogFile(const std::string& log_path) {
        std::lock_guard<std::mutex> lock(log_mutex);

        // 关闭当前日志文件
        if (log_file.is_open()) {
            log_file.close();
        }

        log_file_path = log_path;
    }

    // 记录信息日志
    void Logger::info(const std::string& message) {
        writeLog(LogLevel::INFO, message);
    }

    // 记录错误日志
    void Logger::error(const std::string& message) {
        writeLog(LogLevel::ERROR, message);
    }

    // 关闭日志文件
    void Logger::close() {
        std::lock_guard<std::mutex> lock(log_mutex);
        if (log_file.is_open()) {
            log_file.close();
        }
    }
} // namespace FileManager
=======
//
// Created by 21272 on 2025/12/17.
//

#include "Logger.h"
#include "FileManager.h"
#include <filesystem>
#include <iostream>

namespace FileManager {
    // 静态成员初始化
    std::string Logger::log_file_path = "logs/app.log";
    std::mutex Logger::log_mutex;
    std::ofstream Logger::log_file;

    // 格式化时间戳
    std::string Logger::formatTimestamp() {
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);

        std::stringstream ss;
        ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    // 写入日志
    void Logger::writeLog(LogLevel level, const std::string& message) {
        std::lock_guard<std::mutex> lock(log_mutex);

        // 使用局部变量构建完整路径
        std::string full_log_path;
        std::filesystem::path log_path_obj(log_file_path);
        
        // 如果 log_file_path 是绝对路径，直接使用；否则基于 product_path 构建
        if (log_path_obj.is_absolute()) {
            full_log_path = log_file_path;
        } else {
            std::string product_path = getProductPath();
            std::filesystem::path product_path_obj(product_path);
            std::filesystem::path full_log_path_obj = product_path_obj / log_file_path;
            full_log_path = full_log_path_obj.string();
        }

        // 确保 logs 目录存在
        std::filesystem::path full_log_path_obj(full_log_path);
        if (full_log_path_obj.has_parent_path()) {
            std::filesystem::create_directories(full_log_path_obj.parent_path());
        }

        // 打开日志文件（追加模式）
        if (!log_file.is_open()) {
            log_file.open(full_log_path, std::ios::app);
            if (!log_file.is_open()) {
                // 如果无法打开文件，输出到标准错误
                std::cerr << "[ERROR] Failed to open log file: " << full_log_path << std::endl;
                return;
            }
        }

        // 格式化日志级别字符串
        std::string level_str = (level == LogLevel::INFO) ? "INFO" : "ERROR";

        // 写入日志
        std::string timestamp = formatTimestamp();
        log_file << "[" << level_str << "] [" << timestamp << "] " << message << std::endl;
        log_file.flush();

        // 同时输出到控制台
        std::cout << "[" << level_str << "] [" << timestamp << "] " << message << std::endl;
    }

    // 设置日志文件路径
    void Logger::setLogFile(const std::string& log_path) {
        std::lock_guard<std::mutex> lock(log_mutex);

        // 关闭当前日志文件
        if (log_file.is_open()) {
            log_file.close();
        }

        log_file_path = log_path;
    }

    // 记录信息日志
    void Logger::info(const std::string& message) {
        writeLog(LogLevel::INFO, message);
    }

    // 记录错误日志
    void Logger::error(const std::string& message) {
        writeLog(LogLevel::ERROR, message);
    }

    // 关闭日志文件
    void Logger::close() {
        std::lock_guard<std::mutex> lock(log_mutex);
        if (log_file.is_open()) {
            log_file.close();
        }
    }
} // namespace FileManager
>>>>>>> Stashed changes
