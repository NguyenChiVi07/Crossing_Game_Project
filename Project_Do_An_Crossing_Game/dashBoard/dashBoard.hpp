#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Dashboard {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text titleText;
    sf::Text playText;
    sf::Text loadText;
    sf::Text aboutText;
    sf::Text exitText;

    void initWindow();
    void initText();

public:
    Dashboard();
    void run();
};