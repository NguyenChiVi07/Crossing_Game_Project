#include "MenuState.hpp"
#include "AssetManager.h"
#include "GameManager.h"
#include "PlayState.h"

MenuState::MenuState(GameManager* gameManager) : mGameManager(gameManager)
, m_backgroundSprite(AssetManager::getInstance().getTexture("menu_copy.png"))
, m_titleText(AssetManager::getInstance().getFont("Robus-BWqOd.otf"))
, m_instructionText(AssetManager::getInstance().getFont("Robus-BWqOd.otf"))
, m_bgMusic(AssetManager::getInstance().getSoundBuffer("AdhesiveWombat - Night Shade_copy.mp3"))
{
    // --- KHỞI TẠO TRẠNG THÁI ABOUT US ---
    m_isAboutOpen = false;
    m_isIPressed = false;

    // 1. Icon mở About Us ngoài màn hình chính (Dùng con trỏ sf::Sprite*)
    m_aboutMainBtn = new sf::Sprite(AssetManager::getInstance().getTexture("about.png"));
    m_aboutMainBtn->setScale({ 0.12f, 0.12f });
    sf::FloatRect mainBtnB = m_aboutMainBtn->getLocalBounds();
    m_aboutMainBtn->setOrigin({ mainBtnB.size.x / 2.f, mainBtnB.size.y / 2.f });
    m_aboutMainBtn->setPosition({ 1520.f, 45.f }); // Góc trên bên phải

    // 2. Lớp nền mờ phủ toàn màn hình
    m_aboutOverlay.setSize({ 1600.f, 800.f });
    m_aboutOverlay.setFillColor(sf::Color(0, 0, 0, 210));

    // 3. Khung popup chính giữa (Chuẩn phong cách Pause Menu)
    m_aboutBox.setSize({ 650.f, 520.f });
    m_aboutBox.setOrigin({ 325.f, 260.f });
    m_aboutBox.setPosition({ 800.f, 400.f });
    m_aboutBox.setFillColor(sf::Color(40, 44, 52));
    m_aboutBox.setOutlineThickness(4.f);
    m_aboutBox.setOutlineColor(sf::Color(255, 200, 50));

    // 4. Tiêu đề "DEVELOPER TEAM"
    m_aboutTitleText = new sf::Text(AssetManager::getInstance().getFont("DIMIS___.ttf"), "DEVELOPER TEAM", 38);
    m_aboutTitleText->setFillColor(sf::Color(255, 200, 50));
    sf::FloatRect titleB = m_aboutTitleText->getLocalBounds();
    m_aboutTitleText->setOrigin({ titleB.size.x / 2.f, titleB.size.y / 2.f });
    m_aboutTitleText->setPosition({ 800.f, 175.f });

    // 5. Khởi tạo 5 dòng thành viên
    std::vector<std::string> memberInfos = {
        "1. MSSV: .............. - Ten: ..............",
        "2. MSSV: .............. - Ten: ..............",
        "3. MSSV: .............. - Ten: ..............",
        "4. MSSV: .............. - Ten: ..............",
        "5. MSSV: .............. - Ten: .............."
    };

    float startY = 250.f;
    for (int i = 0; i < 5; ++i) {
        MemberRow row;

        // Cài đặt icon cho từng hàng (dùng con trỏ sf::Sprite*)
        row.iconSprite = new sf::Sprite(AssetManager::getInstance().getTexture("loading.png"));
        row.iconSprite->setScale({ 0.08f, 0.08f });
        sf::FloatRect iconB = row.iconSprite->getLocalBounds();
        row.iconSprite->setOrigin({ iconB.size.x / 2.f, iconB.size.y / 2.f });
        row.iconSprite->setPosition({ 570.f, startY + i * 55.f });

        // Cài đặt nhãn chữ thông tin thành viên
        row.infoText = new sf::Text(AssetManager::getInstance().getFont("DIMIS___.ttf"), memberInfos[i], 20);
        row.infoText->setFillColor(sf::Color::White);
        row.infoText->setPosition({ 615.f, (startY + i * 55.f) - 15.f });

        m_memberRows.push_back(row);
    }

    // 6. Nút BACK ở dưới cùng (Dùng con trỏ sf::Sprite*)
    m_aboutBackIcon = new sf::Sprite(AssetManager::getInstance().getTexture("exit.png"));
    m_aboutBackIcon->setScale({ 0.09f, 0.09f });
    sf::FloatRect backIconB = m_aboutBackIcon->getLocalBounds();
    m_aboutBackIcon->setOrigin({ backIconB.size.x / 2.f, backIconB.size.y / 2.f });
    m_aboutBackIcon->setPosition({ 690.f, 545.f });

    m_aboutBackLabel = new sf::Text(AssetManager::getInstance().getFont("DIMIS___.ttf"), "BACK", 26);
    m_aboutBackLabel->setFillColor(sf::Color(240, 235, 200));
    m_aboutBackLabel->setPosition({ 740.f, 530.f });
}

