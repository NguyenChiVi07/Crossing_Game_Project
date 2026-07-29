#pragma once
#include <SFML/Graphics.hpp>
#include"IGameState.h"
#include"AssetManager.h"
class GameManager;
class Dashboard : public IGameState{
private:
    GameManager* mGameManager;
   
    sf::Text titleText;
    sf::Text playText;
    sf::Text loadText;
    sf::Text aboutText;
    sf::Text exitText;
public:
    Dashboard(GameManager* gameManager);
    ~Dashboard();
    void Init();
    void Update(float delTime, sf::RenderWindow& window);
    void Render(sf::RenderWindow& window);
};