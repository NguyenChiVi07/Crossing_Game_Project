#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include <optional>
<<<<<<< HEAD
=======

>>>>>>> 6a7544a00b8e24c5cb0783ad639abbef5cc4d6ff
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

<<<<<<< HEAD
// --- QUẢN LÝ CÁC TRẠNG THÁI MÀN HÌNH PHỦ (OVERLAY) ---
=======
>>>>>>> 6a7544a00b8e24c5cb0783ad639abbef5cc4d6ff
enum class OverlayState {
    NONE,
    PAUSE_MENU,
    GAME_OVER,
    SETTINGS_MENU
};

class PlayState : public IGameState
{
private:
<<<<<<< HEAD
    // --- CẤU TRÚC DANH SÁCH THÀNH VIÊN (ABOUT US TRONG PAUSE) ---
    struct MemberRow {
        sf::Sprite* iconSprite;
        sf::Text* infoText;
    };

    bool m_isPauseAboutOpen;                  // Trạng thái bật/tắt bảng popup thông tin nhóm trong trận
    sf::Sprite* m_pauseAboutIcon;             // Icon aboutus.png trong menu pause
    sf::Text* m_pauseAboutLabel;              // Nhãn chữ "ABOUT US" trong menu pause

    sf::RectangleShape m_pauseAboutOverlay;   // Lớp nền mờ
    sf::RectangleShape m_pauseAboutBox;       // Khung popup chính giữa
    sf::Text* m_pauseAboutTitleText;          // Tiêu đề "DEVELOPER TEAM"
    std::vector<MemberRow> m_pauseMemberRows; // Vector chứa 5 dòng thành viên

    sf::Sprite* m_pauseAboutBackIcon;         // Icon nút Back trong popup
    sf::Text* m_pauseAboutBackLabel;          // Chữ BACK trong popup

    // --- 1. GAME CORE & ENTITIES ---
=======
    std::optional<sf::Sound> m_coinSound;
    std::optional<sf::Sound> m_jumpSound;
    bool m_isMoveKeyPressed;
>>>>>>> 6a7544a00b8e24c5cb0783ad639abbef5cc4d6ff
    GameManager* mGameManager;
    CPEOPLE* m_Player;
    std::string m_selectedSkin;
    std::vector<LaneData> m_Lanes;
    std::vector<Obstacle*> m_Obstacles;
    std::vector<Coin*> m_Coins;

    sf::Sprite m_fireEffectSprite;
    Animation* m_fireAnimation;
    float m_collisionEffectTimer;
    bool m_isShowingCollisionEffect;

    int mScore;
    int mHighScore;
    int mlevel;
    float m_SpeedBonus;
    float m_SpawnTimer;
    float m_NextSpawnTime;

    bool m_IsGameOver;
    bool m_isMousePressed;
    bool m_isEscPressed;

    OverlayState m_overlayState;
    OverlayState m_previousOverlayState;

    // HUD UI
    sf::Text m_HighScoreText;
    sf::Text m_ScoreText;
    sf::Text m_LevelText;
    sf::Sprite m_pauseHUDBtn;

    sf::RectangleShape m_pauseOverlay;

    // PAUSE POP-UP MENU
    sf::RectangleShape m_pauseMenuBox;
    sf::Text m_pauseTitleText;
    sf::Sprite resumeBtn, loadBtn, volumeBtn, menuBtn, exitBtn;
    sf::Text m_resumeLabel, m_loadLabel, m_volumeLabel, m_menuLabel, m_exitLabel;

    // GAME OVER UI
    sf::Sprite m_gameOverBanner;
    sf::Text m_goScoreText;
    sf::Text m_goHighScoreText;
    sf::Text m_goRestartBtn;
    sf::Text m_goMenuBtn;
    sf::Text m_goSettingBtn;
    sf::Text m_goQuitBtn;

    // SETTINGS MENU UI
    sf::Text m_setSfxBtn;
    sf::Text m_setMusicBtn;
    sf::Text m_setResetBtn;
    sf::Text m_setBackBtn;

    // KÍCH HOẠT QUẢN LÝ ÂM THANH
    std::optional<sf::Sound> m_bgMusicSound;
    std::optional<sf::Sound> m_crashSound;
    std::optional<sf::Sound> m_gameOverSound;
    std::optional<sf::Sound> m_levelUpSound;
    std::optional<sf::Sound> m_honkSound;
    std::optional<sf::Sound> m_meowSound;

    void checkCollision();
    void cleanUpOffScreen(sf::RenderWindow& window);
    void generateLevel();
    void spawnObstacle(float delTime);
    void spawnLane(float yPos, LaneType type);
    void loadHighScore();
    void saveHighScore();

<<<<<<< HEAD
    // --- ÂM THANH ---
    sf::Music m_bgMusic;
    std::optional<sf::Sound> m_crashSound;
    std::optional<sf::Sound> m_gameOverSound;
    std::optional<sf::Sound> m_levelUpSound;
    std::optional<sf::Sound> m_honkSound;
    std::optional<sf::Sound> m_meowSound;

=======
>>>>>>> 6a7544a00b8e24c5cb0783ad639abbef5cc4d6ff
public:
    PlayState(GameManager* gameManager, const std::string& selectedSkin = "Dude_Monster_Idle_4.png");
    ~PlayState();

    void Init() override;
    void Update(float delTime, sf::RenderWindow& window) override;
    void Render(sf::RenderWindow& window) override;

    bool isGameOver() const { return m_IsGameOver; }
    void levelUp();

    void saveGame();
    void loadGame();
};