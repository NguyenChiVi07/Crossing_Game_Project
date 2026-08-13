#pragma once
#include "IGameState.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class GameManager;

enum class MenuOverlayState { MAIN, SETTINGS };

class MenuState : public IGameState {
private:
  GameManager *mGameManager;

  sf::Sprite m_backgroundSprite;
  sf::Sound m_bgMusic;

  // --- TEXT TIÊU ĐỀ VÀ HƯỚNG DẪN ĐÃ ĐƯỢC XÓA THEO YÊU CẦU ---

  // --- HITBOX CÁC NÚT TRÊN ẢNH MENU ---
  sf::RectangleShape m_playBtn;
  sf::RectangleShape m_exitBtn;

  // --- NÚT ICON GÓC DƯỚI TRÁI ---
  // Nút bánh răng (GameSetting) — khớp với icon trong hình nền
  sf::Sprite m_settingBtnSprite;
  bool m_settingBtnHovered;

  // Nút grid/load (ISerializable) — nằm kế bên nút bánh răng
  sf::Sprite m_serializableBtnSprite;
  bool m_serializableBtnHovered;

  // Debounce click chuột
  bool m_wasMousePressed;

public:
  MenuState(GameManager *gameManager);
  ~MenuState() override;

  void Init() override;
  void Update(float delTime, sf::RenderWindow &window) override;
  void Render(sf::RenderWindow &window) override;

  // --- BIẾN CHO BẢNG ABOUT US (PHONG CÁCH PAUSE MENU) ---
  bool m_isAboutOpen;
  bool m_isIPressed;

  sf::RectangleShape m_aboutOverlay; // Lớp nền mờ
  sf::RectangleShape m_aboutBox;     // Khung popup chính giữa
  sf::Text *m_aboutTitleText;        // Tiêu đề "DEVELOPER TEAM"

  sf::Sprite *m_aboutMainBtn; // Nút icon mở About Us ngoài màn hình chính

  // Cấu trúc danh sách thành viên (Dùng con trỏ an toàn cho cả Sprite và Text)
  struct MemberRow {
    sf::Sprite *iconSprite;
    sf::Text *infoText;
  };
  std::vector<MemberRow> m_memberRows;

  sf::Sprite *m_aboutBackIcon; // Icon nút Back
  sf::Text *m_aboutBackLabel;  // Chữ BACK
};