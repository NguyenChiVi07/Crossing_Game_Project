#include "CGame.hpp"
#include "Constants.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <optional>
#include <random>

// ============================================================
// Constructor
// ============================================================
CGame::CGame()
    : axt(nullptr), axh(nullptr), akl(nullptr), ac(nullptr), mNumTrucks(0),
      mNumCars(0), mNumDinos(0), mNumBirds(0), mLevel(1), mPaused(false),
      mRunning(true), mState(GameState::Menu), mAccumulator(0.f),
      mFontLoaded(false), mDifficulty(1), mVolume(50), mMusicLoaded(false),
      mMenuBgSprite(mMenuBgTex), mMenuBgLoaded(false),
      mSettingsBgSprite(mSettingsBgTex), mSettingsBgLoaded(false),
      mAboutBgSprite(mAboutBgTex), mAboutBgLoaded(false),
      mBtnStartGame(nullptr), mBtnLoadGame(nullptr), mBtnSettings(nullptr),
      mBtnAbout(nullptr), mBtnExit(nullptr), mBtnEasy(nullptr),
      mBtnMedium(nullptr), mBtnHard(nullptr), mBtnVolDown(nullptr),
      mBtnVolUp(nullptr), mBtnBackFromSettings(nullptr),
      mBtnBackFromAbout(nullptr) {
  // Try to load a standard macOS system font
  std::string fontPaths[] = {
      "/System/Library/Fonts/Supplemental/Arial.ttf",
      "/System/Library/Fonts/Helvetica.ttc",
      "/System/Library/Fonts/Supplemental/Courier New.ttf",
      "/Library/Fonts/Arial.ttf"};

  for (const auto &path : fontPaths) {
    if (mFont.openFromFile(path)) {
      mFontLoaded = true;
      break;
    }
  }

  if (!mFontLoaded) {
    std::cerr << "Warning: Could not load any system font. Text rendering will "
                 "not work."
              << std::endl;
  }

  // Initialize UI buttons
  initButtons();

  // Load custom background images and sound/music
  loadAssets();

  // Create SFML window
  mWindow.create(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)),
                 "Crossing Game");
  mWindow.setFramerateLimit(60);

  // Initial allocation
  allocateObjects();
}

// ============================================================
// Destructor
// ============================================================
CGame::~CGame() {
  deallocateObjects();
  cleanupButtons();
}

// ============================================================
// Allocation and Deallocation
// ============================================================
void CGame::allocateObjects() {
  deallocateObjects();

  // Calculate count based on level
  mNumTrucks = BASE_TRUCKS + (mLevel - 1) * LEVEL_INCREMENT;
  mNumCars = BASE_CARS + (mLevel - 1) * LEVEL_INCREMENT;
  mNumBirds = BASE_BIRDS + (mLevel - 1) * LEVEL_INCREMENT;
  mNumDinos = BASE_DINASORS + (mLevel - 1) * LEVEL_INCREMENT;

  axt = new CTruck[mNumTrucks];
  axh = new CCar[mNumCars];
  ac = new CBird[mNumBirds];
  akl = new CDinausor[mNumDinos];

  // Seed for random positions
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> disX(0, WINDOW_WIDTH);

  float diffMult = getDifficultyMultiplier();

  // Set positions and directions for Truck lane
  int truckY = getVehicleLaneY(0);
  for (int i = 0; i < mNumTrucks; ++i) {
    axt[i] = CTruck(disX(gen), truckY, 1); // Moves right
    axt[i].setSpeed(TRUCK_SPEED * diffMult);
  }

  // Set positions and directions for Car lane
  int carY = getVehicleLaneY(1);
  for (int i = 0; i < mNumCars; ++i) {
    axh[i] = CCar(disX(gen), carY, -1); // Moves left
    axh[i].setSpeed(CAR_SPEED * diffMult);
  }

  // Set positions and directions for Bird lane
  int birdY = getAnimalLaneY(0);
  for (int i = 0; i < mNumBirds; ++i) {
    ac[i] = CBird(disX(gen), birdY, 1); // Moves right
    ac[i].setSpeed(BIRD_SPEED * diffMult);
  }

  // Set positions and directions for Dinosaur lane
  int dinoY = getAnimalLaneY(1);
  for (int i = 0; i < mNumDinos; ++i) {
    akl[i] = CDinausor(disX(gen), dinoY, -1); // Moves left
    akl[i].setSpeed(DINO_SPEED * diffMult);
  }
}

void CGame::deallocateObjects() {
  delete[] axt;
  axt = nullptr;
  delete[] axh;
  axh = nullptr;
  delete[] ac;
  ac = nullptr;
  delete[] akl;
  akl = nullptr;
  mNumTrucks = mNumCars = mNumBirds = mNumDinos = 0;
}

