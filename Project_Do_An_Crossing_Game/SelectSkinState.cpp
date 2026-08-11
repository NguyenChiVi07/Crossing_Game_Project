#include "SelectSkinState.h"
#include "GameManager.h"
#include "PlayState.h"
#include "AssetManager.h"

void SelectSkinState::updateSkinLayout()
{
    const float centerX = 800;

    const float leftX = 450;

    const float rightX = 1150;

    const float skinY = 350;


    const int skinCount = static_cast<int>(m_skinSprites.size());

    int leftIndex = (m_currentIndex - 1 + skinCount) % skinCount;

    int rightIndex = (m_currentIndex + 1) % skinCount;

    //left
    {
        sf::Sprite& sprite = *m_skinSprites[leftIndex];

        sf::Texture& texture = const_cast<sf::Texture&>(sprite.getTexture());

        float frameWidth = texture.getSize().x / 4.f;

        float frameHeight = texture.getSize().y;

        sprite.setOrigin({ frameWidth / 2.f,frameHeight / 2.f });

        sprite.setScale({ 6,6 });

        sprite.setPosition({ leftX,skinY });
    }
  
	//center
    {
        sf::Sprite& sprite = *m_skinSprites[m_currentIndex];

        sf::Texture& texture = const_cast<sf::Texture&>(sprite.getTexture());

        float frameWidth = texture.getSize().x / 4.f;

        float frameHeight = texture.getSize().y;

        sprite.setOrigin({ frameWidth / 2.f,frameHeight / 2.f });

        sprite.setScale({ 6,6 });

        sprite.setPosition({ centerX,skinY });

    }
   
    //right
    {
        sf::Sprite& sprite = *m_skinSprites[rightIndex];

        sf::Texture& texture = const_cast<sf::Texture&>(sprite.getTexture());

        float frameWidth = texture.getSize().x / 4.f;

        float frameHeight =
            texture.getSize().y;

        sprite.setOrigin({ frameWidth / 2.f,frameHeight / 2.f });

        sprite.setScale({ 6,6 });

        sprite.setPosition({ rightX,skinY });
    }
   
}

void SelectSkinState::updateAnimation(float deltaTime)
{
    for (int i = 0;i < static_cast<int>(m_animations.size());i++)
    {
        m_animations[i]->Update(0,deltaTime,true);

        m_skinSprites[i]->setTextureRect(m_animations[i]->uvRect);
    }

    m_selectedIcon.setTextureRect(m_animations[m_currentIndex]->uvRect);
}

void SelectSkinState::updateSelectedSkin()
{
    const sf::Texture& texture =AssetManager::getInstance().getTexture(m_skinList[m_currentIndex]);

    m_selectedIcon.setTexture(texture);

    float frameWidth = texture.getSize().x / 4.f;

    float frameHeight = texture.getSize().y;
    m_selectedIcon.setOrigin({frameWidth / 2.f, frameHeight / 2.f });

    m_selectedIcon.setTextureRect(m_animations[m_currentIndex]->uvRect);
}

void SelectSkinState::handleMouseClick(sf::Vector2f mousePosition)
{
    for (int i = 0; i < static_cast<int>(m_skinSprites.size()); ++i)
    {
        if (m_skinSprites[i]->getGlobalBounds().contains(mousePosition))
        {
            m_currentIndex = i;
            updateSelectedSkin();
            return;
        }
    }
}

bool SelectSkinState::isMouseOver(const sf::FloatRect& rect, sf::Vector2f mousePosition) const
{
    return
        mousePosition.x >= rect.position.x && mousePosition.x <= rect.position.x + rect.size.x 
        && mousePosition.y >= rect.position.y && mousePosition.y <= rect.position.y + rect.size.y;
}

