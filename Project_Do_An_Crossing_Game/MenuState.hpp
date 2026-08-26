#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include "AssetManager.h"
#include "GameManager.h"
#include "IGameState.h"

// State can thiet
#include "GameSettings.h"
#include "PlayState.h"
#include "SelectSkinState.h"
#include "AboutUs.h"
#include "SaveLoadUI.h"

class GameManager;

class MenuState : public IGameState {
private:
	GameManager* mGameManager;

	sf::Sprite m_backgroundSprite;
	sf::Sprite m_playBtn;
	sf::Sprite m_exitBtn;
	sf::Sprite m_settingBtnSprite;
	sf::Sprite m_SaveLoad;
	sf::Sprite m_Aboutus;
	sf::Sprite m_skin;

	sf::Sprite bottomPanel;

	//Sound 
	sf::Sound m_click;

	//Load&Save
	SaveLoadUI* m_saveLoadUI;
	bool m_isSaveLoadMenuOpen;

public:
	MenuState(GameManager* gameManager);
	~MenuState() override;

	void Init() override;
	void HandleEvent(const sf::Event& event, sf::RenderWindow& window) override;
	void Update(float delTime, sf::RenderWindow& window) override;
	void Render(sf::RenderWindow& window) override;
};