// ============================================================
// Setup Helpers
// ============================================================
void CGame::initButtons() {
  cleanupButtons();

  sf::Vector2f btnSize(240.f, 50.f);
  float startX = (WINDOW_WIDTH - btnSize.x) / 2.0f;

  // ============================================================
  // [USER DIRECTIVE]: ĐƯỜNG DẪN ẢNH CHO CÁC NÚT BẤM (BUTTON IMAGES)
  // Hãy đổi đường dẫn tới các tệp ảnh mong muốn của bạn bên dưới.
  // Ví dụ: mBtnStartGame->loadTexture("assets/start_button_texture.png");
  // ============================================================

  // Menu screen buttons
  mBtnStartGame =
      new CButton(mFont, "START GAME", sf::Vector2f(startX, 240.f), btnSize);
  mBtnStartGame->loadTexture(
      "assets/play.png"); // <- USER: Đổi ảnh nút Start Game tại đây

  mBtnLoadGame =
      new CButton(mFont, "LOAD GAME", sf::Vector2f(startX, 310.f), btnSize);
  mBtnLoadGame->loadTexture(
      "assets/load.png"); // <- USER: Đổi ảnh nút Load Game tại đây

  mBtnSettings =
      new CButton(mFont, "SETTINGS", sf::Vector2f(startX, 380.f), btnSize);
  mBtnSettings->loadTexture(
      "assets/settings.png"); // <- USER: Đổi ảnh nút Settings tại đây

  mBtnAbout =
      new CButton(mFont, "ABOUT GAME", sf::Vector2f(startX, 450.f), btnSize);
  mBtnAbout->loadTexture(
      "assets/about_us.png"); // <- USER: Đổi ảnh nút About tại đây

  mBtnExit =
      new CButton(mFont, "EXIT GAME", sf::Vector2f(startX, 520.f), btnSize);
  mBtnExit->loadTexture(
      "assets/exit_game.png"); // <- USER: Đổi ảnh nút Exit Game tại đây

  // Settings screen buttons
  sf::Vector2f diffBtnSize(120.f, 40.f);
  mBtnEasy =
      new CButton(mFont, "EASY", sf::Vector2f(280.f, 250.f), diffBtnSize);
  mBtnEasy->loadTexture(
      "assets/easy_mode.png"); // <- USER: Đổi ảnh nút Easy tại đây

  mBtnMedium =
      new CButton(mFont, "MEDIUM", sf::Vector2f(440.f, 250.f), diffBtnSize);
  mBtnMedium->loadTexture(
      "assets/medium_mode.png"); // <- USER: Đổi ảnh nút Medium tại đây

  mBtnHard =
      new CButton(mFont, "HARD", sf::Vector2f(600.f, 250.f), diffBtnSize);
  mBtnHard->loadTexture(
      "assets/hard_mode.png"); // <- USER: Đổi ảnh nút Hard tại đây

  sf::Vector2f volBtnSize(60.f, 40.f);
  mBtnVolDown = new CButton(mFont, "-", sf::Vector2f(420.f, 380.f), volBtnSize);
  mBtnVolDown->loadTexture(
      "assets/decreas_vol.png"); // <- USER: Đổi ảnh nút Giảm Âm Lượng tại đây

  mBtnVolUp = new CButton(mFont, "+", sf::Vector2f(560.f, 380.f), volBtnSize);
  mBtnVolUp->loadTexture(
      "assets/increas_vol.png"); // <- USER: Đổi ảnh nút Tăng Âm Lượng tại đây

  mBtnBackFromSettings = new CButton(
      mFont, "BACK", sf::Vector2f((WINDOW_WIDTH - 200.f) / 2.0f, 580.f),
      sf::Vector2f(200.f, 50.f));
  mBtnBackFromSettings->loadTexture(
      "assets/back.png"); // <- USER: Đổi ảnh nút Quay lại (Settings) tại đây

  // About screen buttons
  mBtnBackFromAbout = new CButton(
      mFont, "BACK", sf::Vector2f((WINDOW_WIDTH - 200.f) / 2.0f, 580.f),
      sf::Vector2f(200.f, 50.f));
  mBtnBackFromAbout->loadTexture(
      "assets/back.png"); // <- USER: Đổi ảnh nút Quay lại (About) tại đây
}

