#pragma once
<<<<<<< HEAD
#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include <SFML/Audio.hpp>
#include "IGameState.h"
#include <vector>
=======
#include "IGameState.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
>>>>>>> 6a7544a00b8e24c5cb0783ad639abbef5cc4d6ff

class GameManager;

enum class MenuOverlayState {
    MAIN,
    SETTINGS
};

class MenuState : public IGameState {
private:
    GameManager* mGameManager;

    sf::Sprite m_backgroundSprite;
    sf::Sound m_bgMusic;

    // Hitbox trên ảnh menu_copy.png
    sf::RectangleShape m_playBtn;
    sf::RectangleShape m_exitBtn;
    sf::RectangleShape m_settingBtn; // Hitbox icon bánh răng ở góc dưới bên trái

<<<<<<< HEAD
=======
    // UI Settings Overlay
    MenuOverlayState m_overlayState;
    sf::RectangleShape m_settingsOverlay;

    sf::Text m_setSfxBtn;
    sf::Text m_setMusicBtn;
    sf::Text m_setBackBtn;

    bool m_isMousePressed;

>>>>>>> 6a7544a00b8e24c5cb0783ad639abbef5cc4d6ff
public:
    MenuState(GameManager* gameManager);
    ~MenuState() override;

    void Init() override;
    void Update(float delTime, sf::RenderWindow& window) override;
    void Render(sf::RenderWindow& window) override;
<<<<<<< HEAD

    // --- BIẾN CHO BẢNG ABOUT US (PHONG CÁCH PAUSE MENU) ---
    bool m_isAboutOpen;
    bool m_isIPressed;

    sf::RectangleShape m_aboutOverlay;  // Lớp nền mờ
    sf::RectangleShape m_aboutBox;      // Khung popup chính giữa
    sf::Text* m_aboutTitleText;         // Tiêu đề "DEVELOPER TEAM"

    sf::Sprite* m_aboutMainBtn;          // Nút icon mở About Us ngoài màn hình chính

    // Cấu trúc danh sách thành viên (Dùng con trỏ an toàn cho cả Sprite và Text)
    struct MemberRow {
        sf::Sprite* iconSprite;
        sf::Text* infoText;
    };
    std::vector<MemberRow> m_memberRows;

    sf::Sprite* m_aboutBackIcon;         // Icon nút Back
    sf::Text* m_aboutBackLabel;         // Chữ BACK
=======
>>>>>>> 6a7544a00b8e24c5cb0783ad639abbef5cc4d6ff
};