#pragma once
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include <map>
#include <list>
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
        if (!m_isMusicOn) return;

        if (m_currentMusicPath == filePath && m_bgMusic.getStatus() == sf::Music::Status::Playing)
        {
            return;
        }

        m_currentMusicPath = filePath;
        if (!m_bgMusic.openFromFile(filePath)) return;

        m_bgMusic.setLooping(loop); 
        m_bgMusic.setVolume(100.f);
        m_bgMusic.play();
    }


    void toggleMusic() 
    {
        m_isMusicOn = !m_isMusicOn;
        if (!m_isMusicOn) {
            m_bgMusic.stop(); 
        }
        else
        {
            
            if (!m_currentMusicPath.empty())
            {
                playMusic(m_currentMusicPath, true);
            }
        }
    }
    // --- QUẢN LÝ SFX ---
    void toggleSfx() { m_isSfxOn = !m_isSfxOn;}

    void playSfx(const std::string& filename)
    {
        if (!m_isSfxOn) return;

        m_activeSounds.remove_if([](const sf::Sound& s) {
            return s.getStatus() == sf::Sound::Status::Stopped;
            });

        sf::SoundBuffer& buffer = getSoundBuffer(filename);
        m_activeSounds.emplace_back(buffer);
        m_activeSounds.back().play();
    }
    //--------------------------------
    bool isMusicOn() const { return m_isMusicOn; }
    bool isSfxOn() const { return m_isSfxOn; }
private:
    std::map<string, sf::Texture> m_textures;
    std::map<string, sf::Font> m_fonts;
    std::map<string, sf::SoundBuffer> m_soundBuffers;

    sf::Music m_bgMusic;
    std::string m_currentMusicPath = ""; 
    std::list<sf::Sound> m_activeSounds;

    // --- CÔNG TẮC ÂM THANH TOÀN CỤC ---
    bool m_isMusicOn = true;
    bool m_isSfxOn = true;

    AssetManager() = default;
};

