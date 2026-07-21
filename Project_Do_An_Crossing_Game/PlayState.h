#pragma once
#include <iostream>
#include<SFML/Graphics.hpp>
#include"IGameState.h"
#include"CPeople.h"
#include"CObstacle.h"
#include"CVehicle.h"
#include"CStaticObstacle.h"
#include"AssetManager.h"
#include"CAnimal.h"

// Forward declaration to avoid circular include
class GameManager;
// Thêm include cho class là các vật cản tĩnh (Cây, Đá)
// Nó được kế thừa từ class Obstacle
// Viết làm sao trong đó nó có cơ chế random 
// ra loại vật cản tĩnh (Cây hay Đá), vị trí của nó cũng phải 
//hợp lí. Ví dụ : không sinh ra quá nhiều để chặn người chời
// phần này thì tự nghĩ nhen (logic sinh vật cản tĩnh).
enum LaneType { ROAD, GRASS, GRASS_EMPTY};
struct LaneData {
    float yPos;
    LaneType type;
    sf::Sprite bgSprite;

    LaneData(float y, LaneType t, const sf::Texture& texture)
        : yPos(y), type(t), bgSprite(texture) 
    {
        bgSprite.setPosition({ 0.f, yPos });
    }
};

class PlayState : public  IGameState
{
private:
    GameManager* mGameManager;
    CPEOPLE* m_Player;

    std::vector<LaneData> m_Lanes;
    std::vector<Obstacle*> m_Obstacles;

    // HUD / UI Background
    sf::RectangleShape m_UIBackground;

    float m_SpawnTimer;
    float m_NextSpawnTime;

    int mScore;
    bool m_IsGameOver;
    int mlevel;

    // Biến dùng để tăng độ khó (tốc độ xe) qua mỗi level
    float m_SpeedBonus;

    void checkCollision();
    void cleanUpOffScreen(sf::RenderWindow& window);

    // Hàm sinh level và sinh vật cản
    void generateLevel();
    void spawnObstacle();
    void spawnLane(float yPos, LaneType type);
public:
	PlayState(GameManager* gameManager);
	~PlayState();
	void Init();
	void Update(float delTime, sf::RenderWindow& window);
	void Render(sf::RenderWindow& window);

    bool isGameOver() const { return m_IsGameOver; }
    void levelUp();
};