#include "PlayState.h"
#include "GameManager.h"
#include "MenuState.hpp"
#include <fstream>
#include <iostream>

PlayState::PlayState(GameManager* gameManager)
    : m_HighScoreText(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_ScoreText(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_LevelText(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_pauseTitleText(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_resumeLabel(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_loadLabel(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_volumeLabel(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_exitLabel(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_goScoreText(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_goHighScoreText(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_goRestartBtn(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_goMenuBtn(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_goSettingBtn(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_goQuitBtn(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_setSfxBtn(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_setMusicBtn(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_setResetBtn(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_setBackBtn(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_menuLabel(AssetManager::getInstance().getFont("DIMIS___.ttf")),      
    menuBtn(AssetManager::getInstance().getTexture("menu.png")),          
    m_pauseHUDBtn(AssetManager::getInstance().getTexture("pause-button.png")),
    resumeBtn(AssetManager::getInstance().getTexture("play-button.png")),
    loadBtn(AssetManager::getInstance().getTexture("loading.png")),
    volumeBtn(AssetManager::getInstance().getTexture("settings.png")),
    exitBtn(AssetManager::getInstance().getTexture("exit.png")),
    m_gameOverBanner(AssetManager::getInstance().getTexture("GAMEOVER.png")),
	m_fireEffectSprite(AssetManager::getInstance().getTexture("Fire.png"))
{
    // ... Phần thân hàm bên dưới giữ nguyên chuẩn rồi!
    mGameManager = gameManager;
    m_overlayState = OverlayState::NONE;
    m_previousOverlayState = OverlayState::NONE;

    m_isSFXOn = true;
    m_isMusicOn = true;
    m_isMousePressed = false;
    m_isEscPressed = false;

    mScore = 0;
    mlevel = 1;
    m_SpeedBonus = 0.f;
    m_IsGameOver = false;

    m_Player = new CPEOPLE(510.f, 700.f);
    m_SpawnTimer = 0.f;
    m_NextSpawnTime = 1.5f;

	m_fireAnimation = new Animation(&AssetManager::getInstance().getTexture("Fire.png"), { 8, 1 }, 0.1f);
     m_collisionEffectTimer = 0.f;
	 m_isShowingCollisionEffect = false;
    m_fireEffectSprite.setScale({ 4.f,4.f });

    sf::FloatRect bounds = m_fireEffectSprite.getLocalBounds();
    m_fireEffectSprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });

    loadHighScore();
    generateLevel();

    // ------------------------------------------------------------------------
    // 1. HUD GÓC MÀN HÌNH
    // ------------------------------------------------------------------------
    m_HighScoreText.setCharacterSize(28);
    m_HighScoreText.setFillColor(sf::Color(255, 215, 0));
    m_HighScoreText.setOutlineColor(sf::Color::Black);
    m_HighScoreText.setOutlineThickness(2.f);
    m_HighScoreText.setPosition({ 20.f, 15.f });

    m_ScoreText.setCharacterSize(26);
    m_ScoreText.setFillColor(sf::Color::White);
    m_ScoreText.setOutlineColor(sf::Color::Black);
    m_ScoreText.setOutlineThickness(2.f);
    m_ScoreText.setPosition({ 20.f, 55.f });

    m_LevelText.setCharacterSize(26);
    m_LevelText.setFillColor(sf::Color(100, 255, 100));
    m_LevelText.setOutlineColor(sf::Color::Black);
    m_LevelText.setOutlineThickness(2.f);
    m_LevelText.setPosition({ 20.f, 95.f });

    m_pauseHUDBtn.setScale({ 0.12f, 0.12f });
    sf::FloatRect hudBtnBounds = m_pauseHUDBtn.getLocalBounds();
    m_pauseHUDBtn.setOrigin({ hudBtnBounds.size.x / 2.f, hudBtnBounds.size.y / 2.f });
    m_pauseHUDBtn.setPosition({ 1540.f, 45.f });

    // ------------------------------------------------------------------------
    // 2. PAUSE POPUP UI
    // ------------------------------------------------------------------------
    m_pauseOverlay.setSize({ 1600.f, 800.f });
    m_pauseOverlay.setFillColor(sf::Color(0, 0, 0, 210));

    m_pauseMenuBox.setSize({ 460.f, 550.f });
    m_pauseMenuBox.setOrigin({ 230.f, 275.f });
    m_pauseMenuBox.setPosition({ 800.f, 400.f });
    m_pauseMenuBox.setFillColor(sf::Color(40, 44, 52));
    m_pauseMenuBox.setOutlineThickness(4.f);
    m_pauseMenuBox.setOutlineColor(sf::Color(255, 200, 50));

    m_pauseTitleText.setString("GAME PAUSED");
    m_pauseTitleText.setCharacterSize(42);
    m_pauseTitleText.setFillColor(sf::Color(255, 200, 50));
    sf::FloatRect titleBounds = m_pauseTitleText.getLocalBounds();
    m_pauseTitleText.setOrigin({ titleBounds.size.x / 2.f, titleBounds.size.y / 2.f });
    m_pauseTitleText.setPosition({ 800.f, 160.f });

    auto setupPopupRow = [](sf::Sprite& sprite, sf::Text& text, const std::string& str, float yPos) {
        sprite.setScale({ 0.12f, 0.12f });
        sf::FloatRect b = sprite.getLocalBounds();
        sprite.setOrigin({ b.size.x / 2.f, b.size.y / 2.f });
        sprite.setPosition({ 660.f, yPos });

        text.setString(str);
        text.setCharacterSize(28);
        text.setFillColor(sf::Color::White);
        text.setPosition({ 720.f, yPos - 18.f });
        };

    setupPopupRow(resumeBtn, m_resumeLabel, "RESUME", 240.f);
    setupPopupRow(loadBtn, m_loadLabel, "LOAD GAME", 310.f);
    setupPopupRow(volumeBtn, m_volumeLabel, "SETTING", 380.f);
    setupPopupRow(menuBtn, m_menuLabel, "MENU", 450.f);
    setupPopupRow(exitBtn, m_exitLabel, "EXIT", 520.f);

    // ------------------------------------------------------------------------
    // 3. GAME OVER UI
    // ------------------------------------------------------------------------
    sf::FloatRect goBounds = m_gameOverBanner.getLocalBounds();
    m_gameOverBanner.setOrigin({ goBounds.size.x / 2.f, goBounds.size.y / 2.f });
    m_gameOverBanner.setPosition({ 800.f, 180.f });

    auto setupTextButton = [](sf::Text& text, const std::string& str, float yPos, unsigned int size = 36) {
        text.setString(str);
        text.setCharacterSize(size);
        text.setFillColor(sf::Color(240, 235, 200));
        sf::FloatRect b = text.getLocalBounds();
        text.setOrigin({ b.size.x / 2.f, b.size.y / 2.f });
        text.setPosition({ 800.f, yPos });
        };

    m_goScoreText.setCharacterSize(36);
    m_goScoreText.setFillColor(sf::Color(255, 180, 210));

    m_goHighScoreText.setCharacterSize(36);
    m_goHighScoreText.setFillColor(sf::Color(255, 180, 210));

    setupTextButton(m_goRestartBtn, "RESTART", 420.f);
    setupTextButton(m_goMenuBtn, "MENU", 480.f);
    setupTextButton(m_goSettingBtn, "SETTING", 540.f);
    setupTextButton(m_goQuitBtn, "QUIT", 600.f);

    // ------------------------------------------------------------------------
    // 4. SETTINGS MENU UI
    // ------------------------------------------------------------------------
    setupTextButton(m_setSfxBtn, "SFX: ON", 300.f, 40);
    setupTextButton(m_setMusicBtn, "MUSIC: ON", 370.f, 40);
    setupTextButton(m_setResetBtn, "RESET", 440.f, 40);
    setupTextButton(m_setBackBtn, "BACK", 510.f, 40);

    // 1. Load Nhạc nền (Đảm bảo file CROSSY nằm đúng thư mục ASSETS/AUDIO/)
    //if (m_bgMusic.openFromFile("ASSETS/AuCROSSY.wav")) {
    //    m_bgMusic.setLooping(true);
    //    m_bgMusic.setVolume(50.f);
    //    m_bgMusic.play();
    //}

    // 2. Load SFX (Dùng đúng tên các file có trong thư mục của bạn)
    // Giả sử tất cả nằm trong thư mục ASSETS/AUDIO/
    //m_crashBuffer.loadFromFile("ASSETS/AUDIO/CAR_LARGE.wav");
    //m_crashSound.emplace(m_crashBuffer);

    //m_gameOverBuffer.loadFromFile("ASSETS/AUDIO/GAME_SOUND.wav");
    //m_gameOverSound.emplace(m_gameOverBuffer);

    //m_levelUpBuffer.loadFromFile("ASSETS/AUDIO/COIN.wav");
    //m_levelUpSound.emplace(m_levelUpBuffer);

    //m_honkBuffer.loadFromFile("ASSETS/AUDIO/CAR_SMALL.wav");
    //m_honkSound.emplace(m_honkBuffer);

    //m_meowBuffer.loadFromFile("ASSETS/AUDIO/MEOW.wav");
    //m_meowSound.emplace(m_meowBuffer);
}

PlayState::~PlayState()
{
    for (auto coin : m_Coins) {
        delete coin;
    }
    m_Coins.clear();

    delete m_Player;
    for (auto obs : m_Obstacles) {
        delete obs;
    }

	delete m_fireAnimation;
}

void PlayState::Init() {}

void PlayState::Update(float delTime, sf::RenderWindow& window)
{

    if (m_isShowingCollisionEffect)
    {
        m_collisionEffectTimer -= delTime; 

       
        if (m_fireAnimation) {
             m_fireAnimation->Update(0, delTime, true);
             m_fireEffectSprite.setTextureRect(m_fireAnimation->uvRect);
         }

        
        if (m_collisionEffectTimer <= 0.f)
        {
            m_isShowingCollisionEffect = false;

            m_IsGameOver = true; 
            m_overlayState = OverlayState::GAME_OVER;
            std::cout << "[!] EFFECT FINISHED -> GAME OVER\n";
        }

        return; // *** QUAN TRỌNG: Thoát Update tại đây để "đóng băng" mọi chuyển động khác ***
    }

    
    // 1. BẤM ESC ĐỂ PAUSE / RESUME
    bool isEsc = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape);
    if (isEsc && !m_isEscPressed) {
        if (m_overlayState == OverlayState::NONE) {
            m_overlayState = OverlayState::PAUSE_MENU;
        }
        else if (m_overlayState == OverlayState::PAUSE_MENU) {
            m_overlayState = OverlayState::NONE;
        }
        else if (m_overlayState == OverlayState::SETTINGS_MENU) {
            m_overlayState = m_previousOverlayState;
        }
    }
    m_isEscPressed = isEsc;

    // 2. CẬP NHẬT GAMEPLAY (CHỈ CHẠY KHI KHÔNG CO OVERLAY)
    if (m_overlayState == OverlayState::NONE)
    {
        m_Player->update(delTime);

        if (m_Player->getY() <= 0.f) {
            levelUp();
            return;
        }

        spawnObstacle(delTime);

        for (auto coin : m_Coins)
        {
            coin->Update(delTime);
        }
        for (auto obs : m_Obstacles) {
            obs->UpdateState(delTime);
        }

        checkCollision(); // Hàm này sẽ chuyển m_overlayState = OverlayState::GAME_OVER khi đụng vật cản
        cleanUpOffScreen(window);
    }

    // 3. CẬP NHẬT HIGHSCORE
    if (mScore > mHighScore) {
        mHighScore = mScore;
        saveHighScore();
    }

    m_HighScoreText.setString("HIGH SCORE: " + std::to_string(mHighScore));
    m_ScoreText.setString("Score: " + std::to_string(mScore));
    m_LevelText.setString("Level: " + std::to_string(mlevel));

    // 4. XỬ LÝ CHUỘT (HOVER & CLICK)
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
    if (m_overlayState == OverlayState::PAUSE_MENU)
    {
        auto applyHoverPopup = [&mousePos](sf::Sprite& sprite, sf::Text& text) {
            bool isHovered = sprite.getGlobalBounds().contains(mousePos) || text.getGlobalBounds().contains(mousePos);
            if (isHovered) {
                text.setFillColor(sf::Color(255, 215, 0));        // Chữ đổi sang màu vàng kim sáng
                sprite.setColor(sf::Color(255, 255, 180));        // Icon sáng bừng nhẹ
            }
            else {
                text.setFillColor(sf::Color::White);               // Mặc định chữ trắng
                sprite.setColor(sf::Color::White);                // Icon bình thường
            }
            };

        applyHoverPopup(resumeBtn, m_resumeLabel);
        applyHoverPopup(loadBtn, m_loadLabel);
        applyHoverPopup(volumeBtn, m_volumeLabel);
        applyHoverPopup(menuBtn, m_menuLabel);
        applyHoverPopup(exitBtn, m_exitLabel);
    }
    bool isClicking = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

    // --- HIỆU ỨNG RÊ CHUỘT (HOVER) ---
    auto applyHover = [&mousePos](sf::Text& text) {
        if (text.getGlobalBounds().contains(mousePos)) {
            text.setFillColor(sf::Color(255, 200, 50)); // Màu vàng khi rê chuột vào
        }
        else {
            text.setFillColor(sf::Color(240, 235, 200)); // Màu mặc định
        }
        };

    if (m_overlayState == OverlayState::GAME_OVER) {
        applyHover(m_goRestartBtn);
        applyHover(m_goMenuBtn);
        applyHover(m_goSettingBtn);
        applyHover(m_goQuitBtn);
    }
    else if (m_overlayState == OverlayState::SETTINGS_MENU) {
        applyHover(m_setSfxBtn);
        applyHover(m_setMusicBtn);
        applyHover(m_setResetBtn);
        applyHover(m_setBackBtn);
    }

    // --- XỬ LÝ CLICK CHUỘT ---
    if (isClicking && !m_isMousePressed)
    {
        // ----------------- TƯƠNG TÁC KHI ĐANG CHƠI -----------------
        if (m_overlayState == OverlayState::NONE) {
            if (m_pauseHUDBtn.getGlobalBounds().contains(mousePos)) {
                m_overlayState = OverlayState::PAUSE_MENU;
            }
        }
        // ----------------- TƯƠNG TÁC PAUSE MENU -----------------
        else if (m_overlayState == OverlayState::PAUSE_MENU) {
            if (resumeBtn.getGlobalBounds().contains(mousePos) || m_resumeLabel.getGlobalBounds().contains(mousePos)) {
                m_overlayState = OverlayState::NONE;
            }
            else if (loadBtn.getGlobalBounds().contains(mousePos) || m_loadLabel.getGlobalBounds().contains(mousePos)) {
                std::ifstream gameIn("game.dat", std::ios::binary);
                if (gameIn.is_open()) {
                    float savedX, savedY;
                    gameIn.read(reinterpret_cast<char*>(&savedX), sizeof(savedX));
                    gameIn.read(reinterpret_cast<char*>(&savedY), sizeof(savedY));
                    gameIn.read(reinterpret_cast<char*>(&mScore), sizeof(mScore));
                    gameIn.read(reinterpret_cast<char*>(&mlevel), sizeof(mlevel));
                    gameIn.close();

                    m_Player->resetPosition(savedX, savedY);
                    m_overlayState = OverlayState::NONE;
                }
            }
            else if (volumeBtn.getGlobalBounds().contains(mousePos) || m_volumeLabel.getGlobalBounds().contains(mousePos)) {
                m_previousOverlayState = OverlayState::PAUSE_MENU;
                m_overlayState = OverlayState::SETTINGS_MENU;
            }
            else if (menuBtn.getGlobalBounds().contains(mousePos) || m_menuLabel.getGlobalBounds().contains(mousePos)) {
                mGameManager->setState(new MenuState(mGameManager));
                return;
            }
            // 5. EXIT (Thoát game hẳn)
            else if (exitBtn.getGlobalBounds().contains(mousePos) || m_exitLabel.getGlobalBounds().contains(mousePos)) {
                window.close();
            }
        }
        // ----------------- TƯƠNG TÁC GAME OVER -----------------
        else if (m_overlayState == OverlayState::GAME_OVER) {
            if (m_goRestartBtn.getGlobalBounds().contains(mousePos)) {
                mGameManager->setState(new PlayState(mGameManager));
                return;
            }
            else if (m_goMenuBtn.getGlobalBounds().contains(mousePos)) {
                mGameManager->setState(new MenuState(mGameManager));
                return;
            }
            else if (m_goSettingBtn.getGlobalBounds().contains(mousePos)) {
                m_previousOverlayState = OverlayState::GAME_OVER;
                m_overlayState = OverlayState::SETTINGS_MENU;
            }
            else if (m_goQuitBtn.getGlobalBounds().contains(mousePos)) {
                window.close();
            }
        }
        // ----------------- TƯƠNG TÁC SETTINGS MENU -----------------
        else if (m_overlayState == OverlayState::SETTINGS_MENU) {
            if (m_setSfxBtn.getGlobalBounds().contains(mousePos)) {
                m_isSFXOn = !m_isSFXOn;
                m_setSfxBtn.setString("SFX: " + std::string(m_isSFXOn ? "ON" : "OFF"));
            }
            else if (m_setMusicBtn.getGlobalBounds().contains(mousePos)) {
                m_isMusicOn = !m_isMusicOn;
                m_setMusicBtn.setString("MUSIC: " + std::string(m_isMusicOn ? "ON" : "OFF"));
                sf::Listener::setGlobalVolume(m_isMusicOn ? 100.f : 0.f);
            }
            else if (m_setResetBtn.getGlobalBounds().contains(mousePos)) {
                m_isSFXOn = true;
                m_isMusicOn = true;
                m_setSfxBtn.setString("SFX: ON");
                m_setMusicBtn.setString("MUSIC: ON");
                sf::Listener::setGlobalVolume(100.f);
            }
            else if (m_setBackBtn.getGlobalBounds().contains(mousePos)) {
                m_overlayState = m_previousOverlayState; // Quay lại màn hình trước đó (Game Over hoặc Pause)
            }

            // Căn giữa lại origin chữ khi chuỗi thay đổi
            auto reCenter = [](sf::Text& t) {
                sf::FloatRect b = t.getLocalBounds();
                t.setOrigin({ b.size.x / 2.f, b.size.y / 2.f });
                };
            reCenter(m_setSfxBtn);
            reCenter(m_setMusicBtn);
        }
    }
    m_isMousePressed = isClicking;
}

void PlayState::Render(sf::RenderWindow& window)
{
   

    // 1. VẼ GAME WORLD
    for (auto& lane : m_Lanes) {
        window.draw(lane.bgSprite);
    }
    for (auto obs : m_Obstacles) {
        obs->Draw(window);
    }
    m_Player->Draw(window);

    // 2. VẼ HUD
    window.draw(m_HighScoreText);
    window.draw(m_ScoreText);
    window.draw(m_LevelText);
    window.draw(m_pauseHUDBtn);

    // 3. VẼ OVERLAY MÀN MỜ
    if (m_overlayState != OverlayState::NONE) {
        window.draw(m_pauseOverlay);
    }

    // 4. VẼ THEO TRẠNG THÁI OVERLAY
    if (m_overlayState == OverlayState::PAUSE_MENU)
    {
        window.draw(m_pauseMenuBox);
        window.draw(m_pauseTitleText);
        window.draw(resumeBtn);
        window.draw(m_resumeLabel);
        window.draw(loadBtn);
        window.draw(m_loadLabel);
        window.draw(volumeBtn);
        window.draw(m_volumeLabel);
        window.draw(menuBtn);
        window.draw(m_menuLabel);
        window.draw(exitBtn);
        window.draw(m_exitLabel);
    }
    else if (m_overlayState == OverlayState::GAME_OVER)
    {
        window.draw(m_gameOverBanner);

        // Score & Highscore hiển thị song song ở giữa
        m_goScoreText.setString("SCORE : " + std::to_string(mScore));
        m_goHighScoreText.setString("HIGHSCORE : " + std::to_string(mHighScore));

        m_goScoreText.setPosition({ 600.f, 320.f });
        m_goHighScoreText.setPosition({ 1000.f, 320.f });

        sf::FloatRect sb = m_goScoreText.getLocalBounds();
        m_goScoreText.setOrigin({ sb.size.x / 2.f, sb.size.y / 2.f });

        sf::FloatRect hb = m_goHighScoreText.getLocalBounds();
        m_goHighScoreText.setOrigin({ hb.size.x / 2.f, hb.size.y / 2.f });

        window.draw(m_goScoreText);
        window.draw(m_goHighScoreText);

        window.draw(m_goRestartBtn);
        window.draw(m_goMenuBtn);
        window.draw(m_goSettingBtn);
        window.draw(m_goQuitBtn);
    }
    else if (m_overlayState == OverlayState::SETTINGS_MENU)
    {
        window.draw(m_setSfxBtn);
        window.draw(m_setMusicBtn);
        window.draw(m_setResetBtn);
        window.draw(m_setBackBtn);
    }
    // VẼ ĐỒNG XU
    for (auto coin : m_Coins) {
        coin->Draw(window);
    }

    if (m_isShowingCollisionEffect)
    {
        window.draw(m_fireEffectSprite);
    }
}

void PlayState::generateLevel()
{
    for (auto coin : m_Coins) {
        delete coin;
    }
    m_Coins.clear();

    for (auto obs : m_Obstacles) delete obs;
    m_Obstacles.clear();

    m_Lanes.clear();

    spawnLane(0.f, LaneType::GRASS_EMPTY);
    spawnLane(700.f, LaneType::GRASS_EMPTY);

    int roadCount = 0;
    int consecutiveRoadCount = 0;

    for (int i = 1; i <= 6; i++) {
        float yPos = i * 100.f;
        LaneType chosenType;

        if (roadCount >= 4 || consecutiveRoadCount >= 3) {
            chosenType = LaneType::GRASS;
        }
        else {
            chosenType = (rand() % 2 == 0) ? LaneType::ROAD : LaneType::GRASS;
        }

        if (chosenType == LaneType::ROAD) {
            roadCount++;
            consecutiveRoadCount++;
        }
        else {
            consecutiveRoadCount = 0;
        }

        spawnLane(yPos, chosenType);

        int obstacleCount = 0;
        int consecutiveObstacleCount = 0;
        if (chosenType == LaneType::GRASS) {
            for (int j = 0; j < 16; j++) {
                float xPos = j * 100.f;
                if (rand() % 5 == 0) {
                    m_Coins.push_back(new Coin(xPos + 50, yPos + 50));
                    consecutiveObstacleCount = 0;
                }
                else
                {
                    bool spawnObstacle = (rand() % 5 == 0);

                    if (obstacleCount >= 4 || consecutiveObstacleCount >= 3) {
                        spawnObstacle = false;
                    }

                    if (spawnObstacle) {
                        consecutiveObstacleCount++;
                        obstacleCount++;
                        Obstacle* newObstacle = new StaticObstacle(xPos + 50.f, yPos + 50.f);
                        m_Obstacles.push_back(newObstacle);
                    } 
                    else {
                        consecutiveObstacleCount = 0;
                    }
                }
            }
        }
    }
}

void PlayState::spawnLane(float yPos, LaneType type)
{
    sf::Texture* texture = NULL;
    if (type == LaneType::ROAD) {
        texture = &AssetManager::getInstance().getTexture("road.png");
    }
    else {
        texture = &AssetManager::getInstance().getTexture("Grass.png");
    }

    LaneData newLane(yPos, type, *texture);
    float scaleX = 1600.f / texture->getSize().x;
    newLane.bgSprite.setScale({ scaleX, 1.f });
    m_Lanes.push_back(newLane);
}

void PlayState::spawnObstacle(float delTime)
{
    m_SpawnTimer += delTime;
    if (m_SpawnTimer < m_NextSpawnTime) return;

    std::vector<LaneData> roadLanes;
    for (const auto& lane : m_Lanes) {
        if (lane.type == LaneType::ROAD) roadLanes.push_back(lane);
    }

    if (roadLanes.empty()) return;

    int randomIdx = rand() % roadLanes.size();
    float laneY = roadLanes[randomIdx].yPos;
    float spawnX = 1650.f;

    float rightMostX = -1000.f;
    for (auto obs : m_Obstacles) {
        if (std::abs(obs->getY() - (laneY + 50.f)) < 10.f) {
            if (obs->getX() > rightMostX) rightMostX = obs->getX();
        }
    }

    if (spawnX - rightMostX < 150.f) return;

    Obstacle* newMovingObstacle = nullptr;
    if (rand() % 2 == 0) {
        newMovingObstacle = new CVEHICLE(spawnX + 50.f, laneY + 45.f);
    }
    else {
        newMovingObstacle = new CANIMAL(spawnX + 50.f, laneY + 30.f);
    }

    float laneBaseSpeed = 100.f + (laneY / 100.f) * 15.f;
    newMovingObstacle->setSpeed(laneBaseSpeed + m_SpeedBonus);
    m_Obstacles.push_back(newMovingObstacle);

    m_SpawnTimer = 0.f;
    m_NextSpawnTime = (rand() % 100 + 100) / 100.f;
    
    //if (rand() % 3 == 0 && m_isSFXOn) {
    //    m_honkSound->play();
    //}

   
    //if (m_isSFXOn) {
    //    m_meowSound->play();
    //}
}

void PlayState::levelUp()
{
    mlevel++;
    mScore += 100;
    m_SpeedBonus += 50.f;

    // Phát tiếng kèn ăn mừng qua màn
    //if (m_isSFXOn) {
    //    m_levelUpSound->play();
    //}

    generateLevel();
    m_Player->resetPosition(500.f, 700.f);
}

void PlayState::checkCollision()
{

    if (m_isShowingCollisionEffect) return;

    sf::FloatRect playerBounds = m_Player->getBounds();

    // 1. Kiểm tra va chạm với Đồng Xu trước
    for (int i = 0; i < m_Coins.size(); i++) {
        if (playerBounds.findIntersection(m_Coins[i]->getBounds())) {
            mScore += 10;            // Cộng 10 điểm

            delete m_Coins[i];
            m_Coins.erase(m_Coins.begin() + i);
            i--; 
        }
    }

    for (auto obs : m_Obstacles) {
        if (playerBounds.findIntersection(obs->getBounds())) {

            m_isShowingCollisionEffect = true;
            m_collisionEffectTimer = 2.0f; 

            float obsPosx = m_Player->getX();
            float obsPosy = m_Player->getY();
            m_fireEffectSprite.setPosition({ obsPosx + 480,obsPosy + 25 });

            std::cout << "\n[!] COLLISION! Showing fire effect...\n";
            return;

            

            // Stop nhạc nền và phát tiếng va chạm + giọng nói
            //m_bgMusic.stop();
            //if (m_isSFXOn) {
            //    m_crashSound->play();
            //    m_gameOverSound->play(); // Phát "Abort the mission!"
            //}
          
        }
    }
}

void PlayState::cleanUpOffScreen(sf::RenderWindow& window)
{
    for (int i = m_Obstacles.size() - 1; i >= 0; i--) {
        if (m_Obstacles[i]->getX() < -200.f) {
            delete m_Obstacles[i];
            m_Obstacles.erase(m_Obstacles.begin() + i);
        }
    }
}

void PlayState::loadHighScore() {
    std::ifstream file("highscore.dat", std::ios::binary);
    if (file.is_open()) {
        file.read(reinterpret_cast<char*>(&mHighScore), sizeof(mHighScore));
        file.close();
    }
    else {
        mHighScore = 0;
    }
}

void PlayState::saveHighScore() {
    std::ofstream file("highscore.dat", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<char*>(&mHighScore), sizeof(mHighScore));
        file.close();
    }
}