void CGame::cleanupButtons() {
  delete mBtnStartGame;
  mBtnStartGame = nullptr;
  delete mBtnLoadGame;
  mBtnLoadGame = nullptr;
  delete mBtnSettings;
  mBtnSettings = nullptr;
  delete mBtnAbout;
  mBtnAbout = nullptr;
  delete mBtnExit;
  mBtnExit = nullptr;
  delete mBtnEasy;
  mBtnEasy = nullptr;
  delete mBtnMedium;
  mBtnMedium = nullptr;
  delete mBtnHard;
  mBtnHard = nullptr;
  delete mBtnVolDown;
  mBtnVolDown = nullptr;
  delete mBtnVolUp;
  mBtnVolUp = nullptr;
  delete mBtnBackFromSettings;
  mBtnBackFromSettings = nullptr;
  delete mBtnBackFromAbout;
  mBtnBackFromAbout = nullptr;
}

void CGame::loadAssets() {
  // ============================================================
  // [USER DIRECTIVE]: ĐƯỜNG DẪN ẢNH NỀN CÁC MÀN HÌNH (BACKGROUND IMAGES)
  // Hãy thay đổi đường dẫn ảnh nền của bạn tại đây.
  // ============================================================

  // Menu Background image
  if (mMenuBgTex.loadFromFile(
          "assets/menu.png")) { // <- USER: Đường dẫn ảnh nền Menu
    mMenuBgSprite.setTexture(mMenuBgTex);
    sf::Vector2u size = mMenuBgTex.getSize();
    mMenuBgSprite.setScale(
        sf::Vector2f(static_cast<float>(WINDOW_WIDTH) / size.x,
                     static_cast<float>(WINDOW_HEIGHT) / size.y));
    mMenuBgLoaded = true;
  } else {
    mMenuBgLoaded = false;
  }

  // Settings Background image
  if (mSettingsBgTex.loadFromFile(
          "assets/settings.png")) { // <- USER: Đường dẫn ảnh nền Settings
    mSettingsBgSprite.setTexture(mSettingsBgTex);
    sf::Vector2u size = mSettingsBgTex.getSize();
    mSettingsBgSprite.setScale(
        sf::Vector2f(static_cast<float>(WINDOW_WIDTH) / size.x,
                     static_cast<float>(WINDOW_HEIGHT) / size.y));
    mSettingsBgLoaded = true;
  } else {
    mSettingsBgLoaded = false;
  }

  // About Background image
  if (mAboutBgTex.loadFromFile(
          "assets/about_us.png")) { // <- USER: Đường dẫn ảnh nền About
    mAboutBgSprite.setTexture(mAboutBgTex);
    sf::Vector2u size = mAboutBgTex.getSize();
    mAboutBgSprite.setScale(
        sf::Vector2f(static_cast<float>(WINDOW_WIDTH) / size.x,
                     static_cast<float>(WINDOW_HEIGHT) / size.y));
    mAboutBgLoaded = true;
  } else {
    mAboutBgLoaded = false;
  }

  // ============================================================
  // [USER DIRECTIVE]: ĐƯỜNG DẪN NHẠC NỀN TRÒ CHƠI (BACKGROUND MUSIC)
  // Hãy đổi đường dẫn tới tệp nhạc nền mong muốn (.ogg, .wav, .mp3, v.v...) của
  // bạn ở đây.
  // ============================================================
  if (mMusic.openFromFile(
          "assets/AdhesiveWombat - Night Shade.mp3")) { // <- USER: Đường dẫn
                                                        // file nhạc
    mMusic.setLooping(true);
    mMusic.setVolume(static_cast<float>(mVolume));
    mMusic.play();
    mMusicLoaded = true;
  } else {
    std::cerr
        << "Warning: Could not open assets/AdhesiveWombat - Night Shade.mp3"
        << std::endl;
    mMusicLoaded = false;
  }
}

float CGame::getDifficultyMultiplier() const {
  if (mDifficulty == 0)
    return 0.75f; // Easy
  if (mDifficulty == 2)
    return 1.5f; // Hard
  return 1.0f;   // Medium (default)
}

// ============================================================
// Lane Position Math
// ============================================================
int CGame::getVehicleLaneY(int laneIndex) const {
  return START_ZONE_Y - (laneIndex + 1) * LANE_HEIGHT +
         (LANE_HEIGHT - CAR_HEIGHT) / 2;
}

int CGame::getAnimalLaneY(int laneIndex) const {
  return START_ZONE_Y - (NUM_VEHICLE_LANES + laneIndex + 1) * LANE_HEIGHT +
         (LANE_HEIGHT - BIRD_HEIGHT) / 2;
}

// ============================================================
// Main Game loop
// ============================================================
void CGame::run() {
  mClock.restart();
  while (mWindow.isOpen() && mRunning) {
    handleInput();

    if (!mPaused) {
      float dt = mClock.restart().asSeconds();
      mAccumulator += dt;

      // Physics tick
      while (mAccumulator >= TICK_RATE) {
        update();
        mAccumulator -= TICK_RATE;
      }
    } else {
      mClock.restart(); // discard time when paused
    }

    render();
  }
}

