#pragma once
#include <string>
#include <map>
#include <raylib.h>

class SoundManager {
public:
    // Gibt die einzige Instanz des Managers zurück.
    static SoundManager& GetInstance();

    // Lädt alle Sounds und Musikstücke aus der Config. Wird einmal beim Spielstart aufgerufen.
    void Init();

    // Spielt ein geladenes Musikstück in einer Endlosschleife ab.
    void PlayMusic(const std::string& name);

    // Stoppt den aktuellen Sound.
    void StopCurrentMusic();
    void StopSfx(const std::string& name);

    // Methode für Soundeffekte
    void PlaySfx(const std::string& name, int max_instances = 3); // MAD FRAGEN

    // Muss jeden Frame aufgerufen werden, um die Musik-Streams zu aktualisieren.
    void Update();

    // Entlädt alle geladenen Sounds und Musikstücke.
    void UnloadAll();

    void UpdateMusicVolume();

    // Verhindere das Kopieren und Zuweisen.
    SoundManager(SoundManager const&) = delete;
    void operator=(SoundManager const&) = delete;

    //Getter um zu überprüfen ob Musik spielt
    bool IsMusicPlaying(const std::string& name);

private:
    SoundManager() = default;
    ~SoundManager() = default;

    // Cache für Musik
    std::map<std::string, Music> music_cache_;

    Music* current_music_ = nullptr;

    // Cache für Soundeffekte
    std::map<std::string, Sound> sfx_cache_;

    // Zählt, wie oft ein Sound in diesem Frame schon gespielt wurde
    std::map<std::string, int> sfx_play_counts_this_frame_;

};