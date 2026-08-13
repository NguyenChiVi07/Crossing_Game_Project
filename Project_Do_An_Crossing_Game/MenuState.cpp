#include "MenuState.hpp"
#include "AssetManager.h"
#include "GameManager.h"
#include "GameSettings.h"
#include "ISerializable.h"
#include "PlayState.h"

// Hằng số vị trí icon góc dưới trái (1600×800)
// ─────────────────────────────────────────────
// Hình nền menu_copy.png (1440×720) được scale lên 1600×800 → tỷ lệ 1.111
// Icon bánh răng trong ảnh gốc nằm ở góc dưới trái ~ x=10, y=690
// Sau scale: x ≈ 11, y ≈ 767
// setting_copy.png kích thước 160×152, scale 0.32 → ~51×49px
static constexpr float GEAR_ICON_SCALE = 0.32f;
static constexpr float GEAR_X = 10.f; // vị trí trái màn hình
static constexpr float GEAR_Y =
    745.f; // gần đáy màn hình (icon ~49px cao, bottom=800)

// load_copy.png kích thước 162×148, scale 0.32 → ~52×47px
// Nằm kế bên bánh răng (cách ~4px)
static constexpr float GRID_ICON_SCALE = 0.32f;
static constexpr float GRID_X = 66.f;  // GEAR_X + 51 + 5
static constexpr float GRID_Y = 747.f; // gần bằng GEAR_Y (căn giữa dọc)