// ============================================================
// Handle User Input (Keyboard & Mouse)
// ============================================================
void CGame::handleInput() {
  while (const std::optional<sf::Event> event = mWindow.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      exitGame();
    }

    // --- 1. Mouse Move Handling (For Hover Effects) ---
    if (const auto *mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
      sf::Vector2f mousePos(static_cast<float>(mouseMoved->position.x),
                            static_cast<float>(mouseMoved->position.y));
      if (mState == GameState::Menu) {
        mBtnStartGame->updateHover(mousePos);
        mBtnLoadGame->updateHover(mousePos);
        mBtnSettings->updateHover(mousePos);
        mBtnAbout->updateHover(mousePos);
        mBtnExit->updateHover(mousePos);
      } else if (mState == GameState::Settings) {
        mBtnEasy->updateHover(mousePos);
        mBtnMedium->updateHover(mousePos);
        mBtnHard->updateHover(mousePos);
        mBtnVolDown->updateHover(mousePos);
        mBtnVolUp->updateHover(mousePos);
        mBtnBackFromSettings->updateHover(mousePos);
      } else if (mState == GameState::About) {
        mBtnBackFromAbout->updateHover(mousePos);
      }
    }

    // --- 2. Mouse Click Handling ---
    if (const auto *mousePressed =
            event->getIf<sf::Event::MouseButtonPressed>()) {
      if (mousePressed->button == sf::Mouse::Button::Left) {
        sf::Vector2f mousePos(static_cast<float>(mousePressed->position.x),
                              static_cast<float>(mousePressed->position.y));
        if (mState == GameState::Menu) {
          if (mBtnStartGame->contains(mousePos)) {
            startGame();
          } else if (mBtnLoadGame->contains(mousePos)) {
            std::ifstream inFile("savegame.txt");
            if (inFile) {
              loadGame(inFile);
              mState = GameState::Playing;
            }
          } else if (mBtnSettings->contains(mousePos)) {
            mState = GameState::Settings;
          } else if (mBtnAbout->contains(mousePos)) {
            mState = GameState::About;
          } else if (mBtnExit->contains(mousePos)) {
            exitGame();
          }
        } else if (mState == GameState::Settings) {
          if (mBtnEasy->contains(mousePos)) {
            mDifficulty = 0;
            allocateObjects(); // Apply new speeds
          } else if (mBtnMedium->contains(mousePos)) {
            mDifficulty = 1;
            allocateObjects();
          } else if (mBtnHard->contains(mousePos)) {
            mDifficulty = 2;
            allocateObjects();
          } else if (mBtnVolDown->contains(mousePos)) {
            mVolume = std::max(0, mVolume - 10);
            if (mMusicLoaded) {
              mMusic.setVolume(static_cast<float>(mVolume));
            }
          } else if (mBtnVolUp->contains(mousePos)) {
            mVolume = std::min(100, mVolume + 10);
            if (mMusicLoaded) {
              mMusic.setVolume(static_cast<float>(mVolume));
            }
          } else if (mBtnBackFromSettings->contains(mousePos)) {
            mState = GameState::Menu;
          }
        } else if (mState == GameState::About) {
          if (mBtnBackFromAbout->contains(mousePos)) {
            mState = GameState::Menu;
          }
        }
      }
    }

    // --- 3. Keyboard Handling ---
    if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
      // Escape to exit
      if (keyPressed->code == sf::Keyboard::Key::Escape) {
        exitGame();
      }

      // Game State: Menu
      if (mState == GameState::Menu) {
        if (keyPressed->code == sf::Keyboard::Key::Enter) {
          startGame();
        } else if (keyPressed->code == sf::Keyboard::Key::L) {
          // Try to load game
          std::ifstream inFile("savegame.txt");
          if (inFile) {
            loadGame(inFile);
            mState = GameState::Playing;
          }
        }
      }
      // Game State: Playing
      else if (mState == GameState::Playing) {
        if (keyPressed->code == sf::Keyboard::Key::P) {
          if (mPaused)
            resumeGame();
          else
            pauseGame();
        } else if (keyPressed->code == sf::Keyboard::Key::T) {
          // Force animal tell sounds (prints to console)
          for (int i = 0; i < mNumDinos; ++i)
            akl[i].Tell();
          for (int i = 0; i < mNumBirds; ++i)
            ac[i].Tell();
        } else if (keyPressed->code == sf::Keyboard::Key::K) {
          // Save game
          std::ofstream outFile("savegame.txt");
          if (outFile) {
            saveGame(outFile);
            std::cout << "Game saved successfully!" << std::endl;
          }
        } else if (!mPaused) {
          // Move keys
          if (keyPressed->code == sf::Keyboard::Key::W)
            updatePosPeople('W');
          else if (keyPressed->code == sf::Keyboard::Key::S)
            updatePosPeople('S');
          else if (keyPressed->code == sf::Keyboard::Key::A)
            updatePosPeople('A');
          else if (keyPressed->code == sf::Keyboard::Key::D)
            updatePosPeople('D');
        }
      }
      // Game State: Dead
      else if (mState == GameState::Dead) {
        if (keyPressed->code == sf::Keyboard::Key::Y) {
          resetGame();
        } else {
          exitGame();
        }
      }
      // Game State: LevelUp / MaxLevel
      else if (mState == GameState::LevelUp || mState == GameState::MaxLevel) {
        if (keyPressed->code == sf::Keyboard::Key::Space ||
            keyPressed->code == sf::Keyboard::Key::Enter) {
          if (mState == GameState::MaxLevel) {
            resetGame();
          } else {
            levelUp();
          }
        }
      }
    }
  }
}

