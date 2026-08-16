#include "MenuState.hpp"

MenuState::MenuState(GameManager *gameManager)
    : mGameManager(gameManager),
      m_backgroundSprite(AssetManager::getInstance().getTexture("menu_copy.png")),
      m_playBtn(AssetManager::getInstance().getTexture("play_copy.png")),
      m_exitBtn(AssetManager::getInstance().getTexture("exit_button.png")),
	  bottomPanel(AssetManager::getInstance().getTexture("bottomPanel.png")),
	  m_settingBtnSprite(AssetManager::getInstance().getTexture("Setting.png")),
	  m_ranking(AssetManager::getInstance().getTexture("Ranking.png")),
	  m_Aboutus(AssetManager::getInstance().getTexture("aboutus.png")),
	  m_skin(AssetManager::getInstance().getTexture("skin.png")),
	  // Sound 
	  m_click(AssetManager::getInstance().getSoundBuffer("click.mp3"))
{
    {
        sf::FloatRect bounds = m_backgroundSprite.getLocalBounds();

        m_backgroundSprite.setOrigin({ bounds.position.x + bounds.size.x / 2.f,
                                   bounds.position.y + bounds.size.y / 2.f });
        m_backgroundSprite.setScale({ 1.2 ,1.2 });
        m_backgroundSprite.setPosition({ 800, 400 });
    }

    {
        sf::FloatRect bounds = m_playBtn.getLocalBounds();

        m_playBtn.setOrigin({ bounds.position.x + bounds.size.x / 2.f,
                                   bounds.position.y + bounds.size.y / 2.f });
        m_playBtn.setScale({ 1.3,1.3 });
        m_playBtn.setPosition({ 805, 505 });
    }

    {
        sf::FloatRect bounds = m_exitBtn.getLocalBounds();

        m_exitBtn.setOrigin({ bounds.position.x + bounds.size.x / 2.f,
                                   bounds.position.y + bounds.size.y / 2.f });
        m_exitBtn.setScale({ 1.3 ,1.3 });
        m_exitBtn.setPosition({ 805, 658 });
    }

	{
		sf::FloatRect bounds = bottomPanel.getLocalBounds();

		bottomPanel.setOrigin({ bounds.position.x + bounds.size.x / 2.f,
								   bounds.position.y + bounds.size.y / 2.f });
		bottomPanel.setScale({ 1 ,1 });
		bottomPanel.setPosition({ 800, 780 });
	}

	{
		sf::FloatRect bounds = m_settingBtnSprite.getLocalBounds();

		m_settingBtnSprite.setOrigin({ bounds.position.x + bounds.size.x / 2.f,
								   bounds.position.y + bounds.size.y / 2.f });
		m_settingBtnSprite.setScale({ 0.8 ,0.8 });
		m_settingBtnSprite.setPosition({ 80, 725 });
	}

	{
		sf::FloatRect bounds = m_ranking.getLocalBounds();

		m_ranking.setOrigin({ bounds.position.x + bounds.size.x / 2.f,
								   bounds.position.y + bounds.size.y / 2.f });
		m_ranking.setScale({ 0.7 ,0.7 });
		m_ranking.setPosition({ 254, 725 });
	}

	{
		sf::FloatRect bounds = m_Aboutus.getLocalBounds();

		m_Aboutus.setOrigin({ bounds.position.x + bounds.size.x / 2.f,
								   bounds.position.y + bounds.size.y / 2.f });
		m_Aboutus.setScale({ 0.7 ,0.7 });
		m_Aboutus.setPosition({ 423, 725 });
	}

	{
		sf::FloatRect bounds = m_skin.getLocalBounds();

		m_skin.setOrigin({ bounds.position.x + bounds.size.x / 2.f,
								   bounds.position.y + bounds.size.y / 2.f });
		m_skin.setScale({ 0.9 ,0.9 });
		m_skin.setPosition({ 1500, 725 });
	}

  // Audio 
	if (AssetManager::getInstance().isMusicOn())
	{
		AssetManager::getInstance().playMusic("../ASSETS/AUDIO/AdhesiveWombat - Night Shade_copy.mp3", true);
	}
}

