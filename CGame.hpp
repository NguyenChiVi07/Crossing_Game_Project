#pragma once
#include "CAnimal.h"
#include "CPeople.h"
#include "CVehicle.h"
#include "CButton.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

enum class GameState {
  Menu,
  Settings,
  About,
  Playing,
  Dead,
  LevelUp,
  MaxLevel
};

class CGame {
public:
  CGame();
  ~CGame();

  void run();
  void allocateObjects();
  void deallocateObjects();
  int getVehicleLaneY(int laneIndex) const;
  int getAnimalLaneY(int laneIndex) const;

  void handleInput();
  void update();
  void render();

  void startGame();
  void resetGame();
  void exitGame();
  void levelUp();
  void pauseGame();
  void resumeGame();

  void saveGame(std::ostream &out);
  void loadGame(std::istream &in);

  void updatePosPeople(char key);
  void updatePosVehicle();
  void updatePosAnimal();

  void drawGame();
  void drawLanes();
  void drawHUD();
  void drawDeathScreen();
  void drawLevelUpScreen();

  // Screen drawing methods
  void drawMenuScreen();
  void drawSettingsScreen();
  void drawAboutScreen();

  // Setup helpers
  void initButtons();
  void cleanupButtons();
  void loadAssets();
  float getDifficultyMultiplier() const;

  CPeople getPeople() const;
  CVehicle *getVehicle() const;
  CAnimal *getAnimal() const;

private:
  CTruck *axt;
  CCar *axh;
  CDinausor *akl;
  CBird *ac;
  int mNumTrucks;
  int mNumCars;
  int mNumDinos;
  int mNumBirds;
  int mLevel;
  bool mPaused;
  bool mRunning;
  GameState mState;
  float mAccumulator;
  bool mFontLoaded;

  sf::Font mFont;
  CPeople cn;
  sf::RenderWindow mWindow;
  sf::Clock mClock;

  // === Difficulty and Volume ===
  int mDifficulty;      // 0 = Easy, 1 = Medium, 2 = Hard
  int mVolume;          // 0 to 100

  // === Music ===
  sf::Music mMusic;
  bool mMusicLoaded;

  // === Background Sprites ===
  sf::Texture mMenuBgTex;
  sf::Sprite mMenuBgSprite;
  bool mMenuBgLoaded;

  sf::Texture mSettingsBgTex;
  sf::Sprite mSettingsBgSprite;
  bool mSettingsBgLoaded;

  sf::Texture mAboutBgTex;
  sf::Sprite mAboutBgSprite;
  bool mAboutBgLoaded;

  // === Menu Buttons ===
  CButton *mBtnStartGame;
  CButton *mBtnLoadGame;
  CButton *mBtnSettings;
  CButton *mBtnAbout;
  CButton *mBtnExit;

  // === Settings Buttons ===
  CButton *mBtnEasy;
  CButton *mBtnMedium;
  CButton *mBtnHard;
  CButton *mBtnVolDown;
  CButton *mBtnVolUp;
  CButton *mBtnBackFromSettings;

  // === About Buttons ===
  CButton *mBtnBackFromAbout;
};