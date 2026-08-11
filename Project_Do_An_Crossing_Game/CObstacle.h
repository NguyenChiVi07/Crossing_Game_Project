#pragma once
#include <SFML/Graphics.hpp>
#include "ISerializable.h" // 1. Nhúng bản hợp đồng vào đây

using namespace std;

// 2. Kế thừa ISerializable
class Obstacle : public ISerializable
{
public:
	virtual ~Obstacle() = default;
	virtual void UpdateState(float delTime) = 0;
	virtual void Draw(sf::RenderWindow& window) = 0;

	virtual sf::FloatRect getBounds() = 0;
	virtual float getX() = 0;
	virtual float getY() = 0;
	virtual void increaseSpeed(float speedOffset) = 0;
	virtual void setSpeed(float speed) = 0;

	// 3. THÊM DÒNG NÀY: Ép buộc các class con phải khai báo thẻ căn cước
	virtual int getType() const = 0;
};