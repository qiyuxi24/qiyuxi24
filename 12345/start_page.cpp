#include<SFML/Graphics.hpp>
#include<vector>
#include<string>
#include<cmath>
#include<iostream>

// 歌曲结构体
typedef struct Song {
    std::string name;
    std::string imagePath;
    std::string thumbnailPath;
    std::string audioPath;
    int highScore;
} Song;

// 歌曲选择函数声明
int select(sf::RenderWindow& window, sf::Font& songFont, sf::Cursor& handCursor, sf::Cursor& arrowCursor);

/**
 * 主菜单页面函数
 * @param window SFML窗口对象
 * @param handCursor 手形光标
 * @param arrowCursor 箭头光标
 */
void start_page(sf::RenderWindow& window, sf::Cursor& handCursor, sf::Cursor& arrowCursor) {
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
    
    bool showButton = true; // 控制按钮和文字是否显示
    
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
    
    bool buttonClicked = false;
    
    // 主菜单循环
    while (window.isOpen() && !buttonClicked) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
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
                        buttonClicked = true;
                        break;
                    }
                }
            }
        }
        
        // 处理鼠标悬浮事件
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (button.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            window.setMouseCursor(handCursor);
        } else {
            window.setMouseCursor(arrowCursor); // 使用默认箭头光标
        }
        
        window.clear();
        
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
        
        window.display();
    }
    
    // 如果点击了开始按钮，进入歌曲选择界面
    if (buttonClicked) {
        // 重新加载songFont
        sf::Font songFont;
        if (!songFont.loadFromFile("C:\\Windows\\Fonts\\msyh.ttc")) {
            // 如果没有找到微软雅黑字体，可以使用其他系统字体
        }
        
        // 调用歌曲选择函数
        int selectedIndex = select(window, songFont, handCursor, arrowCursor);
        
        // 这里可以添加游戏开始的逻辑，使用selectedIndex获取选中的歌曲
        std::cout << "Selected song index: " << selectedIndex << std::endl;
    }
}