// ============================================================
// Update game physics and check state transitions
// ============================================================
void CGame::update() {
  if (mState != GameState::Playing)
    return;

  // 1. Move vehicles & animals
  updatePosVehicle();
  updatePosAnimal();

  // 2. Check collision
  if (cn.isImpact(axt, mNumTrucks) || cn.isImpact(axh, mNumCars) ||
      cn.isImpact(akl, mNumDinos) || cn.isImpact(ac, mNumBirds)) {
    mState = GameState::Dead;
    return;
  }

  // 3. Check level completion
  if (cn.isFinish()) {
    if (mLevel >= MAX_LEVEL) {
      mState = GameState::MaxLevel;
    } else {
      mState = GameState::LevelUp;
    }
  }
}

// ============================================================
// Movement Updates
// ============================================================
void CGame::updatePosPeople(char key) {
  if (key == 'W')
    cn.Up(PEOPLE_STEP);
  else if (key == 'S')
    cn.Down(PEOPLE_STEP);
  else if (key == 'A')
    cn.Left(PEOPLE_STEP);
  else if (key == 'D')
    cn.Right(PEOPLE_STEP);
}

void CGame::updatePosVehicle() {
  for (int i = 0; i < mNumTrucks; ++i) {
    axt[i].Move(0, 0);
  }
  for (int i = 0; i < mNumCars; ++i) {
    axh[i].Move(0, 0);
  }
}

void CGame::updatePosAnimal() {
  for (int i = 0; i < mNumDinos; ++i) {
    akl[i].Move(0, 0);
  }
  for (int i = 0; i < mNumBirds; ++i) {
    ac[i].Move(0, 0);
  }
}

// ============================================================
// Draw Game State
// ============================================================
void CGame::drawGame() {
  drawLanes();

  // Draw vehicles
  for (int i = 0; i < mNumTrucks; ++i)
    axt[i].draw(mWindow);
  for (int i = 0; i < mNumCars; ++i)
    axh[i].draw(mWindow);

  // Draw animals
  for (int i = 0; i < mNumDinos; ++i)
    akl[i].draw(mWindow);
  for (int i = 0; i < mNumBirds; ++i)
    ac[i].draw(mWindow);

  // Draw player
  cn.draw(mWindow);
}

void CGame::drawLanes() {
  // 1. Draw Start Zone (bottom)
  sf::RectangleShape startZone(sf::Vector2f(WINDOW_WIDTH, LANE_HEIGHT));
  startZone.setPosition(sf::Vector2f(0.f, START_ZONE_Y));
  startZone.setFillColor(
      sf::Color(Colors::START_R, Colors::START_G, Colors::START_B));
  mWindow.draw(startZone);

  // 2. Draw Vehicle lanes (dark grey road)
  for (int i = 0; i < NUM_VEHICLE_LANES; ++i) {
    sf::RectangleShape road(sf::Vector2f(WINDOW_WIDTH, LANE_HEIGHT));
    road.setPosition(sf::Vector2f(0.f, START_ZONE_Y - (i + 1) * LANE_HEIGHT));
    road.setFillColor(
        sf::Color(Colors::ROAD_R, Colors::ROAD_G, Colors::ROAD_B));
    mWindow.draw(road);

    // Lane separator line
    sf::RectangleShape line(sf::Vector2f(WINDOW_WIDTH, 2.f));
    line.setPosition(sf::Vector2f(0.f, START_ZONE_Y - (i + 1) * LANE_HEIGHT));
    line.setFillColor(sf::Color(100, 100, 100));
    mWindow.draw(line);
  }

  // 3. Draw Animal lanes (green grass)
  for (int i = 0; i < NUM_ANIMAL_LANES; ++i) {
    sf::RectangleShape grass(sf::Vector2f(WINDOW_WIDTH, LANE_HEIGHT));
    grass.setPosition(sf::Vector2f(
        0.f, START_ZONE_Y - (NUM_VEHICLE_LANES + i + 1) * LANE_HEIGHT));
    grass.setFillColor(
        sf::Color(Colors::GRASS_R, Colors::GRASS_G, Colors::GRASS_B));
    mWindow.draw(grass);

    // Lane separator line
    sf::RectangleShape line(sf::Vector2f(WINDOW_WIDTH, 2.f));
    line.setPosition(sf::Vector2f(
        0.f, START_ZONE_Y - (NUM_VEHICLE_LANES + i + 1) * LANE_HEIGHT));
    line.setFillColor(sf::Color(45, 160, 45));
    mWindow.draw(line);
  }

  // 4. Draw Finish Zone (top)
  sf::RectangleShape finishZone(sf::Vector2f(WINDOW_WIDTH, LANE_HEIGHT));
  finishZone.setPosition(sf::Vector2f(0.f, 0.f));
  finishZone.setFillColor(
      sf::Color(Colors::FINISH_R, Colors::FINISH_G, Colors::FINISH_B));
  mWindow.draw(finishZone);
}

