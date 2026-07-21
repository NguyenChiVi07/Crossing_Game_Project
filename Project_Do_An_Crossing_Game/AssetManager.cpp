#include "AssetManager.h"

AssetManager& AssetManager::getInstance()
{
    static AssetManager instance;
    return instance;
}

sf::Texture& AssetManager::getTexture(const string& filename)
{
    auto it = m_textures.find(filename);
    if (it != m_textures.end())
    {
        return it->second;
    }
    else
    {
        sf::Texture texture;
		if (texture.loadFromFile("../ASSETS/IMAGES/" + filename)) // tao thu muc nhu vay trong file explorer
        {
            texture.setSmooth(false);
            m_textures[filename] = move(texture);
            return  m_textures[filename];
        }
        else
        {
            cout << "Co loi load anh !";
            m_textures[filename] = sf::Texture();
            return  m_textures[filename];
        }
    }
}

sf::Font& AssetManager::getFont(const string& filename)
{
    auto it = m_fonts.find(filename);
    if (it != m_fonts.end())
    {
        return it->second;
    }
    else
    {
        sf::Font font;
        if (font.openFromFile("../ASSETS/FONT/" + filename))
        {
            m_fonts[filename] = move(font);
            return  m_fonts[filename];
        }
        else
        {
            cout << "Co loi load font !";
            m_fonts[filename] = sf::Font();
            return  m_fonts[filename];
        }
    }
}
