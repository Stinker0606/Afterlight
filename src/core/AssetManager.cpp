#include "AssetManager.h"

AssetManager& AssetManager::GetInstance()
{
    static AssetManager instance; // Wird nur einmal erstellt
    return instance;
}

Texture2D AssetManager::Load(const std::string& path)
{
    // Prüfe, ob die Textur bereits im Cache ist.
    if (texture_cache_.find(path) == texture_cache_.end())
    {
        // Wenn nicht, lade sie und füge sie zum Cache hinzu.
        texture_cache_[path] = LoadTexture(path.c_str());
    }
    // Gib die Textur aus dem Cache zurück.
    return texture_cache_[path];
}

void AssetManager::UnloadAll()
{
    for (auto const& [path, texture] : texture_cache_)
    {
        UnloadTexture(texture);
    }
    texture_cache_.clear();
}