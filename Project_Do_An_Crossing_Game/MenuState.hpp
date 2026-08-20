#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "GameManager.h"
#include "IGameState.h"

// State can thiet
#include "GameSettings.h"
#include "PlayState.h"
#include "SelectSkinState.h"
#include "AboutUs.h"
class GameManager;

class MenuState : public IGameState {
private:
  GameManager *mGameManager;

  sf::Sprite m_backgroundSprite;
  sf::Sprite m_playBtn;
  sf::Sprite m_exitBtn;
  sf::Sprite m_settingBtnSprite;
  sf::Sprite m_SaveLoad;
  sf::Sprite m_Aboutus;
  sf::Sprite m_skin;

  sf::Sprite bottomPanel;

public:
  MenuState(GameManager *gameManager);
  ~MenuState() override;

  void Init() override;
  void Update(float delTime, sf::RenderWindow &window) override;
  void Render(sf::RenderWindow &window) override;
};