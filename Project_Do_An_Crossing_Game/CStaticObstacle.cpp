#include"CStaticObstacle.h"
#include "AssetManager.h"

//textureName, speed , scale , hitboxWidth, hitboxHeight, OffsetX,  OffsetY;
static StaticObstacleConfig getRandomStaticObstacleConfig() {
	static std::vector<StaticObstacleConfig> vehicleList = {
		{ "Rock1_1.png", 0.f, {1.5f, 1.5f}, 95.f, 95.f, 0.f, 0.f },
		{ "Tree.png", 0.f, {1.5f, 1.5f}, 100.f, 100.f, 0.f, 0.f }
	};
	int randomIndex = rand() % vehicleList.size();
	return vehicleList[randomIndex];
}
void StaticObstacle::Draw(sf::RenderWindow& window)
{
	window.draw(mSprite);

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

StaticObstacle::StaticObstacle(float startX, float startY) : StaticObstacle(startX, startY, getRandomStaticObstacleConfig())
{
}

StaticObstacle::StaticObstacle(float startX, float startY, const StaticObstacleConfig& config)
	:mSprite(AssetManager::getInstance().getTexture(config.textureName))
{
	mX = startX;
	mY = startY;

	mSpeed = config.speed;
	mSprite.setTexture(AssetManager::getInstance().getTexture(config.textureName));

	mSprite.setScale(config.scale);
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
	mSprite.setPosition({ mX, mY });

	mhitboxWidth = config.hitboxWidth;
	mhitboxHeight = config.hitboxHeight;
	mOffsetX = config.OffsetX;
	mOffsetY = config.OffsetY;
	
}

void StaticObstacle::MoveLogic(float delTime)
{
}

StaticObstacle::~StaticObstacle()
{
}

void StaticObstacle::UpdateState(float delTime)
{
	MoveLogic(delTime);
	mSprite.setPosition({ mX, mY });
}

sf::FloatRect StaticObstacle::getBounds()
{
	//return sf::FloatRect({ mX + mOffsetX, mY + mOffsetY }, { mhitboxWidth, mhitboxHeight });
	float left = mX - (mhitboxWidth / 2.f) + mOffsetX;
	float top = mY - (mhitboxHeight / 2.f) + mOffsetY;

	return sf::FloatRect({ left, top }, { mhitboxWidth, mhitboxHeight });
}

float StaticObstacle::getX()
{
	return mX;
}

float StaticObstacle::getY()
{
	return mY;
}

void StaticObstacle::increaseSpeed(float speedOffset)
{
	mSpeed += speedOffset;
}

