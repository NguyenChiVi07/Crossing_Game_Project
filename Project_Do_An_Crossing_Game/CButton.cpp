#include "CButton.hpp"

CButton::CButton(float x, float y, float width, float height,
    const sf::Font& font, const std::string& text,
    sf::Color idle, sf::Color hover, sf::Color active)
    : mIdleColor(idle), mHoverColor(hover), mActiveColor(active), mState(IDLE),
    mText(font, text, 24) {

    mShape.setPosition({ x, y });
    mShape.setSize({ width, height });
    mShape.setFillColor(mIdleColor);
    mText.setFillColor(sf::Color::White);
    sf::FloatRect textBounds = mText.getLocalBounds();
    mText.setPosition({
        x + (width / 2.0f) - (textBounds.size.x / 2.0f),
        y + (height / 2.0f) - (textBounds.size.y / 2.0f) - textBounds.position.y
        });
}

void CButton::update(const sf::Vector2f& mousePos, bool isMouseClicked) {
    mState = IDLE;
    if (mShape.getGlobalBounds().contains(mousePos)) {
        mState = HOVER;
        if (isMouseClicked) {
            mState = ACTIVE;
        }
    }
    switch (mState) {
    case IDLE:   mShape.setFillColor(mIdleColor); break;
    case HOVER:  mShape.setFillColor(mHoverColor); break;
    case ACTIVE: mShape.setFillColor(mActiveColor); break;
    }
}

void CButton::render(sf::RenderWindow& window) {
    window.draw(mShape);
    //window.draw(mText);
}

bool CButton::isPressed() const {
    return mState == ACTIVE;
}