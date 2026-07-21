#include "Animation.h"

Animation::Animation(const sf::Texture* texture, sf::Vector2u imageCount, float switchTime) {
    m_imageCount = imageCount;
    m_switchTime = switchTime;

    m_totalTime = 0.0f;
    m_currentImage.x = 0;
    m_currentImage.y = 0;

    // Tính toán kích thước của MỘT KHUNG HÌNH 
    uvRect.size.x = texture->getSize().x / float(imageCount.x);
    uvRect.size.y = texture->getSize().y / float(imageCount.y);
}

void Animation::Update(int row, float deltaTime, bool faceRight) {
    m_currentImage.y = row;
    m_totalTime += deltaTime;

    // Nếu thời gian tích lũy vượt quá thời gian chuyển frame
    if (m_totalTime >= m_switchTime) {
        m_totalTime -= m_switchTime;
        m_currentImage.x++; // Chuyển sang cột (frame) tiếp theo

        // Nếu chạy hết số cột thì quay lại cột 0 (tùy theo sheet có bao nhiu hàng mà viết)
        if (m_currentImage.x >= m_imageCount.x) {
            m_currentImage.x = 0;
        }
    }

    // CẮT ẢNH THEO TỌA ĐỘ Y (Cú pháp SFML 3.0: Dùng position.y thay cho top)
    uvRect.position.y = m_currentImage.y * std::abs(uvRect.size.y);

    // CẮT ẢNH THEO TỌA ĐỘ X VÀ XỬ LÝ LẬT ẢNH
    if (faceRight) {
        // Hướng sang phải: Giữ nguyên size dương, tọa độ x bình thường
        uvRect.position.x = m_currentImage.x * std::abs(uvRect.size.x);
        uvRect.size.x = std::abs(uvRect.size.x);
    }
    else {
        // Hướng sang trái: Size x mang dấu âm để lật ảnh, dời tọa độ x sang mép phải frame
        uvRect.position.x = (m_currentImage.x + 1) * std::abs(uvRect.size.x);
        uvRect.size.x = -std::abs(uvRect.size.x);
    }
}
