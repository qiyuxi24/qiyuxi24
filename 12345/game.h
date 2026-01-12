#ifndef GAME_H
#define GAME_H

#include<SFML/Graphics.hpp>
#include<string>

// 歌曲结构体
typedef struct Song {
    std::string name;
    std::string imagePath;
    std::string thumbnailPath;
    std::string audioPath;
    int highScore;
} Song;

// 主菜单页面函数声明
void start_page(sf::RenderWindow& window, sf::Cursor& handCursor, sf::Cursor& arrowCursor);

// 歌曲选择函数声明
int select(sf::RenderWindow& window, sf::Font& songFont, sf::Cursor& handCursor, sf::Cursor& arrowCursor);

#endif // GAME_H