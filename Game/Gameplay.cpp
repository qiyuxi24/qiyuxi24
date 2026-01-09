//
// Created by Blokki on 2025/12/17.
//

#include "Gameplay.h"
#include "../Tools/Timer.h"
#include <vector>
#include <algorithm>
#include <cmath>

namespace Game {
    // Constants
    static const unsigned int FRAME_RATE = 60;
    static const unsigned int NUM_TRACKS = 4;
    static const float TRACK_MARGIN_RATIO = 0.025f;  // 2.5% margin on each side
    static const float JUDGE_LINE_Y_RATIO = 0.85f;  // Judge line at 85% from top
    static const float JUDGE_LINE_HEIGHT = 3.0f;     // 3 pixels thick
    
    // Judgment thresholds (in milliseconds)
    static const float PERFECT_THRESHOLD = 20.0f;
    static const float GOOD_THRESHOLD = 40.0f;
    static const float MISS_THRESHOLD = 100.0f;
    
    // Colors
    static const sf::Color TRACK_COLOR(30, 30, 30);           // Dark gray for tracks
    static const sf::Color TRACK_BORDER_COLOR(60, 60, 60);    // Lighter gray for borders
    static const sf::Color JUDGE_LINE_COLOR(255, 255, 255);   // White for judge line
    static const sf::Color BACKGROUND_COLOR(20, 20, 20);      // Very dark background
    static const sf::Color NOTE_COLOR(100, 150, 255);         // Blue color for notes
    
    // Judgment result enum
    enum class Judgment {
        None,
        Perfect,
        Good,
        Miss
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
    // Returns judgment result
    Judgment checkJudgment(
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
            return Judgment::Perfect;
        } else if (time_diff_ms <= GOOD_THRESHOLD) {
            return Judgment::Good;
        } else if (time_diff_ms <= MISS_THRESHOLD) {
            return Judgment::Miss;
        }
        
        return Judgment::None;
    }
    
    void Gameplay() {
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
        
        // Calculate track dimensions
        float total_width = window_width * (1.0f - 2.0f * TRACK_MARGIN_RATIO);
        float track_width = total_width / NUM_TRACKS;
        float start_x = window_width * TRACK_MARGIN_RATIO;
        float track_height = static_cast<float>(window_height);
        
        // Judge line position
        float judge_line_y = window_height * JUDGE_LINE_Y_RATIO;
        
        // Note pool: All possible notes (can be loaded from beatmap)
        struct NoteTemplate {
            unsigned int track_index;
            float pixels_per_millisecond;
            float length;
            float spawn_time_ms;  // When bottom edge should enter screen
        };
        
        // Example notes: bottom edge enters screen at different times
        std::vector<NoteTemplate> note_templates;
        note_templates.push_back({0, 0.5f, 100.0f, 1000.0f});  // Track 0, spawns at 1 second
        note_templates.push_back({1, 0.5f, 100.0f, 2000.0f});  // Track 1, spawns at 2 seconds
        note_templates.push_back({2, 0.5f, 100.0f, 3000.0f});  // Track 2, spawns at 3 seconds
        note_templates.push_back({3, 0.5f, 100.0f, 4000.0f});  // Track 3, spawns at 4 seconds
        
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
        float judge_line_width = total_width;
        float judge_line_x = start_x;
        
        sf::RectangleShape judge_line;
        judge_line.setSize(sf::Vector2f(judge_line_width, JUDGE_LINE_HEIGHT));
        judge_line.setPosition(judge_line_x, judge_line_y);
        judge_line.setFillColor(JUDGE_LINE_COLOR);
        
        // Track space key state to detect key press (not hold)
        bool space_pressed_this_frame = false;
        bool space_was_pressed = false;
        
        // Main game loop
        while (window.isOpen()) {
            space_pressed_this_frame = false;
            
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
                    
                    // Detect space key press (only on key down, not hold)
                    if (event.key.code == sf::Keyboard::Space && !space_was_pressed) {
                        space_pressed_this_frame = true;
                        space_was_pressed = true;
                    }
                }
                
                if (event.type == sf::Event::KeyReleased) {
                    if (event.key.code == sf::Keyboard::Space) {
                        space_was_pressed = false;
                    }
                }
            }
            
            // Get current time in milliseconds
            float current_time_ms = static_cast<float>(timer::current_timing.count());
            
            // Spawn new notes when their bottom edge enters screen
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
            
            // Check judgment when space key is pressed
            if (space_pressed_this_frame) {
                float best_time_diff = MISS_THRESHOLD + 1.0f;
                ActiveNote* best_note = nullptr;
                Judgment best_result = Judgment::None;
                
                // Find the note closest to judge line among all active notes on screen
                for (auto& note : active_notes) {
                    if (note.isOnScreen(current_time_ms, window_height)) {
                        Judgment result = checkJudgment(
                            note,
                            current_time_ms,
                            judge_line_y,
                            note.pixels_per_millisecond
                        );
                        
                        if (result != Judgment::None) {
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
                
                // Process the best judgment found
                if (best_result != Judgment::None && best_note != nullptr) {
                    // Handle judgment result (can add score, effects, etc.)
                    // You can add a flag to ActiveNote to track if it's been judged
                    // and remove it from active_notes to prevent double judgment
                    
                    // Example: Print judgment result
                    // std::cout << "Judgment: ";
                    // switch (best_result) {
                    //     case Judgment::Perfect: std::cout << "PERFECT"; break;
                    //     case Judgment::Good: std::cout << "GOOD"; break;
                    //     case Judgment::Miss: std::cout << "MISS"; break;
                    //     default: break;
                    // }
                    // std::cout << " (Time diff: " << best_time_diff << "ms)" << std::endl;
                }
            }
            
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
            
            // Display everything
            window.display();
        }
        
        // Cleanup timer
        timer::shutdown();
    }
}
