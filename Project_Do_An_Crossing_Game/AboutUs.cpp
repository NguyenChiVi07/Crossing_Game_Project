#include "AboutUs.h"

AboutUs::AboutUs(GameManager* gameManager)
	: mGameManager(gameManager),
	m_bgSprite(AssetManager::getInstance().getTexture("bg.png")),
	//Text
	m_titleText(AssetManager::getInstance().getFont("DIMIS___.ttf")),
	m_members(AssetManager::getInstance().getFont("DIMIS___.ttf")),
	m_BackText(AssetManager::getInstance().getFont("DIMIS___.ttf")),
	m_gvhd(AssetManager::getInstance().getFont("DIMIS___.ttf"))
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

	m_titleText.setString(" ABOUT US ");

	m_titleText.setCharacterSize(60);

	m_titleText.setFillColor(sf::Color::White);

	{
		sf::FloatRect bounds = m_titleText.getLocalBounds();

		m_titleText.setOrigin({ bounds.position.x + bounds.size.x / 2.f,
							   bounds.position.y + bounds.size.y / 2.f });

		m_titleText.setPosition({ 800, 95 });
	}

	//  GVHD TEXT

	m_gvhd.setString("GVHD : Truong Toan Thinh");

	m_gvhd.setCharacterSize(40);

	m_gvhd.setFillColor(sf::Color::White);

	{
		sf::FloatRect bounds = m_gvhd.getLocalBounds();

		m_gvhd.setOrigin({ bounds.position.x + bounds.size.x / 2.f,
							   bounds.position.y + bounds.size.y / 2.f });

		m_gvhd.setPosition({ 800, 500 });
	}

	// MEMBERS
	m_members.setString("1. 25127397 - Nguyen Trung Kien\n2. 25127549 - Nguyen Chi Vi\n 3. 25127530 - Nguyen Vo Minh Tri \n 4. 25127527 - Le Minh Tri \n");

	m_members.setCharacterSize(40);

	m_members.setFillColor(sf::Color::White);

	{
		sf::FloatRect bounds = m_members.getLocalBounds();

		m_members.setOrigin({ bounds.position.x + bounds.size.x / 2.f,
							   bounds.position.y + bounds.size.y / 2.f });

		m_members.setPosition({ 800, 350 });
	}

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

AboutUs::~AboutUs()
{
}

void AboutUs::Init()
{
}

void AboutUs::Update(float delTime, sf::RenderWindow& window)
{
	static bool mouseWasPressed = false;

	bool mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

	sf::Vector2i pixelPosition = sf::Mouse::getPosition(window);
	sf::Vector2f mousePosition = window.mapPixelToCoords(pixelPosition);

	sf::FloatRect m_backbox = m_BackText.getGlobalBounds();

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
				AssetManager::getInstance().playSfx("click.mp3");
			}
			mGameManager->setState(new MenuState(mGameManager));
			mouseWasPressed = true;
			return;
		}
	}
	mouseWasPressed = mousePressed;
}

void AboutUs::Render(sf::RenderWindow& window)
{
	window.draw(m_bgSprite);
	window.draw(m_mainPanel);
	window.draw(m_titleBox);
	window.draw(m_titleText);
	window.draw(m_members);
	window.draw(m_gvhd);
	window.draw(m_BackText);
}