void CGame::drawHUD() {
  if (!mFontLoaded)
    return;

  sf::Text text(mFont);
  text.setCharacterSize(20);
  text.setFillColor(sf::Color::White);

  // Level info
  text.setString("LEVEL: " + std::to_string(mLevel));
  text.setPosition(sf::Vector2f(20.f, 10.f));
  mWindow.draw(text);

  // Pause info
  if (mPaused && mState == GameState::Playing) {
    text.setString("PAUSED - Press 'P' to Resume");
    text.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.f - 120.f, 10.f));
    text.setFillColor(sf::Color::Yellow);
    mWindow.draw(text);
  } else {
    text.setString("Press 'P' to Pause | 'K' to Save");
    text.setPosition(sf::Vector2f(WINDOW_WIDTH - 280.f, 10.f));
    mWindow.draw(text);
  }
}

void CGame::drawDeathScreen() {
  if (!mFontLoaded)
    return;

  sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
  overlay.setFillColor(sf::Color(0, 0, 0, 200));
  mWindow.draw(overlay);

  sf::Text text(mFont);
  text.setCharacterSize(45);
  text.setFillColor(sf::Color::Red);
  text.setString("GAME OVER");
  text.setPosition(
      sf::Vector2f(WINDOW_WIDTH / 2.f - 130.f, WINDOW_HEIGHT / 2.f - 100.f));
  mWindow.draw(text);

  text.setCharacterSize(22);
  text.setFillColor(sf::Color::White);
  text.setString("Press 'Y' to Retry\nPress Any Other Key to Exit");
  text.setPosition(
      sf::Vector2f(WINDOW_WIDTH / 2.f - 140.f, WINDOW_HEIGHT / 2.f));
  mWindow.draw(text);
}

void CGame::drawLevelUpScreen() {
  if (!mFontLoaded)
    return;

  sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
  overlay.setFillColor(sf::Color(0, 0, 0, 180));
  mWindow.draw(overlay);

  sf::Text text(mFont);
  text.setCharacterSize(40);
  text.setFillColor(sf::Color::Green);
  text.setString("LEVEL COMPLETE!");
  text.setPosition(
      sf::Vector2f(WINDOW_WIDTH / 2.f - 170.f, WINDOW_HEIGHT / 2.f - 80.f));
  mWindow.draw(text);

  text.setCharacterSize(22);
  text.setFillColor(sf::Color::White);
  text.setString("Press ENTER or SPACE to start Level " +
                 std::to_string(mLevel + 1));
  text.setPosition(
      sf::Vector2f(WINDOW_WIDTH / 2.f - 190.f, WINDOW_HEIGHT / 2.f + 10.f));
  mWindow.draw(text);
}

// ============================================================
// Screen Rendering
// ============================================================
void CGame::drawMenuScreen() {
  if (mMenuBgLoaded) {
    mWindow.draw(mMenuBgSprite);
  }

  if (mFontLoaded) {
    sf::Text text(mFont);
    text.setCharacterSize(52);
    text.setFillColor(sf::Color::Yellow);
    text.setString("CROSSING ROAD");
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(3.f);

    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(sf::Vector2f(textBounds.position.x + textBounds.size.x / 2.0f,
                                textBounds.position.y + textBounds.size.y / 2.0f));
    text.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.f, 120.f));
    mWindow.draw(text);
  }

  // Draw Menu buttons
  mBtnStartGame->draw(mWindow);
  mBtnLoadGame->draw(mWindow);
  mBtnSettings->draw(mWindow);
  mBtnAbout->draw(mWindow);
  mBtnExit->draw(mWindow);
}

