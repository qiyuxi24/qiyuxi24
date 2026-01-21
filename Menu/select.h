#ifndef SELECT_H
#define SELECT_H

#include<SFML/Graphics.hpp>
#include<string>
#include "game.h"
#include "../Game/Gameplay.h"

// 全局变量声明
extern std::string name_of_song;
extern std::string add_of_audio;
extern std::string chart_path;

/**
 * 歌曲选择函数声明
 * @param window SFML窗口对象
 * @param songFont 歌曲选择界面使用的字体
 * @param handCursor 手形光标
 * @param arrowCursor 箭头光标
 * @return 选中的歌曲索引
 */
int select(sf::RenderWindow& window, sf::Font& songFont, sf::Cursor& handCursor, sf::Cursor& arrowCursor);

#endif // SELECT_H