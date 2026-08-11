#pragma once
#include "Animation.h"
#include "CObstacle.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

struct AnimalConfig {
  std::string textureName;
  float speed;
  sf::Vector2u imageCount;
  sf::Vector2f scale;

  float hitboxWidth;
  float hitboxHeight;

  float OffsetX;
  float OffsetY;
};
class CANIMAL : public Obstacle {
private:
  float m_X;
  float m_Y;
  float m_Speed;
  sf::Sprite m_Sprite;

  Animation *m_animation;
  bool m_faceRight;
  int m_row;

  float m_hitboxWidth;
  float m_hitboxHeight;

  float m_OffsetX;
  float m_OffsetY;
  CANIMAL(float startX, float startY, const AnimalConfig &config);
  void MoveLogic(float delTime);

public:
  CANIMAL(float startX, float startY);
  virtual ~CANIMAL();

<<<<<<< HEAD
    float getX() { return m_X; }
    float getY() { return m_Y; }
    void UpdateState(float delTime);
    void Draw(sf::RenderWindow& window);
    sf::FloatRect getBounds();
    void increaseSpeed(float amount) { m_Speed += amount; }
	void setSpeed(float speed) { m_Speed = speed; }

    int getType() const override { return 1; }
    void saveToFile(std::ofstream& out) override;
    void loadFromFile(std::ifstream& in) override;
=======
  float getX() { return m_X; }
  float getY() { return m_Y; }
  void UpdateState(float delTime);
  void Draw(sf::RenderWindow &window);
  sf::FloatRect getBounds();
  void increaseSpeed(float amount) { m_Speed += amount; }
  void setSpeed(float speed) { m_Speed = speed; }
>>>>>>> 6a7544a00b8e24c5cb0783ad639abbef5cc4d6ff
};