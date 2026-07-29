#include "MenuState.hpp"
#include "AssetManager.h"
#include "GameManager.h"
#include "PlayState.h"
MenuState::MenuState(GameManager *gameManager) : mGameManager(gameManager)
, m_backgroundSprite(AssetManager::getInstance().getTexture("menu_copy.png"))
, m_titleText(AssetManager::getInstance().getFont("Robus-BWqOd.otf"))
, m_instructionText(AssetManager::getInstance().getFont("Robus-BWqOd.otf"))
, m_bgMusic(AssetManager::getInstance().getSoundBuffer("AdhesiveWombat - Night Shade_copy.mp3"))
{ }

MenuState::~MenuState() {
    
}

void MenuState::Init() {
  // Scale ảnh nền vừa màn hình (1600x800)
  sf::FloatRect textureSize = m_backgroundSprite.getLocalBounds();
  if (textureSize.size.x > 0 && textureSize.size.y > 0) {
    m_backgroundSprite.setScale({1600.f / static_cast<float>(textureSize.size.x),
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
}

void MenuState::Update(float delTime, sf::RenderWindow &window) {
  // Nhấn ENTER để chuyển sang PlayState
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
    if (mGameManager) {
      mGameManager->setState(new PlayState(mGameManager));
      return; // SỬA BUG: Thoát ngay để tránh tiếp tục thực thi đối tượng đã
              // giải phóng
    }
  }
}

void MenuState::Render(sf::RenderWindow &window) {
  // 1. Vẽ ảnh nền
  window.draw(m_backgroundSprite);

  // 2. Vẽ tiêu đề và hướng dẫn
  window.draw(m_titleText);
  window.draw(m_instructionText);
}