#include "Dashboard.hpp"
#include <iostream>
Dashboard::Dashboard(GameManager* gameManager)
    : mGameManager(gameManager)
    , titleText(AssetManager::getInstance().getFont("Christmas.otf"))
    , playText(AssetManager::getInstance().getFont("Christmas.otf"))
    , loadText(AssetManager::getInstance().getFont("Christmas.otf"))
    , aboutText(AssetManager::getInstance().getFont("Christmas.otf"))
    , exitText(AssetManager::getInstance().getFont("Christmas.otf"))
{
}

Dashboard::~Dashboard()
{
}

void Dashboard::Init()
{
    titleText.setString("CROSSING ROAD");
    titleText.setCharacterSize(55);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition({ 160.f, 80.f }); // Có ngoặc nhọn {} cho tọa độ

    playText.setString("Play Game");
    playText.setCharacterSize(35);
    playText.setPosition({ 320.f, 220.f });
    playText.setFillColor(sf::Color::White);

    loadText.setString("Load Game");
    loadText.setCharacterSize(35);
    loadText.setPosition({ 320.f, 300.f });
    loadText.setFillColor(sf::Color::White);

    aboutText.setString("About Us");
    aboutText.setCharacterSize(35);
    aboutText.setPosition({ 320.f, 380.f });
    aboutText.setFillColor(sf::Color::White);

    exitText.setString("Exit");
    exitText.setCharacterSize(35);
    exitText.setPosition({ 320.f, 460.f });
    exitText.setFillColor(sf::Color::White);
}

void Dashboard::Update(float delTime, sf::RenderWindow& window)
{
}

void Dashboard::Render(sf::RenderWindow& window)
{
    window.draw(titleText);
    window.draw(playText);
    window.draw(loadText);
    window.draw(aboutText);
    window.draw(exitText);
}