#pragma once
#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include"Animation.h"
class CPEOPLE {
private:
    float m_X, m_Y;
    float m_Speed;
    sf::Sprite m_player;
    bool m_isKeyPressed;

    Animation* m_animation;
    bool m_faceRight;
    int m_row;

public:
    CPEOPLE(float startX, float startY);   
    ~CPEOPLE();
    void update(float delTime);
    void Draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    void resetPosition(float startX, float startY);
    float getY() const;
    float getX() const { return m_X; } 
};