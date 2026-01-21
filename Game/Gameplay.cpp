//
// Created by Blokki on 2025/12/17.
//

#include "Gameplay.h"
#include "../Tools/Timer.h"
<<<<<<< Updated upstream
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <string>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
=======
#include "../Tools/Music.h"
#include "../FileManager/Beatmap.h"
#include "../FileManager/Config.h"
#include "../FileManager/Logger.h"
#include "../Menu/select.h"  // 用于访问全局变量 chart_path
#include <vector>
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
>>>>>>> Stashed changes

namespace Game {
    // Constants
    static const unsigned int FRAME_RATE = 60;
    static const unsigned int NUM_TRACKS = 4;
    static const float TRACK_MARGIN_RATIO = 0.025f;  // 2.5% margin on each side
    static const float JUDGE_LINE_Y_RATIO = 0.85f;  // Judge line at 85% from top
    static const float JUDGE_LINE_HEIGHT = 3.0f;     // 3 pixels thick
<<<<<<< Updated upstream
    
    // Judgment thresholds (in milliseconds)
    static const float PERFECT_THRESHOLD = 20.0f;
    static const float GOOD_THRESHOLD = 40.0f;
    static const float MISS_THRESHOLD = 100.0f;
=======
    static const float DEFAULT_NOTE_SPEED = 1.5f;    // 默认音符速度（像素/毫秒）
    
    // Judgment thresholds (in milliseconds)
    static const float PERFECT_THRESHOLD = 40.0f;
    static const float GOOD_THRESHOLD = 80.0f;
    static const float MISS_THRESHOLD = 200.0f;
>>>>>>> Stashed changes
    
    // Colors
    static const sf::Color TRACK_COLOR(30, 30, 30);           // Dark gray for tracks
    static const sf::Color TRACK_BORDER_COLOR(60, 60, 60);    // Lighter gray for borders
    static const sf::Color JUDGE_LINE_COLOR(255, 255, 255);   // White for judge line
    static const sf::Color BACKGROUND_COLOR(20, 20, 20);      // Very dark background
    static const sf::Color NOTE_COLOR(100, 150, 255);         // Blue color for notes
    
    // Judgment display structure
    struct JudgmentDisplay {
        int judgment_type;  // 0=Perfect, 1=Good, 2=Miss, -1=None
        float start_time_ms;
        float fade_duration_ms;  // 1000ms = 1 second
        sf::Text text;
        sf::Color base_color;  // Base color without alpha (RGB only)
        bool is_active;
        
        JudgmentDisplay() : judgment_type(-1), start_time_ms(0.0f), 
                          fade_duration_ms(1000.0f), is_active(false) {}
    };
    
    // Note structure with lifecycle management
    struct ActiveNote {
        unsigned int track_index;
        float pixels_per_millisecond;
        float length;
        float spawn_time_ms;  // Time when bottom edge enters screen (y = -length)
        
        // Calculate bottom edge Y position at given time
        float getBottomY(float current_time_ms) const {
            float elapsed = current_time_ms - spawn_time_ms;
            return elapsed * pixels_per_millisecond;
        }
        
        // Calculate top edge Y position at given time
        float getTopY(float current_time_ms) const {
            return getBottomY(current_time_ms) - length;
        }
        
        // Check if note should be created (bottom edge just entered screen)
        bool shouldSpawn(float current_time_ms, float window_height) const {
            float bottom_y = getBottomY(current_time_ms);
            return bottom_y >= -length && bottom_y < 0;
        }
        
        // Check if note should be destroyed (top edge left screen)
        bool shouldDestroy(float current_time_ms, float window_height) const {
            float top_y = getTopY(current_time_ms);
            return top_y > window_height;
        }
        
        // Check if note is on screen
        bool isOnScreen(float current_time_ms, float window_height) const {
            float bottom_y = getBottomY(current_time_ms);
            float top_y = getTopY(current_time_ms);
            return bottom_y >= 0 && top_y <= window_height;
        }
        
        // Calculate when bottom edge will reach judge line
        float getJudgeTime(float judge_line_y) const {
            // Bottom edge starts at y = -length, needs to reach judge_line_y
            float distance = judge_line_y + length;
            return spawn_time_ms + (distance / pixels_per_millisecond);
        }
    };
    
    // Function to create a note rectangle that fills a track
    // Note: Position is based on bottom edge starting from top of screen
    sf::RectangleShape createTrackNote(
        const ActiveNote& note,
        float current_time_ms,
        float start_x,
        float track_width
    ) {
        // Calculate bottom edge Y position
        float bottom_y = note.getBottomY(current_time_ms);
        float top_y = bottom_y - note.length;
        
        // Calculate X position based on track index
        float note_x = start_x + note.track_index * track_width;
        
        // Create rectangle that fills the track width
        sf::RectangleShape note_shape;
        note_shape.setSize(sf::Vector2f(track_width, note.length));
        note_shape.setPosition(note_x, top_y);
        note_shape.setFillColor(NOTE_COLOR);
        
        return note_shape;
    }
    
    // Judgment function: detects key press and calculates time difference
    // Returns: 0=Perfect, 1=Good, 2=Miss, -1=None
    int checkJudgment(
        const ActiveNote& note,
        float key_press_time_ms,
        float judge_line_y,
        float pixels_per_millisecond
    ) {
        // Calculate expected time when bottom edge reaches judge line
        float expected_time_ms = note.getJudgeTime(judge_line_y);
        
        // Calculate time difference (positive = pressed too early, negative = pressed too late)
        float time_diff_ms = std::abs(key_press_time_ms - expected_time_ms);
        
        // Check if within judgment window
        if (time_diff_ms <= PERFECT_THRESHOLD) {
            return 0;  // Perfect
        } else if (time_diff_ms <= GOOD_THRESHOLD) {
            return 1;  // Good
        } else if (time_diff_ms <= MISS_THRESHOLD) {
            return 2;  // Miss
        }
        
        return -1;  // None
    }
    
