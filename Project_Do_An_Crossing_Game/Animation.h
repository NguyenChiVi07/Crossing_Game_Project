#pragma once
#include <SFML/Graphics.hpp>

class Animation {
private:
    sf::Vector2u m_imageCount;
    sf::Vector2u m_currentImage;

    float m_totalTime;
    float m_switchTime;

public:
    sf::IntRect uvRect;

    Animation(const sf::Texture* texture, sf::Vector2u imageCount, float switchTime);

    void Update(int row, float deltaTime, bool faceRight);
};