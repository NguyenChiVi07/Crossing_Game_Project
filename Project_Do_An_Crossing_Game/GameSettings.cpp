#include "GameSettings.h"
#include "GameManager.h"
GameSetting::GameSetting(GameManager* gameManager)
	: mGameManager(gameManager),
	m_bgSprite(AssetManager::getInstance().getTexture("bg.png")),
	//Text
	m_titleText(AssetManager::getInstance().getFont("DIMIS___.ttf")),
	m_BackText(AssetManager::getInstance().getFont("DIMIS___.ttf")),
	m_musicText(AssetManager::getInstance().getFont("DIMIS___.ttf")),
	m_soundeffect(AssetManager::getInstance().getFont("DIMIS___.ttf")),

	//Sound
	m_click(AssetManager::getInstance().getSoundBuffer("click.mp3"))
{ 
	// Main Panel
	m_mainPanel.setPosition({ 144, 95.f });

	m_mainPanel.setSize({ 1311, 608 });

	m_mainPanel.setFillColor(sf::Color(20, 45, 55, 185));

	m_mainPanel.setOutlineThickness(4.f);

	m_mainPanel.setOutlineColor(sf::Color(230, 245, 245, 230));

	//  TITLE BOX

	m_titleBox.setPosition({ 447.7, 48 });

	m_titleBox.setSize({ 704.7, 95.7 });

	m_titleBox.setFillColor(sf::Color(40, 145, 205, 255));

	m_titleBox.setOutlineThickness(3.f);

	m_titleBox.setOutlineColor(sf::Color::White);

	//  TITLE TEXT

	m_titleText.setString(" SETTING ");

	m_titleText.setCharacterSize(34);

	m_titleText.setFillColor(sf::Color::White);

	{
		sf::FloatRect bounds = m_titleText.getLocalBounds();

		m_titleText.setOrigin({ bounds.position.x + bounds.size.x / 2.f,
							   bounds.position.y + bounds.size.y / 2.f });

		m_titleText.setPosition({ 800, 95 });
	}

	// BOTTOM PANEL 1
	m_bottomPanel1.setPosition({ 215, 200 });

	m_bottomPanel1.setSize({ 1169, 86 });

	m_bottomPanel1.setFillColor(sf::Color(40, 145, 205, 220));

	m_bottomPanel1.setOutlineThickness(3.f);

	m_bottomPanel1.setOutlineColor(sf::Color(230, 245, 245, 230));

	// Music text
	m_musicText.setString("MUSIC BACKGROUND : " + std::string(AssetManager::getInstance().isSfxOn() ? "ON" : "OFF"));

	m_musicText.setCharacterSize(34);

	m_musicText.setFillColor(sf::Color::White);

	{
		sf::FloatRect bounds = m_musicText.getLocalBounds();

		m_musicText.setOrigin({ bounds.position.x + bounds.size.x / 2.f,
							   bounds.position.y + bounds.size.y / 2.f });

		m_musicText.setPosition({ 800, 243 });
	}

	// BOTTOM PANEL 2
	m_bottomPanel2.setPosition({ 215, 337 });

	m_bottomPanel2.setSize({ 1169, 86 });

	m_bottomPanel2.setFillColor(sf::Color(40, 145, 205, 220));

	m_bottomPanel2.setOutlineThickness(3.f);

	m_bottomPanel2.setOutlineColor(sf::Color(230, 245, 245, 230));

	// sound text
	m_soundeffect.setString("SOUND EFFECT : " + std::string(AssetManager::getInstance().isSfxOn() ? "ON" : "OFF"));

	m_soundeffect.setCharacterSize(34);

	m_soundeffect.setFillColor(sf::Color::White);

	{
		sf::FloatRect bounds = m_soundeffect.getLocalBounds();

		m_soundeffect.setOrigin({ bounds.position.x + bounds.size.x / 2.f,
							   bounds.position.y + bounds.size.y / 2.f });

		m_soundeffect.setPosition({ 800, 380 });
	}

	// ENTER BUTTON


	m_backButton.setFillColor(sf::Color(255, 180, 70));

	m_backButton.setOutlineThickness(3.f);

	m_backButton.setOutlineColor(sf::Color::White);

	m_backButton.setPosition({ 1200, 125 });

	m_backButton.setSize({ 190, 50 });
	

	// BACK TEXT
	m_BackText.setString("BACK");

	m_BackText.setCharacterSize(34);

	m_BackText.setFillColor(sf::Color::Black);

	{
		sf::FloatRect bounds = m_BackText.getLocalBounds();

		m_BackText.setOrigin({ bounds.position.x + bounds.size.x / 2.f,
							   bounds.position.y + bounds.size.y / 2.f });

		m_BackText.setPosition({ 1300, 150 });
	}

	// SOUND
	if (AssetManager::getInstance().isMusicOn())
	{
		AssetManager::getInstance().playMusic("../ASSETS/AUDIO/AdhesiveWombat - Night Shade_copy.mp3", true);
	}
}

