#include "GameManager.h"
#include "MenuState.hpp"
#include "PlayState.h"
#include "dashBoard.hpp"
#include <iostream>

GameManager::GameManager() {
  // Khởi tạo màn hình đầu tiên khi mở game (Vào Menu thay vì Play ngay)
  mCurrentState = new MenuState(this);

  if (mCurrentState != nullptr) {
    mCurrentState->Init();
  }
}

GameManager::~GameManager() {
  if (mCurrentState != nullptr) {
    delete mCurrentState;
  }
}

IGameState *GameManager::getState() { return mCurrentState; }

void GameManager::setState(IGameState *state) {
  // Xóa state cũ đi để giải phóng bộ nhớ (tránh rò rỉ RAM)
  if (mCurrentState != nullptr) {
    delete mCurrentState;
  }

  // Đổi sang state mới
  mCurrentState = state;

  // Khởi tạo state mới
  if (mCurrentState != nullptr) {
    mCurrentState->Init();
  }
}

void GameManager::Update(float deltime, sf::RenderWindow &window) {
  // Chỉ làm đúng 1 việc: Yêu cầu màn hình hiện tại cập nhật logic của riêng nó
  if (mCurrentState != nullptr) {
    mCurrentState->Update(deltime, window);
  }
}

void GameManager::Render(sf::RenderWindow &window) {
  // Yêu cầu màn hình hiện tại tự vẽ đồ họa của riêng nó
  if (mCurrentState != nullptr) {
    mCurrentState->Render(window);
  }
}