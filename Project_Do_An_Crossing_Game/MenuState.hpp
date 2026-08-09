#pragma once
#include "IGameState.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

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

    // UI Settings Overlay
    MenuOverlayState m_overlayState;
    sf::RectangleShape m_settingsOverlay;

    sf::Text m_setSfxBtn;
    sf::Text m_setMusicBtn;
    sf::Text m_setBackBtn;

    bool m_isMousePressed;

public:
    MenuState(GameManager* gameManager);
    ~MenuState() override;

    void Init() override;
    void Update(float delTime, sf::RenderWindow& window) override;
    void Render(sf::RenderWindow& window) override;
};