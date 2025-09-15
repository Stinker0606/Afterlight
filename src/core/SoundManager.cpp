#include "SoundManager.h"
#include "raylib.h"
#include "../config_audio.h.in"
#include <iostream>
#include "SettingsManager.h"

SoundManager& SoundManager::GetInstance() {
    static SoundManager instance;
    return instance;
}

void SoundManager::Init() {
    // Lade alle Musikstücke aus der Config in den Cache.
    music_cache_["menu_music"] = LoadMusicStream(game::AudioConfig::kMenuMusicPath.c_str());
    music_cache_["ingame_music"] = LoadMusicStream(game::AudioConfig::kIngameMusicPath.c_str());
    music_cache_["death_music"] = LoadMusicStream(game::AudioConfig::kDeathMusicPath.c_str());

    // Lade die Soundeffekte aus der Config in den Cache
    // UI
    sfx_cache_["ui_navigate"] = LoadSound(game::AudioConfig::kUIMenuNavigateSfxPath.c_str());
    sfx_cache_["ui_select"] = LoadSound(game::AudioConfig::kUIMenuSelectSfxPath.c_str());
    sfx_cache_["game_start"] = LoadSound(game::AudioConfig::kGameStartSfxPath.c_str());

    // Spieler
    sfx_cache_["player_walk"] = LoadSound(game::AudioConfig::kPlayerWalkSfxPath.c_str());
    sfx_cache_["player_idle"] = LoadSound(game::AudioConfig::kPlayerIdleSfxPath.c_str());
    sfx_cache_["player_hit"] = LoadSound(game::AudioConfig::kPlayerHitSfxPath.c_str());
    sfx_cache_["player_death"] = LoadSound(game::AudioConfig::kPlayerDeathSfxPath.c_str());
    sfx_cache_["player_throw"] = LoadSound(game::AudioConfig::kPlayerThrowSfxPath.c_str());
    sfx_cache_["player_sweep"] = LoadSound(game::AudioConfig::kPlayerSweepSfxPath.c_str());
    sfx_cache_["player_push_block"] = LoadSound(game::AudioConfig::kPlayerPushBlockSfxPath.c_str());
    sfx_cache_["player_place_bomb"] = LoadSound(game::AudioConfig::kPlayerPlaceBombSfxPath.c_str());

    // Gegner: Insektenmonster
    sfx_cache_["enemy_insect_hit"] = LoadSound(game::AudioConfig::kEnemyInsectHitSfxPath.c_str());
    sfx_cache_["enemy_insect_death"] = LoadSound(game::AudioConfig::kEnemyInsectDeathSfxPath.c_str());
    sfx_cache_["enemy_insect_attack"] = LoadSound(game::AudioConfig::kEnemyInsectAttackSfxPath.c_str());

    // Gegner: Drowned Sniper
    sfx_cache_["enemy_sniper_hit"] = LoadSound(game::AudioConfig::kEnemySniperHitSfxPath.c_str());
    sfx_cache_["enemy_sniper_death"] = LoadSound(game::AudioConfig::kEnemySniperDeathSfxPath.c_str());
    sfx_cache_["enemy_sniper_shoot"] = LoadSound(game::AudioConfig::kEnemySniperShootSfxPath.c_str());

    // Gegner: Wood Sniper
    sfx_cache_["enemy_wood_sniper_hit"] = LoadSound(game::AudioConfig::kEnemyWoodSniperHitSfxPath.c_str());
    sfx_cache_["enemy_wood_sniper_death"] = LoadSound(game::AudioConfig::kEnemyWoodSniperDeathSfxPath.c_str());
    sfx_cache_["enemy_wood_sniper_shoot"] = LoadSound(game::AudioConfig::kEnemyWoodSniperShootSfxPath.c_str());

    // Gegner: Walking Corpse
    sfx_cache_["enemy_corpse_hit"] = LoadSound(game::AudioConfig::kEnemyCorpseHitSfxPath.c_str());
    sfx_cache_["enemy_corpse_death"] = LoadSound(game::AudioConfig::kEnemyCorpseDeathSfxPath.c_str());
    sfx_cache_["enemy_corpse_attack"] = LoadSound(game::AudioConfig::kEnemyCorpseAttackSfxPath.c_str());

    // Gegner: Corpse Lvl3
    sfx_cache_["enemy_l3_corpse_hit"] = LoadSound(game::AudioConfig::kEnemyL3CorpseHitSfxPath.c_str());
    sfx_cache_["enemy_l3_corpse_death"] = LoadSound(game::AudioConfig::kEnemyL3CorpseDeathSfxPath.c_str());
    sfx_cache_["enemy_l3_corpse_attack"] = LoadSound(game::AudioConfig::kEnemyL3CorpseAttackSfxPath.c_str());

    // Projektile
    sfx_cache_["projectile_player_fly"] = LoadSound(game::AudioConfig::kPlayerProjectileFlySfxPath.c_str());
    sfx_cache_["projectile_player_hit"] = LoadSound(game::AudioConfig::kPlayerProjectileHitSfxPath.c_str());
    sfx_cache_["projectile_enemy_fly"] = LoadSound(game::AudioConfig::kEnemyProjectileFlySfxPath.c_str());
    sfx_cache_["projectile_enemy_hit"] = LoadSound(game::AudioConfig::kEnemyProjectileHitSfxPath.c_str());

    // Items & Welt
    sfx_cache_["item_pickup_heal"] = LoadSound(game::AudioConfig::kItemPickupHealthSfxPath.c_str());
    sfx_cache_["item_pickup_key"] = LoadSound(game::AudioConfig::kItemPickupKeySfxPath.c_str());
    sfx_cache_["item_pickup_bomb"] = LoadSound(game::AudioConfig::kItemPickupBombSfxPath.c_str());
    sfx_cache_["item_pickup_damage"] = LoadSound(game::AudioConfig::kItemPickupDamageSfxPath.c_str());
    sfx_cache_["bomb_explosion"] = LoadSound(game::AudioConfig::kBombExplosionSfxPath.c_str());
    sfx_cache_["wall_break"] = LoadSound(game::AudioConfig::kWorldWallBreakSfxPath.c_str());
    sfx_cache_["keywall_open"] = LoadSound(game::AudioConfig::kWorldKeywallOpenSfxPath.c_str());
    sfx_cache_["door_transition"] = LoadSound(game::AudioConfig::kWorldDoorTransitionSfxPath.c_str());

    std::cout << "SoundManager initialisiert und alle Sounds geladen." << std::endl;
}

