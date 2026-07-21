#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "AssetManager.h"
#include"GameManager.h"

const int screenWidth = 1600;
const int screenHeight = 800;
int main() {
    // 1. Khởi tạo hạt giống random
    srand(static_cast<unsigned int>(time(NULL)));

    // 2. Tạo cửa sổ
    sf::RenderWindow window(sf::VideoMode({ screenWidth, screenHeight }), "Crossing Game OOP");
    window.setFramerateLimit(60);


    sf::Clock clock;
    float delTime = 0.f;

    GameManager gameManager;

    // VÒNG LẶP CHÍNH
    while (window.isOpen()) {
        delTime = clock.restart().asSeconds();

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
        }
        gameManager.Update(delTime, window);

        window.clear(sf::Color::Black);
        gameManager.Render(window);
        window.display();
    }
    return 0;
}