#include "GameManager.h"
#include "MenuState.hpp"
#include "PlayState.h"
#include "dashBoard.hpp"
#include <iostream>

GameManager::GameManager() : mCurrentState(nullptr), mPendingState(nullptr) {
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
  // Phòng trường hợp setState() được gọi ở lượt Update() cuối cùng nhưng
  // GameManager bị hủy trước khi kịp áp dụng — tránh rò rỉ mPendingState.
  if (mPendingState != nullptr) {
    delete mPendingState;
  }
}

IGameState *GameManager::getState() { return mCurrentState; }

void GameManager::setState(IGameState *state) {
  // QUAN TRỌNG: KHÔNG delete mCurrentState ở đây.
  // setState() thường được gọi từ chính bên trong mCurrentState->Update(...)
  // (ví dụ MenuState::Update gọi mGameManager->setState(new PlayState(...))).
  // Nếu delete mCurrentState ngay tại đây, ta sẽ giải phóng đối tượng đang
  // thực thi hàm Update() hiện hành (delete this khi hàm thành viên vẫn còn
  // trên call stack) — undefined behavior, có thể crash hoặc lỗi ngẫu nhiên
  // tùy trình biên dịch/optimizer, chứ không phải là an toàn.
  //
  // Thay vào đó, chỉ lưu state mới làm "pending" và để Update() xử lý việc
  // chuyển đổi thật sự sau khi mCurrentState->Update() đã chạy xong.
  if (mPendingState != nullptr) {
    // Nếu setState() được gọi nhiều lần trước khi Update() kịp áp dụng,
    // xóa pending cũ để tránh rò rỉ bộ nhớ.
    delete mPendingState;
  }
  mPendingState = state;
}

void GameManager::Update(float deltime, sf::RenderWindow &window) {
  // Chỉ làm đúng 1 việc: Yêu cầu màn hình hiện tại cập nhật logic của riêng nó
  if (mCurrentState != nullptr) {
    mCurrentState->Update(deltime, window);
  }

  // Sau khi Update() của state hiện tại đã chạy xong hoàn toàn (không còn
  // nằm trên call stack), giờ mới an toàn để xóa nó và chuyển sang state mới.
  if (mPendingState != nullptr) {
    IGameState *oldState = mCurrentState;
    mCurrentState = mPendingState;
    mPendingState = nullptr;

    delete oldState;

    if (mCurrentState != nullptr) {
      mCurrentState->Init();
    }
  }
}

void GameManager::Render(sf::RenderWindow &window) {
  // Yêu cầu màn hình hiện tại tự vẽ đồ họa của riêng nó
  if (mCurrentState != nullptr) {
    mCurrentState->Render(window);
  }
}