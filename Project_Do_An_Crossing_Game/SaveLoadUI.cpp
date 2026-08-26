#include "SaveLoadUI.h"
#include <iostream>
#include <cstdio>
#include <algorithm>

// Đưa TẤT CẢ các biến sf::Text vào danh sách khởi tạo để SFML 3.x vui lòng nè
SaveLoadUI::SaveLoadUI() 
    : m_title(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_saveBtn(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_loadBtn(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_closeBtn(AssetManager::getInstance().getFont("DIMIS___.ttf")),
    m_deleteBtn(AssetManager::getInstance().getFont("DIMIS___.ttf")), // ---> CHISA ĐÃ THÊM BẠN ẤY VÀO ĐÂY RỒI NÈ
    m_pendingAction(SaveLoadAction::NONE),
    m_targetFileName(""),
    m_selectedFile("")
{
    sf::Font& font = AssetManager::getInstance().getFont("DIMIS___.ttf");

    // 1. Dựng nền mờ để nổi bật Menu
    m_overlay.setSize({1600.f, 800.f});
    m_overlay.setFillColor(sf::Color(0, 0, 0, 200));

    // 2. Dựng bảng điều khiển chính
    m_panel.setSize({700.f, 550.f});
    m_panel.setOrigin({350.f, 275.f});
    m_panel.setPosition({800.f, 400.f});
    m_panel.setFillColor(sf::Color(40, 44, 52));
    m_panel.setOutlineThickness(4.f);
    m_panel.setOutlineColor(sf::Color(255, 200, 50));

    // 3. Tiêu đề (Đã có Font từ trên kia rồi nên tớ xóa dòng setFont đi)
    m_title.setString("SAVE / LOAD GAME");
    m_title.setCharacterSize(42);
    m_title.setFillColor(sf::Color(255, 200, 50));
    sf::FloatRect bounds = m_title.getLocalBounds();
    m_title.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    m_title.setPosition({800.f, 180.f});

    // 4. Khởi tạo thanh nhập chữ (InputBox)
    m_inputBox = new InputBox(550.f, 240.f, 500.f, 45.f, font, 20);

    // 5. Khởi tạo các nút bấm chức năng
    auto setupBtn = [](sf::Text& btn, const std::string& str, float x, float y) {
        btn.setString(str);
        btn.setCharacterSize(34);
        btn.setFillColor(sf::Color::White);
        sf::FloatRect b = btn.getLocalBounds();
        btn.setOrigin({b.size.x / 2.f, b.size.y / 2.f});
        btn.setPosition({x, y});
    };

    setupBtn(m_saveBtn, "SAVE", 520.f, 600.f);
    setupBtn(m_loadBtn, "LOAD", 710.f, 600.f);
    setupBtn(m_deleteBtn, "DELETE", 900.f, 600.f);
    setupBtn(m_closeBtn, "CLOSE", 1090.f, 600.f);

    // 6. Tải danh bạ các khe lưu hiện có
    loadSaveList();
}

SaveLoadUI::~SaveLoadUI() {
    delete m_inputBox;
}

void SaveLoadUI::loadSaveList() {
    m_saveFiles.clear();
    std::ifstream file("save_list.txt");
    std::string line;
    // Đọc từng dòng trong file danh bạ
    while (std::getline(file, line)) {
        if (!line.empty()) m_saveFiles.push_back(line);
    }
    updateSlotTexts();
}

void SaveLoadUI::saveSaveList() {
    std::ofstream file("save_list.txt");
    for (const auto& f : m_saveFiles) {
        file << f << "\n";
    }
}

void SaveLoadUI::updateSlotTexts() {
    m_slotTexts.clear();
    sf::Font& font = AssetManager::getInstance().getFont("DIMIS___.ttf");
    
    float startY = 320.f; // Vị trí bắt đầu của danh sách
    for (size_t i = 0; i < m_saveFiles.size(); ++i) {
        // Khởi tạo trực tiếp với Font ngay tại đây!
        sf::Text text(font); 
        text.setString(m_saveFiles[i]);
        text.setCharacterSize(28);
        text.setFillColor(sf::Color(200, 200, 200));
        
        sf::FloatRect b = text.getLocalBounds();
        text.setOrigin({b.size.x / 2.f, b.size.y / 2.f});
        text.setPosition({800.f, startY + i * 45.f});
        
        m_slotTexts.push_back(text);
        
        // Giới hạn hiển thị tối đa 5 slot gần nhất cho đẹp mắt
        if (i >= 4) break; 
    }
}

void SaveLoadUI::HandleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    m_inputBox->HandleEvent(event, window);

    if (const auto* mouseBtn = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseBtn->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            // NÚT SAVE
            if (m_saveBtn.getGlobalBounds().contains(mousePos)) {
                std::string name = m_inputBox->getText();
                if (!name.empty()) {
                    m_targetFileName = name + ".dat";
                    m_pendingAction = SaveLoadAction::SAVE;
                    
                    bool exists = false;
                    for (const auto& f : m_saveFiles) {
                        if (f == name) exists = true;
                    }
                    if (!exists) {
                        m_saveFiles.insert(m_saveFiles.begin(), name); 
                        saveSaveList();
                        updateSlotTexts();
                    }
                }
            } 
            // NÚT LOAD
            else if (m_loadBtn.getGlobalBounds().contains(mousePos)) {
                std::string name = m_inputBox->getText();
                if (name.empty()) name = m_selectedFile; 
                
                if (!name.empty()) {
                    m_targetFileName = name + ".dat";
                    m_pendingAction = SaveLoadAction::LOAD;
                }
            }
            // NÚT CLOSE
            else if (m_closeBtn.getGlobalBounds().contains(mousePos)) {
                m_pendingAction = SaveLoadAction::CLOSE;
            }
            // NÚT DELETE
            else if (m_deleteBtn.getGlobalBounds().contains(mousePos)) {
                if (!m_selectedFile.empty()) {
                    // 1. Xóa file .dat thật trong ổ cứng
                    std::string fileName = m_selectedFile + ".dat";
                    std::remove(fileName.c_str());

                    // 2. Xóa tên khỏi quyển danh bạ
                    m_saveFiles.erase(std::remove(m_saveFiles.begin(), m_saveFiles.end(), m_selectedFile), m_saveFiles.end());

                    // 3. Lưu danh bạ mới và cập nhật giao diện
                    saveSaveList();
                    updateSlotTexts();
                    m_selectedFile = "";
                }
            }
            
            // CHỌN SLOT ĐÃ LƯU
            for (size_t i = 0; i < m_slotTexts.size(); ++i) {
                if (m_slotTexts[i].getGlobalBounds().contains(mousePos)) {
                    m_selectedFile = m_saveFiles[i];
                    break;
                }
            }

        }
    }
}

void SaveLoadUI::Update(const sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    auto applyHover = [&mousePos](sf::Text& t) {
        if (t.getGlobalBounds().contains(mousePos)) t.setFillColor(sf::Color(255, 200, 50));
        else t.setFillColor(sf::Color::White);
    };

    applyHover(m_saveBtn);
    applyHover(m_loadBtn);
    applyHover(m_closeBtn);

    for (size_t i = 0; i < m_slotTexts.size(); ++i) {
        if (m_saveFiles.size() > i && m_saveFiles[i] == m_selectedFile) {
            m_slotTexts[i].setFillColor(sf::Color(100, 255, 100)); 
        } else if (m_slotTexts[i].getGlobalBounds().contains(mousePos)) {
            m_slotTexts[i].setFillColor(sf::Color(255, 255, 150)); 
        } else {
            m_slotTexts[i].setFillColor(sf::Color(200, 200, 200)); 
        }
    }
}

void SaveLoadUI::Draw(sf::RenderWindow& window) const {
    window.draw(m_overlay);
    window.draw(m_panel);
    window.draw(m_title);
    m_inputBox->Draw(window);
    
    for (const auto& t : m_slotTexts) {
        window.draw(t);
    }

    window.draw(m_saveBtn);
    window.draw(m_loadBtn);
    window.draw(m_deleteBtn);
    window.draw(m_closeBtn);
}

void SaveLoadUI::refreshList() {
    loadSaveList();
}