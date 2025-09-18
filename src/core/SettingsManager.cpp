#include "SettingsManager.h"
#include <fstream>
#include <iomanip>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Implementiere den privaten Konstruktor
SettingsManager::SettingsManager() {
    // Fülle die Liste der Auflösungen.
    available_resolutions_.push_back({1920, 1080});
    available_resolutions_.push_back({1600, 900});
    available_resolutions_.push_back({1280, 720});
    // Setze den Standard-Index auf die höchste Auflösung
    current_resolution_index_ = 0;
}

SettingsManager& SettingsManager::GetInstance()
{
    static SettingsManager instance;
    return instance;
}

void SettingsManager::LoadSettings()
{
    std::ifstream f("settings.json");
    if (f.good()) {
        json data = json::parse(f);
        display_mode_ = data.value("display_mode", DisplayMode::BORDERLESS);
        current_resolution_index_ = data.value("resolution_index", 0);
        master_volume_ = data.value("master_volume", 0.75f);
        music_volume_ = data.value("music_volume", 1.0f);
        sfx_volume_ = data.value("sfx_volume", 1.0f);
        vsync_enabled_ = data.value("vsync_enabled", true);
    }
}

void SettingsManager::SaveSettings()
{
    json data;
    data["display_mode"] = display_mode_;
    data["resolution_index"] = current_resolution_index_;
    data["master_volume"] = master_volume_;
    data["music_volume"] = music_volume_;
    data["sfx_volume"] = sfx_volume_;
    data["vsync_enabled"] = vsync_enabled_;

    std::ofstream o("settings.json");
    o << std::setw(4) << data << std::endl;
}

void SettingsManager::ApplyDisplaySettings()
{
    // V-Sync an- oder ausschalten
    if (vsync_enabled_) {
        if (!IsWindowState(FLAG_VSYNC_HINT)) SetWindowState(FLAG_VSYNC_HINT);
    } else {
        if (IsWindowState(FLAG_VSYNC_HINT)) ClearWindowState(FLAG_VSYNC_HINT);
    }

    Vector2 new_size = GetCurrentResolution();

    switch (display_mode_)
    {
        case DisplayMode::WINDOWED:
            if (IsWindowFullscreen()) ToggleFullscreen();
        ClearWindowState(FLAG_WINDOW_UNDECORATED);
        SetWindowSize(new_size.x, new_size.y);
        break;
        case DisplayMode::BORDERLESS:
            if (IsWindowFullscreen()) ToggleFullscreen();
        SetWindowState(FLAG_WINDOW_UNDECORATED);
        SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
        SetWindowPosition(0, 0);
        break;
        case DisplayMode::FULLSCREEN:
            if (!IsWindowFullscreen()) ToggleFullscreen();
        break;
    }
}

void SettingsManager::SetCurrentResolutionIndex(int index) {
    if (index >= 0 && index < available_resolutions_.size()) {
        current_resolution_index_ = index;
    }
}

Vector2 SettingsManager::GetCurrentResolution() const {
    if (current_resolution_index_ >= 0 && current_resolution_index_ < available_resolutions_.size()) {
        return available_resolutions_[current_resolution_index_];
    }
    return {1920, 1080}; // Fallback
}