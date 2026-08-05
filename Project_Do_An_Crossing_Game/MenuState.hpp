#pragma once
#include "IGameState.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

// Forward declaration
class GameManager;

class MenuState : public IGameState {
private:
  GameManager *mGameManager;

  sf::Sprite m_backgroundSprite;
  sf::Sound m_bgMusic;

  sf::Text m_titleText;
  sf::Text m_instructionText;

  // Nút Play
  sf::RectangleShape m_playBtn;
  sf::Text m_playText;

  // Nút Exit
  sf::RectangleShape m_exitBtn;
  sf::Text m_exitText;

  // bool m_isMusicLoaded;

public:
  MenuState(GameManager *gameManager);
  ~MenuState() override;

  void Init() override;
  void Update(float delTime, sf::RenderWindow &window) override;
  void Render(sf::RenderWindow &window) override;
};
