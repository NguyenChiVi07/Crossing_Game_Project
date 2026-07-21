#include "PlayState.h"
#include "GameManager.h"

PlayState::PlayState(GameManager* gameManager)
{
    mGameManager = gameManager;

    mScore = 0;
    mlevel = 1;
    m_SpeedBonus = 0.f; // Chưa có tốc độ cộng thêm ở level 1
    m_IsGameOver = false;

    m_Player = new CPEOPLE(550.f, 700.f);

    m_SpawnTimer = 0.f;
    m_NextSpawnTime = 1.5f;

    // Cài đặt UI Panel (400x800, đặt từ X=1200)
    m_UIBackground.setSize({ 400.f, 800.f });
    m_UIBackground.setPosition({ 1200.f, 0.f });
    m_UIBackground.setFillColor(sf::Color(255, 200, 50)); // Màu vàng tượng trưng

    // Khởi tạo map cho Level 1
    generateLevel();
}

PlayState::~PlayState()
{
    delete m_Player;
    for (auto obs : m_Obstacles) {
        delete obs;
    }
}

void PlayState::Init()
{
}

//Cần ai đó viết class vật cản tĩnh. Đọc ghi chú ở file PlayState.cpp
//Mục tiêu của hàm này là khi mình lên level thì tự tạo ra map mới
void PlayState::generateLevel()
{
    // 1. Xóa toàn bộ chướng ngại vật & làn đường cũ
    for (auto obs : m_Obstacles) {
        delete obs;
    }
    m_Obstacles.clear();
    m_Lanes.clear();

    // 2. Tạo Làn Trên Cùng và Dưới Cùng
    spawnLane(0.f, LaneType::GRASS_EMPTY);
    spawnLane(700.f, LaneType::GRASS_EMPTY);

    // 3. Random 6 Làn Ở Giữa (Y từ 100 đến 600)
    int roadCount = 0;
    int consecutiveRoadCount = 0;

    for (int i = 1; i <= 6; i++) {
        float yPos = i * 100.f;
        LaneType chosenType;

        // Xử lý ràng buộc: Max 4 làn đường, Không quá 3 làn đường liên tiếp
        if (roadCount >= 4 || consecutiveRoadCount >= 3) {
            chosenType = LaneType::GRASS;
        }
        else {
            // Tỉ lệ 50-50 ra Road hoặc Grass
            chosenType = (rand() % 2 == 0) ? LaneType::ROAD : LaneType::GRASS;
        }

        if (chosenType == LaneType::ROAD) {
            roadCount++;
            consecutiveRoadCount++;
        }
        else {
            consecutiveRoadCount = 0; // Reset số làn đường liên tiếp
        }

        spawnLane(yPos, chosenType);

        //Chõ này là để tạo ra vật cản tĩnh, nó tạo ra chung với lúc 
        // gen ra cái đường.
        int obstacleCount = 0;
        int consecutiveObstacleCount = 0;
        if (chosenType == LaneType::GRASS) {
            for (int j = 0; j < 12; j++) 
            {
                bool spawnObstacle = (rand() % 2 == 0);
                float xPos = j * 100.f;
                if (obstacleCount >= 6 || consecutiveObstacleCount >= 4) {
                    spawnObstacle = false;
                }

                if (spawnObstacle) {
                    consecutiveObstacleCount++;
                    obstacleCount++;
                    Obstacle* newObstacle = new StaticObstacle(xPos + 50.f, yPos + 50.f);
                    m_Obstacles.push_back(newObstacle);
                }
                else 
                    consecutiveObstacleCount = 0;
            }
        }
    }
}

//spawn ra lane với vị trí và loại lane.
void PlayState::spawnLane(float yPos, LaneType type)
{
    // Lấy texture tương ứng
    sf::Texture* texture = NULL;
    if (type == LaneType::ROAD) {
        texture = &AssetManager::getInstance().getTexture("road.png");
    }
    else{
        texture = &AssetManager::getInstance().getTexture("Grass.png");
    }
    LaneData newLane(yPos, type, *texture);
    m_Lanes.push_back(newLane);
}