void CGame::drawSettingsScreen() {
  if (mSettingsBgLoaded) {
    mWindow.draw(mSettingsBgSprite);
  }

  if (mFontLoaded) {
    // Title
    sf::Text titleText(mFont);
    titleText.setCharacterSize(44);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setString("SETTINGS");
    titleText.setOutlineColor(sf::Color::Black);
    titleText.setOutlineThickness(2.f);

    sf::FloatRect tBounds = titleText.getLocalBounds();
    titleText.setOrigin(sf::Vector2f(tBounds.position.x + tBounds.size.x / 2.0f,
                                     tBounds.position.y + tBounds.size.y / 2.0f));
    titleText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.f, 80.f));
    mWindow.draw(titleText);

    // Difficulty label
    sf::Text diffText(mFont);
    diffText.setCharacterSize(26);
    diffText.setFillColor(sf::Color::White);
    diffText.setString("DIFFICULTY:");
    diffText.setOutlineColor(sf::Color::Black);
    diffText.setOutlineThickness(2.f);
    diffText.setPosition(sf::Vector2f(80.f, 250.f));
    mWindow.draw(diffText);

    // Volume label
    sf::Text volLabelText(mFont);
    volLabelText.setCharacterSize(26);
    volLabelText.setFillColor(sf::Color::White);
    volLabelText.setString("VOLUME:");
    volLabelText.setOutlineColor(sf::Color::Black);
    volLabelText.setOutlineThickness(2.f);
    volLabelText.setPosition(sf::Vector2f(80.f, 380.f));
    mWindow.draw(volLabelText);

    // Volume value
    sf::Text volValText(mFont);
    volValText.setCharacterSize(26);
    volValText.setFillColor(sf::Color::Green);
    volValText.setString(std::to_string(mVolume) + "%");
    volValText.setOutlineColor(sf::Color::Black);
    volValText.setOutlineThickness(2.f);
    volValText.setPosition(sf::Vector2f(495.f, 380.f));
    mWindow.draw(volValText);
  }

  // Highlight active difficulty button
  mBtnEasy->setHighlighted(mDifficulty == 0);
  mBtnMedium->setHighlighted(mDifficulty == 1);
  mBtnHard->setHighlighted(mDifficulty == 2);

  // Draw Settings buttons
  mBtnEasy->draw(mWindow);
  mBtnMedium->draw(mWindow);
  mBtnHard->draw(mWindow);
  mBtnVolDown->draw(mWindow);
  mBtnVolUp->draw(mWindow);
  mBtnBackFromSettings->draw(mWindow);
}

void CGame::drawAboutScreen() {
  if (mAboutBgLoaded) {
    mWindow.draw(mAboutBgSprite);
  }

  if (mFontLoaded) {
    // Title
    sf::Text titleText(mFont);
    titleText.setCharacterSize(44);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setString("ABOUT THE GAME");
    titleText.setOutlineColor(sf::Color::Black);
    titleText.setOutlineThickness(2.f);

    sf::FloatRect tBounds = titleText.getLocalBounds();
    titleText.setOrigin(sf::Vector2f(tBounds.position.x + tBounds.size.x / 2.0f,
                                     tBounds.position.y + tBounds.size.y / 2.0f));
    titleText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.f, 80.f));
    mWindow.draw(titleText);

    // Description text
    sf::Text infoText(mFont);
    infoText.setCharacterSize(20);
    infoText.setFillColor(sf::Color::White);
    infoText.setString(
        "Welcome to Crossing Road!\n\n"
        "Help the character cross the busy streets and dangerous fields\n"
        "filled with birds, dinosaurs, trucks, and cars.\n\n"
        "Controls:\n"
        " - Use W, A, S, D to move the player around.\n"
        " - Press P to pause the game at any time.\n"
        " - Press K to save your current progress.\n"
        " - Press L on the menu to load your saved game.\n\n"
        "Avoid obstacles and reach the green finish zone at the top!");
    infoText.setOutlineColor(sf::Color::Black);
    infoText.setOutlineThickness(2.f);
    infoText.setLineSpacing(1.2f);

    sf::FloatRect iBounds = infoText.getLocalBounds();
    infoText.setOrigin(sf::Vector2f(iBounds.position.x + iBounds.size.x / 2.0f,
                                    iBounds.position.y + iBounds.size.y / 2.0f));
    infoText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.f, 320.f));
    mWindow.draw(infoText);
  }

  // Draw Back button
  mBtnBackFromAbout->draw(mWindow);
}

