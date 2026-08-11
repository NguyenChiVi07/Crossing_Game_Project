#pragma once
#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "Animation.h"
#include "ISerializable.h"

class CPEOPLE : public ISerializable {
private:
    float m_X, m_Y;
    float m_Speed;
    sf::RectangleShape m_player;
    //bool m_isKeyPressed;
public:
    CPEOPLE(float startX, float startY);
    ~CPEOPLE();
    void update(float delTime);
    void Draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    void resetPosition(float startX, float startY);
    float getY() const;
    float getX() const { return m_X; }

    void saveToFile(std::ofstream& out) override;
    void loadFromFile(std::ifstream& in) override;
};