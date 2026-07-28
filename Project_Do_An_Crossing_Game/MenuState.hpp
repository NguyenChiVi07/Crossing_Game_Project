#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <SFML/Audio.hpp>
#include "IGameState.h"

// Forward declaration
class GameManager;

class MenuState : public IGameState {
private:
    GameManager* mGameManager;

    std::optional<sf::Sprite> m_backgroundSprite;
    sf::Music m_bgMusic;

    std::optional<sf::Text> m_titleText;
    std::optional<sf::Text> m_instructionText;

    bool m_isMusicLoaded;

public:
    MenuState(GameManager* gameManager);
    ~MenuState() override;

    void Init() override;
    void Update(float delTime, sf::RenderWindow& window) override;
    void Render(sf::RenderWindow& window) override;
};