void PlayState::spawnObstacle()
{
    int maxPerLane = 2 + (mlevel / 2);
    if (maxPerLane > 4) {
        maxPerLane = 4;
    }

    // 2. Tính khoảng cách an toàn (tạo không gian đều nhau giữa các xe)
    // Chiều dài vùng chơi là 1200px. Chia đều khoảng cách này.
    float safeDistance = 1200.f / maxPerLane;
    float spawnX = 1200.f; // Xe sinh ra ở sát mép phải vùng chơi

    for (const auto& lane : m_Lanes) {
        if (lane.type != LaneType::ROAD) continue;

        float laneY = lane.yPos;
        int currentObstaclesCount = 0;
        float rightMostX = -1000.f; // Biến lưu tọa độ X của vật cản đứng gần điểm sinh (1200) nhất

        // Đếm số lượng và tìm khoảng cách của vật cản gần nhất trên làn này
        for (auto obs : m_Obstacles) {
            // Kiểm tra xem vật cản có thuộc làn này không (So sánh Y)
            // Lấy laneY + 50.f vì Origin của vật cản đang nằm ở tâm
            if (std::abs(obs->getY() - (laneY + 50.f)) < 10.f) {
                currentObstaclesCount++;
                if (obs->getX() > rightMostX) {
                    rightMostX = obs->getX();
                }
            }
        }

        // Nếu làn này đã có đủ số lượng vật cản -> Bỏ qua, sang làn đường khác
        if (currentObstaclesCount >= maxPerLane) continue;

        // ĐẢM BẢO GIÃN CÁCH: Nếu vật cản cuối cùng vừa sinh ra chưa đi đủ xa -> Bỏ qua
        if (spawnX - rightMostX < safeDistance) continue;

        // --- ĐỦ ĐIỀU KIỆN SINH VẬT CẢN MỚI CHO LÀN NÀY ---
        Obstacle* newMovingObstacle = nullptr;

        // Tỉ lệ 50% ra Xe, 50% ra Động vật
        if (rand() % 2 == 0) {
            newMovingObstacle = new CVEHICLE(spawnX + 50.f, laneY + 50.f);
        }
        else {
            newMovingObstacle = new CANIMAL(spawnX + 50.f, laneY + 50.f);
        }

        // 4. GIẢI QUYẾT LỖI VƯỢT MẶT: Tính tốc độ dựa trên CHÍNH LÀN ĐƯỜNG ĐÓ
        // Bằng cách này, dù là Xe hay Thú trên cùng 1 làn đều có chung 1 vận tốc gốc
        float laneBaseSpeed = 100.f + (laneY / 100.f) * 15.f; // Làn càng dưới tốc độ có thể khác đi chút ít

        // Cài đặt vận tốc: Tốc độ chuẩn của làn + Tốc độ thưởng của Level
        newMovingObstacle->setSpeed(laneBaseSpeed + m_SpeedBonus);

        m_Obstacles.push_back(newMovingObstacle);
    }
}

void PlayState::levelUp()
{
    mlevel++;
    mScore += 100; // Cộng điểm khi qua màn
    m_SpeedBonus += 50.f; // Mỗi level xe chạy nhanh hơn 50px/s

    std::cout << "\n[!] LEVEL UP! LEVEL HIEN TAI: " << mlevel << "\n";

    // Random lại map mới
    generateLevel();

    // Reset người chơi về làn dưới cùng (Tọa độ X=550, Y=700)
    m_Player->resetPosition(550.f, 700.f);
}

void PlayState::Update(float delTime, sf::RenderWindow& window)
{
    //chỗ này là đẻ chuyển state
    if (m_IsGameOver)
    {
        mGameManager->setState(new PlayState(mGameManager));
        return;
    };

    m_Player->update(delTime);

    // ==========================================
    // 1. KIỂM TRA ĐIỀU KIỆN THẮNG (LEVEL UP)
    // ==========================================
    // Nếu nhảy đến làn trên cùng (Y = 0)
    if (m_Player->getY() <= 0.f) {
        levelUp();
        return; // Dừng Update frame này lại để tránh lỗi vẽ sai map
    }

    // ==========================================
    // 2. SINH VẬT CẢN ĐỘNG (XE CỘ, hay động vật) LÊN LÀN ĐƯỜNG
    // ==========================================
    m_SpawnTimer += delTime;
    if (m_SpawnTimer >= m_NextSpawnTime) {
        spawnObstacle();
        m_SpawnTimer = 0.f;
        m_NextSpawnTime = (rand() % 100 + 50) / 100.f; // Random 0.5s -> 1.5s
    }

    // ==========================================
    // 3. CẬP NHẬT TỌA ĐỘ CHƯỚNG NGẠI VẬT
    // ==========================================
    for (auto obs : m_Obstacles) {
        obs->UpdateState(delTime);
    }

    // ==========================================
    // 4. KIỂM TRA VA CHẠM & DỌN RÁC
    // ==========================================
    checkCollision();
    cleanUpOffScreen(window);
}

void PlayState::checkCollision()
{
    sf::FloatRect playerBounds = m_Player->getBounds();
    for (auto obs : m_Obstacles) {
        if (playerBounds.findIntersection(obs->getBounds())) {
            m_IsGameOver = true;
            std::cout << "\n[!] GAME OVER! BAN DA CHAM VAO VAT CAN \n";
            return;
        }
    }
}

void PlayState::cleanUpOffScreen(sf::RenderWindow& window)
{
    // Xóa vật cản trôi ra khỏi màn hình bên trái (X < -200)
    for (int i = m_Obstacles.size() - 1; i >= 0; i--) {
        if (m_Obstacles[i]->getX() < -200.f) {
            delete m_Obstacles[i];
            m_Obstacles.erase(m_Obstacles.begin() + i);
        }
    }
}

void PlayState::Render(sf::RenderWindow& window)
{
    // 1. Vẽ Map (Chỉ vẽ 1200px)
    for (auto& lane : m_Lanes) {
        window.draw(lane.bgSprite);
    }

    // 2. Vẽ Chướng ngại vật & Người chơi
    for (auto obs : m_Obstacles) {
        obs->Draw(window);
    }
    m_Player->Draw(window);

    // 3. Vẽ Màn hình thông tin (UI) đè lên bên phải
    window.draw(m_UIBackground);
}