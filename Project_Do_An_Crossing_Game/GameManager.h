#pragma once
#include "IGameState.h"
#include "MenuState.hpp"
#include "MenuState.hpp"
#include "PlayState.h"
#include "PlayState.h"
#include "dashBoard.hpp"
#include "dashBoard.hpp"
#include <SFML/Graphics.hpp>
class GameManager {
private:
  IGameState *mCurrentState;

public:
  GameManager();                    // Constructor
  ~GameManager();                   // Destructor
  IGameState *getState();           // Get the current game state
  void setState(IGameState *state); // Set the current game state
  void Update(float deltime,
              sf::RenderWindow &window); // Update the current game state
  void Render(sf::RenderWindow &window); // Render the current game state
};