void CGame::render() {
  mWindow.clear(sf::Color::Black);

  if (mState == GameState::Menu) {
    drawMenuScreen();
  } else if (mState == GameState::Settings) {
    drawSettingsScreen();
  } else if (mState == GameState::About) {
    drawAboutScreen();
  } else {
    drawGame();
    drawHUD();

    if (mState == GameState::Dead) {
      drawDeathScreen();
    } else if (mState == GameState::LevelUp) {
      drawLevelUpScreen();
    } else if (mState == GameState::MaxLevel) {
      if (mFontLoaded) {
        sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        overlay.setFillColor(sf::Color(0, 0, 0, 200));
        mWindow.draw(overlay);

        sf::Text text(mFont);
        text.setCharacterSize(45);
        text.setFillColor(sf::Color::Green);
        text.setString("CONGRATULATIONS!");
        text.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.f - 220.f,
                                      WINDOW_HEIGHT / 2.f - 100.f));
        mWindow.draw(text);

        text.setCharacterSize(22);
        text.setFillColor(sf::Color::White);
        text.setString("You completed the game!\nPress ENTER or SPACE to "
                       "restart from Level 1");
        text.setPosition(
            sf::Vector2f(WINDOW_WIDTH / 2.f - 210.f, WINDOW_HEIGHT / 2.f));
        mWindow.draw(text);
      }
    }
  }

  mWindow.display();
}

// ============================================================
// Game Logic Control
// ============================================================
void CGame::startGame() {
  mState = GameState::Playing;
  cn.revive();
  cn.resetPosition(WINDOW_WIDTH / 2 - PEOPLE_SIZE / 2,
                   START_ZONE_Y + (LANE_HEIGHT - PEOPLE_SIZE) / 2);
  mClock.restart();
}

void CGame::resetGame() {
  mLevel = 1;
  allocateObjects();
  startGame();
}

void CGame::exitGame() {
  mRunning = false;
  mWindow.close();
}

void CGame::levelUp() {
  mLevel++;
  allocateObjects();
  startGame();
}

void CGame::pauseGame() { mPaused = true; }

void CGame::resumeGame() {
  mPaused = false;
  mClock.restart();
}

// ============================================================
// Save and Load Game State
// ============================================================
void CGame::saveGame(std::ostream &out) {
  // 1. Level
  out << mLevel << "\n";

  // 2. Player position and state
  out << cn.getX() << " " << cn.getY() << " " << cn.getState() << "\n";

  // 3. Vehicles
  out << mNumTrucks << "\n";
  for (int i = 0; i < mNumTrucks; ++i) {
    out << axt[i].getX() << " " << axt[i].getY() << "\n";
  }

  out << mNumCars << "\n";
  for (int i = 0; i < mNumCars; ++i) {
    out << axh[i].getX() << " " << axh[i].getY() << "\n";
  }

  // 4. Animals
  out << mNumDinos << "\n";
  for (int i = 0; i < mNumDinos; ++i) {
    out << akl[i].getX() << " " << akl[i].getY() << "\n";
  }

  out << mNumBirds << "\n";
  for (int i = 0; i < mNumBirds; ++i) {
    out << ac[i].getX() << " " << ac[i].getY() << "\n";
  }
}

void CGame::loadGame(std::istream &in) {
  // 1. Level
  in >> mLevel;

  // 2. Player position and state
  int px, py;
  bool pstate;
  in >> px >> py >> pstate;
  cn.setPosition(px, py);
  if (pstate)
    cn.revive();
  else
    cn.isImpact(axt, 0); // triggers death state locally

  // Deallocate old object structures and allocate sizes based on loaded level
  deallocateObjects();

  // 3. Load Trucks
  in >> mNumTrucks;
  axt = new CTruck[mNumTrucks];
  int tx, ty;
  for (int i = 0; i < mNumTrucks; ++i) {
    in >> tx >> ty;
    axt[i] = CTruck(tx, ty, 1);
  }

  // Load Cars
  in >> mNumCars;
  axh = new CCar[mNumCars];
  int cx, cy;
  for (int i = 0; i < mNumCars; ++i) {
    in >> cx >> cy;
    axh[i] = CCar(cx, cy, -1);
  }

  // 4. Load Dinosaurs
  in >> mNumDinos;
  akl = new CDinausor[mNumDinos];
  int dx, dy;
  for (int i = 0; i < mNumDinos; ++i) {
    in >> dx >> dy;
    akl[i] = CDinausor(dx, dy, -1);
  }

  // Load Birds
  in >> mNumBirds;
  ac = new CBird[mNumBirds];
  int bx, by;
  for (int i = 0; i < mNumBirds; ++i) {
    in >> bx >> by;
    ac[i] = CBird(bx, by, 1);
  }
}

// ============================================================
// Getters
// ============================================================
CPeople CGame::getPeople() const { return cn; }

CVehicle *CGame::getVehicle() const {
  return axt; // Returns the first vehicle array
}

CAnimal *CGame::getAnimal() const {
  return akl; // Returns the first animal array
}
