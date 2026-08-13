#pragma once
#include "Animation.h"
#include "IGameState.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>

class GameManager;

class SelectSkinState : public IGameState {
private:
  GameManager *mGameManager;

  std::vector<std::string> m_skinList;
  int m_currentIndex;

  std::vector<std::unique_ptr<sf::Sprite>> m_skinSprites;

  std::vector<std::unique_ptr<Animation>> m_animations;

  // background
  sf::Sprite m_bgSprite;

  // GUI
  sf::RectangleShape m_mainPanel;

  sf::RectangleShape m_titleBox;
  sf::Text m_titleText;

  sf::RectangleShape m_bottomPanel;

  sf::Sprite m_selectedIcon;

  sf::RectangleShape m_enterButton;
  sf::Text m_enterText;

  sf::Text m_instructionText;

  void updateSkinLayout();

  void updateAnimation(float deltaTime);

  void updateSelectedSkin();

  void handleMouseClick(sf::Vector2f mousePosition);

  bool isMouseOver(const sf::FloatRect &rect, sf::Vector2f mousePosition) const;

public:
  SelectSkinState(GameManager *gameManager);
  ~SelectSkinState() override = default;

  void Init() override;
  void Update(float delTime, sf::RenderWindow &window) override;
  void Render(sf::RenderWindow &window) override;
};