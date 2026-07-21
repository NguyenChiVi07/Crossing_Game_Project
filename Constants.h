#pragma once

// Window
constexpr int WINDOW_WIDTH = 1024;
constexpr int WINDOW_HEIGHT = 768;
constexpr int CELL_SIZE = 48;
constexpr int LANE_HEIGHT = CELL_SIZE;

// Levels
constexpr int MAX_LEVEL = 5;
constexpr float TICK_RATE = 1.f / 60.f;
constexpr int BASE_TRUCKS = 2;
constexpr int BASE_CARS = 2;
constexpr int BASE_BIRDS = 2;
constexpr int BASE_DINASORS = 1;
constexpr int LEVEL_INCREMENT = 1; // +1 of each type per level

// Speeds (pixels per update tick)
constexpr float TRUCK_SPEED = 2.0f;
constexpr float CAR_SPEED = 3.0f;
constexpr float BIRD_SPEED = 2.5f;
constexpr float DINO_SPEED = 1.5f;
constexpr float PEOPLE_STEP = static_cast<float>(CELL_SIZE);

// Lane layout (from bottom to top)
// Bottom: Start zone (1 lane)
// Then: Vehicle lanes
// Then: Animal lanes
// Top: Finish zone (1 lane)
constexpr int NUM_VEHICLE_LANES = 2;
constexpr int NUM_ANIMAL_LANES = 2;
constexpr int TOTAL_LANES = NUM_VEHICLE_LANES + NUM_ANIMAL_LANES;

// Lane Y positions (calculated from bottom)
// The window is divided into: start zone + vehicle lanes + animal lanes +
// finish zone
constexpr int START_ZONE_Y = WINDOW_HEIGHT - LANE_HEIGHT;
constexpr int FINISH_ZONE_Y = LANE_HEIGHT; // Top area

// Object dimensions
constexpr int TRUCK_WIDTH = CELL_SIZE * 3;
constexpr int TRUCK_HEIGHT = CELL_SIZE - 4;
constexpr int CAR_WIDTH = CELL_SIZE * 2;
constexpr int CAR_HEIGHT = CELL_SIZE - 4;
constexpr int BIRD_WIDTH = CELL_SIZE;
constexpr int BIRD_HEIGHT = CELL_SIZE - 8;
constexpr int DINO_WIDTH = CELL_SIZE * 2;
constexpr int DINO_HEIGHT = CELL_SIZE - 4;
constexpr int PEOPLE_SIZE = CELL_SIZE - 8;

// Colors (as RGB)
namespace Colors {
constexpr unsigned char ROAD_R = 60, ROAD_G = 60, ROAD_B = 60;
constexpr unsigned char GRASS_R = 34, GRASS_G = 139, GRASS_B = 34;
constexpr unsigned char START_R = 50, START_G = 50, START_B = 80;
constexpr unsigned char FINISH_R = 30, FINISH_G = 100, FINISH_B = 30;
} // namespace Colors
