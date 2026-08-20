#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "GameManager.h"
#include "IGameState.h"

// State can thiet
#include "MenuState.hpp"

class GameManager;
class AboutUs : public IGameState
{
public:
	AboutUs(GameManager* gameManager);
	~AboutUs() override;

	void Init() override;
	void Update(float delTime, sf::RenderWindow& window) override;
	void Render(sf::RenderWindow& window) override;

private:
	GameManager* mGameManager;

	// background
	sf::Sprite m_bgSprite;

	// GUI
	sf::RectangleShape m_mainPanel;

	sf::RectangleShape m_titleBox;
	sf::Text m_titleText;

	sf::Text m_members;
	sf::Text m_gvhd;


	sf::Text m_BackText;

};

