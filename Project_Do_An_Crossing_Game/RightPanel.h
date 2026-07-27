#pragma once
#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include <vector>
#include <string>

enum class ButtonAction {
    NONE,
    PAUSE_RESUME,
    SAVE_GAME,
    TOGGLE_SOUND,
    MAIN_MENU,
    EXIT
};

struct Button {
    sf::RectangleShape shape;
    sf::Text text;
    ButtonAction action;
};

class RightPanel {
private:
    float m_StartX, m_StartY;
    sf::RectangleShape m_Background;
    sf::Text m_Title;
    std::vector<Button> m_Buttons;

    bool m_isPaused;
    bool m_isSoundOn;

   
    void createButton(float yOffset, const std::string& label, ButtonAction action);

public:
    RightPanel(float startX = 1200.f, float startY = 0.f);
    ~RightPanel();

    
    ButtonAction HandleEvent(const sf::Event& event, const sf::RenderWindow& window);

 
    void Draw(sf::RenderWindow& window);
};