#include "MenuState.hpp"

MenuState::MenuState(GameManager* gameManager)
	: mGameManager(gameManager),
	m_backgroundSprite(AssetManager::getInstance().getTexture("menu_copy.png")),
	m_playBtn(AssetManager::getInstance().getTexture("play_copy.png")),
	m_exitBtn(AssetManager::getInstance().getTexture("exit_button.png")),
	bottomPanel(AssetManager::getInstance().getTexture("bottomPanel.png")),
	m_settingBtnSprite(AssetManager::getInstance().getTexture("Setting.png")),
	m_SaveLoad(AssetManager::getInstance().getTexture("saveload.png")),
	m_Aboutus(AssetManager::getInstance().getTexture("aboutus.png")),
	m_skin(AssetManager::getInstance().getTexture("skin.png")),
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
		sf::FloatRect bounds = m_SaveLoad.getLocalBounds();

		m_SaveLoad.setOrigin({ bounds.position.x + bounds.size.x / 2.f,
								   bounds.position.y + bounds.size.y / 2.f });
		m_SaveLoad.setScale({ 0.7 ,0.7 });
		m_SaveLoad.setPosition({ 254, 725 });
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

	// Khởi tạo bảng Menu Save/Load
	m_saveLoadUI = new SaveLoadUI();
	m_isSaveLoadMenuOpen = false;

	// Audio 
	if (AssetManager::getInstance().isMusicOn())
	{
		AssetManager::getInstance().playMusic("../ASSETS/AUDIO/AdhesiveWombat - Night Shade_copy.mp3", true);
	}
}

MenuState::~MenuState() {
	delete m_saveLoadUI;
}

void MenuState::Init()
{
}

void MenuState::HandleEvent(const sf::Event& event, sf::RenderWindow& window) {
	if (m_isSaveLoadMenuOpen) {
		m_saveLoadUI->HandleEvent(event, window);
	}
}

void MenuState::Update(float delTime, sf::RenderWindow& window)
{
	// Nếu Menu Save/Load đang mở, chỉ cập nhật nó và không cho phép bấm các nút phía sau
	if (m_isSaveLoadMenuOpen) {
		m_saveLoadUI->Update(window);

		SaveLoadAction action = m_saveLoadUI->getPendingAction();
		if (action == SaveLoadAction::LOAD) {
			// PHÉP THUẬT OOP: Tạo màn hình Play, yêu cầu nó load file, rồi mới đưa lên GameManager
			PlayState* newGame = new PlayState(mGameManager);
			newGame->loadGame(m_saveLoadUI->getTargetFileName());
			mGameManager->setState(newGame);

			m_saveLoadUI->resetAction();
			return;
		}
		else if (action == SaveLoadAction::CLOSE) {
			m_saveLoadUI->resetAction();
			m_isSaveLoadMenuOpen = false; // Đóng Menu lại
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
			m_isSaveLoadMenuOpen = false;
		}
		return; // Thoát Update, không bấm được các nút Play/Exit phía sau lưng
	}
	//----------

	static bool mouseWasPressed = false;
	bool mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

	sf::Vector2i pixelPosition = sf::Mouse::getPosition(window);
	sf::Vector2f mousePosition = window.mapPixelToCoords(pixelPosition);

	sf::FloatRect playbox = m_playBtn.getGlobalBounds();
	sf::FloatRect exitbox = m_exitBtn.getGlobalBounds();
	sf::FloatRect settingbox = m_settingBtnSprite.getGlobalBounds();
	sf::FloatRect saveloadbox = m_SaveLoad.getGlobalBounds();
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

	// Hiệu ứng Hover của cúp vàng SaveLoad
	if (saveloadbox.contains(mousePosition))
	{
		m_SaveLoad.setScale({ 0.8f, 0.8f });
	}
	else
	{
		m_SaveLoad.setScale({ 0.7f, 0.7f });
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
				AssetManager::getInstance().playSfx("click.mp3");
			}
			mGameManager->setState(new PlayState(mGameManager));
			return;
		}
		else if (exitbox.contains(mousePosition))
		{
			if (AssetManager::getInstance().isSfxOn())
			{
				AssetManager::getInstance().playSfx("click.mp3");
			}
			window.close();
		}
		else if (settingbox.contains(mousePosition))
		{
			if (AssetManager::getInstance().isSfxOn())
			{
				AssetManager::getInstance().playSfx("click.mp3");
			}
			mGameManager->setState(new GameSetting(mGameManager));
			return;
		}
		else if (saveloadbox.contains(mousePosition))
		{
			// Âm thanh và mở menu khi bấm cúp vàng!
			if (AssetManager::getInstance().isSfxOn())
			{
				AssetManager::getInstance().playSfx("click.mp3");
			}
			m_isSaveLoadMenuOpen = true;
			m_saveLoadUI->refreshList();
		}
		else if (aboutusbox.contains(mousePosition))
		{
			if (AssetManager::getInstance().isSfxOn())
			{
				AssetManager::getInstance().playSfx("click.mp3");
			}
			mGameManager->setState(new AboutUs(mGameManager));
			return;

		}
		else if (skinbox.contains(mousePosition))
		{
			if (AssetManager::getInstance().isSfxOn())
			{
				AssetManager::getInstance().playSfx("click.mp3");
			}
			mGameManager->setState(new SelectSkinState(mGameManager));
			return;
		}
	}
}

void MenuState::Render(sf::RenderWindow& window)
{
	window.draw(m_backgroundSprite);
	window.draw(m_playBtn);
	window.draw(m_exitBtn);
	window.draw(bottomPanel);
	window.draw(m_settingBtnSprite);
	window.draw(m_SaveLoad);
	window.draw(m_Aboutus);
	window.draw(m_skin);

	if (m_isSaveLoadMenuOpen) {
		m_saveLoadUI->Draw(window);
	}
}