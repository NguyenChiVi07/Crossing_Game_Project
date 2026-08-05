#include "MenuState.hpp"
#include "AssetManager.h"
#include "GameManager.h"
#include "PlayState.h"
MenuState::MenuState(GameManager *gameManager)
    : mGameManager(gameManager),
      m_backgroundSprite(
          AssetManager::getInstance().getTexture("menu_copy.png")),
      m_titleText(AssetManager::getInstance().getFont("Robus-BWqOd.otf")),
      m_instructionText(AssetManager::getInstance().getFont("Robus-BWqOd.otf")),
      m_playText(AssetManager::getInstance().getFont("Robus-BWqOd.otf")),
      m_exitText(AssetManager::getInstance().getFont("Robus-BWqOd.otf")),
      m_bgMusic(AssetManager::getInstance().getSoundBuffer(
          "AdhesiveWombat - Night Shade_copy.mp3")) {}

MenuState::~MenuState() {}

void MenuState::Init() {
  // Scale ảnh nền vừa màn hình (1600x800)
  sf::FloatRect textureSize = m_backgroundSprite.getLocalBounds();
  if (textureSize.size.x > 0 && textureSize.size.y > 0) {
    m_backgroundSprite.setScale(
        {1600.f / static_cast<float>(textureSize.size.x),
         800.f / static_cast<float>(textureSize.size.y)});
  }

  // 2. phát nhạc nền
  m_bgMusic.setLooping(true);
  m_bgMusic.play();

  m_titleText.setString("CROSSING GAME");
  m_titleText.setCharacterSize(60);
  m_titleText.setFillColor(sf::Color::Yellow);
  m_titleText.setStyle(sf::Text::Bold);

  // Căn giữa tiêu đề (màn hình 1600x800)
  sf::FloatRect titleBounds = m_titleText.getLocalBounds();
  m_titleText.setOrigin({titleBounds.position.x + titleBounds.size.x / 2.f,
                         titleBounds.position.y + titleBounds.size.y / 2.f});
  m_titleText.setPosition({800.f, 250.f});

  m_instructionText.setString("Press ENTER to Start Game");
  m_instructionText.setCharacterSize(30);
  m_instructionText.setFillColor(sf::Color::White);

  sf::FloatRect instBounds = m_instructionText.getLocalBounds();
  m_instructionText.setOrigin(
      {instBounds.position.x + instBounds.size.x / 2.f,
       instBounds.position.y + instBounds.size.y / 2.f});
  m_instructionText.setPosition({800.f, 450.f});

  // --- Nút PLAY ---
  const float btnW = 200.f, btnH = 60.f;
  const float centerX = 800.f;
  const float playY = 530.f; // nằm trên vị trí nút đỏ trong ảnh

  m_playBtn.setSize({btnW, btnH});
  m_playBtn.setFillColor(sf::Color::Transparent);
  m_playBtn.setOutlineThickness(0.f);
  m_playBtn.setOrigin({btnW / 2.f, btnH / 2.f});
  m_playBtn.setPosition({centerX, playY});

  m_playText.setString("PLAY");
  m_playText.setCharacterSize(28);
  m_playText.setFillColor(sf::Color::White);
  m_playText.setStyle(sf::Text::Bold);
  {
    sf::FloatRect b = m_playText.getLocalBounds();
    m_playText.setOrigin(
        {b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f});
  }
  m_playText.setPosition({centerX, playY});

  // --- Nút EXIT ---
  const float exitY = playY + btnH + 20.f; // ngay dưới nút Play

  m_exitBtn.setSize({btnW, btnH});
  m_exitBtn.setFillColor(sf::Color::Transparent);
  m_exitBtn.setOutlineThickness(0.f);
  m_exitBtn.setOrigin({btnW / 2.f, btnH / 2.f});
  m_exitBtn.setPosition({centerX, exitY});

  m_exitText.setString("EXIT");
  m_exitText.setCharacterSize(28);
  m_exitText.setFillColor(sf::Color::White);
  m_exitText.setStyle(sf::Text::Bold);
  {
    sf::FloatRect b = m_exitText.getLocalBounds();
    m_exitText.setOrigin(
        {b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f});
  }
  m_exitText.setPosition({centerX, exitY});
} // end Init()

void MenuState::Update(float delTime, sf::RenderWindow &window) {
  // Chỉ xử lý click chuột để chuyển state / thoát game
  if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
    sf::Vector2f mousePos =
        window.mapPixelToCoords(sf::Mouse::getPosition(window));

    // Click nút PLAY → chuyển sang PlayState
    if (m_playBtn.getGlobalBounds().contains(mousePos)) {
      if (mGameManager) {
        mGameManager->setState(new PlayState(mGameManager));
        return;
      }
    }

    // Click nút EXIT → thoát game
    if (m_exitBtn.getGlobalBounds().contains(mousePos)) {
      window.close();
      return;
    }
  }
}

void MenuState::Render(sf::RenderWindow &window) {
  // 1. Vẽ ảnh nền
  window.draw(m_backgroundSprite);

  // 2. Vẽ tiêu đề và hướng dẫn
  window.draw(m_titleText);
  window.draw(m_instructionText);

  // 3. Vẽ các nút
  window.draw(m_playBtn);
  window.draw(m_playText);
  window.draw(m_exitBtn);
  window.draw(m_exitText);
}