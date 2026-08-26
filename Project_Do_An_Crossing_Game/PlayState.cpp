#include "PlayState.h"
#include "GameManager.h"
#include "MenuState.hpp"
#include <fstream>
#include <iostream>

PlayState::PlayState(GameManager* gameManager, const std::string& selectedSkin)
    : m_pauseAboutIcon(AssetManager::getInstance().getTexture("aboutus.png")),
    m_pauseAboutLabel(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_pauseAboutTitleText(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_pauseAboutBackIcon(AssetManager::getInstance().getTexture("exit.png")),
    m_pauseAboutBackLabel(AssetManager::getInstance().getFont("DIMIS___.ttf")),

    
    
    m_fireEffectSprite(AssetManager::getInstance().getTexture("Fire.png")), 

    m_HighScoreText(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_ScoreText(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_LevelText(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_pauseHUDBtn(AssetManager::getInstance().getTexture("pause-button.png")),
    m_pauseTitleText(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    resumeBtn(AssetManager::getInstance().getTexture("play-button.png")),
    loadBtn(AssetManager::getInstance().getTexture("loading.png")),
    volumeBtn(AssetManager::getInstance().getTexture("settings.png")),
    menuBtn(AssetManager::getInstance().getTexture("menu.png")),
    exitBtn(AssetManager::getInstance().getTexture("exit.png")),
    m_resumeLabel(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_loadLabel(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_volumeLabel(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_menuLabel(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_exitLabel(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_gameOverBanner(AssetManager::getInstance().getTexture("GAMEOVER.png")),
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
    
    // --- ÂM THANH CHÍNH CỦA GAME NẠP TẠI ĐÂY ---
    m_crashSound(AssetManager::getInstance().getSoundBuffer("CAR_LARGE.wav")),
    m_gameOverSound(AssetManager::getInstance().getSoundBuffer("GAME_SOUND.mp3")),
    m_levelUpSound(AssetManager::getInstance().getSoundBuffer("levelup.mp3")),
    m_honkSound(AssetManager::getInstance().getSoundBuffer("CAR_LARGE.wav")), //?
    m_meowSound(AssetManager::getInstance().getSoundBuffer("MEOW.wav")),
    m_coinSound(AssetManager::getInstance().getSoundBuffer("COIN.wav")),
    m_jumpSound(AssetManager::getInstance().getSoundBuffer("JUMP.mp3"))
{
    //------ Fire and coin -----------
    m_fireAnimation = new Animation(&AssetManager::getInstance().getTexture("Fire.png"), { 8, 1 }, 0.1f);
    m_collisionEffectTimer = 0.f;
    m_isShowingCollisionEffect = false;
    m_fireEffectSprite.setScale({ 4.f,4.f });
    sf::Vector2u texSize = AssetManager::getInstance().getTexture("Fire.png").getSize();
    float frameWidth = static_cast<float>(texSize.x) / 8.f; // 8 frames
    float frameHeight = static_cast<float>(texSize.y);
    m_fireEffectSprite.setOrigin({ frameWidth / 2.f, frameHeight / 2.f });

    //----------------------
    mGameManager = gameManager;
    m_selectedSkin = selectedSkin;
    m_overlayState = OverlayState::NONE;
    m_previousOverlayState = OverlayState::NONE;

    m_isMousePressed = false;
    m_isEscPressed = false;

    mScore = 0;
    mlevel = 1;
    m_SpeedBonus = 0.f;
    m_IsGameOver = false;

    m_Player = new CPEOPLE(500.f, 700.f, m_selectedSkin);
    m_SpawnTimer = 0.f;
    m_NextSpawnTime = 1.5f;

    m_isPauseAboutOpen = false;
    m_pauseAboutIcon.setScale({ 0.08f, 0.08f });
    m_pauseAboutIcon.setPosition({ 400.f, 480.f });

    m_pauseAboutLabel.setString("ABOUT US");
    m_pauseAboutLabel.setCharacterSize(22);
    m_pauseAboutLabel.setFillColor(sf::Color::White);
    m_pauseAboutLabel.setPosition({ 460.f, 465.f });

    m_pauseAboutTitleText.setString("DEVELOPER TEAM");
    m_pauseAboutTitleText.setCharacterSize(38);
    m_pauseAboutTitleText.setFillColor(sf::Color(255, 200, 50));
    sf::FloatRect titleB = m_pauseAboutTitleText.getLocalBounds();
    m_pauseAboutTitleText.setOrigin({ titleB.size.x / 2.f, titleB.size.y / 2.f });
    m_pauseAboutTitleText.setPosition({ 800.f, 175.f });

    m_pauseAboutBackIcon.setScale({ 0.09f, 0.09f });
    sf::FloatRect backIconB = m_pauseAboutBackIcon.getLocalBounds();
    m_pauseAboutBackIcon.setOrigin({ backIconB.size.x / 2.f, backIconB.size.y / 2.f });
    m_pauseAboutBackIcon.setPosition({ 690.f, 545.f });

    m_pauseAboutBackLabel.setString("BACK");
    m_pauseAboutBackLabel.setCharacterSize(26);
    m_pauseAboutBackLabel.setFillColor(sf::Color(240, 235, 200));
    m_pauseAboutBackLabel.setPosition({ 740.f, 530.f });

    m_pauseAboutBox.setSize({ 650.f, 520.f });
    m_pauseAboutBox.setOrigin({ 325.f, 260.f });
    m_pauseAboutBox.setPosition({ 800.f, 400.f });
    m_pauseAboutBox.setFillColor(sf::Color(40, 44, 52));
    m_pauseAboutBox.setOutlineThickness(4.f);
    m_pauseAboutBox.setOutlineColor(sf::Color(255, 200, 50));

    std::vector<std::string> memberInfos = {
        "1. MSSV: 25127530 - Name: Nguyen Vo Minh Tri",
        "2. MSSV: 25127549 - Name: Nguyen Chi Vi",
        "3. MSSV: 25127397 - Name: Nguyen Trung Kien",
        "4. MSSV: 25127527 - Name: Le Minh Tri",
    };

    float startY = 250.f;
    for (int i = 0; i < 4; ++i) {
        MemberRow row(AssetManager::getInstance().getTexture("loading.png"), AssetManager::getInstance().getFont("DIMIS___.ttf"));
        row.iconSprite.setScale({ 0.08f, 0.08f });
        sf::FloatRect iconB = row.iconSprite.getLocalBounds();
        row.iconSprite.setOrigin({ iconB.size.x / 2.f, iconB.size.y / 2.f });
        row.iconSprite.setPosition({ 570.f, startY + i * 55.f });

        row.infoText.setString(memberInfos[i]);
        row.infoText.setCharacterSize(20);
        row.infoText.setFillColor(sf::Color::White);
        row.infoText.setPosition({ 615.f, (startY + i * 55.f) - 15.f });

        m_pauseMemberRows.push_back(row);
    }

    m_pauseMenuBox.setSize({ 460.f, 620.f });
    m_pauseMenuBox.setOrigin({ 230.f, 310.f });

    loadHighScore();
    generateLevel();

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
    setupPopupRow(m_pauseAboutIcon, m_pauseAboutLabel, "ABOUT US", 590.f);

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

    setupTextButton(m_setSfxBtn, "SFX: ON", 300.f, 40);
    setupTextButton(m_setMusicBtn, "MUSIC: ON", 370.f, 40);
    setupTextButton(m_setResetBtn, "RESET", 440.f, 40);
    setupTextButton(m_setBackBtn, "BACK", 510.f, 40);

    AssetManager::getInstance().playMusic("../ASSETS/AUDIO/CROSSY.wav", true);

	m_saveLoadUI = new SaveLoadUI(); // Khởi tạo SaveLoadUI
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
    m_Obstacles.clear();
    m_pauseMemberRows.clear();

    delete m_fireAnimation;

	delete m_saveLoadUI;// Giải phóng SaveLoadUI
}

void PlayState::Init() {}

void PlayState::HandleEvent(const sf::Event& event, sf::RenderWindow& window) {
    // Chỉ đưa sự kiện gõ phím cho SaveLoadUI khi màn hình này đang được bật
    if (m_overlayState == OverlayState::SAVE_LOAD_MENU) {
        m_saveLoadUI->HandleEvent(event, window);
    }
}

void PlayState::Update(float delTime, sf::RenderWindow& window)
{
	//----------- deadth effect -----------
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

    //---------------------
    static bool isTPressed = false;
    bool pressT = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T);
    if (pressT && !isTPressed) {
        saveGame("quicksave.dat");
        std::cout << "[Chisa] Da ghi lai ky uc Level " << mlevel << " thanh cong!\n";
        if (AssetManager::getInstance().isSfxOn()) {
            m_levelUpSound.play(); 
        }
    }
    isTPressed = pressT;

    static bool isYPressed = false;
    bool pressY = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Y);
    if (pressY && !isYPressed) {
        loadGame("quicksave.dat");
        std::cout << "[Chisa] Da dao nguoc thoi gian tro lai!\n";
        if (AssetManager::getInstance().isSfxOn()) {
            m_levelUpSound.play();
        }
    }
    isYPressed = pressY;

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

        checkCollision();
        cleanUpOffScreen(window);
    }

    if (mScore > mHighScore) {
        mHighScore = mScore;
        saveHighScore();
    }

    m_HighScoreText.setString("HIGH SCORE: " + std::to_string(mHighScore));
    m_ScoreText.setString("Score: " + std::to_string(mScore));
    m_LevelText.setString("Level: " + std::to_string(mlevel));

    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);

    if (m_overlayState == OverlayState::PAUSE_MENU)
    {
        auto applyHoverPopup = [&mousePos](sf::Sprite& sprite, sf::Text& text) {
            bool isHovered = sprite.getGlobalBounds().contains(mousePos) ||
                text.getGlobalBounds().contains(mousePos);
            if (isHovered) {
                text.setFillColor(sf::Color(255, 215, 0));
                sprite.setColor(sf::Color(255, 255, 180));
            }
            else {
                text.setFillColor(sf::Color::White);
                sprite.setColor(sf::Color::White);
            }
            };

        applyHoverPopup(resumeBtn, m_resumeLabel);
        applyHoverPopup(loadBtn, m_loadLabel);
        applyHoverPopup(volumeBtn, m_volumeLabel);
        applyHoverPopup(menuBtn, m_menuLabel);
        applyHoverPopup(exitBtn, m_exitLabel);
        applyHoverPopup(m_pauseAboutIcon, m_pauseAboutLabel);
    }
    bool isClicking = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

    auto applyHover = [&mousePos](sf::Text& text) {
        if (text.getGlobalBounds().contains(mousePos)) {
            text.setFillColor(sf::Color(255, 200, 50));
        }
        else {
            text.setFillColor(sf::Color(240, 235, 200));
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

    if (isClicking && !m_isMousePressed)
    {
        if (m_overlayState == OverlayState::NONE) {
            if (m_pauseHUDBtn.getGlobalBounds().contains(mousePos)) {
                m_overlayState = OverlayState::PAUSE_MENU;
            }
        }
        else if (m_overlayState == OverlayState::PAUSE_MENU) {
            if (m_isPauseAboutOpen) {
                if (m_pauseAboutBackLabel.getGlobalBounds().contains(mousePos) ||
                    m_pauseAboutBackIcon.getGlobalBounds().contains(mousePos)) {
                    m_isPauseAboutOpen = false;
                }
            }
            else {
                if (resumeBtn.getGlobalBounds().contains(mousePos) || m_resumeLabel.getGlobalBounds().contains(mousePos)) {
                    m_overlayState = OverlayState::NONE;
                }
                else if (loadBtn.getGlobalBounds().contains(mousePos) || m_loadLabel.getGlobalBounds().contains(mousePos)) {
                    // Thay vì gọi loadGame ngay, tụi mình sẽ bật Menu Save/Load lên!
                    m_previousOverlayState = OverlayState::PAUSE_MENU;
                    m_overlayState = OverlayState::SAVE_LOAD_MENU;
                    m_saveLoadUI->refreshList(); // Đọc lại danh bạ mới nhất
                }
                else if (m_pauseAboutIcon.getGlobalBounds().contains(mousePos) || m_pauseAboutLabel.getGlobalBounds().contains(mousePos)) {
                    m_isPauseAboutOpen = true;
                }
                else if (volumeBtn.getGlobalBounds().contains(mousePos) || m_volumeLabel.getGlobalBounds().contains(mousePos)) {
                    m_previousOverlayState = OverlayState::PAUSE_MENU;
                    m_overlayState = OverlayState::SETTINGS_MENU;
                }
                else if (menuBtn.getGlobalBounds().contains(mousePos) || m_menuLabel.getGlobalBounds().contains(mousePos)) {
                    mGameManager->setState(new MenuState(mGameManager));
                    return;
                }
                else if (exitBtn.getGlobalBounds().contains(mousePos) || m_exitLabel.getGlobalBounds().contains(mousePos)) {
                    window.close();
                }
            }
        }
        else if (m_overlayState == OverlayState::GAME_OVER) {
            if (m_goRestartBtn.getGlobalBounds().contains(mousePos)) {
                mGameManager->setState(new PlayState(mGameManager, m_selectedSkin));
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
        else if (m_overlayState == OverlayState::SETTINGS_MENU) {
            if (m_setSfxBtn.getGlobalBounds().contains(mousePos)) {
				AssetManager::getInstance().toggleSfx();
                m_setSfxBtn.setString("SFX: " + std::string(AssetManager::getInstance().isSfxOn() ? "ON" : "OFF"));
            }
            else if (m_setMusicBtn.getGlobalBounds().contains(mousePos)) {
                AssetManager::getInstance().toggleMusic();
                m_setMusicBtn.setString("MUSIC: " + std::string(AssetManager::getInstance().isMusicOn() ? "ON" : "OFF"));
            }
            else if (m_setResetBtn.getGlobalBounds().contains(mousePos)) {
     
                if (!AssetManager::getInstance().isSfxOn()) AssetManager::getInstance().toggleSfx();
                if (!AssetManager::getInstance().isMusicOn()) AssetManager::getInstance().toggleMusic();

                m_setSfxBtn.setString("SFX: ON");
                m_setMusicBtn.setString("MUSIC: ON");
            }
            else if (m_setBackBtn.getGlobalBounds().contains(mousePos)) {
                m_overlayState = m_previousOverlayState;
            }

            auto reCenter = [](sf::Text& t) {
                sf::FloatRect b = t.getLocalBounds();
                t.setOrigin({ b.size.x / 2.f, b.size.y / 2.f });
                };
            reCenter(m_setSfxBtn);
            reCenter(m_setMusicBtn);
        }
        else if (m_overlayState == OverlayState::SAVE_LOAD_MENU) {
            m_saveLoadUI->Update(window);
            SaveLoadAction action = m_saveLoadUI->getPendingAction();

            if (action == SaveLoadAction::SAVE) {
                saveGame(m_saveLoadUI->getTargetFileName());
                m_saveLoadUI->resetAction();
                m_overlayState = m_previousOverlayState; // Quay về màn hình Pause
            }
            else if (action == SaveLoadAction::LOAD) {
                loadGame(m_saveLoadUI->getTargetFileName());
                m_saveLoadUI->resetAction();
                m_overlayState = OverlayState::NONE; // Vào thẳng game để chơi
            }
            else if (action == SaveLoadAction::CLOSE) {
                m_saveLoadUI->resetAction();
                m_overlayState = m_previousOverlayState; // Trở lại Pause
            }
        }
    }
    m_isMousePressed = isClicking;
}

void PlayState::Render(sf::RenderWindow& window)
{

    for (auto& lane : m_Lanes) {
        window.draw(lane.bgSprite);
    }
    for (auto obs : m_Obstacles) {
        obs->Draw(window);
    }
    for (auto coin : m_Coins) {
        coin->Draw(window);
    }
    m_Player->Draw(window);

    if (m_isShowingCollisionEffect)
    {
        window.draw(m_fireEffectSprite);
    }

    window.draw(m_HighScoreText);
    window.draw(m_ScoreText);
    window.draw(m_LevelText);
    window.draw(m_pauseHUDBtn);

    if (m_overlayState != OverlayState::NONE) {
        window.draw(m_pauseOverlay);
    }

    if (m_overlayState == OverlayState::PAUSE_MENU)
    {
        if (m_isPauseAboutOpen) {
            window.draw(m_pauseAboutOverlay);
            window.draw(m_pauseAboutBox);
            window.draw(m_pauseAboutTitleText);

            for (const auto& row : m_pauseMemberRows) {
                window.draw(row.iconSprite);
                window.draw(row.infoText);
            }

            window.draw(m_pauseAboutBackIcon);
            window.draw(m_pauseAboutBackLabel);
        }
        else {
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
            window.draw(m_pauseAboutIcon);
            window.draw(m_pauseAboutLabel);
        }
    }
    else if (m_overlayState == OverlayState::GAME_OVER)
    {
        window.draw(m_gameOverBanner);
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
    else if (m_overlayState == OverlayState::SAVE_LOAD_MENU)
    {
        m_saveLoadUI->Draw(window);
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

                if (rand() % 5 == 0)
                {
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
                    else
                    {
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
        newMovingObstacle = new CVEHICLE(spawnX + 50.f, laneY + 50.f);
        if (AssetManager::getInstance().isSfxOn()) {
            m_honkSound.play();
        }
    }
    else {
        newMovingObstacle = new CANIMAL(spawnX + 50.f, laneY + 50.f);
        if (AssetManager::getInstance().isSfxOn()) {
            m_meowSound.play();
        }
    }

    float laneBaseSpeed = 100.f + (laneY / 100.f) * 15.f;
    newMovingObstacle->setSpeed(laneBaseSpeed + m_SpeedBonus);
    m_Obstacles.push_back(newMovingObstacle);
    m_SpawnTimer = 0.f;
    m_NextSpawnTime = (rand() % 100 + 100) / 100.f;
}

void PlayState::levelUp()
{
    mlevel++;
    mScore += 100;
    m_SpeedBonus += 50.f;
    if (AssetManager::getInstance().isSfxOn()) {
        m_levelUpSound.play(); 
    }

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
            mScore += 10;           
            if (AssetManager::getInstance().isSfxOn())
            {
                m_coinSound.play();
            }
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
            m_fireEffectSprite.setPosition({ obsPosx + 25,obsPosy + 25});

            std::cout << "\n[!] COLLISION! Showing fire effect...\n";
            //m_IsGameOver = true;
            //m_overlayState = OverlayState::GAME_OVER;

            if (AssetManager::getInstance().isSfxOn()) {
                m_crashSound.play();
                m_gameOverSound.play();
            }
            return;
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

void PlayState::saveGame(const std::string& fileName) {
    std::ofstream out("game.dat", std::ios::binary);
    if (!out.is_open()) return;

    out.write(reinterpret_cast<const char*>(&mlevel), sizeof(mlevel));
    out.write(reinterpret_cast<const char*>(&mScore), sizeof(mScore));
    out.write(reinterpret_cast<const char*>(&m_SpeedBonus), sizeof(m_SpeedBonus));

    m_Player->saveToFile(out);

    size_t laneCount = m_Lanes.size();
    out.write(reinterpret_cast<const char*>(&laneCount), sizeof(laneCount));
    for (const auto& lane : m_Lanes) {
        int type = static_cast<int>(lane.type);
        float y = lane.yPos;
        out.write(reinterpret_cast<const char*>(&type), sizeof(type));
        out.write(reinterpret_cast<const char*>(&y), sizeof(y));
    }

    size_t obsCount = m_Obstacles.size();
    out.write(reinterpret_cast<const char*>(&obsCount), sizeof(obsCount));

    for (auto obs : m_Obstacles) {
        int type = obs->getType();
        out.write(reinterpret_cast<const char*>(&type), sizeof(type));
        obs->saveToFile(out);
    }

    out.close();
}

void PlayState::loadGame(const std::string& fileName) {
    std::ifstream in("game.dat", std::ios::binary);
    if (!in.is_open()) return;

    in.read(reinterpret_cast<char*>(&mlevel), sizeof(mlevel));
    in.read(reinterpret_cast<char*>(&mScore), sizeof(mScore));
    in.read(reinterpret_cast<char*>(&m_SpeedBonus), sizeof(m_SpeedBonus));

    m_Player->loadFromFile(in);
    for (auto obs : m_Obstacles) {
        delete obs;
    }
    m_Obstacles.clear();
    m_Lanes.clear();

    size_t laneCount = 0;
    in.read(reinterpret_cast<char*>(&laneCount), sizeof(laneCount));
    for (size_t i = 0; i < laneCount; ++i) {
        int type = 0;
        float y = 0.f;
        in.read(reinterpret_cast<char*>(&type), sizeof(type));
        in.read(reinterpret_cast<char*>(&y), sizeof(y));
        spawnLane(y, static_cast<LaneType>(type));
    }

    size_t obsCount = 0;
    in.read(reinterpret_cast<char*>(&obsCount), sizeof(obsCount));
    for (size_t i = 0; i < obsCount; ++i) {
        int type = 0;
        in.read(reinterpret_cast<char*>(&type), sizeof(type));

        Obstacle* newObs = nullptr;
        if (type == 1) {
            newObs = new CANIMAL(0.f, 0.f);
        }
        else if (type == 2) {
            newObs = new CVEHICLE(0.f, 0.f);
        }
        else if (type == 3) {
            newObs = new StaticObstacle(0.f, 0.f);
        }

        if (newObs != nullptr) {
            newObs->loadFromFile(in);
            m_Obstacles.push_back(newObs);
        }
    }

    in.close();
}

