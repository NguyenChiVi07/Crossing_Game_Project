#pragma once
#include "AssetManager.h"
#include "CObstacle.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
struct StaticObstacleConfig {
  std::string textureName;
  float speed;
  sf::Vector2f scale;

  float hitboxWidth;
  float hitboxHeight;

  float OffsetX;
  float OffsetY;
};

class StaticObstacle : public Obstacle {
private:
  float mX;
  float mY;
  float mSpeed;
  sf::Sprite mSprite;

  float mhitboxWidth;
  float mhitboxHeight;

  float mOffsetX;
  float mOffsetY;

  StaticObstacle(float startX, float startY,
                 const StaticObstacleConfig &config);
  void MoveLogic(float delTime);

public:
<<<<<<< HEAD
    int getType() const override { return 3; } // Thẻ căn cước số 3
    void saveToFile(std::ofstream& out) override;
    void loadFromFile(std::ifstream& in) override;

    StaticObstacle(float startX, float startY);
    virtual ~StaticObstacle();
=======
  StaticObstacle(float startX, float startY);
  virtual ~StaticObstacle();
>>>>>>> 6a7544a00b8e24c5cb0783ad639abbef5cc4d6ff

  void UpdateState(float delTime);
  void Draw(sf::RenderWindow &window);

<<<<<<< HEAD
    sf::FloatRect getBounds();
    float getX();
    float getY();
    void increaseSpeed(float speedOffset);
    void setSpeed(float speed) { mSpeed = speed; }

};

=======
  sf::FloatRect getBounds();
  float getX();
  float getY();
  void increaseSpeed(float speedOffset);
  void setSpeed(float speed) { mSpeed = speed; }
};
>>>>>>> 6a7544a00b8e24c5cb0783ad639abbef5cc4d6ff
