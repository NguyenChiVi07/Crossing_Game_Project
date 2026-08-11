#pragma once
#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include <SFML/Audio.hpp>
#include "IGameState.h"
#include <vector>

class GameManager;

class MenuState : public IGameState {
private:
    GameManager* mGameManager;

    sf::Sprite m_backgroundSprite;
    sf::Sound m_bgMusic;

    sf::Text m_titleText;
    sf::Text m_instructionText;

public:
    MenuState(GameManager* gameManager);
    ~MenuState() override;

    void Init() override;
    void Update(float delTime, sf::RenderWindow& window) override;
    void Render(sf::RenderWindow& window) override;

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
};