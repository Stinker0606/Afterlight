#pragma once
#include <string>
#include <vector>
#include "raylib.h"

// Ein Enum für die verschiedenen Anzeigemodi
enum class DisplayMode { WINDOWED, BORDERLESS, FULLSCREEN };

class SettingsManager
{
public:
    static SettingsManager& GetInstance();

    // Lösche Kopier-Konstruktor und Zuweisungsoperator
    SettingsManager(SettingsManager const&) = delete;
    void operator=(SettingsManager const&) = delete;

    void LoadSettings();
    void SaveSettings();
    void ApplyDisplaySettings(); // Wendet die Grafikeinstellungen an

    // Getter & Setter
    DisplayMode GetDisplayMode() const { return display_mode_; }
    void SetDisplayMode(DisplayMode mode) { display_mode_ = mode; }

    // Auflösungs-Management
    int GetCurrentResolutionIndex() const { return current_resolution_index_; }
    void SetCurrentResolutionIndex(int index);
    Vector2 GetCurrentResolution() const;
    const std::vector<Vector2>& GetAvailableResolutions() const { return available_resolutions_; }


    float GetMasterVolume() const { return master_volume_; }
    void SetMasterVolume(float vol) { master_volume_ = vol; }

    float GetMusicVolume() const { return music_volume_; }
    void SetMusicVolume(float vol) { music_volume_ = vol; }

    float GetSfxVolume() const { return sfx_volume_; }
    void SetSfxVolume(float vol) { sfx_volume_ = vol; }

    bool IsVsyncEnabled() const { return vsync_enabled_; }
    void SetVsync(bool enabled) { vsync_enabled_ = enabled; }

private:
    SettingsManager();
    ~SettingsManager() = default; // Privater Destruktor

    // Standardwerte
    DisplayMode display_mode_ = DisplayMode::WINDOWED;
    float master_volume_ = 0.75f;
    float music_volume_ = 1.0f;
    float sfx_volume_ = 1.0f;
    bool vsync_enabled_ = true;

    // Liste der verfügbaren Auflösungen
    int current_resolution_index_ = 0;
    std::vector<Vector2> available_resolutions_;
};