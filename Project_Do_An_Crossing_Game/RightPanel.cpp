#include "RightPanel.h"

RightPanel::RightPanel(float startX, float startY)
    : m_Title(AssetManager::getInstance().getFont("Christmas.otf"))
{
    m_StartX = startX;
    m_StartY = startY;
    m_isPaused = false;
    m_isSoundOn = true;

    
    m_Background.setPosition({ m_StartX, m_StartY });
    m_Background.setSize({ 400.f, 800.f });
    m_Background.setFillColor(sf::Color(40, 40, 40));
    m_Background.setOutlineThickness(2.f);
    m_Background.setOutlineColor(sf::Color::White);

    
    m_Title.setString("CONTROL PANEL");
    m_Title.setCharacterSize(30);
    m_Title.setFillColor(sf::Color::Yellow);
    m_Title.setPosition({ m_StartX + 75.f, m_StartY + 50.f });

    
    createButton(200.f, "PAUSE", ButtonAction::PAUSE_RESUME);
    createButton(300.f, "SAVE GAME", ButtonAction::SAVE_GAME);
    createButton(400.f, "SOUND: ON", ButtonAction::TOGGLE_SOUND);
    createButton(500.f, "MAIN MENU", ButtonAction::MAIN_MENU);
    createButton(600.f, "EXIT", ButtonAction::EXIT);
}

void RightPanel::createButton(float yOffset, const std::string& label, ButtonAction action) {
    
    Button btn = {
        sf::RectangleShape(),
        sf::Text(AssetManager::getInstance().getFont("Christmas.otf")),
        action
    };

    
    btn.shape.setSize({ 250.f, 60.f });
    btn.shape.setPosition({ m_StartX + 75.f, m_StartY + yOffset });
    btn.shape.setFillColor(sf::Color(100, 100, 100));
    btn.shape.setOutlineThickness(2.f);
    btn.shape.setOutlineColor(sf::Color::White);

   
    btn.text.setString(label);
    btn.text.setCharacterSize(24);
    btn.text.setFillColor(sf::Color::White);

    
    sf::FloatRect textRect = btn.text.getLocalBounds();
    btn.text.setOrigin({ textRect.position.x + textRect.size.x / 2.0f,
                        textRect.position.y + textRect.size.y / 2.0f });
    btn.text.setPosition({ btn.shape.getPosition().x + btn.shape.getSize().x / 2.0f,
                          btn.shape.getPosition().y + btn.shape.getSize().y / 2.0f });

    m_Buttons.push_back(btn);
}

RightPanel::~RightPanel() {}

ButtonAction RightPanel::HandleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    
    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePressed->button == sf::Mouse::Button::Left) {

            
            sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
            sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

            
            for (auto& btn : m_Buttons) {
                if (btn.shape.getGlobalBounds().contains(worldPos)) {

                    if (btn.action == ButtonAction::PAUSE_RESUME) {
                        m_isPaused = !m_isPaused;
                        btn.text.setString(m_isPaused ? "RESUME" : "PAUSE");
                    }
                    else if (btn.action == ButtonAction::TOGGLE_SOUND) {
                        m_isSoundOn = !m_isSoundOn;
                        btn.text.setString(m_isSoundOn ? "SOUND: OFF" : "SOUND: ON");
                    }

                    return btn.action;
                }
            }
        }
    }
    return ButtonAction::NONE;
}

void RightPanel::Draw(sf::RenderWindow& window) {
    window.draw(m_Background);
    window.draw(m_Title);
    for (const auto& btn : m_Buttons) {
        window.draw(btn.shape);
        window.draw(btn.text);
    }
}