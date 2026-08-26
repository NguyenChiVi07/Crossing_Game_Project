#pragma once
#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "Animation.h"
class Coin {
private:
    float mX, mY;
    sf::Sprite m_sprite;

	Animation* m_animation; 
    float mhitboxWidth;
    float mhitboxHeight;
    float mOffsetX;
    float mOffsetY;

public:
    Coin(float x, float y) : mX(x), mY(y), m_sprite(AssetManager::getInstance().getTexture("Coin.png")) {
        m_animation = new Animation(&AssetManager::getInstance().getTexture("Coin.png"), { 5, 1 }, 0.1f);
        m_sprite.setScale({ 3, 3 });
        float frameWidth = static_cast<float>(m_animation->uvRect.size.x);
        float frameHeight = static_cast<float>(m_animation->uvRect.size.y);
        m_sprite.setOrigin({ frameWidth / 2.f, frameHeight / 2.f });
        m_sprite.setPosition({ mX, mY });

        mhitboxWidth = 40;
        mhitboxHeight = 40;
        mOffsetX = 0;
        mOffsetY = 0;
    }
    ~Coin() {
        delete m_animation;
    }

    sf::FloatRect getBounds()  const {
        float left = mX - (mhitboxWidth / 2.f) + mOffsetX;
        float top = mY - (mhitboxHeight / 2.f) + mOffsetY;

        return sf::FloatRect({ left, top }, { mhitboxWidth, mhitboxHeight });
    }

	void Update(float deltaTime) {
		m_animation->Update(0, deltaTime, true);
		m_sprite.setTextureRect(m_animation->uvRect);
	}
    void Draw(sf::RenderWindow& window) {
        window.draw(m_sprite);

        //// BẬT CHẾ ĐỘ NHÌN THẤY HITBOX (Dùng để debug)
        //sf::FloatRect hitbox = getBounds();
        //sf::RectangleShape debugBox({ hitbox.size.x, hitbox.size.y });
        //debugBox.setPosition({ hitbox.position.x, hitbox.position.y });
        //debugBox.setFillColor(sf::Color(255, 0, 0, 100)); // Màu đỏ mờ (Alpha = 100)
        //debugBox.setOutlineColor(sf::Color::Red);
        //debugBox.setOutlineThickness(2.f);

        //// Vẽ hitbox đè lên nhân vật
        //window.draw(debugBox);
    }
};
