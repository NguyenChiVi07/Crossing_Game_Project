#include "CVehicle.h"
static VehicleConfig getRandomVehicleConfig() {
	static std::vector<VehicleConfig> vehicleList = {
		{ "Police.png", 150.f, {2.f, 2.f}, 180.f, 100.f, 0.f, 0.f },
		{ "Green Sedan.png", 150.f, {2.f, 2.f}, 180.f, 100.f, 0.f, 0.f }
	};
	int randomIndex = rand() % vehicleList.size();
	return vehicleList[randomIndex];
}

CVEHICLE::CVEHICLE(float startX, float startY, const VehicleConfig& config)
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
void CVEHICLE::MoveLogic(float delTime)
{
	mX -= mSpeed * delTime;
}

CVEHICLE::CVEHICLE(float startX, float startY)
	: CVEHICLE(startX, startY, getRandomVehicleConfig())
{
}

CVEHICLE::~CVEHICLE()
{
}

void CVEHICLE::UpdateState(float delTime)
{
	MoveLogic(delTime);
	mSprite.setPosition({ mX, mY });
}

void CVEHICLE::Draw(sf::RenderWindow& window)
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

sf::FloatRect CVEHICLE::getBounds()
{
	float left = mX - (mhitboxWidth / 2.f) + mOffsetX;
	float top = mY - (mhitboxHeight / 2.f) + mOffsetY;

	return sf::FloatRect({ left, top }, { mhitboxWidth, mhitboxHeight });
    //return sf::FloatRect({ mX + mOffsetX, mY + mOffsetY }, { mhitboxWidth, mhitboxHeight });
}

float CVEHICLE::getX()
{
    return mX;
}

float CVEHICLE::getY()
{
	return mY;
}

void CVEHICLE::increaseSpeed(float speedOffset)
{
    mSpeed += speedOffset;
}