MenuState::MenuState(GameManager *gameManager)
    : mGameManager(gameManager),
      m_backgroundSprite(
          AssetManager::getInstance().getTexture("menu_copy.png")),
      m_bgMusic(AssetManager::getInstance().getSoundBuffer(
          "AdhesiveWombat - Night Shade_copy.mp3")),
      m_settingBtnSprite(
          AssetManager::getInstance().getTexture("setting_copy.png")),
      m_settingBtnHovered(false),
      m_serializableBtnSprite(
          AssetManager::getInstance().getTexture("load_copy.png")),
      m_serializableBtnHovered(false), m_wasMousePressed(false) {
  // --- KHỞI TẠO TRẠNG THÁI ABOUT US ---
  m_isAboutOpen = false;
  m_isIPressed = false;

  // 1. Icon mở About Us ngoài màn hình chính (Dùng con trỏ sf::Sprite*)
  m_aboutMainBtn = new sf::Sprite(
      AssetManager::getInstance().getTexture("about_us copy.png"));
  m_aboutMainBtn->setScale({0.12f, 0.12f});
  sf::FloatRect mainBtnB = m_aboutMainBtn->getLocalBounds();
  m_aboutMainBtn->setOrigin({mainBtnB.size.x / 2.f, mainBtnB.size.y / 2.f});
  m_aboutMainBtn->setPosition({1520.f, 45.f}); // Góc trên bên phải

  // 2. Lớp nền mờ phủ toàn màn hình
  m_aboutOverlay.setSize({1600.f, 800.f});
  m_aboutOverlay.setFillColor(sf::Color(0, 0, 0, 210));

  // 3. Khung popup chính giữa (Chuẩn phong cách Pause Menu)
  m_aboutBox.setSize({650.f, 520.f});
  m_aboutBox.setOrigin({325.f, 260.f});
  m_aboutBox.setPosition({800.f, 400.f});
  m_aboutBox.setFillColor(sf::Color(40, 44, 52));
  m_aboutBox.setOutlineThickness(4.f);
  m_aboutBox.setOutlineColor(sf::Color(255, 200, 50));

  // 4. Tiêu đề "DEVELOPER TEAM"
  m_aboutTitleText =
      new sf::Text(AssetManager::getInstance().getFont("DIMIS___.ttf"),
                   "DEVELOPER TEAM", 38);
  m_aboutTitleText->setFillColor(sf::Color(255, 200, 50));
  sf::FloatRect titleB = m_aboutTitleText->getLocalBounds();
  m_aboutTitleText->setOrigin({titleB.size.x / 2.f, titleB.size.y / 2.f});
  m_aboutTitleText->setPosition({800.f, 175.f});

  // 5. Khởi tạo 5 dòng thành viên
  std::vector<std::string> memberInfos = {
      "1. MSSV: .............. - Ten: ..............",
      "2. MSSV: .............. - Ten: ..............",
      "3. MSSV: .............. - Ten: ..............",
      "4. MSSV: .............. - Ten: ..............",
      "5. MSSV: .............. - Ten: .............."};

  float startY = 250.f;
  for (int i = 0; i < 5; ++i) {
    MemberRow row;

    row.iconSprite =
        new sf::Sprite(AssetManager::getInstance().getTexture("loading.png"));
    row.iconSprite->setScale({0.08f, 0.08f});
    sf::FloatRect iconB = row.iconSprite->getLocalBounds();
    row.iconSprite->setOrigin({iconB.size.x / 2.f, iconB.size.y / 2.f});
    row.iconSprite->setPosition({570.f, startY + i * 55.f});

    row.infoText =
        new sf::Text(AssetManager::getInstance().getFont("DIMIS___.ttf"),
                     memberInfos[i], 20);
    row.infoText->setFillColor(sf::Color::White);
    row.infoText->setPosition({615.f, (startY + i * 55.f) - 15.f});

    m_memberRows.push_back(row);
  }

  // 6. Nút BACK ở dưới cùng
  m_aboutBackIcon =
      new sf::Sprite(AssetManager::getInstance().getTexture("exit.png"));
  m_aboutBackIcon->setScale({0.09f, 0.09f});
  sf::FloatRect backIconB = m_aboutBackIcon->getLocalBounds();
  m_aboutBackIcon->setOrigin({backIconB.size.x / 2.f, backIconB.size.y / 2.f});
  m_aboutBackIcon->setPosition({690.f, 545.f});

  m_aboutBackLabel = new sf::Text(
      AssetManager::getInstance().getFont("DIMIS___.ttf"), "BACK", 26);
  m_aboutBackLabel->setFillColor(sf::Color(240, 235, 200));
  m_aboutBackLabel->setPosition({740.f, 530.f});

  // ── KHỞI TẠO NÚT BÁNH RĂNG (GameSetting) ────────────────────────────────
  m_settingBtnSprite.setScale({GEAR_ICON_SCALE, GEAR_ICON_SCALE});
  m_settingBtnSprite.setPosition({GEAR_X, GEAR_Y});

  // ── KHỞI TẠO NÚT GRID (ISerializable) ───────────────────────────────────
  m_serializableBtnSprite.setScale({GRID_ICON_SCALE, GRID_ICON_SCALE});
  m_serializableBtnSprite.setPosition({GRID_X, GRID_Y});

  // ── KHỞI TẠO NÚT PLAY VÀ EXIT (TRONG SUỐT) ───────────────────────────────
  m_playBtn.setSize({120.f, 100.f});
  m_playBtn.setOrigin({60.f, 50.f});
  m_playBtn.setPosition({800.f, 510.f}); // Tọa độ tương đối theo hình nền
  m_playBtn.setFillColor(sf::Color::Transparent);

  m_exitBtn.setSize({160.f, 80.f});
  m_exitBtn.setOrigin({80.f, 40.f});
  m_exitBtn.setPosition({800.f, 630.f}); // Tọa độ tương đối theo hình nền
  m_exitBtn.setFillColor(sf::Color::Transparent);
}

MenuState::~MenuState() {
  // Dọn dẹp toàn bộ con trỏ tránh rò rỉ bộ nhớ
  delete m_aboutTitleText;
  delete m_aboutBackLabel;
  delete m_aboutMainBtn;
  delete m_aboutBackIcon;
  for (auto &row : m_memberRows) {
    delete row.iconSprite;
    delete row.infoText;
  }
  m_memberRows.clear();
}

