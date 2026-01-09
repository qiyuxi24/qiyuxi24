#include<SFML/Graphics.hpp>
#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<cmath>

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

int main(){
    sf::RenderWindow window(sf::VideoMode(800, 600), "Test Window", sf::Style::Default);
    window.setVerticalSyncEnabled(true);
    
    // 创建手型光标
    sf::Cursor handCursor;
    sf::Cursor arrowCursor; // 默认箭头光标
    if (handCursor.loadFromSystem(sf::Cursor::Hand)) {
        // 手型光标加载成功
    }
    if (arrowCursor.loadFromSystem(sf::Cursor::Arrow)) {
        // 默认箭头光标加载成功
    }
    
    // 调用主菜单函数，包含歌曲选择逻辑
    start_page(window, handCursor, arrowCursor);
    
    return 0;
}