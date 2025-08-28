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

    // Stoppt die aktuell spielende Musik.
    void StopCurrentMusic();

    // Muss jeden Frame aufgerufen werden, um die Musik-Streams zu aktualisieren.
    void Update();

    // Entlädt alle geladenen Sounds und Musikstücke.
    void UnloadAll();

    // Verhindere das Kopieren und Zuweisen.
    SoundManager(SoundManager const&) = delete;
    void operator=(SoundManager const&) = delete;

private:
    SoundManager() = default;
    ~SoundManager() = default;

    std::map<std::string, Music> music_cache_;
    Music* current_music_ = nullptr;
};