void GameSetting::Init()
{
}

void GameSetting::Update(float delTime, sf::RenderWindow& window)
{
	static bool mouseWasPressed = false;

	bool mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

	sf::Vector2i pixelPosition = sf::Mouse::getPosition(window);
	sf::Vector2f mousePosition = window.mapPixelToCoords(pixelPosition);

	sf::FloatRect m_musicbox = m_musicText.getGlobalBounds();
	sf::FloatRect m_soundeffectbox = m_soundeffect.getGlobalBounds();
	sf::FloatRect m_backbox = m_BackText.getGlobalBounds();

	if (m_musicbox.contains(mousePosition))
	{
		m_musicText.setFillColor(sf::Color::Yellow);
	}
	else
	{
		m_musicText.setFillColor(sf::Color::White);
	}

	if (m_soundeffectbox.contains(mousePosition))
	{
		m_soundeffect.setFillColor(sf::Color::Yellow);
	}
	else
	{
		m_soundeffect.setFillColor(sf::Color::White);
	}

	if (m_backbox.contains(mousePosition))
	{
		m_BackText.setFillColor(sf::Color::Yellow);
	}
	else
	{
		m_BackText.setFillColor(sf::Color::White);
	}

	if (mousePressed && !mouseWasPressed)
	{
		if (m_backbox.contains(mousePosition))
		{
			if (AssetManager::getInstance().isSfxOn())
			{
				m_click.play();
			}
			mGameManager->setState(new MenuState(mGameManager));
			mouseWasPressed = true;
			return;
		}
		else if (m_musicbox.contains(mousePosition))
		{
			if (AssetManager::getInstance().isSfxOn())
			{
				m_click.play();
			}
			AssetManager::getInstance().toggleMusic();
			m_musicText.setString("MUSIC BACKGROUND : " + std::string(AssetManager::getInstance().isMusicOn() ? "ON" : "OFF"));
		}
		else if (m_soundeffectbox.contains(mousePosition))
		{
			if (AssetManager::getInstance().isSfxOn())
			{
				m_click.play();
			}
			AssetManager::getInstance().toggleSfx();
			m_soundeffect.setString("SOUND EFFECT : " + std::string(AssetManager::getInstance().isSfxOn() ? "ON" : "OFF"));
		}
	}
	mouseWasPressed = mousePressed;
}

void GameSetting::Render(sf::RenderWindow& window)
{
	window.clear(sf::Color(20, 30, 40));

	window.draw(m_bgSprite);

	window.draw(m_mainPanel);

	window.draw(m_titleBox);

	window.draw(m_titleText);

	window.draw(m_bottomPanel1);

	window.draw(m_musicText);

	window.draw(m_bottomPanel2);

	window.draw(m_soundeffect);

	window.draw(m_backButton);

	window.draw(m_BackText);
}
