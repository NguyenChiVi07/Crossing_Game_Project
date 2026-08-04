#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <fstream>
#include <iostream>

#include "IGameState.h"
#include "CPeople.h"
#include "CObstacle.h"
#include "CVehicle.h"
#include "CStaticObstacle.h"
#include "CAnimal.h"
#include "AssetManager.h"
#include "Coin.h"


class GameManager;

enum LaneType { ROAD, GRASS, GRASS_EMPTY };

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

// --- QUẢN LÝ CÁC TRẠNG THÁI MAN HÌNH PHỦ (OVERLAY) ---
enum class OverlayState {
    NONE,
    PAUSE_MENU,
    GAME_OVER,
    SETTINGS_MENU
};

class PlayState : public IGameState
{
private:
    // --- 1. GAME CORE & ENTITIES ---
    GameManager* mGameManager;
    CPEOPLE* m_Player;
    std::vector<LaneData> m_Lanes;
    std::vector<Obstacle*> m_Obstacles;
    std::vector<Coin*> m_Coins;

	// -- EFFECTS ---
    sf::Sprite m_fireEffectSprite;    
    Animation* m_fireAnimation;       
    float m_collisionEffectTimer;    
    bool m_isShowingCollisionEffect;

    // --- 2. GAMEPLAY STATS ---
    int mScore;
    int mHighScore;
    int mlevel;
    float m_SpeedBonus;
    float m_SpawnTimer;
    float m_NextSpawnTime;

    // --- 3. STATE FLAGS ---
    bool m_IsGameOver;
    bool m_isMousePressed;
    bool m_isEscPressed;

    OverlayState m_overlayState;
    OverlayState m_previousOverlayState; // Lưu trạng thái trước để nút BACK quay về đúng chỗ

    // Settings Options
    bool m_isSFXOn;
    bool m_isMusicOn;

    // --- 4. HUD UI ---
    sf::Text m_HighScoreText;
    sf::Text m_ScoreText;
    sf::Text m_LevelText;
    sf::Sprite m_pauseHUDBtn;

    // --- 5. OVERLAY BACKGROUND COMMON ---
    sf::RectangleShape m_pauseOverlay;

    // --- 6. PAUSE POP-UP MENU ---
    sf::RectangleShape m_pauseMenuBox;
    sf::Text m_pauseTitleText;
    sf::Sprite resumeBtn, loadBtn, volumeBtn, menuBtn, exitBtn;
    sf::Text m_resumeLabel, m_loadLabel, m_volumeLabel, m_menuLabel, m_exitLabel;

    // --- 7. GAME OVER UI ---
    sf::Sprite m_gameOverBanner;
    sf::Text m_goScoreText;
    sf::Text m_goHighScoreText;
    sf::Text m_goRestartBtn;
    sf::Text m_goMenuBtn;
    sf::Text m_goSettingBtn;
    sf::Text m_goQuitBtn;

    // --- 8. SETTINGS MENU UI ---
    sf::Text m_setSfxBtn;
    sf::Text m_setMusicBtn;
    sf::Text m_setResetBtn;
    sf::Text m_setBackBtn;

    // --- HELPER FUNCTIONS ---
    void checkCollision();
    void cleanUpOffScreen(sf::RenderWindow& window);
    void generateLevel();
    void spawnObstacle(float delTime);
    void spawnLane(float yPos, LaneType type);
    void loadHighScore();
    void saveHighScore();


    // 1. Nhạc nền (Music)
    //sf::Music m_bgMusic;

    // 2. Sound Buffers (Bộ nhớ chứa file âm thanh ngắn)
    //sf::SoundBuffer m_crashBuffer;
    //sf::SoundBuffer m_gameOverBuffer;
    //sf::SoundBuffer m_levelUpBuffer;
    //sf::SoundBuffer m_honkBuffer;
    //sf::SoundBuffer m_meowBuffer;

    // 3. Sound Objects (Vật thể dùng để phát âm thanh)
    //std::optional<sf::Sound> m_crashSound;
    //std::optional<sf::Sound> m_gameOverSound;
    //std::optional<sf::Sound> m_levelUpSound;
    //std::optional<sf::Sound> m_honkSound;
    //std::optional<sf::Sound> m_meowSound;

public:
    PlayState(GameManager* gameManager);
    ~PlayState();

    void Init() override;
    void Update(float delTime, sf::RenderWindow& window) override;
    void Render(sf::RenderWindow& window) override;

    bool isGameOver() const { return m_IsGameOver; }
    void levelUp();
};