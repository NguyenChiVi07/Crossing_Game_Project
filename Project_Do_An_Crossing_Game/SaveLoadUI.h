#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>
#include <string>
#include <fstream>
#include "InputBox.h"
#include "AssetManager.h"

// Biến trạng thái để báo tin cho PlayState/MenuState biết người chơi vừa bấm nút gì
enum class SaveLoadAction {
    NONE,
    SAVE,
    LOAD,
    DELETE_FILE,
    CLOSE
};

class SaveLoadUI {
private:
    sf::RectangleShape m_overlay; // Nền mờ đằng sau
    sf::RectangleShape m_panel;   // Khung Menu chính
    sf::Text m_title;             // Tiêu đề "SAVE / LOAD GAME"

    InputBox* m_inputBox;         // Thanh nhập tên file tụi mình vừa làm nè

    sf::Text m_saveBtn;
    sf::Text m_loadBtn;
    sf::Text m_deleteBtn;
    sf::Text m_closeBtn;

    std::vector<std::string> m_saveFiles; // Sổ danh bạ chứa tên các file save
    std::vector<sf::Text> m_slotTexts;    // Nút bấm cho từng file trên màn hình

    std::string m_selectedFile;           // Tên file đang được click chọn

    SaveLoadAction m_pendingAction;       // Lệnh đang chờ xử lý
    std::string m_targetFileName;         // Tên file đích cần Save/Load

    void loadSaveList();                  // Hàm đọc file "save_list.txt"
    void saveSaveList();                  // Hàm ghi đè lại file "save_list.txt"
    void updateSlotTexts();               // Cập nhật lại giao diện danh sách file

public:
    SaveLoadUI();
    ~SaveLoadUI();

    void HandleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void Update(const sf::RenderWindow& window);
    void Draw(sf::RenderWindow& window) const;

    // Các hàm để các State bên ngoài (như PlayState) giao tiếp với Menu này
    SaveLoadAction getPendingAction() const { return m_pendingAction; }
    std::string getTargetFileName() const { return m_targetFileName; }
    void resetAction() { m_pendingAction = SaveLoadAction::NONE; m_targetFileName = ""; }

    void refreshList(); // Gọi hàm này khi vừa save xong để load lại danh bạ
};  