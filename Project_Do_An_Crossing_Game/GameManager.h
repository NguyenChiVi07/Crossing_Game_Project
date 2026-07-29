#pragma once
#include <SFML/Graphics.hpp>
#include "IGameState.h"

// Forward declaration thay vì include để tránh lỗi include chéo (circular dependency)
class PlayState;
class MenuState;
class Dashboard;

class GameManager
{
private:
    IGameState* mCurrentState;

public:
    GameManager(); // Constructor
    ~GameManager(); // Destructor

    IGameState* getState();
    void setState(IGameState* state);

    void Update(float deltime, sf::RenderWindow& window);
    void Render(sf::RenderWindow& window);
};