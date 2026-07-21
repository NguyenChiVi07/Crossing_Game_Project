#pragma once
#include <SFML/Graphics.hpp>
#include "IGameState.h"
#include"PlayState.h"
class GameManager
{
private:
	IGameState* mCurrentState;
public:
	GameManager(); // Constructor
	~GameManager(); // Destructor
	IGameState* getState(); // Get the current game state
	void setState(IGameState* state); // Set the current game state
	void Update(float deltime, sf::RenderWindow& window); // Update the current game state
	void Render(sf::RenderWindow& window); // Render the current game state
};