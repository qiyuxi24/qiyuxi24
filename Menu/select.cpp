#include<SFML/Graphics.hpp>
#include<iostream>
#include<vector>
#include<string>
#include<cmath>
#include "select.h"
#include "../FileManager/FileManager.h"
#include "../FileManager/Beatmap.h"
#include "../FileManager/LocalScore.h"
#include "../FileManager/Menu.h"
#include "../FileManager/Logger.h"

// 全局变量定义
std::string name_of_song;
std::string add_of_audio;
std::string chart_path;

/**
 * 歌曲选择函数
 * @param window SFML窗口对象
 * @param songFont 歌曲选择界面使用的字体
 * @param handCursor 手形光标
 * @param arrowCursor 箭头光标
 * @return 选中的歌曲索引
 */
int select(sf::RenderWindow& window, sf::Font& songFont, sf::Cursor& handCursor, sf::Cursor& arrowCursor) {
    // 歌曲相关变量
    std::vector<Song> songs;
    std::vector<FileManager::BeatmapInfo> beatmaps; // 保存 beatmap 信息以便后续访问 chart_path
    int songCount = 0;
    int pageCount = 0;
    int currentPage = 1; // 当前页码，从1开始
    int selectedSongIndex = 0;
    bool songSelected = false;
    
    // 歌曲选择界面UI元素
    // 三角形按钮
    sf::ConvexShape prevButton;
    sf::ConvexShape nextButton;
    
    // 初始化三角形按钮
    prevButton.setPointCount(3);
    prevButton.setFillColor(sf::Color(200, 200, 200, 200));
    prevButton.setOutlineColor(sf::Color::White);
    prevButton.setOutlineThickness(2);
    
    nextButton.setPointCount(3);
    nextButton.setFillColor(sf::Color(200, 200, 200, 200));
    nextButton.setOutlineColor(sf::Color::White);
    nextButton.setOutlineThickness(2);
    
    // 页面指示器文字
    sf::Text pageIndicator;
    pageIndicator.setFont(songFont);
    pageIndicator.setCharacterSize(20);
    pageIndicator.setFillColor(sf::Color::White);
    
    // 歌曲区域矩形
    std::vector<sf::RectangleShape> songRects(3);
    std::vector<sf::Text> songNames(3);
    std::vector<sf::Sprite> songThumbnails(3);
    std::vector<sf::Texture> songThumbnailTextures(3);
    
    // 当前选中歌曲的图片
    sf::Texture selectedSongTexture;
    sf::Sprite selectedSongSprite;
    
    // 歌曲选择界面背景
    sf::RectangleShape leftBg;
    sf::RectangleShape rightBg;
    leftBg.setFillColor(sf::Color(50, 50, 50, 200));
    rightBg.setFillColor(sf::Color(30, 30, 30, 200));
    
    // Bang按钮相关变量
    sf::RectangleShape bangButton;
    sf::Text bangButtonText;
    bool showFullscreenPrompt = false; // 控制是否显示全屏提示框
    
    // 全屏提示框相关变量
    sf::RectangleShape promptBox;
    sf::Text promptText;
    
    // 读取歌曲信息（从 Beatmap 获取曲目列表）
    try {
        // 获取曲目列表
        beatmaps = FileManager::Beatmap::getBeatmapList();
        
        // 加载本地成绩数据
        FileManager::LocalScore localScore;
        localScore.load();
        
        // 清空之前的歌曲列表
        songs.clear();
        
        // 将 BeatmapInfo 转换为 Song 结构体
        for (const auto& beatmap : beatmaps) {
            Song song;
            
            // 组合歌曲名字：标题 - 艺术家
            song.name = beatmap.title + " - " + beatmap.artist;
            
            // 路径处理：Beatmap 返回的是相对于 product_path 的路径（如 "beatmap/xxx/background.jpg"）
            // Menu 期望的路径是相对于 "./resource/" 的，但根据成品结构，背景图在 beatmap 目录下
            // 这里直接使用相对路径，后续加载时可能需要调整路径前缀
            song.imagePath = beatmap.background_path;
            song.thumbnailPath = beatmap.background_path; // 背景图也用作缩略图
            song.audioPath = beatmap.music_path;
            
            // 获取最高分
            auto bestScore = localScore.getBestScore(beatmap.id);
            if (!bestScore.is_null() && bestScore.contains("acc")) {
                // acc 是 0-1 之间的值，转换为 0-100 的整数
                song.highScore = static_cast<int>(bestScore["acc"].get<double>() * 100);
            } else {
                song.highScore = 0;
            }
            
            songs.push_back(song);
        }
        
        songCount = static_cast<int>(songs.size());
        
        // 计算页数
        pageCount = static_cast<int>(std::ceil(static_cast<double>(songCount) / 3.0));
        currentPage = 1;
        selectedSongIndex = 0;
        
        // 加载初始选中歌曲的图片
        if (!songs.empty()) {
            // 尝试加载图片
            if (FileManager::Menu::loadImageWithPath(selectedSongTexture, songs[0].imagePath)) {
                selectedSongSprite.setTexture(selectedSongTexture);
                // 调整图片大小以适应右侧区域
                float rightWidth = window.getSize().x * 14.0f / 18.0f;
                float rightHeight = window.getSize().y;
                float scaleX = rightWidth / selectedSongTexture.getSize().x;
                float scaleY = rightHeight / selectedSongTexture.getSize().y;
                selectedSongSprite.setScale(scaleX, scaleY);
                selectedSongSprite.setPosition(window.getSize().x * 4.0f / 18.0f, 0);
            }
        }
    } catch (const std::exception& e) {
        FileManager::Logger::error("Failed to load beatmap list in select: " + std::string(e.what()));
        return -1;
    }
    
    // 歌曲选择界面主循环
    while (window.isOpen() && !songSelected) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return -1;
            } else if (event.type == sf::Event::Resized) {
                // 调整视图以适应新的窗口大小
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
            // 处理鼠标点击事件
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    float windowWidth = window.getSize().x;
                    float windowHeight = window.getSize().y;
                    float leftWidth = windowWidth * 4.0f / 18.0f; // 左侧宽度：4/18
                    float rightWidth = windowWidth * 14.0f / 18.0f; // 右侧宽度：14/18
                    
                    // 如果显示全屏提示框，处理提示框内的点击
                    if (showFullscreenPrompt) {
                        // 检查是否点击了开始按钮
                        float startButtonWidth = windowWidth * 0.2f;
                        float startButtonHeight = windowHeight * 0.08f;
                        sf::FloatRect startButtonRect(
                            windowWidth / 2.0f - startButtonWidth / 2.0f,
                            (windowHeight / 2.0f - 30.0f) + 3 * windowHeight * 0.04f + 40.0f,
                            startButtonWidth,
                            startButtonHeight
                        );
                        
                        if (startButtonRect.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                            // 设置全局变量，存储选中歌曲的名字、音频地址和谱面路径
                            if (!songs.empty() && selectedSongIndex >= 0 && selectedSongIndex < songs.size()) {
                                name_of_song = songs[selectedSongIndex].name;
                                add_of_audio = songs[selectedSongIndex].audioPath;
                                // 从 beatmaps 中获取对应的 chart_path
                                if (selectedSongIndex >= 0 && selectedSongIndex < static_cast<int>(beatmaps.size())) {
                                    chart_path = beatmaps[selectedSongIndex].chart_path;
                                }
                            }
                            
                            // 开始游戏，设置为全屏模式
                            window.create(sf::VideoMode::getFullscreenModes()[0], "Music Bang", sf::Style::Fullscreen);
                            showFullscreenPrompt = false;
                            songSelected = true;
                        }
                    } else {
                        // 检查是否点击了上一页按钮
                        if (prevButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                            if (currentPage > 1) {
                                currentPage--;
                            }
                        } 
                        // 检查是否点击了下一页按钮
                        else if (nextButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                            if (currentPage < pageCount) {
                                currentPage++;
                            }
                        }
                        // 检查是否点击了Bang按钮
                        else if (bangButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                            // 显示全屏提示框
                            showFullscreenPrompt = true;
                        }
                        // 检查是否点击了歌曲区域
                        else if (mousePos.x < leftWidth) {
                            float songAreaHeight = windowHeight * 0.7f;
                            float songAreaY = windowHeight * 0.15f;
                            float songHeight = songAreaHeight / 3.0f;
                            
                            // 计算点击的是哪个歌曲区域
                            for (int i = 0; i < 3; i++) {
                                float songY = songAreaY + i * songHeight;
                                if (mousePos.y >= songY && mousePos.y <= songY + songHeight) {
                                    int startIndex = (currentPage - 1) * 3;
                                    int songIndex = startIndex + i;
                                    if (songIndex < songCount) {
                                        selectedSongIndex = songIndex;
                                        
                                        // 加载选中歌曲的图片
                                        if (FileManager::Menu::loadImageWithPath(selectedSongTexture, songs[songIndex].imagePath)) {
                                            selectedSongSprite.setTexture(selectedSongTexture);
                                            // 调整图片大小以适应右侧区域
                                            float scaleX = rightWidth / selectedSongTexture.getSize().x;
                                            float scaleY = windowHeight / selectedSongTexture.getSize().y;
                                            selectedSongSprite.setScale(scaleX, scaleY);
                                            selectedSongSprite.setPosition(leftWidth, 0);
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        
        // 处理鼠标悬浮事件
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        float windowWidth = window.getSize().x;
        float windowHeight = window.getSize().y;
        float leftWidth = windowWidth * 4.0f / 18.0f;
        
        // 默认使用箭头光标
        window.setMouseCursor(arrowCursor);
        
        // 如果没有显示提示框，检查鼠标悬停状态
        if (!showFullscreenPrompt) {
            // 检查是否悬停在Bang按钮上
            if (bangButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                window.setMouseCursor(handCursor);
            }
            
            // 检查是否悬停在三角形按钮上
            else if (prevButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)) || 
                     nextButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                window.setMouseCursor(handCursor);
            }
            
            // 检查是否悬停在歌曲区域上
            else if (mousePos.x < leftWidth) {
                float songAreaHeight = windowHeight * 0.7f;
                float songAreaY = windowHeight * 0.15f;
                float songHeight = songAreaHeight / 3.0f;
                
                for (int i = 0; i < 3; i++) {
                    float songY = songAreaY + i * songHeight;
                    if (mousePos.y >= songY && mousePos.y <= songY + songHeight) {
                        int startIndex = (currentPage - 1) * 3;
                        int songIndex = startIndex + i;
                        if (songIndex < songCount) {
                            window.setMouseCursor(handCursor);
                            break;
                        }
                    }
                }
            }
        } else {
            // 检查是否悬停在开始按钮上
            float startButtonWidth = windowWidth * 0.2f;
            float startButtonHeight = windowHeight * 0.08f;
            sf::FloatRect startButtonRect(
                windowWidth / 2.0f - startButtonWidth / 2.0f,
                (windowHeight / 2.0f - 30.0f) + 3 * windowHeight * 0.04f + 40.0f,
                startButtonWidth,
                startButtonHeight
            );
            
            if (startButtonRect.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                window.setMouseCursor(handCursor);
            }
        }
        
        window.clear();
        
        // 计算界面元素位置和大小
        float windowWidthDraw = window.getSize().x;
        float windowHeightDraw = window.getSize().y;
        
        // 左侧宽度：4/18，右侧宽度：14/18
        float leftWidthDraw = windowWidthDraw * 4.0f / 18.0f;
        float rightWidthDraw = windowWidthDraw * 14.0f / 18.0f;
        
        // 设置背景位置和大小
        leftBg.setPosition(0, 0);
        leftBg.setSize(sf::Vector2f(leftWidthDraw, windowHeightDraw));
        rightBg.setPosition(leftWidthDraw, 0);
        rightBg.setSize(sf::Vector2f(rightWidthDraw, windowHeightDraw));
        
        // 绘制背景
        window.draw(leftBg);
        window.draw(rightBg);
        
        // 绘制右侧选中歌曲图片
        // 动态调整选中歌曲图片的大小和位置
        if (selectedSongTexture.getSize().x > 0 && selectedSongTexture.getSize().y > 0) {
            float scaleX = rightWidthDraw / static_cast<float>(selectedSongTexture.getSize().x);
            float scaleY = windowHeightDraw / static_cast<float>(selectedSongTexture.getSize().y);
            selectedSongSprite.setScale(scaleX, scaleY);
            selectedSongSprite.setPosition(leftWidthDraw, 0);
        }
        window.draw(selectedSongSprite);
        
        // 动态调整Bang按钮大小和位置
        float buttonWidth = rightWidthDraw * 0.3f;
        float buttonHeight = windowHeightDraw * 0.08f;
        bangButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
        bangButton.setFillColor(sf::Color(100, 100, 255, 200));
        bangButton.setOutlineColor(sf::Color::White);
        bangButton.setOutlineThickness(2);
        bangButton.setPosition(leftWidthDraw + (rightWidthDraw - buttonWidth) / 2.0f, windowHeightDraw - buttonHeight - 20.0f);
        
        // 设置Bang按钮文字
        bangButtonText.setFont(songFont);
        bangButtonText.setString(L"Bang！");
        bangButtonText.setCharacterSize(static_cast<unsigned int>(buttonHeight * 0.6f));
        bangButtonText.setFillColor(sf::Color::White);
        
        // 居中按钮文字
        sf::FloatRect buttonTextBounds = bangButtonText.getLocalBounds();
        bangButtonText.setPosition(
            bangButton.getPosition().x + buttonWidth / 2.0f - buttonTextBounds.width / 2.0f,
            bangButton.getPosition().y + buttonHeight / 2.0f - buttonTextBounds.height / 2.0f
        );
        
        // 绘制Bang按钮和文字
        window.draw(bangButton);
        window.draw(bangButtonText);
        
        // 如果显示全屏提示框
        if (showFullscreenPrompt) {
            // 设置全屏半透明蒙层
            promptBox.setSize(sf::Vector2f(windowWidthDraw, windowHeightDraw));
            promptBox.setFillColor(sf::Color(0, 0, 0, 180)); // 半透明黑色蒙层
            promptBox.setPosition(0, 0);
            
            // 设置提示框文字，手动换行
            promptText.setFont(songFont);
            promptText.setString(L"为了更好的游玩体验，\n将会默认全屏加载。\n您可以通过Esc退出全屏模式");
            promptText.setCharacterSize(static_cast<unsigned int>(windowHeightDraw * 0.04f));
            promptText.setFillColor(sf::Color::White);
            
            // 计算文字位置，留出下方按钮空间
            sf::FloatRect promptTextBounds = promptText.getLocalBounds();
            float textX = windowWidthDraw / 2.0f - promptTextBounds.width / 2.0f;
            float textY = windowHeightDraw / 2.0f - promptTextBounds.height / 2.0f - 30.0f;
            promptText.setPosition(textX, textY);
            
            // 设置开始按钮
            sf::RectangleShape startButton;
            float startButtonWidth = windowWidthDraw * 0.2f;
            float startButtonHeight = windowHeightDraw * 0.08f;
            startButton.setSize(sf::Vector2f(startButtonWidth, startButtonHeight));
            startButton.setFillColor(sf::Color(100, 100, 255, 200));
            startButton.setOutlineColor(sf::Color::White);
            startButton.setOutlineThickness(2);
            startButton.setPosition(
                windowWidthDraw / 2.0f - startButtonWidth / 2.0f,
                textY + promptTextBounds.height + 40.0f
            );
            
            // 设置开始按钮文字
            sf::Text startButtonText;
            startButtonText.setFont(songFont);
            startButtonText.setString(L"开始游戏");
            startButtonText.setCharacterSize(static_cast<unsigned int>(startButtonHeight * 0.6f));
            startButtonText.setFillColor(sf::Color::White);
            
            // 居中开始按钮文字
            sf::FloatRect startButtonTextBounds = startButtonText.getLocalBounds();
            startButtonText.setPosition(
                startButton.getPosition().x + startButtonWidth / 2.0f - startButtonTextBounds.width / 2.0f,
                startButton.getPosition().y + startButtonHeight / 2.0f - startButtonTextBounds.height / 2.0f
            );
            
            // 绘制半透明蒙层、文字和按钮
            window.draw(promptBox);
            window.draw(promptText);
            window.draw(startButton);
            window.draw(startButtonText);
        }
        
        // 设置三角形按钮位置和大小
        float triangleButtonWidth = leftWidthDraw * 0.6f;
        float triangleButtonHeight = 30;
        
        // 上一页按钮（向上三角形）
        prevButton.setPoint(0, sf::Vector2f(leftWidthDraw / 2.0f - triangleButtonWidth / 2.0f, triangleButtonHeight));
        prevButton.setPoint(1, sf::Vector2f(leftWidthDraw / 2.0f + triangleButtonWidth / 2.0f, triangleButtonHeight));
        prevButton.setPoint(2, sf::Vector2f(leftWidthDraw / 2.0f, 0));
        
        // 下一页按钮（向下三角形）
        nextButton.setPoint(0, sf::Vector2f(leftWidthDraw / 2.0f - triangleButtonWidth / 2.0f, windowHeightDraw - triangleButtonHeight));
        nextButton.setPoint(1, sf::Vector2f(leftWidthDraw / 2.0f + triangleButtonWidth / 2.0f, windowHeightDraw - triangleButtonHeight));
        nextButton.setPoint(2, sf::Vector2f(leftWidthDraw / 2.0f, windowHeightDraw));
        
        // 绘制三角形按钮
        window.draw(prevButton);
        window.draw(nextButton);
        
        // 设置页面指示器，字符大小随窗口高度动态调整
        float pageIndicatorSize = windowHeightDraw * 0.0333f; // 原始20px对应600px窗口高度
        pageIndicator.setCharacterSize(static_cast<unsigned int>(pageIndicatorSize));
        pageIndicator.setString("Page " + std::to_string(currentPage) + "/" + std::to_string(pageCount));
        sf::FloatRect indicatorBounds = pageIndicator.getLocalBounds();
        pageIndicator.setPosition(leftWidthDraw / 2.0f - indicatorBounds.width / 2.0f, windowHeightDraw - 2 * pageIndicatorSize);
        window.draw(pageIndicator);
        
        // 计算歌曲区域的位置和大小
        float songAreaHeight = windowHeightDraw * 0.7f; // 歌曲区域占70%高度，留出更多空间
        float songAreaY = windowHeightDraw * 0.15f; // 上移，让顶部留出更多空间
        float songHeight = songAreaHeight / 3.0f;
        
        // 计算当前页显示的歌曲索引范围
        int startIndex = (currentPage - 1) * 3;
        int endIndex = std::min(startIndex + 3, songCount);
        
        // 绘制歌曲区域
        for (int i = 0; i < 3; i++) {
            int songIndex = startIndex + i;
            
            // 设置歌曲区域位置和大小
            songRects[i].setPosition(0, songAreaY + i * songHeight);
            songRects[i].setSize(sf::Vector2f(leftWidthDraw, songHeight));
            songRects[i].setFillColor(sf::Color(80, 80, 80, 200));
            songRects[i].setOutlineColor(sf::Color::White);
            songRects[i].setOutlineThickness(1);
            
            if (songIndex < endIndex) {
                // 加载并显示缩略图
                if (FileManager::Menu::loadImageWithPath(songThumbnailTextures[i], songs[songIndex].thumbnailPath)) {
                    songThumbnails[i].setTexture(songThumbnailTextures[i]);
                    // 调整缩略图大小以适应歌曲区域
                    float scaleX = leftWidthDraw / songThumbnailTextures[i].getSize().x;
                    float scaleY = songHeight / songThumbnailTextures[i].getSize().y;
                    songThumbnails[i].setScale(scaleX, scaleY);
                    songThumbnails[i].setPosition(0, songAreaY + i * songHeight);
                    window.draw(songThumbnails[i]);
                } else {
                    // 如果缩略图加载失败，绘制纯色背景
                    window.draw(songRects[i]);
                }
                
                // 设置歌曲名字，字符大小随窗口高度动态调整
                songNames[i].setFont(songFont);
                songNames[i].setString(songs[songIndex].name);
                // 根据窗口高度动态调整字符大小（原始20px对应600px窗口高度）
                float songNameSize = windowHeightDraw * 0.0333f; // 20/600 = 0.0333
                songNames[i].setCharacterSize(static_cast<unsigned int>(songNameSize));
                songNames[i].setFillColor(sf::Color::White);
                
                // 居中显示歌曲名字
                sf::FloatRect nameBounds = songNames[i].getLocalBounds();
                songNames[i].setPosition(
                    leftWidthDraw / 2.0f - nameBounds.width / 2.0f,
                    songAreaY + i * songHeight + songHeight / 2.0f - nameBounds.height / 2.0f
                );
                window.draw(songNames[i]);
            } else {
                // 空的歌曲区域
                songRects[i].setFillColor(sf::Color(60, 60, 60, 150));
                window.draw(songRects[i]);
            }
        }
        
        window.display();
    }
    
    // 调用游戏主函数
    if (songSelected) {
        Game::Gameplay(chart_path);
    }
    
    return selectedSongIndex;
}