// Spielt einen Soundeffekt ab (mit Limit)
void SoundManager::PlaySfx(const std::string& name, int max_instances) {
    if (sfx_cache_.find(name) == sfx_cache_.end()) {
        std::cerr << "FEHLER: Soundeffekt '" << name << "' nicht gefunden!" << std::endl;
        return;
    }

    // Prüfe, ob das Limit für diesen Sound in diesem Frame erreicht ist
    if (sfx_play_counts_this_frame_[name] >= max_instances) {
        return; // Limit erreicht, spiele den Sound nicht noch einmal
    }

    // Spiele den Sound ab und erhöhe den Zähler für diesen Frame
    Sound& sound = sfx_cache_.at(name);
    float final_volume = game::AudioConfig::kSfxVolume * SettingsManager::GetInstance().GetSfxVolume() * SettingsManager::GetInstance().GetMasterVolume();
    SetSoundVolume(sound, final_volume);
    PlaySound(sfx_cache_.at(name));
    sfx_play_counts_this_frame_[name]++;
}

void SoundManager::PlayMusic(const std::string& name) {
    if (music_cache_.find(name) == music_cache_.end()) {
        std::cerr << "FEHLER: Musikstück '" << name << "' nicht gefunden!" << std::endl;
        return;
    }

    StopCurrentMusic(); // Stoppe erst die alte Musik

    current_music_ = &music_cache_.at(name);
    current_music_->looping = true;
    UpdateMusicVolume();
    PlayMusicStream(*current_music_);
}

void SoundManager::StopCurrentMusic() {
    if (current_music_ != nullptr) {
        StopMusicStream(*current_music_);
    }
    current_music_ = nullptr;
}

void SoundManager::UpdateMusicVolume() {
    if (current_music_) {
        float final_volume = game::AudioConfig::kMusicVolume * SettingsManager::GetInstance().GetMusicVolume() * SettingsManager::GetInstance().GetMasterVolume();
        SetAudioStreamVolume(current_music_->stream, final_volume);
    }
}

void SoundManager::Update() {
    if (current_music_ != nullptr) {
        UpdateMusicStream(*current_music_);
    }
    // Setze die Zähler für den nächsten Frame zurück
    sfx_play_counts_this_frame_.clear();
}

void SoundManager::UnloadAll() {
    for (auto const& [name, music] : music_cache_) {
        UnloadMusicStream(music);
    }
    music_cache_.clear();

    for (auto const& [name, sound] : sfx_cache_) {
        UnloadSound(sound);
    }
    sfx_cache_.clear();

    std::cout << "Alle Sounds und Musikstücke entladen." << std::endl;
}

void SoundManager::StopSfx(const std::string& name)
{
    // Prüfe, ob der Sound überhaupt im Cache existiert
    if (sfx_cache_.count(name))
    {
        // Stoppe alle laufenden Instanzen dieses Sounds
        StopSound(sfx_cache_.at(name));
    }
}

bool SoundManager::IsMusicPlaying(const std::string& name)
{
    auto it = music_cache_.find(name);
    if (it == music_cache_.end()) return false;
    return IsMusicStreamPlaying(it->second);
}