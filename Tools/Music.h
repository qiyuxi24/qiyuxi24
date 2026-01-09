//
// Created by Blokki on 2025/12/17.
//

#ifndef NOJ_SOLUTION_MUSIC_H
#define NOJ_SOLUTION_MUSIC_H

#include <string>
#include <chrono>
#include <SFML/Audio/Music.hpp>

namespace music {
    // 加载音乐文件并设置开始播放的时间点
    // file_path: 音乐文件路径
    // start_time: 开始播放的时间点（毫秒），从timer::current_timing开始计算
    // 返回是否加载成功
    bool loadAndPlay(const std::string& file_path, std::chrono::milliseconds start_time);
    
    // 停止播放音乐
    void stop();
    
    // 暂停播放音乐
    void pause();
    
    // 继续播放音乐
    void resume();
    
    // 获取音乐播放状态
    sf::Music::Status getStatus();
    
    // 获取当前播放位置（毫秒）
    std::chrono::milliseconds getPlayingOffset();
    
    // 设置音量 (0-100)
    void setVolume(float volume);
    
    // 获取音量
    float getVolume();
    
    // 清理资源（停止线程和音乐）
    void cleanup();
}

#endif //NOJ_SOLUTION_MUSIC_H

