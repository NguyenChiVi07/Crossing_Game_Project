#pragma once
#include "IGameState.h"
#include "MenuState.hpp"
#include "PlayState.h"
#include "dashBoard.hpp"
#include <SFML/Graphics.hpp>
class GameManager {
private:
  IGameState *mCurrentState;
  // Trạng thái "chờ chuyển" — dùng để hoãn việc delete/khởi tạo state cho
  // đến khi Update() của state hiện tại đã chạy xong hoàn toàn.
  // Lý do: các state (MenuState/PlayState/SelectSkinState) tự gọi
  // mGameManager->setState(...) ngay TRONG hàm Update() của chính chúng.
  // Nếu setState() delete mCurrentState ngay lập tức, ta sẽ delete chính đối
  // tượng đang thực thi hàm Update() đó (delete this khi hàm thành viên vẫn
  // còn trên call stack) — đây là undefined behavior, không phải "an toàn vì
  // return ngay sau đó". mPendingState giúp việc chuyển state chỉ thực sự
  // diễn ra sau khi mCurrentState->Update() đã trả về.
  IGameState *mPendingState;

public:
  GameManager();                    // Constructor
  ~GameManager();                   // Destructor
  IGameState *getState();           // Get the current game state
  void setState(IGameState *state); // Set the current game state
  void Update(float deltime,
              sf::RenderWindow &window); // Update the current game state
  void Render(sf::RenderWindow &window); // Render the current game state
};