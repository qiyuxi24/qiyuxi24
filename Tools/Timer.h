//
// Created by Blokki on 2025/12/17.
//

#ifndef NOJ_SOLUTION_TIMER_H
#define NOJ_SOLUTION_TIMER_H

#include <chrono>

namespace timer {
    // 当前时间变量（所有文件可见，只读）
    // 返回自计时器启动以来的毫秒数
    extern const std::chrono::milliseconds& current_timing;
    
    // 初始化计时器（启动后台线程）
    void initialize();
    
    // 停止计时器（停止后台线程）
    void shutdown();
}

#endif //NOJ_SOLUTION_TIMER_H

