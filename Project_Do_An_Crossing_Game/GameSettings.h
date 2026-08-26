#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "AssetManager.h"
#include "IGameState.h"
#include "MenuState.hpp"
#include <string>
#include <vector>

class GameManager;

class GameSetting : public IGameState {
private:
  GameManager *mGameManager;
  // background
  sf::Sprite m_bgSprite;
  // GUI
  sf::RectangleShape m_mainPanel;

  sf::RectangleShape m_titleBox;
  sf::Text m_titleText;

  // Music background
  sf::RectangleShape m_bottomPanel1;
  sf::Text m_musicText;

  // Sound effect
  sf::RectangleShape m_bottomPanel2;
  sf::Text m_soundeffect;


  sf::RectangleShape m_backButton;
  sf::Text m_BackText;


public:
  GameSetting(GameManager *gameManager);
  ~GameSetting() override = default;

  void Init() override;
  void Update(float delTime, sf::RenderWindow &window) override;
  void Render(sf::RenderWindow &window) override;
};