SelectSkinState::SelectSkinState(GameManager* gameManager) : mGameManager(gameManager),
m_skinList({
    "Dude_Monster_Idle_4.png",
    "Owlet_Monster_Idle_4.png",
    "Pink_Monster_Idle_4.png"
    }),

    m_currentIndex(0),
    m_bgSprite(
        AssetManager::getInstance().getTexture("bg.png")
    ),

    m_titleText(
        AssetManager::getInstance().getFont("DIMIS___.ttf")
    ),

    m_enterText(
        AssetManager::getInstance().getFont("DIMIS___.ttf")
    ),

    m_instructionText(
        AssetManager::getInstance().getFont("DIMIS___.ttf")
    ),

	m_selectedIcon(AssetManager::getInstance().getTexture(m_skinList[m_currentIndex]))
{
	m_bgSprite.setPosition({ 0.f, 0.f });

	//Main Panel
    m_mainPanel.setPosition({ 144, 95.f });

    m_mainPanel.setSize({ 1311, 608 });

    m_mainPanel.setFillColor(
        sf::Color(20, 45, 55, 185)
    );

    m_mainPanel.setOutlineThickness(4.f);

    m_mainPanel.setOutlineColor(
        sf::Color(230, 245, 245, 230)
    );
    //  TITLE BOX

    m_titleBox.setPosition({ 447.7, 48 });

    m_titleBox.setSize({ 704.7, 95.7 });

    m_titleBox.setFillColor(sf::Color(40, 145, 205, 255));

    m_titleBox.setOutlineThickness(3.f);

    m_titleBox.setOutlineColor(
        sf::Color::White
    );

	//  TITLE TEXT

    m_titleText.setString("SELECT YOUR SKIN");

    m_titleText.setCharacterSize(34);

    m_titleText.setFillColor(sf::Color::White);

    {
        sf::FloatRect bounds = m_titleText.getLocalBounds();

        m_titleText.setOrigin({ bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f });

        m_titleText.setPosition({ 800, 95 });
    }

    // BOTTOM PANEL
    m_bottomPanel.setPosition({ 215,504});

    m_bottomPanel.setSize({ 1169,162});

    m_bottomPanel.setFillColor(sf::Color(40, 145, 205, 220));

    m_bottomPanel.setOutlineThickness(3.f);

    m_bottomPanel.setOutlineColor(sf::Color(230, 245, 245, 230));

    // ENTER BUTTON

    m_enterButton.setPosition({ 988,545});

    m_enterButton.setSize({ 328,82 });

    m_enterButton.setFillColor(sf::Color(255, 180, 70));

    m_enterButton.setOutlineThickness(3.f);

    m_enterButton.setOutlineColor(sf::Color::White);

    // ENTER TEXT

    m_enterText.setString("ENTER TO PLAY");

    m_enterText.setCharacterSize(26);

    m_enterText.setFillColor(sf::Color::White);

    {
        sf::FloatRect bounds = m_enterText.getLocalBounds();

        m_enterText.setOrigin({ bounds.position.x + bounds.size.x / 2.f,bounds.position.y + bounds.size.y / 2.f });

        m_enterText.setPosition({ 1144,584 });
    }
    
  
    
    // INSTRUCTION
    m_instructionText.setString(
        "CLICK A SKIN TO SELECT"
    );

    m_instructionText.setCharacterSize(20);

    m_instructionText.setFillColor(sf::Color::White);

    {
        sf::FloatRect bounds = m_instructionText.getLocalBounds();

        m_instructionText.setOrigin({ bounds.position.x + bounds.size.x / 2.f,bounds.position.y + bounds.size.y / 2.f });

        m_instructionText.setPosition({ 707, 584 });
    }

  
    for (const std::string& skin : m_skinList)
    {
        const sf::Texture& texture = AssetManager::getInstance().getTexture(skin);
        // Sprite

        m_skinSprites.push_back(std::make_unique<sf::Sprite>(texture));

        // Animation

        m_animations.push_back(std::make_unique<Animation>(&texture,sf::Vector2u(4, 1), 0.1f));
    }

    // SELECTED ICON

    m_selectedIcon.setScale({4, 4});

    m_selectedIcon.setPosition({ 300, 575 });

    updateSkinLayout();

    updateSelectedSkin();
}

void SelectSkinState::Init()
{
}

void SelectSkinState::Update(float delTime, sf::RenderWindow& window)
{
    updateAnimation(delTime);

    static bool mouseWasPressed = false;
    static bool enterWasPressed = false;

    bool mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    bool enterPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter);

    if (mousePressed && !mouseWasPressed)
    {
        sf::Vector2i pixelPosition = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosition = window.mapPixelToCoords(pixelPosition);
        handleMouseClick(mousePosition);
    }

    if (enterPressed && !enterWasPressed)
    {
        std::string chosenSkin = m_skinList[m_currentIndex];
        mGameManager->setState(new PlayState(mGameManager, chosenSkin));
        return;
    }

    mouseWasPressed = mousePressed;
    enterWasPressed = enterPressed;
}

void SelectSkinState::Render(sf::RenderWindow& window)
{
    window.clear(sf::Color(20, 30, 40));

    window.draw(m_bgSprite);

    window.draw(m_mainPanel);

    window.draw(m_titleBox);

    window.draw(m_titleText);


    int leftIndex = (m_currentIndex - 1 + static_cast<int>(m_skinSprites.size()))  %  static_cast<int>(m_skinSprites.size());

    int rightIndex = (m_currentIndex + 1)  % static_cast<int>(m_skinSprites.size());

    window.draw( *m_skinSprites[leftIndex]);

    window.draw(  *m_skinSprites[m_currentIndex] );

    window.draw(  *m_skinSprites[rightIndex]  );

    // BOTTOM PANEL
  
    window.draw(m_bottomPanel);

    // Selected icon
    window.draw(m_selectedIcon);

    // Enter
    window.draw(m_enterButton);

    window.draw(m_enterText);
    // Instruction
    window.draw(m_instructionText);
}
