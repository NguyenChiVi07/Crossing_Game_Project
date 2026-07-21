#pragma once
#include"CObstacle.h"
#include<SFML\Graphics.hpp>
#include <string>
#include <vector>
#include"AssetManager.h"
struct VehicleConfig {
    std::string textureName;
    float speed;
    sf::Vector2f scale;

    float hitboxWidth;
    float hitboxHeight;

    float OffsetX;
    float OffsetY;
};

class CVEHICLE : public Obstacle {
private:
    float mX;
    float mY;
    float mSpeed;
    sf::Sprite mSprite;

    float mhitboxWidth;
    float mhitboxHeight;

    float mOffsetX;
    float mOffsetY;

    CVEHICLE(float startX, float startY, const VehicleConfig& config);
    void MoveLogic(float delTime);
public:
    CVEHICLE(float startX, float startY);
    virtual ~CVEHICLE();

    void UpdateState(float delTime);
    void Draw(sf::RenderWindow& window);

    sf::FloatRect getBounds();
    float getX();
    float getY();
    void increaseSpeed(float speedOffset);
    void setSpeed(float speed) { mSpeed = speed; }
};