#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class CButton {
private:
    sf::RectangleShape mShape;
    sf::Text mText;

    sf::Color mIdleColor;
    sf::Color mHoverColor;
    sf::Color mActiveColor;

    enum ButtonState { IDLE = 0, HOVER, ACTIVE };
    ButtonState mState;

public:
    CButton(float x, float y, float width, float height,
        const sf::Font& font, const std::string& text,
        sf::Color idle, sf::Color hover, sf::Color active);
    void update(const sf::Vector2f& mousePos, bool isMouseClicked);
    void render(sf::RenderWindow& window);
    bool isPressed() const;
};