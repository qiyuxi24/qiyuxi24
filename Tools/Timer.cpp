//
// Created by Blokki on 2025/12/17.
//

#include "Timer.h"
#include <thread>
#include <atomic>
#include <mutex>

namespace timer {
    // 内部变量：当前时间（毫秒）
    static std::chrono::milliseconds internal_current_timing(0);
    
    // 对外暴露的只读引用
    const std::chrono::milliseconds& current_timing = internal_current_timing;
    
    // 线程控制
    static std::atomic<bool> running(false);
    static std::thread timer_thread;
    static std::mutex timing_mutex;
    
    // 时间更新线程函数
    void updateTimingThread() {
        auto start_time = std::chrono::steady_clock::now();
        
        while (running.load()) {
            // 计算自启动以来的时间
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                now - start_time
            );
            
            // 更新internal_current_timing（需要加锁保护）
            {
                std::lock_guard<std::mutex> lock(timing_mutex);
                internal_current_timing = elapsed;
            }
            
            // 每1毫秒更新一次（高精度）
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    
    // 初始化计时器（启动后台线程）
    void initialize() {
        if (!running.load()) {
            running.store(true);
            {
                std::lock_guard<std::mutex> lock(timing_mutex);
                internal_current_timing = std::chrono::milliseconds(0);
            }
            timer_thread = std::thread(updateTimingThread);
        }
    }
    
    // 停止计时器（停止后台线程）
    void shutdown() {
        if (running.load()) {
            running.store(false);
            if (timer_thread.joinable()) {
                timer_thread.join();
            }
        }
    }
}

