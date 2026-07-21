#include "CANIMAL.h"
//textureName;
//speed;
//imageCount;
//scale;
//hitboxWidth;
//hitboxHeight;
//OffsetX;
//OffsetY;
static AnimalConfig getRandomAnimalConfig() {
	static std::vector<AnimalConfig> animalList = {
		{ "Bird.png", 150.f, {6, 1}, {3.0f, 3.0f}, 100.f, 100.f, 0.f, 0.f },

		{ "Cat.png", 180.f, {6, 1}, {2.0f, 2.0f}, 100.f, 80.f, 0.f, 0.f },

		{ "Dog.png", 100.f, {6, 1}, {2.0f, 2.0f}, 100.f, 80.f, 0.f, 0.f }
	};
	int randomIndex = rand() % animalList.size();
	return animalList[randomIndex];
}

void CANIMAL::Draw(sf::RenderWindow& window)
{
	window.draw(m_Sprite);
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

void CANIMAL::UpdateState(float delTime)
{
	MoveLogic(delTime);
	m_animation->Update(0, delTime, m_faceRight);
	m_Sprite.setTextureRect(m_animation->uvRect);
	m_Sprite.setPosition({ m_X, m_Y });
}

sf::FloatRect CANIMAL::getBounds()
{
	//return sf::FloatRect({ m_X + m_OffsetX, m_Y + m_OffsetY }, { m_hitboxWidth, m_hitboxHeight });
	float left = m_X - (m_hitboxWidth / 2.f) + m_OffsetX;
	float top = m_Y - (m_hitboxHeight / 2.f) + m_OffsetY;

	return sf::FloatRect({ left, top }, { m_hitboxWidth, m_hitboxHeight });
}

CANIMAL::CANIMAL(float startX, float startY, const AnimalConfig& config)
	: m_Sprite(AssetManager::getInstance().getTexture(config.textureName)) // CHÍNH XÁC: Nạp ảnh ngay lập tức!
{
	m_X = startX;
	m_Y = startY;
	m_faceRight = false; // tùy vào hướng di chuyển của con vật
	m_row = 0;


	m_Speed = config.speed;
	m_Sprite.setTexture(AssetManager::getInstance().getTexture(config.textureName));

	m_Sprite.setScale(config.scale);
	m_animation = new Animation(&AssetManager::getInstance().getTexture(config.textureName), config.imageCount, 0.1f);
	float frameWidth = static_cast<float>(m_animation->uvRect.size.x);
	float frameHeight = static_cast<float>(m_animation->uvRect.size.y);
	m_Sprite.setOrigin({ frameWidth / 2.f, frameHeight / 2.f });
	m_Sprite.setPosition({ m_X, m_Y });

	m_hitboxWidth = config.hitboxWidth;
	m_hitboxHeight = config.hitboxHeight;
	m_OffsetX = config.OffsetX;
	m_OffsetY = config.OffsetY;
}

void CANIMAL::MoveLogic(float delTime)
{
	m_X -= m_Speed * delTime;
}

CANIMAL::CANIMAL(float startX, float startY) : CANIMAL(startX, startY, getRandomAnimalConfig())
{
}

CANIMAL::~CANIMAL()
{
	delete m_animation;
}

