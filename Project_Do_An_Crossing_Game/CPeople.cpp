#include "CPeople.h"

CPEOPLE::CPEOPLE(float startX, float startY, const std::string& skinTextureName)
    : m_X(startX), m_Y(startY),
    m_skinTextureName(skinTextureName),
    m_player(AssetManager::getInstance().getTexture(skinTextureName)) 
{
    m_Speed = 100.f;
    m_isKeyPressed = false;

    
    m_faceRight = true;
    m_row = 0;
    m_animation = new Animation(&AssetManager::getInstance().getTexture(skinTextureName), sf::Vector2u(4, 1), 0.1f);
    m_player.setScale({ 2.5f, 2.5f });
}

CPEOPLE::~CPEOPLE()
{
    if (m_animation != NULL)
    {
        delete m_animation;
    }
}

void CPEOPLE::update(float delTime)
{
    const float STEP_SIZE = 100.f; 

    bool wPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);
    bool sPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
    bool aPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
    bool dPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);

    //// Chỉ thực hiện nhảy 1 lần khi bắt đầu bấm phím
    if (!m_isKeyPressed)
    {
        if (wPressed)
        {
            m_Y -= STEP_SIZE ;
            m_isKeyPressed = true;
        }
        else if (sPressed)
        {
            m_Y += STEP_SIZE ;
            m_isKeyPressed = true;
        }
        else if (aPressed)
        {
            m_faceRight = false;
            m_X -= STEP_SIZE ;
            m_isKeyPressed = true;
        }
        else if (dPressed)
        {
            m_faceRight = true;
            m_X += STEP_SIZE ;
            m_isKeyPressed = true;
        }
    }
    //}

    //// Reset lại trạng thái khi người chơi thả hết các phím di chuyển
    if (!wPressed && !sPressed && !aPressed && !dPressed)
    {
        m_isKeyPressed = false;
    }

    //// Cập nhật vị trí hiển thị (Đã sửa lỗi cộng trùng vị trí)
    m_animation->Update(0, delTime, m_faceRight);
    m_player.setTextureRect(m_animation->uvRect);
    m_player.setPosition({ m_X, m_Y });
}

void CPEOPLE::Draw(sf::RenderWindow& window)
{
	// Vẽ nhân vật bình thường
	window.draw(m_player);

	// BẬT CHẾ ĐỘ NHÌN THẤY HITBOX (Dùng để debug)
	sf::FloatRect hitbox = getBounds();
	sf::RectangleShape debugBox({ hitbox.size.x, hitbox.size.y });
	debugBox.setPosition({ hitbox.position.x, hitbox.position.y });
	debugBox.setFillColor(sf::Color(255, 0, 0, 100)); // Màu đỏ mờ (Alpha = 100)
	debugBox.setOutlineColor(sf::Color::Red);
	debugBox.setOutlineThickness(2.f);

	// Vẽ hitbox đè lên nhân vật
	window.draw(debugBox);
}

sf::FloatRect CPEOPLE::getBounds() const
{
	// 1. Kích thước hitbox cố định theo ý bạn muốn
	  float hitboxWidth = 80.f;
	  float hitboxHeight = 80.f;

	  // 2. Độ lệch 30px để đẩy ô hitbox 60x60 vào tâm của Sprite 120x120
	  float offsetX = 0.f;
	  float offsetY = 5.f;

	  // 3. CÚ PHÁP CHUẨN SFML 3.0: Nhận vào 2 Vector {tọa độ}, {kích thước}
	  return sf::FloatRect({ m_X + offsetX, m_Y + offsetY }, { hitboxWidth, hitboxHeight });
}

float CPEOPLE::getY() const
{
	return m_Y;
}

void CPEOPLE::resetPosition(float startX, float startY)
{
	m_X = startX;
	m_Y = startY;
	m_player.setPosition({ m_X,m_Y });
}

void CPEOPLE::saveToFile(std::ofstream& out) {
    // Ép kiểu tọa độ X và Y thành chuỗi byte để ghi vào file .dat
    out.write(reinterpret_cast<const char*>(&m_X), sizeof(m_X));
    out.write(reinterpret_cast<const char*>(&m_Y), sizeof(m_Y));
}

void CPEOPLE::loadFromFile(std::ifstream& in) {
    // Đọc các byte từ file .dat và đắp lại vào tọa độ X, Y
    in.read(reinterpret_cast<char*>(&m_X), sizeof(m_X));
    in.read(reinterpret_cast<char*>(&m_Y), sizeof(m_Y));

    // Cực kỳ quan trọng: Cập nhật lại hình ảnh hiển thị trên màn hình
    m_player.setPosition({ m_X, m_Y });
}
