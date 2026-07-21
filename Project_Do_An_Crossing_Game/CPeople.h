#pragma once
#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include"Animation.h"
class CPEOPLE {
private:
    float m_X, m_Y;
    float m_Speed;
    sf::RectangleShape m_player;
    bool m_isKeyPressed;
public:
    CPEOPLE(float startX, float startY);   
    ~CPEOPLE();
    void update(float delTime);
    void Draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
      
    float getY() const;

    void resetPosition(float startX, float startY);
};