void MenuState::Init() {
  // Scale ảnh nền vừa màn hình (1600x800)
  sf::FloatRect textureSize = m_backgroundSprite.getLocalBounds();
  if (textureSize.size.x > 0 && textureSize.size.y > 0) {
    m_backgroundSprite.setScale(
        {1600.f / static_cast<float>(textureSize.size.x),
         800.f / static_cast<float>(textureSize.size.y)});
  }

  // Phát nhạc nền
  m_bgMusic.setLooping(true);
  m_bgMusic.play();
}

void MenuState::Update(float delTime, sf::RenderWindow &window) {
  // 1. Bắt phím tắt 'I' để mở/đóng About Us bất cứ lúc nào
  bool isIKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I);
  if (isIKey && !m_isIPressed) {
    m_isAboutOpen = !m_isAboutOpen;
  }
  m_isIPressed = isIKey;

  // Lấy tọa độ chuột
  sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
  sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
  bool isClicking = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

  // 2. Cập nhật state hover cho 2 nút góc dưới trái
  m_settingBtnHovered = m_settingBtnSprite.getGlobalBounds().contains(mousePos);
  m_serializableBtnHovered =
      m_serializableBtnSprite.getGlobalBounds().contains(mousePos);

  // 3. Xử lý tương tác chuột
  if (!m_isAboutOpen) {
    // Khi ở màn hình chính
    if (isClicking && !m_wasMousePressed) {
      // Click nút đỏ (PlayState)
      if (m_playBtn.getGlobalBounds().contains(mousePos)) {
        mGameManager->setState(new PlayState(mGameManager));
        return;
      }
      // Click nút gỗ dưới nút đỏ (Exit)
      if (m_exitBtn.getGlobalBounds().contains(mousePos)) {
        window.close();
        return;
      }
      // Click nút bánh răng → GameSettingState
      if (m_settingBtnHovered) {
        mGameManager->setState(new GameSettings(mGameManager));
        return;
      }
      // Click nút grid → ISerializableState
      if (m_serializableBtnHovered) {
        mGameManager->setState(new ISerializable(mGameManager));
        return;
      }
      // Click icon About Us góc trên bên phải để mở
      if (m_aboutMainBtn->getGlobalBounds().contains(mousePos)) {
        m_isAboutOpen = true;
      }
    }
  } else {
    // Khi bảng About Us đang mở, kiểm tra hiệu ứng Hover & Click cho nút BACK
    bool isBackHovered =
        m_aboutBackLabel->getGlobalBounds().contains(mousePos) ||
        m_aboutBackIcon->getGlobalBounds().contains(mousePos);
    if (isBackHovered) {
      m_aboutBackLabel->setFillColor(
          sf::Color(255, 215, 0)); // Sáng vàng khi rê chuột
      m_aboutBackIcon->setColor(sf::Color(255, 255, 180));
      if (isClicking && !m_wasMousePressed) {
        m_isAboutOpen = false; // Đóng bảng popup khi click BACK
      }
    } else {
      m_aboutBackLabel->setFillColor(sf::Color(240, 235, 200));
      m_aboutBackIcon->setColor(sf::Color::White);
    }
  }

  m_wasMousePressed = isClicking;
}

void MenuState::Render(sf::RenderWindow &window) {
  // 1. Vẽ ảnh nền
  window.draw(m_backgroundSprite);

  // 2. Vẽ icon mở About Us ở góc trên bên phải màn hình chính
  window.draw(*m_aboutMainBtn);

  // Các nút play, exit, setting, serializable được ẩn (trong suốt)
  // nên không cần gọi hàm draw() cho chúng.

  // 5. Nếu bảng About Us được mở, vẽ đè khung popup tương tác lên trên cùng
  if (m_isAboutOpen) {
    window.draw(m_aboutOverlay);
    window.draw(m_aboutBox);
    window.draw(*m_aboutTitleText);

    // Vẽ từng dòng thành viên (Icon + Text)
    for (const auto &row : m_memberRows) {
      window.draw(*(row.iconSprite));
      window.draw(*(row.infoText));
    }

    // Vẽ nút Back tương tác
    window.draw(*m_aboutBackIcon);
    window.draw(*m_aboutBackLabel);
  }
}