#include "Dashboard.hpp"
#include <iostream>
#include <optional> // Thư viện bắt buộc cho vòng lặp sự kiện của SFML 3

Dashboard::Dashboard() 
    // Gắn chặt Font vào Text ngay từ khoảnh khắc khởi tạo theo chuẩn SFML 3
    : titleText(font), playText(font), loadText(font), aboutText(font), exitText(font) 
{
    initWindow();
    initText();
}

void Dashboard::initWindow() {
    // SFML 3 yêu cầu dùng ngoặc nhọn {} cho VideoMode
    window.create(sf::VideoMode({800, 600}), "Crossing Road - Dashboard"); 
}

void Dashboard::initText() {
    // Lệnh tải font chữ đã được đổi tên
    if (!font.openFromFile("arial.ttf")) { 
        std::cout << "Khong tim thay file font!\n";
    }

    // Không cần dùng hàm setFont() nữa vì đã gắn ở hàm khởi tạo bên trên
    titleText.setString("CROSSING ROAD");
    titleText.setCharacterSize(55);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition({160.f, 80.f}); // Có ngoặc nhọn {} cho tọa độ

    playText.setString("Play Game");
    playText.setCharacterSize(35);
    playText.setPosition({320.f, 220.f});
    playText.setFillColor(sf::Color::White);

    loadText.setString("Load Game");
    loadText.setCharacterSize(35);
    loadText.setPosition({320.f, 300.f});
    loadText.setFillColor(sf::Color::White);

    aboutText.setString("About Us");
    aboutText.setCharacterSize(35);
    aboutText.setPosition({320.f, 380.f});
    aboutText.setFillColor(sf::Color::White);

    exitText.setString("Exit");
    exitText.setCharacterSize(35);
    exitText.setPosition({320.f, 460.f});
    exitText.setFillColor(sf::Color::White);
}

void Dashboard::run() {
    while (window.isOpen()) {
        // Vòng lặp xử lý sự kiện kiểu mới, vô cùng an toàn của SFML 3
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear(sf::Color(105, 105, 105)); // Sắc xám lông chuột thanh lịch

        window.draw(titleText);
        window.draw(playText);
        window.draw(loadText);
        window.draw(aboutText);
        window.draw(exitText);

        window.display();
    }
}