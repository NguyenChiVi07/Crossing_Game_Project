#pragma once
#include<SFML/Graphics.hpp>
class IGameState
{
public:
	virtual ~IGameState() {}
	virtual void Init() = 0;
	virtual void Update(float delTime, sf::RenderWindow& window) = 0;
	virtual void Render(sf::RenderWindow& window) = 0;
};
