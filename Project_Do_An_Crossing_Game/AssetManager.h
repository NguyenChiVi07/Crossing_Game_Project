#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <iostream>
using namespace std;

class AssetManager
{
public:
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    static AssetManager& getInstance();

    // Các hàm cung cấp tài nguyên cho game
    sf::Texture& getTexture(const string& filename);
    sf::Font& getFont(const string& filename);

private:
    // "Từ điển" lưu trữ hình ảnh và font chữ
    std::map<string, sf::Texture> m_textures;
    std::map<string, sf::Font> m_fonts;
    // Ép Constructor vào private để không ai có thể tạo ra AssetManager thứ 2
    AssetManager() {}
};