MenuState::~MenuState() {
}

void MenuState::Init() 
{

}

void MenuState::Update(float delTime, sf::RenderWindow &window) 
{
  
	static bool mouseWasPressed = false;

	bool mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

	sf::Vector2i pixelPosition = sf::Mouse::getPosition(window);
	sf::Vector2f mousePosition = window.mapPixelToCoords(pixelPosition);

	sf::FloatRect playbox = m_playBtn.getGlobalBounds();
	sf::FloatRect exitbox = m_exitBtn.getGlobalBounds();
	sf::FloatRect settingbox = m_settingBtnSprite.getGlobalBounds();
	sf::FloatRect rankingbox = m_ranking.getGlobalBounds();
	sf::FloatRect aboutusbox = m_Aboutus.getGlobalBounds();
	sf::FloatRect skinbox = m_skin.getGlobalBounds();


	if (playbox.contains(mousePosition))
	{
		m_playBtn.setScale({ 1.4,1.4 });
	}
	else
	{
		m_playBtn.setScale({ 1.3,1.3 });
	}

	if (exitbox.contains(mousePosition))
	{
		m_exitBtn.setScale({ 1.4,1.4 });
	}
	else
	{
		m_exitBtn.setScale({ 1.3,1.3 });
	}

	if (settingbox.contains(mousePosition))
	{
		m_settingBtnSprite.setScale({ 0.9,0.9 });
	}
	else
	{
		m_settingBtnSprite.setScale({ 0.8,0.8 });
	}
	if (rankingbox.contains(mousePosition))
	{
		m_ranking.setScale({ 0.8,0.8 });
	}
	else
	{
		m_ranking.setScale({ 0.7,0.7 });
	}
	if (aboutusbox.contains(mousePosition))
	{
		m_Aboutus.setScale({ 0.8,0.8 });
	}
	else
	{
		m_Aboutus.setScale({ 0.7,0.7 });
	}
	if (skinbox.contains(mousePosition))
	{
		m_skin.setScale({ 1,1 });
	}
	else
	{
		m_skin.setScale({ 0.9,0.9 });
	}

	if (mousePressed && !mouseWasPressed)
	{
		if (playbox.contains(mousePosition))
		{
			if (AssetManager::getInstance().isSfxOn())
			{
				m_click.play();
			}
			mGameManager->setState(new PlayState(mGameManager));
			return;
		}
		else if (exitbox.contains(mousePosition))
		{
			if (AssetManager::getInstance().isSfxOn())
			{
				m_click.play();
			}
			window.close();
		}
		else if (settingbox.contains(mousePosition))
		{
			if (AssetManager::getInstance().isSfxOn())
			{
				m_click.play();
			}
			mGameManager->setState(new GameSetting(mGameManager));
			return;
		}
		else if (rankingbox.contains(mousePosition))
		{
			//if (AssetManager::getInstance().isSfxOn())
			//{
			//	m_click.play();
			//}
			//mGameManager->setState(new GameSetting(mGameManager));
	
		}
		else if (aboutusbox.contains(mousePosition))
		{
			if (AssetManager::getInstance().isSfxOn())
			{
				m_click.play();
			}
			mGameManager->setState(new AboutUs(mGameManager));
			return;
			
		}
		else if (skinbox.contains(mousePosition))
		{
			if (AssetManager::getInstance().isSfxOn())
			{
				m_click.play();
			}
			mGameManager->setState(new SelectSkinState(mGameManager));
			return;
		}
	}
}
void MenuState::Render(sf::RenderWindow &window) 
{
  window.draw(m_backgroundSprite);

  window.draw(m_playBtn);

  window.draw(m_exitBtn);
  
  window.draw(bottomPanel);

  window.draw(m_settingBtnSprite);

  window.draw(m_ranking);

  window.draw(m_Aboutus);

  window.draw(m_skin);
}