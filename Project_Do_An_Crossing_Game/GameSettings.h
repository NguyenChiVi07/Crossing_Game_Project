#pragma once

class GameSettings {
private:
    bool m_isSFXOn;
    bool m_isMusicOn;

    GameSettings() : m_isSFXOn(true), m_isMusicOn(true) {}

public:
    static GameSettings& getInstance() {
        static GameSettings instance;
        return instance;
    }

    bool isSFXOn() const { return m_isSFXOn; }
    bool isMusicOn() const { return m_isMusicOn; }

    void setSFX(bool status) { m_isSFXOn = status; }
    void setMusic(bool status) { m_isMusicOn = status; }

    void toggleSFX() { m_isSFXOn = !m_isSFXOn; }
    void toggleMusic() { m_isMusicOn = !m_isMusicOn; }
};