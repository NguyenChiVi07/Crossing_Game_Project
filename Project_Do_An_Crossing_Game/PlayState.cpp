#include "PlayState.h"
#include "GameManager.h"

PlayState::PlayState(GameManager* gameManager)
{
    mGameManager = gameManager;

    mScore = 0;
    mlevel = 1;
    m_SpeedBonus = 0.f; // Chưa có tốc độ cộng thêm ở level 1
    m_IsGameOver = false;

    m_Player = new CPEOPLE(500.f, 700.f);

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
                bool spawnObstacle = (rand() % 3 == 0);
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

void PlayState::spawnObstacle(float delTime)
{
    // 1. Tính toán thời gian (Timer)
    m_SpawnTimer += delTime;
    if (m_SpawnTimer < m_NextSpawnTime) {
        return; // Chưa tới lúc sinh vật cản, thoát hàm
    }

    // 2. Tìm tất cả các làn đường là ROAD
    std::vector<LaneData> roadLanes;
    for (const auto& lane : m_Lanes) {
        if (lane.type == LaneType::ROAD) {
            roadLanes.push_back(lane);
        }
    }

    // Nếu map ngẫu nhiên không có đường nhựa nào thì thoát
    if (roadLanes.empty()) return;

    // 3. Chọn ngẫu nhiên 1 làn đường bất kỳ
    int randomIdx = rand() % roadLanes.size();
    float laneY = roadLanes[randomIdx].yPos;
    float spawnX = 1200.f; // Vị trí mép phải màn hình (chỗ bắt đầu sinh ra)

    // BẢO VỆ CHỐNG ĐÈ: Tìm xem xe/con vật cuối cùng trên làn này đang ở đâu
    float rightMostX = -1000.f;
    for (auto obs : m_Obstacles) {
        if (std::abs(obs->getY() - (laneY + 50.f)) < 10.f) {
            if (obs->getX() > rightMostX) {
                rightMostX = obs->getX();
            }
        }
    }

    // Nếu con vật trước đó chưa đi đủ xa (cách chưa được 150px) 
    // -> Bỏ qua lượt này để chúng không dính vào nhau.
    if (spawnX - rightMostX < 150.f) {
        return;
    }

    // 4. Sinh ngẫu nhiên Xe hoặc Động vật
    Obstacle* newMovingObstacle = nullptr;
    if (rand() % 2 == 0) {
        newMovingObstacle = new CVEHICLE(spawnX + 50.f, laneY + 50.f);
    }
    else {
        newMovingObstacle = new CANIMAL(spawnX + 50.f, laneY + 50.f);
    }

    // Giữ lại logic chung vận tốc gốc cho cùng 1 làn để hiệu ứng nối đuôi không bị lỗi
    float laneBaseSpeed = 100.f + (laneY / 100.f) * 15.f;
    newMovingObstacle->setSpeed(laneBaseSpeed + m_SpeedBonus);

    // Đưa vào danh sách
    m_Obstacles.push_back(newMovingObstacle);

    // 5. Reset Timer và Random thời gian cho con tiếp theo (Ví dụ: Từ 0.5s đến 1.5s)
    m_SpawnTimer = 0.f;
    m_NextSpawnTime = (rand() % 100 + 100) / 100.f;
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
    m_Player->resetPosition(500.f, 700.f);
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
    spawnObstacle(delTime);

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