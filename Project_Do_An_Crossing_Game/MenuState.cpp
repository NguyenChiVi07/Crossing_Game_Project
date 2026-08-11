#include "MenuState.hpp"
#include "AssetManager.h"
#include "GameManager.h"
#include "PlayState.h"
#include "SelectSkinState.h"
#include "GameSettings.h"

MenuState::MenuState(GameManager* gameManager)
    : mGameManager(gameManager),
    m_backgroundSprite(AssetManager::getInstance().getTexture("menu_copy.png")),
    m_bgMusic(AssetManager::getInstance().getSoundBuffer("AdhesiveWombat - Night Shade_copy.mp3")),
    m_setSfxBtn(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_setMusicBtn(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_setBackBtn(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_overlayState(MenuOverlayState::MAIN),
    m_isMousePressed(false) {
}

MenuState::~MenuState() {
    m_bgMusic.stop();
}

void MenuState::Init() {
    // 1. Co giãn background 1600x800
    sf::FloatRect textureSize = m_backgroundSprite.getLocalBounds();
    if (textureSize.size.x > 0 && textureSize.size.y > 0) {
        m_backgroundSprite.setScale(
            { 1600.f / static_cast<float>(textureSize.size.x),
             800.f / static_cast<float>(textureSize.size.y) });
    }

    if (GameSettings::getInstance().isMusicOn()) {
        m_bgMusic.setLooping(true);
        m_bgMusic.play();
    }

    // 2. HITBOX CÁC NÚT BẤM TRÊN ẢNH MENU
    m_playBtn.setSize({ 140.f, 110.f });
    m_playBtn.setPosition({ 730.f, 565.f });
    m_playBtn.setFillColor(sf::Color::Transparent);

    m_exitBtn.setSize({ 140.f, 80.f });
    m_exitBtn.setPosition({ 730.f, 690.f });
    m_exitBtn.setFillColor(sf::Color::Transparent);

    // Hitbox nút Settings (Bánh răng ở góc dưới bên trái ảnh)
    m_settingBtn.setSize({ 70.f, 70.f });
    m_settingBtn.setPosition({ 30.f, 750.f });
    m_settingBtn.setFillColor(sf::Color::Transparent);

    // 3. MÀN HÌNH SETTINGS OVERLAY
    m_settingsOverlay.setSize({ 1600.f, 800.f });
    m_settingsOverlay.setFillColor(sf::Color(0, 0, 0, 210)); // Lớp phủ tối mờ

    auto setupSettingText = [](sf::Text& text, const std::string& str, float yPos) {
        text.setString(str);
        text.setCharacterSize(45);
        text.setFillColor(sf::Color(240, 235, 200));
        sf::FloatRect b = text.getLocalBounds();
        text.setOrigin({ b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f });
        text.setPosition({ 800.f, yPos });
        };

    setupSettingText(m_setSfxBtn, GameSettings::getInstance().isSFXOn() ? "SFX: ON" : "SFX: OFF", 320.f);
    setupSettingText(m_setMusicBtn, GameSettings::getInstance().isMusicOn() ? "MUSIC: ON" : "MUSIC: OFF", 420.f);
    setupSettingText(m_setBackBtn, "BACK", 520.f); // Đã loại bỏ nút RESET
}

void MenuState::Update(float delTime, sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    bool isClicking = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

    // Hiệu ứng rê chuột (Hover) đổi màu vàng
    if (m_overlayState == MenuOverlayState::SETTINGS) {
        auto applyHover = [&mousePos](sf::Text& text) {
            if (text.getGlobalBounds().contains(mousePos)) {
                text.setFillColor(sf::Color(255, 200, 50));
            }
            else {
                text.setFillColor(sf::Color(240, 235, 200));
            }
            };

        applyHover(m_setSfxBtn);
        applyHover(m_setMusicBtn);
        applyHover(m_setBackBtn);
    }

    if (isClicking && !m_isMousePressed) {
        // TRẠNG THÁI MENU CHÍNH
        if (m_overlayState == MenuOverlayState::MAIN) {
            if (m_playBtn.getGlobalBounds().contains(mousePos)) {
                if (mGameManager) {
                    m_bgMusic.stop();
                    mGameManager->setState(new SelectSkinState(mGameManager));
                    return;
                }
            }

            if (m_exitBtn.getGlobalBounds().contains(mousePos)) {
                window.close();
                return;
            }

            // Mở Settings khi click icon Bánh Răng
            if (m_settingBtn.getGlobalBounds().contains(mousePos)) {
                m_overlayState = MenuOverlayState::SETTINGS;
            }
        }
        // TRẠNG THÁI MÀN HÌNH SETTINGS
        else if (m_overlayState == MenuOverlayState::SETTINGS) {
            // Đổi trạng thái SFX
            if (m_setSfxBtn.getGlobalBounds().contains(mousePos)) {
                GameSettings::getInstance().toggleSFX();
            }
            // Đổi trạng thái MUSIC
            else if (m_setMusicBtn.getGlobalBounds().contains(mousePos)) {
                GameSettings::getInstance().toggleMusic();

                // Cập nhật phát/tắt nhạc Menu ngay lập tức
                if (GameSettings::getInstance().isMusicOn()) {
                    if (m_bgMusic.getStatus() != sf::Sound::Status::Playing) {
                        m_bgMusic.play();
                    }
                }
                else {
                    m_bgMusic.pause();
                }
            }
            // Bấm BACK để quay lại Main Menu
            else if (m_setBackBtn.getGlobalBounds().contains(mousePos)) {
                m_overlayState = MenuOverlayState::MAIN;
            }

            // Cập nhật lại nhãn chữ Bật/Tắt
            m_setSfxBtn.setString("SFX: " + std::string(GameSettings::getInstance().isSFXOn() ? "ON" : "OFF"));
            m_setMusicBtn.setString("MUSIC: " + std::string(GameSettings::getInstance().isMusicOn() ? "ON" : "OFF"));

            auto reCenter = [](sf::Text& t) {
                sf::FloatRect b = t.getLocalBounds();
                t.setOrigin({ b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f });
                };
            reCenter(m_setSfxBtn);
            reCenter(m_setMusicBtn);
        }
    }
    m_isMousePressed = isClicking;
}

void MenuState::Render(sf::RenderWindow& window) {
    // Vẽ background menu
    window.draw(m_backgroundSprite);

    // Nếu mở Settings -> Vẽ lớp mờ và danh sách nút
    if (m_overlayState == MenuOverlayState::SETTINGS) {
        window.draw(m_settingsOverlay);
        window.draw(m_setSfxBtn);
        window.draw(m_setMusicBtn);
        window.draw(m_setBackBtn);
    }
}