MenuState::~MenuState() {
    // Dọn dẹp toàn bộ con trỏ tránh rò rỉ bộ nhớ
    delete m_aboutTitleText;
    delete m_aboutBackLabel;
    delete m_aboutMainBtn;
    delete m_aboutBackIcon;
    for (auto& row : m_memberRows) {
        delete row.iconSprite;
        delete row.infoText;
    }
    m_memberRows.clear();
}

void MenuState::Init() {
    // Scale ảnh nền vừa màn hình (1600x800)
    sf::FloatRect textureSize = m_backgroundSprite.getLocalBounds();
    if (textureSize.size.x > 0 && textureSize.size.y > 0) {
        m_backgroundSprite.setScale({ 1600.f / static_cast<float>(textureSize.size.x),
                                      800.f / static_cast<float>(textureSize.size.y) });
    }

    // Phát nhạc nền
    m_bgMusic.setLooping(true);
    m_bgMusic.play();

    m_titleText.setString("CROSSING GAME");
    m_titleText.setCharacterSize(60);
    m_titleText.setFillColor(sf::Color::Yellow);
    m_titleText.setStyle(sf::Text::Bold);

    // Căn giữa tiêu đề (màn hình 1600x800)
    sf::FloatRect titleBounds = m_titleText.getLocalBounds();
    m_titleText.setOrigin({ titleBounds.position.x + titleBounds.size.x / 2.f,
                            titleBounds.position.y + titleBounds.size.y / 2.f });
    m_titleText.setPosition({ 800.f, 250.f });

    m_instructionText.setString("Press ENTER to Start Game");
    m_instructionText.setCharacterSize(30);
    m_instructionText.setFillColor(sf::Color::White);

    sf::FloatRect instBounds = m_instructionText.getLocalBounds();
    m_instructionText.setOrigin(
        { instBounds.position.x + instBounds.size.x / 2.f,
         instBounds.position.y + instBounds.size.y / 2.f });
    m_instructionText.setPosition({ 800.f, 450.f });
}

void MenuState::Update(float delTime, sf::RenderWindow& window) {
    // 1. Nhấn ENTER để chuyển sang PlayState
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
        if (mGameManager) {
            mGameManager->setState(new PlayState(mGameManager));
            return;
        }
    }

    // 2. Bắt phím tắt 'I' để mở/đóng About Us bất cứ lúc nào
    bool isIKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I);
    if (isIKey && !m_isIPressed) {
        m_isAboutOpen = !m_isAboutOpen;
    }
    m_isIPressed = isIKey;

    // Lấy tọa độ chuột
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
    bool isClicking = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

    static bool isMousePressedPrev = false;

    // 3. Xử lý tương tác chuột
    if (!m_isAboutOpen) {
        // Khi ở màn hình chính, click vào icon About Us góc trên bên phải để mở
        if (isClicking && !isMousePressedPrev) {
            if (m_aboutMainBtn->getGlobalBounds().contains(mousePos)) {
                m_isAboutOpen = true;
            }
        }
    }
    else {
        // Khi bảng About Us đang mở, kiểm tra hiệu ứng Hover & Click cho nút BACK
        bool isBackHovered = m_aboutBackLabel->getGlobalBounds().contains(mousePos) || m_aboutBackIcon->getGlobalBounds().contains(mousePos);
        if (isBackHovered) {
            m_aboutBackLabel->setFillColor(sf::Color(255, 215, 0)); // Sáng vàng khi rê chuột
            m_aboutBackIcon->setColor(sf::Color(255, 255, 180));
            if (isClicking && !isMousePressedPrev) {
                m_isAboutOpen = false; // Đóng bảng popup khi click BACK
            }
        }
        else {
            m_aboutBackLabel->setFillColor(sf::Color(240, 235, 200));
            m_aboutBackIcon->setColor(sf::Color::White);
        }
    }
    isMousePressedPrev = isClicking;
}

void MenuState::Render(sf::RenderWindow& window) {
    // 1. Vẽ ảnh nền
    window.draw(m_backgroundSprite);

    // 2. Vẽ tiêu đề và hướng dẫn
    window.draw(m_titleText);
    window.draw(m_instructionText);

    // 3. Vẽ icon mở About Us ở góc trên bên phải màn hình chính
    window.draw(*m_aboutMainBtn);

    // 4. Nếu bảng About Us được mở, vẽ đè khung popup tương tác lên trên cùng
    if (m_isAboutOpen) {
        window.draw(m_aboutOverlay);
        window.draw(m_aboutBox);
        window.draw(*m_aboutTitleText);

        // Vẽ từng dòng thành viên (Icon + Text)
        for (const auto& row : m_memberRows) {
            window.draw(*(row.iconSprite));
            window.draw(*(row.infoText));
        }

        // Vẽ nút Back tương tác
        window.draw(*m_aboutBackIcon);
        window.draw(*m_aboutBackLabel);
    }
}   