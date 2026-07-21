#pragma once
#include<SFML/Graphics.hpp>
using namespace std;

class Obstacle
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
};