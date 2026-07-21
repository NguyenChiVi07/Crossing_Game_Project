#include "CPeople.h"

CPEOPLE::CPEOPLE(float startX, float startY)
{
	m_player.setFillColor(sf::Color::Green);
	m_player.setPosition({ m_X, m_Y });
	m_X = startX; m_Y = startY;
	m_Speed = 100.f;
	m_isKeyPressed = false;
}

CPEOPLE::~CPEOPLE()
{
}

void CPEOPLE::update(float delTime)
{
    const float STEP_SIZE = 100.f; // Khoảng cách 1 làn đường = 100px

    bool wPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);
    bool sPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
    bool aPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
    bool dPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);

    // Chỉ thực hiện nhảy 1 lần khi bắt đầu bấm phím
    if (!m_isKeyPressed)
    {
        if (wPressed)
        {
            m_Y -= STEP_SIZE;
            m_isKeyPressed = true;
        }
        else if (sPressed)
        {
            m_Y += STEP_SIZE;
            m_isKeyPressed = true;
        }
        else if (aPressed)
        {
            m_X -= STEP_SIZE;
            m_isKeyPressed = true;
        }
        else if (dPressed)
        {
            m_X += STEP_SIZE;
            m_isKeyPressed = true;
        }
    }

    // Reset lại trạng thái khi người chơi thả hết các phím di chuyển
    if (!wPressed && !sPressed && !aPressed && !dPressed)
    {
        m_isKeyPressed = false;
    }

    // Cập nhật vị trí hiển thị (Đã sửa lỗi cộng trùng vị trí)
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
	  float hitboxWidth = 100.f;
	  float hitboxHeight = 100.f;

	  // 2. Độ lệch 30px để đẩy ô hitbox 60x60 vào tâm của Sprite 120x120
	  float offsetX = 0.f;
	  float offsetY = 0.f;

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
