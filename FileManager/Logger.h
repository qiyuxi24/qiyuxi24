//
// Created by 21272 on 2025/12/17.
//

#ifndef NOJ_SOLUTION_LOGGER_H
#define NOJ_SOLUTION_LOGGER_H

#include <string>
#include <fstream>
#include <mutex>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace FileManager {
    enum class LogLevel {
        INFO,
        ERROR
    };

    class Logger {
    private:
        static std::string log_file_path;
        static std::mutex log_mutex;
        static std::ofstream log_file;

        // 格式化时间戳
        static std::string formatTimestamp();

        // 写入日志
        static void writeLog(LogLevel level, const std::string& message);

    public:
        // 设置日志文件路径
        static void setLogFile(const std::string& log_path);

        // 记录信息日志
        static void info(const std::string& message);

        // 记录错误日志
        static void error(const std::string& message);

        // 关闭日志文件
        static void close();
    };
} // namespace FileManager

#endif //NOJ_SOLUTION_LOGGER_H
