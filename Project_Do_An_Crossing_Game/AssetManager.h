#pragma once
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
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

    sf::Texture& getTexture(const string& filename);
    sf::Font& getFont(const string& filename);
    sf::SoundBuffer& getSoundBuffer(const string& filename);

    // --- QUẢN LÝ MUSIC ---
    void playMusic(const std::string& filePath, bool loop = true)
    {
        if (!m_bgMusic.openFromFile(filePath)) return;
        m_bgMusic.setLooping(loop);
        m_bgMusic.setVolume(m_isMusicOn ? 100.f : 0.f);
        m_bgMusic.play();
    }

    void toggleMusic() 
    {
        m_isMusicOn = !m_isMusicOn;
        if (!m_isMusicOn) {
            m_bgMusic.stop(); // Dừng nhạc ngay lập tức khi người chơi chọn OFF
        }
        else 
        {
            playMusic("../ASSETS/AUDIO/AdhesiveWombat - Night Shade_copy.mp3", true); // Phát lại khi ON
        }
    }
    // --- QUẢN LÝ SFX ---
    void toggleSfx() { m_isSfxOn = !m_isSfxOn;}
    //--------------------------------
    bool isMusicOn() const { return m_isMusicOn; }
    bool isSfxOn() const { return m_isSfxOn; }
private:
    std::map<string, sf::Texture> m_textures;
    std::map<string, sf::Font> m_fonts;
    std::map<string, sf::SoundBuffer> m_soundBuffers;

    sf::Music m_bgMusic;
    // --- CÔNG TẮC ÂM THANH TOÀN CỤC ---
    bool m_isMusicOn = true;
    bool m_isSfxOn = true;

    AssetManager() = default;
};

