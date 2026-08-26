#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <string>

class InputBox {
private:
    sf::RectangleShape m_box;
    sf::Text m_textDisplay;
    std::string m_text;
    bool m_isSelected;
    int m_charLimit;

public:
    InputBox(float x, float y, float width, float height, const sf::Font& font, int charLimit = 15)
        : m_textDisplay(font), m_isSelected(false), m_charLimit(charLimit)
    {
        m_box.setPosition({ x, y });
        m_box.setSize({ width, height });
        m_box.setFillColor(sf::Color(40, 44, 52));
        m_box.setOutlineThickness(3.f);
        m_box.setOutlineColor(sf::Color(100, 100, 100));

        m_textDisplay.setCharacterSize(24);
        m_textDisplay.setFillColor(sf::Color::White);
        m_textDisplay.setPosition({ x + 10.f, y + 5.f });

        m_text = "";
        m_textDisplay.setString("Enter save name...");
    }

    void HandleEvent(const sf::Event& event, const sf::RenderWindow& window) {
        if (const auto* mouseButton = event.getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseButton->button == sf::Mouse::Button::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if (m_box.getGlobalBounds().contains(mousePos)) {
                    m_isSelected = true;
                    m_box.setOutlineColor(sf::Color(255, 200, 50));
                    if (m_text.empty()) m_textDisplay.setString("_");
                }
                else {
                    m_isSelected = false;
                    m_box.setOutlineColor(sf::Color(100, 100, 100));
                    if (m_text.empty()) m_textDisplay.setString("Enter save name...");
                }
            }
        }

        if (m_isSelected) {
            if (const auto* textEntered = event.getIf<sf::Event::TextEntered>()) {
                char32_t unicode = textEntered->unicode;
                if (unicode == '\b') {
                    if (!m_text.empty()) m_text.pop_back();
                }
                else if (unicode < 128 && unicode > 31 && m_text.length() < m_charLimit) {
                    m_text += static_cast<char>(unicode);
                }
                m_textDisplay.setString(m_text + "_");
            }
        }
    }

    void Draw(sf::RenderWindow& window) const {
        window.draw(m_box);
        window.draw(m_textDisplay);
    }

    std::string getText() const { return m_text; }
};