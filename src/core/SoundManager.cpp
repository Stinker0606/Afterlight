#include "SoundManager.h"
#include "../config_audio.h.in"
#include <iostream>

SoundManager& SoundManager::GetInstance() {
    static SoundManager instance;
    return instance;
}

void SoundManager::Init() {
    // Lade alle Musikstücke aus der Config in den Cache.
    music_cache_["menu_music"] = LoadMusicStream(game::AudioConfig::kMenuMusicPath.c_str());
    music_cache_["ingame_music"] = LoadMusicStream(game::AudioConfig::kIngameMusicPath.c_str());

    std::cout << "SoundManager initialisiert und Musik geladen." << std::endl;
}

void SoundManager::PlayMusic(const std::string& name) {
    if (music_cache_.find(name) == music_cache_.end()) {
        std::cerr << "FEHLER: Musikstück '" << name << "' nicht gefunden!" << std::endl;
        return;
    }

    StopCurrentMusic(); // Stoppe erst die alte Musik

    current_music_ = &music_cache_.at(name);
    current_music_->looping = true;
    SetMusicVolume(*current_music_, game::AudioConfig::kMusicVolume * game::AudioConfig::kMasterVolume);
    PlayMusicStream(*current_music_);
}

void SoundManager::StopCurrentMusic() {
    if (current_music_ != nullptr) {
        StopMusicStream(*current_music_);
    }
    current_music_ = nullptr;
}

void SoundManager::Update() {
    if (current_music_ != nullptr) {
        UpdateMusicStream(*current_music_);
    }
}

void SoundManager::UnloadAll() {
    for (auto const& [name, music] : music_cache_) {
        UnloadMusicStream(music);
    }
    music_cache_.clear();
    std::cout << "Alle Sounds und Musikstücke entladen." << std::endl;
}