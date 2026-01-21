//
// Created by Blokki on 2025/12/17.
//

#include "Music.h"
#include "Timer.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

namespace music {
    // 音乐对象
    static sf::Music music_instance;
    
    // 线程控制
    static std::atomic<bool> running(false);
    static std::thread music_thread;
    static std::mutex music_mutex;
    static std::condition_variable cv;
    
    // 播放控制
    static std::atomic<bool> should_play(false);
    static std::chrono::milliseconds start_time_point(0);  // 受music_mutex保护
    static std::atomic<bool> music_loaded(false);
    
    // 音乐播放线程函数
    void musicPlayThread() {
        while (running.load()) {
            std::unique_lock<std::mutex> lock(music_mutex);
            
            // 等待播放信号或线程停止信号
            cv.wait(lock, []() {
                return should_play.load() || !running.load();
            });
            
            if (!running.load()) {
                break;
            }
            
            if (should_play.load() && music_loaded.load()) {
                // 在锁内读取start_time_point
                auto target_time = start_time_point;
                lock.unlock();
                
                // 等待到指定的时间点
                while (running.load() && should_play.load()) {
                    // 从timer读取当前时间
                    auto current_time = timer::current_timing;
                    
                    // 如果当前时间达到或超过开始播放的时间点，开始播放
                    if (current_time >= target_time) {
                        std::lock_guard<std::mutex> music_lock(music_mutex);
                        if (music_instance.getStatus() != sf::Music::Playing) {
                            music_instance.play();
                        }
                        break;
                    }
                    
                    // 短暂休眠，避免CPU占用过高
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
            }
        }
    }
    
    // 加载音乐文件并设置开始播放的时间点
    // 参数：file_path - 音乐文件的地址（路径）
    // 参数：start_time - 开始播放的时间点
    bool loadAndPlay(const std::string& file_path, std::chrono::milliseconds start_time) {
        std::lock_guard<std::mutex> lock(music_mutex);
        
        // 如果之前有音乐在播放，先停止
        if (music_instance.getStatus() == sf::Music::Playing) {
            music_instance.stop();
        }
        
        // 加载音乐文件 - 这里使用传入的file_path作为音乐地址
        if (!music_instance.openFromFile(file_path)) { // file_path是音乐文件的地址
            music_loaded.store(false);
            return false;
        }
        
        music_loaded.store(true);
        start_time_point = start_time;
        should_play.store(true);
        
        // 如果线程还没启动，启动线程
        if (!running.load()) {
            running.store(true);
            music_thread = std::thread(musicPlayThread);
        }
        
        // 通知线程开始工作
        cv.notify_one();
        
        return true;
    }
    
    // 停止播放音乐
    void stop() {
        std::lock_guard<std::mutex> lock(music_mutex);
        should_play.store(false);
        music_instance.stop();
        cv.notify_one();
    }
    
<<<<<<< Updated upstream
    // 暂停播放音乐
    void pause() {
        std::lock_guard<std::mutex> lock(music_mutex);
        music_instance.pause();
    }
    
    // 继续播放音乐
    void resume() {
        std::lock_guard<std::mutex> lock(music_mutex);
        if (music_loaded.load() && music_instance.getStatus() == sf::Music::Paused) {
            music_instance.play();
        }
    }
    
=======
>>>>>>> Stashed changes
    // 获取音乐播放状态
    sf::Music::Status getStatus() {
        std::lock_guard<std::mutex> lock(music_mutex);
        return music_instance.getStatus();
    }
    
    // 获取当前播放位置（毫秒）
    std::chrono::milliseconds getPlayingOffset() {
        std::lock_guard<std::mutex> lock(music_mutex);
        if (music_loaded.load()) {
            auto offset = music_instance.getPlayingOffset();
            return std::chrono::milliseconds(offset.asMilliseconds());
        }
        return std::chrono::milliseconds(0);
    }
    
<<<<<<< Updated upstream
=======
    // 获取音乐总时长（毫秒）
    std::chrono::milliseconds getDuration() {
        std::lock_guard<std::mutex> lock(music_mutex);
        if (music_loaded.load()) {
            auto duration = music_instance.getDuration();
            return std::chrono::milliseconds(duration.asMilliseconds());
        }
        return std::chrono::milliseconds(0);
    }
    
>>>>>>> Stashed changes
    // 设置音量 (0-100)
    void setVolume(float volume) {
        std::lock_guard<std::mutex> lock(music_mutex);
        if (volume < 0.0f) volume = 0.0f;
        if (volume > 100.0f) volume = 100.0f;
        music_instance.setVolume(volume);
    }
    
    // 获取音量
    float getVolume() {
        std::lock_guard<std::mutex> lock(music_mutex);
        return music_instance.getVolume();
    }
    
    // 清理资源（停止线程和音乐）
    void cleanup() {
        {
            std::lock_guard<std::mutex> lock(music_mutex);
            should_play.store(false);
            music_instance.stop();
            running.store(false);
            cv.notify_one();
        }
        
        if (music_thread.joinable()) {
            music_thread.join();
        }
    }
}

