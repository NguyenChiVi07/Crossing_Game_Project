#include "MenuState.hpp"
#include "AssetManager.h"
#include "GameManager.h"
#include "PlayState.h"
MenuState::MenuState(GameManager *gameManager)
    : mGameManager(gameManager), m_isMusicLoaded(false) {}

MenuState::~MenuState() {
  if (m_isMusicLoaded) {
    m_bgMusic.stop();
  }
}

void MenuState::Init() {
  // 1. Load và cấu hình ảnh nền Menu
  sf::Texture &bgTexture =
      AssetManager::getInstance().getTexture("../ASSETS/IMAGES/menu_copy.png");
  m_backgroundSprite->setTexture(bgTexture);

  // Scale ảnh nền vừa màn hình (1600x800)
  sf::Vector2u textureSize = bgTexture.getSize();
  if (textureSize.x > 0 && textureSize.y > 0) {
    m_backgroundSprite->setScale({1600.f / static_cast<float>(textureSize.x),
                                  800.f / static_cast<float>(textureSize.y)});
  }

  // 2. Load và phát nhạc nền
  if (m_bgMusic.openFromFile(
          "../ASSETS/IMAGES/AdhesiveWombat - Night Shade_copy.mp3")) {
    m_bgMusic.setLooping(true);
    m_bgMusic.play();
    m_isMusicLoaded = true;
  } else {
    std::cout << "[MenuState] Khong the load file nhac: "
                 "../ASSETS/IMAGES/AdhesiveWombat - Night Shade_copy.mp3\n";
  }

  // 3. Khởi tạo Text hiển thị trên Menu
  sf::Font &font =
      AssetManager::getInstance().getFont("../ASSETS/FONTS/Robus-BWqOd.otf");

  m_titleText->setFont(font);
  m_titleText->setString("CROSSING GAME");
  m_titleText->setCharacterSize(60);
  m_titleText->setFillColor(sf::Color::Yellow);
  m_titleText->setStyle(sf::Text::Bold);

  // Căn giữa tiêu đề (màn hình 1600x800)
  sf::FloatRect titleBounds = m_titleText->getLocalBounds();
  m_titleText->setOrigin({titleBounds.position.x + titleBounds.size.x / 2.f,
                          titleBounds.position.y + titleBounds.size.y / 2.f});
  m_titleText->setPosition({800.f, 250.f});

  m_instructionText->setFont(font);
  m_instructionText->setString("Press ENTER to Start Game");
  m_instructionText->setCharacterSize(30);
  m_instructionText->setFillColor(sf::Color::White);

  sf::FloatRect instBounds = m_instructionText->getLocalBounds();
  m_instructionText->setOrigin(
      {instBounds.position.x + instBounds.size.x / 2.f,
       instBounds.position.y + instBounds.size.y / 2.f});
  m_instructionText->setPosition({800.f, 450.f});
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
  window.draw(*m_backgroundSprite);

  // 2. Vẽ tiêu đề và hướng dẫn
  window.draw(*m_titleText);
  window.draw(*m_instructionText);
}