    // Display judgment result function
    // Parameters: judgment_type (0=Perfect, 1=Good, 2=Miss)
    void showJudgment(
        JudgmentDisplay& display,
        int judgment_type,
        float current_time_ms,
        float judge_line_y,
        float window_width,
        sf::Font& font
    ) {
        // Clear previous display
        display.is_active = false;
        display.judgment_type = -1;
        
        // Set new judgment
        if (judgment_type >= 0 && judgment_type <= 2) {
            display.judgment_type = judgment_type;
            display.start_time_ms = current_time_ms;
            display.is_active = true;
            
            // Set text content and color based on judgment type
            std::string text_str;
            switch (judgment_type) {
                case 0:  // Perfect
                    text_str = "PERFECT";
                    display.base_color = sf::Color(255, 215, 0);  // Gold
                    break;
                case 1:  // Good
                    text_str = "GOOD";
                    display.base_color = sf::Color(50, 205, 50);   // Green
                    break;
                case 2:  // Miss
                    text_str = "MISS";
                    display.base_color = sf::Color(255, 69, 0);   // Red-Orange
                    break;
                default:
                    return;
            }
            
            // Set up text
            display.text.setString(text_str);
            display.text.setFont(font);
            display.text.setCharacterSize(80);  // Large font size
            
            // Center the text horizontally
            sf::FloatRect text_bounds = display.text.getLocalBounds();
            display.text.setOrigin(text_bounds.left + text_bounds.width / 2.0f,
                                  text_bounds.top + text_bounds.height / 2.0f);
            display.text.setPosition(window_width / 2.0f, judge_line_y - 80.0f);  // Above judge line
            
<<<<<<< Updated upstream
            // Set initial opacity to 50% (127/255)
            sf::Color initial_color = display.base_color;
            initial_color.a = 127;  // 50% opacity
=======
            // Set initial opacity to 100% (255/255) for maximum visibility
            sf::Color initial_color = display.base_color;
            initial_color.a = 255;  // 100% opacity
>>>>>>> Stashed changes
            display.text.setFillColor(initial_color);
        }
    }
    
    // Update judgment display (fade out effect)
    void updateJudgmentDisplay(
        JudgmentDisplay& display,
        float current_time_ms
    ) {
        if (!display.is_active || display.judgment_type < 0) {
            return;
        }
        
        // Calculate elapsed time
        float elapsed_ms = current_time_ms - display.start_time_ms;
        
        // Check if fade duration has passed
        if (elapsed_ms >= display.fade_duration_ms) {
            display.is_active = false;
            display.judgment_type = -1;
            return;
        }
        
<<<<<<< Updated upstream
        // Calculate opacity: start at 50% (127), fade to 100% transparent (0)
        // Linear interpolation: opacity = 127 * (1 - elapsed / duration)
        float opacity_ratio = elapsed_ms / display.fade_duration_ms;
        sf::Uint8 alpha = static_cast<sf::Uint8>(127 * (1.0f - opacity_ratio));
=======
        // Calculate opacity: start at 100% (255), fade to 100% transparent (0)
        // Linear interpolation: opacity = 255 * (1 - elapsed / duration)
        float opacity_ratio = elapsed_ms / display.fade_duration_ms;
        sf::Uint8 alpha = static_cast<sf::Uint8>(255 * (1.0f - opacity_ratio));
>>>>>>> Stashed changes
        
        // Clamp alpha to valid range [0, 255]
        if (alpha > 255) alpha = 255;
        if (opacity_ratio > 1.0f) alpha = 0;
        
        // Update text color with new opacity (preserve RGB, update alpha)
        sf::Color current_color = display.base_color;
        current_color.a = alpha;
        display.text.setFillColor(current_color);
    }
<<<<<<< Updated upstream
    
