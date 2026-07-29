#pragma once
#include <SFML/Graphics.hpp>
#include"AssetManager.h"
#include <SFML/Audio.hpp>
#include "IGameState.h"

// Forward declaration
class GameManager;

class MenuState : public IGameState {
private:
    GameManager* mGameManager;

    sf::Sprite m_backgroundSprite;
    sf::Sound m_bgMusic;

    sf::Text m_titleText;
    sf::Text m_instructionText;

    //bool m_isMusicLoaded;

public:
    MenuState(GameManager* gameManager);
    ~MenuState() override;

    void Init() override;
    void Update(float delTime, sf::RenderWindow& window) override;
    void Render(sf::RenderWindow& window) override;
};
