#include<SFML/Graphics.hpp>
#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<cmath>

// 歌曲结构体
struct Song {
    std::string name;
    std::string imagePath;
    std::string thumbnailPath;
    std::string audioPath; // 新增：歌曲音频文件路径
    int highScore;
};

int main(){
    sf::RenderWindow window(sf::VideoMode(800, 600), "Test Window", sf::Style::Default);
    window.setVerticalSyncEnabled(true);
    
    // 创建按钮
    sf::RectangleShape button(sf::Vector2f(150, 50));
    button.setFillColor(sf::Color::Green);
    button.setPosition(325, 500); // 窗口下方居中位置
    
    // 创建按钮文字
    sf::Font font;
    // 使用系统微软雅黑字体
    if (!font.loadFromFile("C:\\Windows\\Fonts\\msyh.ttc")) {
        // 如果没有找到微软雅黑字体，可以使用其他系统字体
    }
    
    sf::Text buttonText;
    buttonText.setFont(font);
    buttonText.setString(L"开始游戏");
    buttonText.setCharacterSize(20);
    buttonText.setFillColor(sf::Color::White);
    buttonText.setPosition(350, 510); // 按钮上居中位置
    
    // 加载开始界面图片
    sf::Texture startTexture;
    sf::Sprite startSprite;
    // 使用相对路径加载图片（避免中文路径问题）
    if (startTexture.loadFromFile("./start_screen.png")) {
        startSprite.setTexture(startTexture);
        // 调整图片大小以适应窗口
        startSprite.setScale(800.0f / startTexture.getSize().x, 600.0f / startTexture.getSize().y);
    }

    // 加载游戏图片
    sf::Texture texture;
    sf::Sprite sprite;
    bool showImage = false;
    bool showButton = true; // 控制按钮和文字是否显示
    bool showSongSelection = false; // 控制是否显示歌曲选择界面
    
    // 歌曲相关变量
    std::vector<Song> songs;
    int songCount = 0;
    int pageCount = 0;
    int currentPage = 1; // 当前页码，从1开始
    int selectedSongIndex = 0;
    
    // Bang按钮相关变量
    sf::RectangleShape bangButton;
    sf::Text bangButtonText;
    bool showFullscreenPrompt = false; // 控制是否显示全屏提示框
    
    // 全屏提示框相关变量
    sf::RectangleShape promptBox;
    sf::Text promptText;
    
    // 歌曲选择界面UI元素
    sf::Font songFont;
    if (!songFont.loadFromFile("C:\\Windows\\Fonts\\msyh.ttc")) {
        // 如果没有找到微软雅黑字体，可以使用其他系统字体
    }
    
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
    
    // 创建手型光标
    sf::Cursor handCursor;
    sf::Cursor arrowCursor; // 默认箭头光标
    if (handCursor.loadFromSystem(sf::Cursor::Hand)) {
        // 手型光标加载成功
    }
    if (arrowCursor.loadFromSystem(sf::Cursor::Arrow)) {
        // 默认箭头光标加载成功
    }
    
    // 加载游戏标题字体
    sf::Font titleFont;
    // 优先使用美观的装饰性字体，同时确保能显示感叹号
    // 尝试加载Algerian字体（装饰性字体）
    if (!titleFont.loadFromFile("C:\\Windows\\Fonts\\ALGER.TTF")) {
        // 尝试加载Agency FB Bold字体（装饰性字体）
        if (!titleFont.loadFromFile("C:\\Windows\\Fonts\\AGENCYB.TTF")) {
            // 尝试加载Impact字体（装饰性字体）
            if (!titleFont.loadFromFile("C:\\Windows\\Fonts\\IMPACT.TTF")) {
                // 尝试加载Cooper Black字体（装饰性字体）
                if (!titleFont.loadFromFile("C:\\Windows\\Fonts\\COOPBL.TTF")) {
                    // 如果都失败，使用微软雅黑字体
                    titleFont.loadFromFile("C:\\Windows\\Fonts\\msyh.ttc");
                    }
                }
            }
        }
    
    // 加载副标题字体
    sf::Font subtitleFont;
    if (!subtitleFont.loadFromFile("C:\\Windows\\Fonts\\simfang.ttf")) {
        // 如果没有找到仿宋字体，可以使用其他字体
    }
    
    // 动画控制变量
    sf::Clock animationClock;
    bool animationStarted = false;
    const float ANIMATION_DURATION = 2.0f; // 动画总时长（秒）
    const float ANIMATION_DELAY = 0.5f;    // 动画延迟开始时间（秒）
    
    // 拆分游戏标题为多个部分
    sf::Text musicText;
    musicText.setFont(titleFont);
    musicText.setString("Music");
    musicText.setCharacterSize(50);
    musicText.setFillColor(sf::Color::White);
    
    sf::Text bangText;
    bangText.setFont(titleFont);
    bangText.setString("Bang");
    bangText.setCharacterSize(50);
    bangText.setFillColor(sf::Color::White);
    
    sf::Text exclamationText;
    exclamationText.setFont(titleFont);
    exclamationText.setString("!");
    exclamationText.setCharacterSize(50);
    exclamationText.setFillColor(sf::Color::White);
    
    // 计算标题各部分的最终位置（居中排列）
    float musicWidth = musicText.getLocalBounds().width;
    float bangWidth = bangText.getLocalBounds().width;
    float exclamationWidth = exclamationText.getLocalBounds().width;
    
    // 确保各部分之间有足够的间距
    float spacing = 10.0f; // 文字之间的间距
    float exclamationSpacing = 5.0f; // 感叹号与Bang之间的额外间距
    
    // 计算总宽度
    float totalWidth = musicWidth + spacing + bangWidth + exclamationSpacing + exclamationWidth;
    float startX = 400.0f - totalWidth / 2.0f;
    
    // 英文标题固定在窗口高度的2/5处
    float windowHeight = 600.0f; // 初始窗口高度
    float englishTitleY = windowHeight * 2.0f / 5.0f; // 英文标题固定在2/5处
    sf::Vector2f musicFinalPos(startX + musicWidth / 2.0f, englishTitleY);
    sf::Vector2f bangFinalPos(startX + musicWidth + spacing + bangWidth / 2.0f, englishTitleY);
    sf::Vector2f exclamationFinalPos(startX + musicWidth + spacing + bangWidth + exclamationSpacing + exclamationWidth / 2.0f, englishTitleY);
    
    // 设置各部分的原点
    musicText.setOrigin(musicText.getLocalBounds().left + musicText.getLocalBounds().width / 2.0f, 
                        musicText.getLocalBounds().top + musicText.getLocalBounds().height / 2.0f);
    bangText.setOrigin(bangText.getLocalBounds().left + bangText.getLocalBounds().width / 2.0f, 
                       bangText.getLocalBounds().top + bangText.getLocalBounds().height / 2.0f);
    exclamationText.setOrigin(exclamationText.getLocalBounds().left + exclamationText.getLocalBounds().width / 2.0f, 
                              exclamationText.getLocalBounds().top + exclamationText.getLocalBounds().height / 2.0f);
    
    // 初始位置（动画开始前的位置）
    musicText.setPosition(musicFinalPos.x, -50.0f); // 上方屏幕外
    bangText.setPosition(850.0f, englishTitleY);     // 右侧屏幕外
    exclamationText.setPosition(exclamationFinalPos);
    exclamationText.setFillColor(sf::Color::Transparent); // 初始透明
    
    // 创建副标题文字（拆分为单个字符）
    std::wstring subtitleText = L"音核响裂";
    std::vector<sf::Text> subtitleChars;
    float subtitleFontSize = 30.0f; // 副标题文字大小
    
    // 中文标题固定在窗口高度的3/5处
    float chineseTitleY = windowHeight * 3.0f / 5.0f; // 中文标题固定在3/5处
    float subtitleY = chineseTitleY;
    
    // 先创建所有字符，然后计算总宽度
    for (size_t i = 0; i < subtitleText.length(); i++) {
        sf::Text charText;
        charText.setFont(subtitleFont);
        charText.setString(std::wstring(1, subtitleText[i]));
        charText.setCharacterSize(static_cast<unsigned int>(subtitleFontSize));
        charText.setFillColor(sf::Color::White);
        
        // 设置原点
        charText.setOrigin(charText.getLocalBounds().left + charText.getLocalBounds().width / 2.0f, 
                          charText.getLocalBounds().top + charText.getLocalBounds().height / 2.0f);
        
        subtitleChars.push_back(charText);
    }
    
    // 计算副标题总宽度
    float subtitleTotalWidth = 0.0f;
    float charSpacing = 5.0f; // 每个中文字符之间的间距
    for (const auto& charText : subtitleChars) {
        subtitleTotalWidth += charText.getLocalBounds().width + charSpacing;
    }
    if (!subtitleChars.empty()) {
        subtitleTotalWidth -= charSpacing; // 减去最后一个字符的额外间距
    }
    
    // 计算副标题起始位置
    float subtitleStartX = 400.0f - subtitleTotalWidth / 2.0f;
    float currentX = subtitleStartX;
    
    // 设置每个字符的最终位置和初始位置
    for (size_t i = 0; i < subtitleChars.size(); i++) {
        sf::Text& charText = subtitleChars[i];
        float charWidth = charText.getLocalBounds().width;
        
        // 计算最终位置
        sf::Vector2f finalPos(currentX + charWidth / 2.0f, subtitleY);
        
        // 设置初始位置（屏幕下方外）
        charText.setPosition(finalPos.x, 650.0f);
        
        currentX += charWidth + charSpacing; // 更新下一个字符的起始位置
    }
    
    // 预加载图片
    if (texture.loadFromFile("D:\\North\\Documents\\GitHub\\qiyuxi24\\songmap_demo\\0\\BG.jpg")) {
        sprite.setTexture(texture);
        // 调整图片大小以适应窗口
        sprite.setScale(800.0f / texture.getSize().x, 600.0f / texture.getSize().y);
    }
    
    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
            } else if (event.type == sf::Event::Resized) {
                // 调整视图以适应新的窗口大小
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
            // 处理鼠标点击事件
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    
                    if (button.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        showButton = false; // 隐藏按钮和文字
                        showSongSelection = true; // 显示歌曲选择界面
                        
                        // 读取歌曲信息
                        std::ifstream songFile("./resource/song_info.txt");
                        if (songFile.is_open()) {
                            // 读取歌曲数量
                            songFile >> songCount;
                            songFile.ignore(); // 忽略换行符
                            
                            // 清空之前的歌曲列表
                            songs.clear();
                            
                            // 读取每首歌曲的信息
                        for (int i = 0; i < songCount; i++) {
                            Song song;
                            
                            // 读取歌曲名字
                            std::getline(songFile, song.name);
                            
                            // 读取歌曲图片路径
                            std::getline(songFile, song.imagePath);
                            
                            // 读取歌曲缩略图路径
                            std::getline(songFile, song.thumbnailPath);
                            
                            // 新增：读取歌曲音频文件路径
                            std::getline(songFile, song.audioPath);
                            
                            // 读取历史最高分数
                            songFile >> song.highScore;
                            songFile.ignore(); // 忽略换行符
                            
                            songs.push_back(song);
                        }
                            
                            songFile.close();
                            
                            // 计算页数
                            pageCount = static_cast<int>(std::ceil(static_cast<double>(songCount) / 3.0));
                            currentPage = 1;
                            selectedSongIndex = 0;
                            
                            // 加载初始选中歌曲的图片
                            if (!songs.empty()) {
                                if (selectedSongTexture.loadFromFile("./resource/" + songs[0].imagePath)) {
                                    selectedSongSprite.setTexture(selectedSongTexture);
                                    // 调整图片大小以适应右侧区域
                                    float rightWidth = window.getSize().x * 13.0f / 18.0f;
                                    float rightHeight = window.getSize().y;
                                    float scaleX = rightWidth / selectedSongTexture.getSize().x;
                                    float scaleY = rightHeight / selectedSongTexture.getSize().y;
                                    selectedSongSprite.setScale(scaleX, scaleY);
                                    selectedSongSprite.setPosition(window.getSize().x * 5.0f / 18.0f, 0);
                                }
                            }
                        } else {
                            std::cerr << "Failed to open song info file!" << std::endl;
                        }
                    } else if (showSongSelection) {
                        // 处理歌曲选择界面的点击事件
                        float windowWidth = window.getSize().x;
                        float windowHeight = window.getSize().y;
                        float leftWidth = windowWidth * 4.0f / 18.0f; // 左侧宽度：4/18
                        float rightWidth = windowWidth * 14.0f / 18.0f; // 右侧宽度：14/18
                        
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
                                        if (selectedSongTexture.loadFromFile("./resource/" + songs[songIndex].imagePath)) {
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
        if (showButton) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (button.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                window.setMouseCursor(handCursor);
            } else {
                window.setMouseCursor(arrowCursor); // 使用默认箭头光标
            }
        } else if (showSongSelection) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            
            // 默认使用箭头光标
            window.setMouseCursor(arrowCursor);
            
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
            else if (mousePos.x < window.getSize().x * 4.0f / 18.0f) {
                float windowHeight = window.getSize().y;
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
            window.setMouseCursor(arrowCursor); // 使用默认箭头光标
        }
        
        window.clear();
        
        if (showImage) {
            window.draw(sprite);
        } else if (showButton) {
            // 动态计算当前窗口大小
            float currentWindowWidth = window.getSize().x;
            float currentWindowHeight = window.getSize().y;
            
            // 1. 动态调整背景图大小以适应窗口
            if (startTexture.getSize().x > 0 && startTexture.getSize().y > 0) {
                float scaleX = currentWindowWidth / static_cast<float>(startTexture.getSize().x);
                float scaleY = currentWindowHeight / static_cast<float>(startTexture.getSize().y);
                startSprite.setScale(scaleX, scaleY);
                startSprite.setPosition(0, 0);
            }
            window.draw(startSprite);
            
            // 2. 动态调整按钮大小和位置
            float buttonWidth = currentWindowWidth * 0.1875f; // 150/800 = 0.1875
            float buttonHeight = currentWindowHeight * 0.0833f; // 50/600 = 0.0833
            button.setSize(sf::Vector2f(buttonWidth, buttonHeight));
            button.setPosition(currentWindowWidth * 0.40625f, currentWindowHeight * 0.8333f); // 325/800 = 0.40625, 500/600 = 0.8333
            
            // 3. 动态调整按钮文字大小和位置
            float buttonTextSize = currentWindowHeight * 0.0333f; // 20/600 = 0.0333
            buttonText.setCharacterSize(static_cast<unsigned int>(buttonTextSize));
            buttonText.setPosition(
                currentWindowWidth * 0.4375f, // 350/800 = 0.4375
                currentWindowHeight * 0.85f // 510/600 = 0.85
            );
            
            // 4. 动态调整标题文字大小
            float titleTextSize = currentWindowHeight * 0.0833f; // 50/600 = 0.0833
            musicText.setCharacterSize(static_cast<unsigned int>(titleTextSize));
            bangText.setCharacterSize(static_cast<unsigned int>(titleTextSize));
            exclamationText.setCharacterSize(static_cast<unsigned int>(titleTextSize));
            
            // 5. 动态调整副标题文字大小
            float subtitleTextSize = currentWindowHeight * 0.05f; // 30/600 = 0.05
            for (auto& charText : subtitleChars) {
                charText.setCharacterSize(static_cast<unsigned int>(subtitleTextSize));
            }
            
            // 6. 重新设置文字原点，因为文字大小改变后，localBounds也会改变
            // 设置标题文字原点
            musicText.setOrigin(musicText.getLocalBounds().left + musicText.getLocalBounds().width / 2.0f, 
                               musicText.getLocalBounds().top + musicText.getLocalBounds().height / 2.0f);
            bangText.setOrigin(bangText.getLocalBounds().left + bangText.getLocalBounds().width / 2.0f, 
                              bangText.getLocalBounds().top + bangText.getLocalBounds().height / 2.0f);
            exclamationText.setOrigin(exclamationText.getLocalBounds().left + exclamationText.getLocalBounds().width / 2.0f, 
                                     exclamationText.getLocalBounds().top + exclamationText.getLocalBounds().height / 2.0f);
            
            // 设置副标题文字原点
            for (auto& charText : subtitleChars) {
                charText.setOrigin(charText.getLocalBounds().left + charText.getLocalBounds().width / 2.0f, 
                                  charText.getLocalBounds().top + charText.getLocalBounds().height / 2.0f);
            }
            
            // 英文标题固定在窗口高度的2/5处
            float englishTitleY = currentWindowHeight * 2.0f / 5.0f;
            
            // 重新计算标题各部分的最终位置（居中排列）
            float musicWidth = musicText.getLocalBounds().width;
            float bangWidth = bangText.getLocalBounds().width;
            float exclamationWidth = exclamationText.getLocalBounds().width;
            
            // 简化标题位置计算，使用更直接的方法避免重叠
            // 计算整个标题区域的中心点
            float centerX = currentWindowWidth / 2.0f;
            
            // 计算每个元素的起始位置，确保有足够间距
            float elementSpacing = 30.0f; // 元素之间的固定间距
            
            // 计算整个标题的总宽度
            float totalTitleWidth = musicWidth + bangWidth + exclamationWidth + elementSpacing * 2;
            
            // 计算起始X位置
            float titleStartX = centerX - totalTitleWidth / 2.0f;
            
            // 直接设置每个元素的位置
            sf::Vector2f musicFinalPos(titleStartX + musicWidth / 2.0f, englishTitleY);
            sf::Vector2f bangFinalPos(titleStartX + musicWidth + elementSpacing + bangWidth / 2.0f, englishTitleY);
            sf::Vector2f exclamationFinalPos(titleStartX + musicWidth + elementSpacing + bangWidth + elementSpacing + exclamationWidth / 2.0f, englishTitleY);
            
            // 动画逻辑
            float elapsedTime = animationClock.getElapsedTime().asSeconds();
            
            // 检查是否应该开始动画
            if (!animationStarted && elapsedTime >= ANIMATION_DELAY) {
                animationStarted = true;
                animationClock.restart(); // 重新计时动画时长
                elapsedTime = 0.0f;
            }
            
            if (animationStarted) {
                // 计算动画进度（不限制在1.0f，以便实现感叹号的延迟动画）
                float progress = elapsedTime / ANIMATION_DURATION;
                
                // 其他字符的动画在进度达到1.0f时完成
                float otherCharsProgress = std::min(progress, 1.0f);
                
                // 实现music文字从上方飞入
                float musicY = -50.0f + (musicFinalPos.y - (-50.0f)) * otherCharsProgress;
                musicText.setPosition(musicFinalPos.x, musicY);
                
                // 实现bang文字从右方飞入
                float bangX = currentWindowWidth + 50.0f + (bangFinalPos.x - (currentWindowWidth + 50.0f)) * otherCharsProgress;
                bangText.setPosition(bangX, bangFinalPos.y);
                
                // 实现副标题文字逐个从下方进入
                for (size_t i = 0; i < subtitleChars.size(); i++) {
                    // 计算每个字符的延迟开始时间，使它们依次进入
                    float charDelay = static_cast<float>(i) * 0.1f;
                    float charProgress = std::max(0.0f, (otherCharsProgress - charDelay) / (1.0f - charDelay));
                    charProgress = std::min(charProgress, 1.0f);
                    
                    sf::Text& charText = subtitleChars[i];
                    
                    // 中文标题固定在窗口高度的3/5处
                    float chineseTitleY = currentWindowHeight * 3.0f / 5.0f;
                    
                    // 重新计算副标题位置
                    float subtitleTotalWidth = 0.0f;
                    float charSpacing = 5.0f; // 每个中文字符之间的间距
                    for (const auto& c : subtitleChars) {
                        subtitleTotalWidth += c.getLocalBounds().width + charSpacing;
                    }
                    if (!subtitleChars.empty()) {
                        subtitleTotalWidth -= charSpacing; // 减去最后一个字符的额外间距
                    }
                    
                    // 计算副标题起始位置
                    float subtitleStartX = currentWindowWidth / 2.0f - subtitleTotalWidth / 2.0f;
                    float currentX = subtitleStartX;
                    
                    // 更新副标题字符位置
                    for (size_t j = 0; j < subtitleChars.size(); j++) {
                        sf::Text& c = subtitleChars[j];
                        float charWidth = c.getLocalBounds().width;
                        sf::Vector2f finalPos(currentX + charWidth / 2.0f, chineseTitleY);
                        c.setPosition(finalPos.x, c.getPosition().y);
                        currentX += charWidth + charSpacing;
                    }
                    
                    // 从下方飞入，最终停在3/5处
                    float charY = currentWindowHeight + 50.0f + (chineseTitleY - (currentWindowHeight + 50.0f)) * charProgress;
                    charText.setPosition(charText.getPosition().x, charY);
                }
                
                // 实现感叹号逐渐浮现并带有缩放效果（在其他字符就位后等待0.3秒开始）
                const float EXCLAMATION_DELAY = 0.3f; // 其他字符就位后等待的时间
                const float EXCLAMATION_START_TIME = ANIMATION_DURATION + EXCLAMATION_DELAY;
                const float EXCLAMATION_ANIMATION_DURATION = 1.0f; // 感叹号动画本身的时长
                
                if (elapsedTime >= EXCLAMATION_START_TIME) {
                    // 感叹号动画开始
                    float exclamationProgress = std::min((elapsedTime - EXCLAMATION_START_TIME) / EXCLAMATION_ANIMATION_DURATION, 1.0f);
                    
                    // 透明度变化：从透明到完全显示
                    float alpha = 255.0f * exclamationProgress;
                    exclamationText.setFillColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(alpha)));
                    
                    // 更新感叹号位置
                    exclamationText.setPosition(exclamationFinalPos);
                    
                    // 缩放效果：逐渐放大超过原大小50%，再缩小成正常大小
                    float scaleProgress;
                    if (exclamationProgress <= 0.6f) {
                        // 前60%时间：从0放大到1.5f倍（超过原大小50%）
                        scaleProgress = 1.5f * (exclamationProgress / 0.6f);
                    } else {
                        // 后40%时间：从1.5f倍缩小到1倍
                        scaleProgress = 1.5f - 0.5f * ((exclamationProgress - 0.6f) / 0.4f);
                    }
                    
                    exclamationText.setScale(scaleProgress, scaleProgress);
                } else {
                    // 确保在其他字符就位并等待期间，感叹号保持完全透明和初始缩放
                    exclamationText.setFillColor(sf::Color::Transparent);
                    exclamationText.setScale(1.0f, 1.0f);
                    exclamationText.setPosition(exclamationFinalPos); // 确保位置正确
                }
            } else {
                // 非动画状态下，确保标题位置正确
                musicText.setPosition(musicFinalPos);
                bangText.setPosition(bangFinalPos);
                exclamationText.setPosition(exclamationFinalPos);
                
                // 确保副标题位置正确
                float chineseTitleY = currentWindowHeight * 3.0f / 5.0f;
                float subtitleTotalWidth = 0.0f;
                float charSpacing = 5.0f;
                for (const auto& c : subtitleChars) {
                    subtitleTotalWidth += c.getLocalBounds().width + charSpacing;
                }
                if (!subtitleChars.empty()) {
                    subtitleTotalWidth -= charSpacing;
                }
                float subtitleStartX = currentWindowWidth / 2.0f - subtitleTotalWidth / 2.0f;
                float currentX = subtitleStartX;
                for (size_t j = 0; j < subtitleChars.size(); j++) {
                    sf::Text& c = subtitleChars[j];
                    float charWidth = c.getLocalBounds().width;
                    sf::Vector2f finalPos(currentX + charWidth / 2.0f, chineseTitleY);
                    c.setPosition(finalPos);
                    currentX += charWidth + charSpacing;
                }
            }
            
            window.draw(button);
            window.draw(buttonText);
            window.draw(musicText);
            window.draw(bangText);
            window.draw(exclamationText);
            
            // 绘制副标题字符
            for (const auto& charText : subtitleChars) {
                window.draw(charText);
            }
        } else if (showSongSelection) {
            // 计算界面元素位置和大小
            float windowWidth = window.getSize().x;
            float windowHeight = window.getSize().y;
            
            // 左侧宽度：4/18，右侧宽度：14/18
            float leftWidth = windowWidth * 4.0f / 18.0f;
            float rightWidth = windowWidth * 14.0f / 18.0f;
            
            // 设置背景位置和大小
            leftBg.setPosition(0, 0);
            leftBg.setSize(sf::Vector2f(leftWidth, windowHeight));
            rightBg.setPosition(leftWidth, 0);
            rightBg.setSize(sf::Vector2f(rightWidth, windowHeight));
            
            // 绘制背景
            window.draw(leftBg);
            window.draw(rightBg);
            
            // 绘制右侧选中歌曲图片
            // 动态调整选中歌曲图片的大小和位置
            if (selectedSongTexture.getSize().x > 0 && selectedSongTexture.getSize().y > 0) {
                float scaleX = rightWidth / static_cast<float>(selectedSongTexture.getSize().x);
                float scaleY = windowHeight / static_cast<float>(selectedSongTexture.getSize().y);
                selectedSongSprite.setScale(scaleX, scaleY);
                selectedSongSprite.setPosition(leftWidth, 0);
            }
            window.draw(selectedSongSprite);
            
            // 动态调整Bang按钮大小和位置
            float buttonWidth = rightWidth * 0.3f;
            float buttonHeight = windowHeight * 0.08f;
            bangButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
            bangButton.setFillColor(sf::Color(100, 100, 255, 200));
            bangButton.setOutlineColor(sf::Color::White);
            bangButton.setOutlineThickness(2);
            bangButton.setPosition(leftWidth + (rightWidth - buttonWidth) / 2.0f, windowHeight - buttonHeight - 20.0f);
            
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
                promptBox.setSize(sf::Vector2f(windowWidth, windowHeight));
                promptBox.setFillColor(sf::Color(0, 0, 0, 180)); // 半透明黑色蒙层
                promptBox.setPosition(0, 0);
                
                // 设置提示框文字，手动换行
                promptText.setFont(songFont);
                promptText.setString(L"为了更好的游玩体验，\n将会默认全屏加载。\n您可以通过Esc退出全屏模式");
                promptText.setCharacterSize(static_cast<unsigned int>(windowHeight * 0.04f));
                promptText.setFillColor(sf::Color::White);
                
                // 计算文字位置，留出下方按钮空间
                sf::FloatRect promptTextBounds = promptText.getLocalBounds();
                float textX = windowWidth / 2.0f - promptTextBounds.width / 2.0f;
                float textY = windowHeight / 2.0f - promptTextBounds.height / 2.0f - 30.0f;
                promptText.setPosition(textX, textY);
                
                // 设置开始按钮
                sf::RectangleShape startButton;
                float startButtonWidth = windowWidth * 0.2f;
                float startButtonHeight = windowHeight * 0.08f;
                startButton.setSize(sf::Vector2f(startButtonWidth, startButtonHeight));
                startButton.setFillColor(sf::Color(100, 100, 255, 200));
                startButton.setOutlineColor(sf::Color::White);
                startButton.setOutlineThickness(2);
                startButton.setPosition(
                    windowWidth / 2.0f - startButtonWidth / 2.0f,
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
                
                // 检查鼠标是否悬停在开始按钮上
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (startButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    window.setMouseCursor(handCursor);
                }
                
                // 绘制半透明蒙层、文字和按钮
                window.draw(promptBox);
                window.draw(promptText);
                window.draw(startButton);
                window.draw(startButtonText);
                
                // 检查开始按钮点击
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    if (startButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        // 开始游戏，设置为全屏模式
                        window.create(sf::VideoMode::getFullscreenModes()[0], "Music Bang", sf::Style::Fullscreen);
                        showFullscreenPrompt = false;
                        // 这里可以添加游戏开始的逻辑
                    }
                }
            }
            
            // 设置三角形按钮位置和大小
            float triangleButtonWidth = leftWidth * 0.6f;
            float triangleButtonHeight = 30;
            
            // 上一页按钮（向上三角形）
            prevButton.setPoint(0, sf::Vector2f(leftWidth / 2.0f - triangleButtonWidth / 2.0f, triangleButtonHeight));
            prevButton.setPoint(1, sf::Vector2f(leftWidth / 2.0f + triangleButtonWidth / 2.0f, triangleButtonHeight));
            prevButton.setPoint(2, sf::Vector2f(leftWidth / 2.0f, 0));
            
            // 下一页按钮（向下三角形）
            nextButton.setPoint(0, sf::Vector2f(leftWidth / 2.0f - triangleButtonWidth / 2.0f, windowHeight - triangleButtonHeight));
            nextButton.setPoint(1, sf::Vector2f(leftWidth / 2.0f + triangleButtonWidth / 2.0f, windowHeight - triangleButtonHeight));
            nextButton.setPoint(2, sf::Vector2f(leftWidth / 2.0f, windowHeight));
            
            // 绘制三角形按钮
            window.draw(prevButton);
            window.draw(nextButton);
            
            // 设置页面指示器，字符大小随窗口高度动态调整
            float pageIndicatorSize = windowHeight * 0.0333f; // 原始20px对应600px窗口高度
            pageIndicator.setCharacterSize(static_cast<unsigned int>(pageIndicatorSize));
            pageIndicator.setString("Page " + std::to_string(currentPage) + "/" + std::to_string(pageCount));
            sf::FloatRect indicatorBounds = pageIndicator.getLocalBounds();
            pageIndicator.setPosition(leftWidth / 2.0f - indicatorBounds.width / 2.0f, windowHeight - 2 * pageIndicatorSize);
            window.draw(pageIndicator);
            
            // 计算歌曲区域的位置和大小
            float songAreaHeight = windowHeight * 0.7f; // 歌曲区域占70%高度，留出更多空间
            float songAreaY = windowHeight * 0.15f; // 上移，让顶部留出更多空间
            float songHeight = songAreaHeight / 3.0f;
            
            // 计算当前页显示的歌曲索引范围
            int startIndex = (currentPage - 1) * 3;
            int endIndex = std::min(startIndex + 3, songCount);
            
            // 绘制歌曲区域
            for (int i = 0; i < 3; i++) {
                int songIndex = startIndex + i;
                
                // 设置歌曲区域位置和大小
                songRects[i].setPosition(0, songAreaY + i * songHeight);
                songRects[i].setSize(sf::Vector2f(leftWidth, songHeight));
                songRects[i].setFillColor(sf::Color(80, 80, 80, 200));
                songRects[i].setOutlineColor(sf::Color::White);
                songRects[i].setOutlineThickness(1);
                
                if (songIndex < endIndex) {
                    // 加载并显示缩略图
                    if (songThumbnailTextures[i].loadFromFile("./resource/" + songs[songIndex].thumbnailPath)) {
                        songThumbnails[i].setTexture(songThumbnailTextures[i]);
                        // 调整缩略图大小以适应歌曲区域
                        float scaleX = leftWidth / songThumbnailTextures[i].getSize().x;
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
                    float songNameSize = windowHeight * 0.0333f; // 20/600 = 0.0333
                    songNames[i].setCharacterSize(static_cast<unsigned int>(songNameSize));
                    songNames[i].setFillColor(sf::Color::White);
                    
                    // 居中显示歌曲名字
                    sf::FloatRect nameBounds = songNames[i].getLocalBounds();
                    songNames[i].setPosition(
                        leftWidth / 2.0f - nameBounds.width / 2.0f,
                        songAreaY + i * songHeight + songHeight / 2.0f - nameBounds.height / 2.0f
                    );
                    window.draw(songNames[i]);
                } else {
                    // 空的歌曲区域
                    songRects[i].setFillColor(sf::Color(60, 60, 60, 150));
                    window.draw(songRects[i]);
                }
            }
        }
        
        window.display();
    }
    return 0;
}