    void Gameplay() {
=======

    // 音符模板结构体
    struct NoteTemplate {
        unsigned int track_index;
        float pixels_per_millisecond;
        float length;
        float spawn_time_ms;  // When bottom edge should enter screen
        float judge_time_ms;  // When bottom edge should reach judge line (relative to music start)
    };
    
    // 对象池类：管理音符模板
    class NoteTemplatePool {
    private:
        std::vector<NoteTemplate> pool;
        
    public:
        NoteTemplatePool() {}
        
        // 清空对象池
        void clear() {
            pool.clear();
        }
        
        // 添加音符模板到对象池
        void addTemplate(const NoteTemplate& template_note) {
            pool.push_back(template_note);
        }
        
        // 批量添加音符模板
        void addTemplates(const std::vector<NoteTemplate>& templates) {
            pool.insert(pool.end(), templates.begin(), templates.end());
        }
        
        // 获取所有模板（用于遍历）
        const std::vector<NoteTemplate>& getTemplates() const {
            return pool;
        }
        
        // 获取模板数量
        size_t size() const {
            return pool.size();
        }
        
        // 按生成时间排序（用于优化查找）
        void sortBySpawnTime() {
            std::sort(pool.begin(), pool.end(), 
                [](const NoteTemplate& a, const NoteTemplate& b) {
                    return a.spawn_time_ms < b.spawn_time_ms;
                });
        }
        
        // 调整所有音符的生成时间（用于时间同步）
        void adjustSpawnTime(float offset_ms) {
            for (auto& template_note : pool) {
                template_note.spawn_time_ms += offset_ms;
            }
        }
    };
    
    // 从谱面加载音符数据到对象池
    void playNotesFromBeatmap(const std::string& chart_path, NoteTemplatePool& pool, float window_height) {
        try {
            // 加载谱面
            FileManager::Beatmap beatmap(chart_path);
            
            if (!beatmap.isLoaded()) {
                throw std::runtime_error("Failed to load beatmap");
            }
            
            // 获取元数据
            const auto& meta = beatmap.getMetadata();
            const auto& song = meta.at("song");
            
            // 获取时间数据（用于处理BPM变化和获取BPM）
            const auto& time_data = beatmap.getTimeData();
            
            // 获取音符数据
            const auto& note_data = beatmap.getNoteData();
            
            // 获取 BPM：优先从 time[0].bpm 获取，如果不存在则从 meta.song.bpm 获取
            double bpm = 120.0; // 默认 BPM
            if (time_data.is_array() && !time_data.empty() && time_data[0].contains("bpm") && time_data[0]["bpm"].is_number()) {
                bpm = time_data[0]["bpm"].get<double>();
            } else if (song.contains("bpm") && song["bpm"].is_number()) {
                bpm = song["bpm"].get<double>();
            }
            
            // 从配置读取音符速度，默认值为 DEFAULT_NOTE_SPEED
            float base_note_speed = DEFAULT_NOTE_SPEED;
            try {
                FileManager::Config config;
                config.load();
                if (config.has("base_note_speed") && config.get("base_note_speed").is_number()) {
                    base_note_speed = config.get("base_note_speed").get<float>();
                }
            } catch (...) {
                // 如果读取配置失败，使用默认值
            }
            
            // 计算基础速度（像素/毫秒）
            // 假设音符从屏幕顶部到底部需要一定时间
            // 这里使用一个合理的默认速度，可以根据BPM调整
            float base_speed = base_note_speed;  // 默认速度，可以根据BPM和窗口高度调整
            if (bpm > 0 && window_height > 0) {
                // 根据BPM计算速度：BPM越高，速度越快
                // 假设在60BPM时速度为base_note_speed，按比例调整
                base_speed = base_note_speed * (bpm / 60.0f);
            }
            
            // 清空对象池
            pool.clear();
            
            // 解析音符数据 - Malody 格式: [{"beat":[x,y,z],"column":c}, ...]
            // beat 时间计算: x + y/z 节拍
            if (note_data.is_array()) {
                for (const auto& note : note_data) {
                    try {
                        NoteTemplate template_note;
                        
                        // Malody 格式: {"beat": [x, y, z], "column": 列索引, "endbeat": [结束节拍数组]}
                        if (!note.contains("beat") || !note.contains("column")) {
                            continue; // 跳过不符合格式的音符
                        }
                        
                        unsigned int column = note.at("column").get<unsigned int>();
                        
                        // 将 beat 数组转换为总节拍数
                        auto beat_array = note.at("beat");
                        if (!beat_array.is_array() || beat_array.size() < 3) {
                            continue; // 跳过无效的 beat 数据
                        }
                        
                        int x = beat_array[0].get<int>();
                        int y = beat_array[1].get<int>();
                        int z = beat_array[2].get<int>();
                        
                        // 转换为总节拍数：x + y/z
                        double total_beats = x + static_cast<double>(y) / static_cast<double>(z);
                        
                        // 计算结束节拍
                        bool has_endbeat = false;
                        double end_total_beats = total_beats;
                        if (note.contains("endbeat") && note["endbeat"].is_array()) {
                            auto endbeat_array = note["endbeat"];
                            if (endbeat_array.size() >= 3) {
                                int end_x = endbeat_array[0].get<int>();
                                int end_y = endbeat_array[1].get<int>();
                                int end_z = endbeat_array[2].get<int>();
                                end_total_beats = end_x + static_cast<double>(end_y) / static_cast<double>(end_z);
                                has_endbeat = true;
                            }
                        }
                        
                        // 将节拍转换为时间（毫秒）
                        double note_time_ms = (total_beats / bpm) * 60000.0;  // 节拍转毫秒
                        
                        // 计算音符长度（像素）
                        double note_length_pixels;
                        if (has_endbeat) {
                            double end_time_ms = (end_total_beats / bpm) * 60000.0;
                            double note_length_ms = end_time_ms - note_time_ms;
                            note_length_pixels = note_length_ms * static_cast<double>(base_speed);
                        } else {
                            // 如果没有 endbeat，使用默认长度 100.0f 像素
                            note_length_pixels = 100.0;
                        }
                        
                        // 计算生成时间
                        double judge_line_y = static_cast<double>(window_height) * JUDGE_LINE_Y_RATIO;
                        double distance_to_judge = judge_line_y + note_length_pixels;
                        double spawn_time_ms = note_time_ms - (distance_to_judge / static_cast<double>(base_speed));
                        
                        template_note.track_index = column % NUM_TRACKS;
                        template_note.pixels_per_millisecond = base_speed;
                        template_note.length = static_cast<float>(note_length_pixels);
                        template_note.spawn_time_ms = static_cast<float>(spawn_time_ms);
                        template_note.judge_time_ms = static_cast<float>(note_time_ms);  // 保存判定时间（相对于音乐开始）
                        
                        pool.addTemplate(template_note);
                    } catch (const std::exception& e) {
                        // 跳过无法解析的音符，继续处理下一个
                        FileManager::Logger::error("Failed to parse note in playNotesFromBeatmap: " + std::string(e.what()));
                        continue;
                    }
                }
            }
            
            // 按生成时间排序，优化后续查找
            pool.sortBySpawnTime();
            
        } catch (const FileManager::BeatmapException& e) {
            // 如果加载失败，使用默认测试音符
            FileManager::Logger::error("Failed to load beatmap in playNotesFromBeatmap: " + std::string(e.what()) + ". Using default test notes.");
            pool.clear();
            // 计算默认测试音符的判定时间（假设判定时间 = spawn_time + 距离/速度）
            double judge_line_y = static_cast<double>(window_height) * JUDGE_LINE_Y_RATIO;
            double distance_to_judge = judge_line_y + 100.0;
            double judge_time_offset = distance_to_judge / static_cast<double>(DEFAULT_NOTE_SPEED);
            pool.addTemplate({0, DEFAULT_NOTE_SPEED, 100.0f, 1000.0f, 1000.0f + static_cast<float>(judge_time_offset)});
            pool.addTemplate({1, DEFAULT_NOTE_SPEED, 100.0f, 2000.0f, 2000.0f + static_cast<float>(judge_time_offset)});
            pool.addTemplate({2, DEFAULT_NOTE_SPEED, 100.0f, 3000.0f, 3000.0f + static_cast<float>(judge_time_offset)});
            pool.addTemplate({3, DEFAULT_NOTE_SPEED, 100.0f, 4000.0f, 4000.0f + static_cast<float>(judge_time_offset)});
        } catch (const std::exception& e) {
            // 处理其他异常
            FileManager::Logger::error("Unexpected error in playNotesFromBeatmap: " + std::string(e.what()) + ". Using default test notes.");
            pool.clear();
            // 计算默认测试音符的判定时间（假设判定时间 = spawn_time + 距离/速度）
            double judge_line_y = static_cast<double>(window_height) * JUDGE_LINE_Y_RATIO;
            double distance_to_judge = judge_line_y + 100.0;
            double judge_time_offset = distance_to_judge / static_cast<double>(DEFAULT_NOTE_SPEED);
            pool.addTemplate({0, DEFAULT_NOTE_SPEED, 100.0f, 1000.0f, 1000.0f + static_cast<float>(judge_time_offset)});
            pool.addTemplate({1, DEFAULT_NOTE_SPEED, 100.0f, 2000.0f, 2000.0f + static_cast<float>(judge_time_offset)});
            pool.addTemplate({2, DEFAULT_NOTE_SPEED, 100.0f, 3000.0f, 3000.0f + static_cast<float>(judge_time_offset)});
            pool.addTemplate({3, DEFAULT_NOTE_SPEED, 100.0f, 4000.0f, 4000.0f + static_cast<float>(judge_time_offset)});
        }
    }

    void Gameplay(const std::string& chart_path_param) {
>>>>>>> Stashed changes
        // Initialize timer
        timer::initialize();
        
        // Get desktop video mode for fullscreen
        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        unsigned int window_width = desktop.width;
        unsigned int window_height = desktop.height;
        
        // Create fullscreen window
        sf::RenderWindow window(
            sf::VideoMode(window_width, window_height),
            "Rhythm Game - 4K Mode",
            sf::Style::Fullscreen
        );
        
        // Set frame rate limit to 60 FPS
        window.setFramerateLimit(FRAME_RATE);
        
        // Calculate track dimensions (tracks are half width but still centered)
        float total_width = window_width * (1.0f - 2.0f * TRACK_MARGIN_RATIO);
        float tracks_total_width = total_width * 0.5f;  // Half of original width
        float track_width = tracks_total_width / NUM_TRACKS;
        float start_x = (window_width - tracks_total_width) / 2.0f;  // Center the tracks
        float track_height = static_cast<float>(window_height);
        
        // Judge line position
        float judge_line_y = window_height * JUDGE_LINE_Y_RATIO;
        
<<<<<<< Updated upstream
        // Note pool: All possible notes (can be loaded from beatmap)
        struct NoteTemplate {
            unsigned int track_index;
            float pixels_per_millisecond;
            float length;
            float spawn_time_ms;  // When bottom edge should enter screen
        };
        
        // Example notes: bottom edge enters screen at different times
        std::vector<NoteTemplate> note_templates;
        note_templates.push_back({0, 3.0f, 100.0f, 1000.0f});  // Track 0, spawns at 1 second
        note_templates.push_back({1, 3.0f, 100.0f, 2000.0f});  // Track 1, spawns at 2 seconds
        note_templates.push_back({2, 3.0f, 100.0f, 3000.0f});  // Track 2, spawns at 3 seconds
        note_templates.push_back({3, 3.0f, 100.0f, 4000.0f});  // Track 3, spawns at 4 seconds
=======
        // 音符对象池：管理所有音符模板
        NoteTemplatePool note_pool;
        
        // 从谱面加载音符：优先使用传入的参数，如果为空则使用全局变量，最后使用默认值
        std::string chart_path_to_use = chart_path_param;
        if (chart_path_to_use.empty()) {
            chart_path_to_use = chart_path;  // 使用全局变量
        }
        if (chart_path_to_use.empty()) {
            chart_path_to_use = "beatmap/example/chart.mc";  // 默认路径（回退方案）
        }
        playNotesFromBeatmap(chart_path_to_use, note_pool, static_cast<float>(window_height));
        
        // Progress bar - duration will be set from music duration
        double progress_bar_duration_seconds = 60.0;  // Default, will be updated from music
        
        // 获取当前时间作为游戏开始时间
        auto game_start_time = timer::current_timing;
        
        // 找到第一个音符的判定时间（相对于音乐开始的时间，单位：毫秒）
        // 第一个音符的 judge_time_ms 就是它下边界触碰到检测线的时间（相对于音乐开始）
        float first_note_judge_time_ms = 0.0f;
        float first_note_speed = DEFAULT_NOTE_SPEED;
        const std::vector<NoteTemplate>& note_templates_temp = note_pool.getTemplates();
        if (!note_templates_temp.empty()) {
            first_note_judge_time_ms = note_templates_temp[0].judge_time_ms;
            first_note_speed = note_templates_temp[0].pixels_per_millisecond;
        }
        
        // 计算滑块下落时间：从屏幕顶部到检测线的时间
        // 滑块下落时间 = 检测线位置 / 速度
        float note_fall_time_ms = judge_line_y / first_note_speed;
        
        // 计算音乐开始时间：取消延时，音乐立即开始
        auto music_start_time = game_start_time;
        
        // 调整所有音符的 spawn_time_ms，使其相对于 timer
        // 原来的 spawn_time_ms 是相对于音乐开始的，现在需要转换为相对于 timer 的
        // 新的 spawn_time_ms = 原来的 spawn_time_ms + game_start_time
        // 取消所有延时，直接使用 game_start_time
        float time_offset_ms = static_cast<float>(game_start_time.count());
        note_pool.adjustSpawnTime(time_offset_ms);
        
        try {
            // 加载 Beatmap 以获取音乐路径
            FileManager::Beatmap beatmap(chart_path_to_use);
            if (beatmap.isLoaded()) {
                std::string music_path = beatmap.getMusicPath();
                if (music::loadAndPlay(music_path, music_start_time)) {
                    FileManager::Logger::info("Music loaded and scheduled to play: " + music_path);
                    FileManager::Logger::info("First note judge time (relative to music): " + std::to_string(first_note_judge_time_ms) + "ms");
                    FileManager::Logger::info("Music start time (timer): " + std::to_string(music_start_time.count()) + "ms");
                    FileManager::Logger::info("Game start time (timer): " + std::to_string(game_start_time.count()) + "ms");
                    // Get music duration for progress bar
                    auto music_duration_ms = music::getDuration();
                    if (music_duration_ms.count() > 0) {
                        progress_bar_duration_seconds = music_duration_ms.count() / 1000.0;
                    }
                } else {
                    FileManager::Logger::error("Failed to load music file: " + music_path);
                }
            } else {
                FileManager::Logger::error("Failed to load beatmap for music path: " + chart_path_to_use);
            }
        } catch (const std::exception& e) {
            FileManager::Logger::error("Error loading music in Gameplay: " + std::string(e.what()));
        }
        
        // 获取音符模板列表（用于后续遍历）
        const std::vector<NoteTemplate>& note_templates = note_pool.getTemplates();
>>>>>>> Stashed changes
        
        // Active notes on screen
        std::vector<ActiveNote> active_notes;
        
        // Create 4 tracks using SFML RectangleShape
        std::vector<sf::RectangleShape> tracks(NUM_TRACKS);
        for (unsigned int i = 0; i < NUM_TRACKS; ++i) {
            tracks[i].setSize(sf::Vector2f(track_width, track_height));
            tracks[i].setPosition(start_x + i * track_width, 0.0f);
            tracks[i].setFillColor(TRACK_COLOR);
            tracks[i].setOutlineColor(TRACK_BORDER_COLOR);
            tracks[i].setOutlineThickness(2.0f);
        }
        
        // Create judge line using SFML RectangleShape
        float judge_line_width = tracks_total_width;
        float judge_line_x = start_x;
        
        sf::RectangleShape judge_line;
        judge_line.setSize(sf::Vector2f(judge_line_width, JUDGE_LINE_HEIGHT));
        judge_line.setPosition(judge_line_x, judge_line_y);
        judge_line.setFillColor(JUDGE_LINE_COLOR);
        
<<<<<<< Updated upstream
        // Load default system font (try Windows system fonts)
        sf::Font default_font;
        bool font_loaded = false;
        
        // Try to load beautiful system fonts (prioritized by aesthetics)
        const char* font_paths[] = {
            // Premium modern fonts (best looking)
            "C:/Windows/Fonts/segoeui.ttf",        // Segoe UI (Windows 10/11 default, clean and modern)
            "C:/Windows/Fonts/segoeuib.ttf",       // Segoe UI Bold (bold version)
            "C:/Windows/Fonts/segoeuil.ttf",       // Segoe UI Light (elegant)
            "C:/Windows/Fonts/consola.ttf",        // Consolas (monospace, crisp for games)
            "C:/Windows/Fonts/consolab.ttf",       // Consolas Bold
=======
        // Load default system font (try Windows system fonts with extensive fallback list)
        sf::Font default_font;
        bool font_loaded = false;
        
        // Try to load system fonts (prioritized by availability and aesthetics)
        // This extensive list ensures we can find at least one working font on Windows
        const char* font_paths[] = {
            // Premium modern fonts (best looking, Windows 10/11 default)
            "C:/Windows/Fonts/segoeui.ttf",        // Segoe UI (Windows 10/11 default, clean and modern)
            "C:/Windows/Fonts/segoeuib.ttf",       // Segoe UI Bold
            "C:/Windows/Fonts/segoeuil.ttf",       // Segoe UI Light
            
            // Monospace fonts (crisp for games)
            "C:/Windows/Fonts/consola.ttf",        // Consolas (monospace, crisp for games)
            "C:/Windows/Fonts/consolab.ttf",       // Consolas Bold
            
            // Readable modern fonts
>>>>>>> Stashed changes
            "C:/Windows/Fonts/verdana.ttf",        // Verdana (very readable, modern)
            "C:/Windows/Fonts/verdanab.ttf",       // Verdana Bold
            "C:/Windows/Fonts/trebuc.ttf",         // Trebuchet MS (modern, friendly)
            "C:/Windows/Fonts/trebucbd.ttf",       // Trebuchet MS Bold
<<<<<<< Updated upstream
            "C:/Windows/Fonts/georgia.ttf",        // Georgia (elegant serif)
            "C:/Windows/Fonts/georgiab.ttf",       // Georgia Bold
=======
>>>>>>> Stashed changes
            
            // Game-style fonts
            "C:/Windows/Fonts/impact.ttf",         // Impact (bold, striking for game UI)
            "C:/Windows/Fonts/framd.ttf",          // Franklin Gothic Medium
<<<<<<< Updated upstream
            "C:/Windows/Fonts/framdit.ttf",        // Franklin Gothic Medium Italic
            
            // Modern UI fonts
            "C:/Windows/Fonts/calibri.ttf",        // Calibri (modern sans-serif)
            "C:/Windows/Fonts/calibrib.ttf",      // Calibri Bold
            "C:/Windows/Fonts/arial.ttf",          // Arial (classic, reliable)
            "C:/Windows/Fonts/arialbd.ttf",        // Arial Bold
            
            // Chinese fonts
            "C:/Windows/Fonts/msyh.ttf",           // Microsoft YaHei (微软雅黑)
            "C:/Windows/Fonts/msyhbd.ttf",         // Microsoft YaHei Bold
            
            // Fallback fonts
            "C:/Windows/Fonts/tahoma.ttf",         // Tahoma
            "C:/Windows/Fonts/simsun.ttc"          // SimSun (宋体, fallback for Chinese)
        };
        
        for (const char* path : font_paths) {
            if (default_font.loadFromFile(path)) {
                font_loaded = true;
=======
            
            // Modern UI fonts
            "C:/Windows/Fonts/calibri.ttf",        // Calibri (modern sans-serif)
            "C:/Windows/Fonts/calibrib.ttf",       // Calibri Bold
            "C:/Windows/Fonts/arial.ttf",          // Arial (classic, reliable)
            "C:/Windows/Fonts/arialbd.ttf",        // Arial Bold
            
            // Chinese fonts (for better Unicode support)
            "C:/Windows/Fonts/msyh.ttf",           // Microsoft YaHei (微软雅黑)
            "C:/Windows/Fonts/msyhbd.ttf",         // Microsoft YaHei Bold
            
            // Fallback fonts (most common, should always exist)
            "C:/Windows/Fonts/tahoma.ttf",         // Tahoma
            "C:/Windows/Fonts/simsun.ttc",         // SimSun (宋体, fallback for Chinese)
            
            // Additional fallbacks
            "C:/Windows/Fonts/cour.ttf",           // Courier New
            "C:/Windows/Fonts/courbd.ttf",         // Courier New Bold
            "C:/Windows/Fonts/times.ttf",          // Times New Roman
            "C:/Windows/Fonts/timesbd.ttf"         // Times New Roman Bold
        };
        
        // Try to load fonts in order
        for (const char* path : font_paths) {
            if (default_font.loadFromFile(path)) {
                font_loaded = true;
                FileManager::Logger::info("Font loaded successfully: " + std::string(path));
>>>>>>> Stashed changes
                break;
            }
        }
        
<<<<<<< Updated upstream
        // If no font loaded, create a fallback (text won't display but won't crash)
        if (!font_loaded) {
            // Create a minimal font or handle error
            // For now, we'll proceed - the text just won't display if font fails
=======
        // If no font loaded, try alternative paths (for different Windows versions)
        if (!font_loaded) {
            // Try alternative font directory paths
            const char* alt_paths[] = {
                "C:/Windows/Fonts/arial.ttf",
                "C:/Windows/Fonts/calibri.ttf",
                "C:/Windows/Fonts/tahoma.ttf"
            };
            
            for (const char* path : alt_paths) {
                if (default_font.loadFromFile(path)) {
                    font_loaded = true;
                    FileManager::Logger::info("Font loaded from alternative path: " + std::string(path));
                    break;
                }
            }
        }
        
        // Final fallback: log warning but continue (text won't display but game won't crash)
        if (!font_loaded) {
            FileManager::Logger::error("Failed to load any system font. Text display will be disabled.");
            FileManager::Logger::error("Please ensure Windows Fonts directory is accessible at C:/Windows/Fonts/");
>>>>>>> Stashed changes
        }
        
        // Judgment display
        JudgmentDisplay judgment_display;
<<<<<<< Updated upstream
        
=======

>>>>>>> Stashed changes
        // Game start time (in milliseconds, converted to seconds)
        double game_start_time_seconds = 0.0;
        bool game_started = false;
        
<<<<<<< Updated upstream
        // Progress bar (1 minute from 0 to 100%)
        const double PROGRESS_BAR_DURATION_SECONDS = 60.0;  // 1 minute
=======
        // Progress bar
>>>>>>> Stashed changes
        double progress_percentage = 0.0;
        sf::RectangleShape progress_bar_background;
        sf::RectangleShape progress_bar_fill;
        
        // Combo counter
        unsigned int combo_count = 0;
        sf::Text combo_text;
        
        // Score statistics
        unsigned int perfect_count = 0;
        unsigned int good_count = 0;
        unsigned int miss_count = 0;
        sf::Text score_text;
        
        // Initialize progress bar (full width)
        float progress_bar_width = static_cast<float>(window_width);
        float progress_bar_height = 20.0f;
        float progress_bar_x = 0.0f;
        float progress_bar_y = 40.0f;
        
<<<<<<< Updated upstream
        // Initialize combo text
        if (font_loaded) {
            combo_text.setFont(default_font);
            combo_text.setCharacterSize(120);  // Increased to 120
=======
        progress_bar_background.setSize(sf::Vector2f(progress_bar_width, progress_bar_height));
        progress_bar_background.setPosition(progress_bar_x, progress_bar_y);
        progress_bar_background.setFillColor(sf::Color(50, 50, 50));  // Dark gray background
        progress_bar_background.setOutlineColor(sf::Color::White);
        progress_bar_background.setOutlineThickness(2.0f);
        
        progress_bar_fill.setSize(sf::Vector2f(0.0f, progress_bar_height));
        progress_bar_fill.setPosition(progress_bar_x, progress_bar_y);
        progress_bar_fill.setFillColor(sf::Color(0, 200, 255));  // Cyan color
        
        // Initialize combo text
        if (font_loaded) {
            combo_text.setFont(default_font);
            combo_text.setCharacterSize(120);  // Large font size
>>>>>>> Stashed changes
            combo_text.setFillColor(sf::Color::White);
            combo_text.setString("0");
            // Position below judge line
            sf::FloatRect text_bounds = combo_text.getLocalBounds();
            combo_text.setOrigin(text_bounds.left + text_bounds.width / 2.0f,
                               text_bounds.top + text_bounds.height / 2.0f);
            combo_text.setPosition(window_width / 2.0f, judge_line_y + 50.0f);  // Below judge line
        }
        
        // Initialize score text (below progress bar)
        if (font_loaded) {
            score_text.setFont(default_font);
            score_text.setCharacterSize(160);
            score_text.setFillColor(sf::Color::White);
            score_text.setString("Score: 0");
            score_text.setPosition(10.0f, progress_bar_y + progress_bar_height + 15.0f);  // Below progress bar
        }
<<<<<<< Updated upstream
        
        progress_bar_background.setSize(sf::Vector2f(progress_bar_width, progress_bar_height));
        progress_bar_background.setPosition(progress_bar_x, progress_bar_y);
        progress_bar_background.setFillColor(sf::Color(50, 50, 50));  // Dark gray background
        progress_bar_background.setOutlineColor(sf::Color::White);
        progress_bar_background.setOutlineThickness(2.0f);
        
        progress_bar_fill.setSize(sf::Vector2f(0.0f, progress_bar_height));
        progress_bar_fill.setPosition(progress_bar_x, progress_bar_y);
        progress_bar_fill.setFillColor(sf::Color(0, 200, 255));  // Cyan color

        // Track key states for each track (S, F, H, K)
=======

        // Track key states for each track
>>>>>>> Stashed changes
        struct TrackKey {
            sf::Keyboard::Key key;
            int track_index;
            bool pressed_this_frame;
            bool was_pressed;
        };

<<<<<<< Updated upstream
        TrackKey track_keys[4] = {
            {sf::Keyboard::S, 0, false, false},  // S key -> track 0
            {sf::Keyboard::F, 1, false, false},  // F key -> track 1
            {sf::Keyboard::H, 2, false, false},  // H key -> track 2
=======
        // 直接使用DFJK键位，不区分大小写（SFML的Keyboard::D等已经处理了大小写）
        TrackKey track_keys[NUM_TRACKS] = {
            {sf::Keyboard::D, 0, false, false},  // D key -> track 0
            {sf::Keyboard::F, 1, false, false},  // F key -> track 1
            {sf::Keyboard::J, 2, false, false},  // J key -> track 2
>>>>>>> Stashed changes
            {sf::Keyboard::K, 3, false, false}   // K key -> track 3
        };
        
        // Main game loop
        while (window.isOpen()) {
            // Reset key press flags for this frame
            for (auto& track_key : track_keys) {
                track_key.pressed_this_frame = false;
            }

            // Handle events using SFML Event system
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }

                // Handle keyboard input
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }

                    // Detect track key presses (only on key down, not hold)
                    for (auto& track_key : track_keys) {
                        if (event.key.code == track_key.key && !track_key.was_pressed) {
                            track_key.pressed_this_frame = true;
                            track_key.was_pressed = true;
                        }
                    }
                }

                if (event.type == sf::Event::KeyReleased) {
                    // Reset key states when released
                    for (auto& track_key : track_keys) {
                        if (event.key.code == track_key.key) {
                            track_key.was_pressed = false;
                        }
                    }
                }
            }
            
            // Get current time in milliseconds
            float current_time_ms = static_cast<float>(timer::current_timing.count());
            
            // Initialize game start time on first frame
            if (!game_started) {
                game_start_time_seconds = current_time_ms / 1000.0;
                game_started = true;
            }
            
            // Calculate elapsed time in seconds (double precision)
            double current_time_seconds = (current_time_ms / 1000.0) - game_start_time_seconds;
            
<<<<<<< Updated upstream
            // Update progress bar (1 minute from 0 to 100%)
            progress_percentage = (current_time_seconds / PROGRESS_BAR_DURATION_SECONDS) * 100.0;
=======
            // Update progress bar based on music duration
            progress_percentage = (current_time_seconds / progress_bar_duration_seconds) * 100.0;
>>>>>>> Stashed changes
            if (progress_percentage > 100.0) {
                progress_percentage = 100.0;
            }
            float fill_width = static_cast<float>(progress_percentage / 100.0 * progress_bar_width);
            progress_bar_fill.setSize(sf::Vector2f(fill_width, progress_bar_height));
            
<<<<<<< Updated upstream
            // Calculate and update score (perfect*10 + good*3 + miss*1)
            unsigned int current_score = perfect_count * 10 + good_count * 3 + miss_count * 1;
            if (font_loaded) {
                score_text.setString("Score: " + std::to_string(current_score));
            }
            
=======
>>>>>>> Stashed changes
            // Update judgment display (fade out effect)
            updateJudgmentDisplay(judgment_display, current_time_ms);
            
            // Spawn new notes when their bottom edge enters screen
<<<<<<< Updated upstream
=======
            // 使用对象池中的模板生成活跃音符
>>>>>>> Stashed changes
            for (const auto& template_note : note_templates) {
                // Check if this note should spawn
                ActiveNote test_note = {
                    template_note.track_index,
                    template_note.pixels_per_millisecond,
                    template_note.length,
                    template_note.spawn_time_ms
                };
                
                if (test_note.shouldSpawn(current_time_ms, window_height)) {
                    // Check if note is already active
                    bool already_exists = false;
                    for (const auto& active : active_notes) {
                        if (active.track_index == template_note.track_index &&
                            std::abs(active.spawn_time_ms - template_note.spawn_time_ms) < 1.0f) {
                            already_exists = true;
                            break;
                        }
                    }
                    
                    if (!already_exists) {
                        active_notes.push_back(test_note);
                    }
                }
            }
            
            // Check judgment for each track key press
            for (auto& track_key : track_keys) {
                if (track_key.pressed_this_frame) {
                    float best_time_diff = MISS_THRESHOLD + 1.0f;
                    ActiveNote* best_note = nullptr;
                    int best_result = -1;

                    // Find the note closest to judge line among active notes on this specific track
                    for (auto& note : active_notes) {
                        // Only check notes on the same track as the pressed key
                        if (note.track_index == track_key.track_index && note.isOnScreen(current_time_ms, window_height)) {
                            int result = checkJudgment(
                                note,
                                current_time_ms,
                                judge_line_y,
                                note.pixels_per_millisecond
                            );

                            if (result >= 0) {
                                // Calculate time difference to find closest note
                                float expected_time = note.getJudgeTime(judge_line_y);
                                float time_diff = std::abs(current_time_ms - expected_time);

                                if (time_diff < best_time_diff) {
                                    best_time_diff = time_diff;
                                    best_note = &note;
                                    best_result = result;
                                }
                            }
                        }
                    }

                    // Process the best judgment found for this track
<<<<<<< Updated upstream
=======
                    // 按照111.cpp的逻辑：需要font_loaded为true才处理判定，且不立即移除音符
>>>>>>> Stashed changes
                    if (best_result >= 0 && best_note != nullptr && font_loaded) {
                        // Show judgment display
                        showJudgment(judgment_display, best_result, current_time_ms,
                                   judge_line_y, window_width, default_font);
                        
                        // Update judgment counts
                        if (best_result == 0) {
                            // Perfect
                            perfect_count++;
                        } else if (best_result == 1) {
                            // Good
                            good_count++;
                        } else if (best_result == 2) {
                            // Miss
                            miss_count++;
                        }
                        
                        // Update combo count
                        if (best_result == 0 || best_result == 1) {
                            // Perfect or Good: increase combo
                            combo_count++;
                        } else if (best_result == 2) {
                            // Miss: reset combo
                            combo_count = 0;
                        }
                        
                        // Update combo text display
                        if (font_loaded) {
                            combo_text.setString(std::to_string(combo_count));
                            // Re-center the text
                            sf::FloatRect text_bounds = combo_text.getLocalBounds();
                            combo_text.setOrigin(text_bounds.left + text_bounds.width / 2.0f,
                                               text_bounds.top + text_bounds.height / 2.0f);
                            combo_text.setPosition(window_width / 2.0f, judge_line_y + 50.0f);
                        }
<<<<<<< Updated upstream
=======
                        
                        // 注意：按照111.cpp的逻辑，这里不立即移除音符
                        // 音符会在离开屏幕时自动移除（在后面的代码中处理）
>>>>>>> Stashed changes
                    }
                }
            }
            
<<<<<<< Updated upstream
=======
            // Calculate and update score (perfect*10 + good*3 + miss*1)
            unsigned int current_score = perfect_count * 10 + good_count * 3 + miss_count * 1;
            if (font_loaded) {
                score_text.setString("Score: " + std::to_string(current_score));
            }
            
>>>>>>> Stashed changes
            // Remove notes that have left screen (top edge passed bottom)
            active_notes.erase(
                std::remove_if(
                    active_notes.begin(),
                    active_notes.end(),
                    [current_time_ms, window_height](const ActiveNote& note) {
                        return note.shouldDestroy(current_time_ms, window_height);
                    }
                ),
                active_notes.end()
            );
            
            // Clear the window with background color
            window.clear(BACKGROUND_COLOR);
            
            // Draw tracks using SFML rendering
            for (const auto& track : tracks) {
                window.draw(track);
            }
            
            // Draw active notes
            for (const auto& note : active_notes) {
                if (note.isOnScreen(current_time_ms, window_height)) {
                    sf::RectangleShape note_shape = createTrackNote(
                        note,
                        current_time_ms,
                        start_x,
                        track_width
                    );
                    window.draw(note_shape);
                }
            }
            
            // Draw judge line (on top of tracks)
            window.draw(judge_line);
            
<<<<<<< Updated upstream
            // Draw judgment display if active
=======
            // ===== UI Elements - Rendered last to be on top of everything =====
            
            // Draw progress bar at top
            window.draw(progress_bar_background);
            window.draw(progress_bar_fill);
            
            // Draw judgment display (PERFECT/GOOD/MISS) - on top of everything
>>>>>>> Stashed changes
            if (judgment_display.is_active && judgment_display.judgment_type >= 0) {
                window.draw(judgment_display.text);
            }
            
<<<<<<< Updated upstream
            // Draw progress bar at top - rendered last to be on top
            window.draw(progress_bar_background);
            window.draw(progress_bar_fill);
            
            // Draw combo count - rendered last to be on top of everything
=======
            // Draw combo count - on top of everything
>>>>>>> Stashed changes
            if (font_loaded) {
                window.draw(combo_text);
            }
            
<<<<<<< Updated upstream
            // Draw score - rendered last to be on top of everything
=======
            // Draw score - on top of everything
>>>>>>> Stashed changes
            if (font_loaded) {
                window.draw(score_text);
            }
            
            // Display everything
            window.display();
        }
        
<<<<<<< Updated upstream
        // Cleanup timer
        timer::shutdown();
=======
        // Stop music before cleanup
        music::stop();
        
        // Cleanup timer
        timer::shutdown();
        
        // Cleanup music resources
        music::cleanup();
>>>>>>> Stashed changes
    }
}
