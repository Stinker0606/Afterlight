//
// Created by Manza on 7/7/2025.
//

#include "FogManager.h"
#include <iostream>
#include <algorithm>
#include "../config.h.in"

// --- Konstruktor ---
FogManager::FogManager()
    : fogLoaded(false), fogActive(false), fogStrength(1.0f), timeAccumulator(0.0f),
      resolution({0, 0}), playerPosLocation(-1), resolutionLocation(-1),
      timeLocation(-1), fogStrengthLocation(-1), fogMaskLocation(-1)
{
    // Definiert auf welchen Maps der Nebel standardmäßig aktiv sein soll.
    fogMaps = {"Swamp_0.json", "Swamp_1.json", "Vorhof_1_1.json","Übergang_1_1.json","Vorhof_1_2.json",}; // LEVEL NAMEN
}

// --- Destruktor ---
FogManager::~FogManager()
{
    // Stellt sicher dass der Shader beim Beenden entladen wird.
    UnloadFog();
}

// --- Initialisierung ---
void FogManager::InitializeFog(const std::string& mapName, Vector2 screenResolution)
{
    resolution = screenResolution;
    // Prüfe ob die aktuelle Map in unserer Liste der Nebel-Maps ist.
    fogActive = ShouldUseFog(mapName);

    if (fogActive && !fogLoaded)
    {
        LoadFogShader(); // Lade den Shader wenn er gebraucht wird und noch nicht geladen ist.
    }
    else if (!fogActive && fogLoaded)
    {
        UnloadFog(); // Entlade den Shader wenn er nicht mehr gebraucht wird.
    }
}

// --- Update-Schleife ---
void FogManager::Update(Vector2 playerPosition, float deltaTime)
{
    if (!fogActive || !fogLoaded) return; // Tue nichts wenn der Nebel nicht aktiv ist.

    // Aktualisiere den Zeit-Akkumulator für den Shader-Effekt.
    timeAccumulator += deltaTime;

    // Wende den Y-Offset aus der Config an, um den sichtbaren Kreis zu verschieben.
    playerPosition.y += game::Config::kFogPlayerCenterOffsetY;

    // Sende die aktualisierten Werte (Spielerposition, Zeit, usw.) an den Shader.
    UpdateShaderUniforms(playerPosition);
}

// --- Draw-Funktionen ---
void FogManager::BeginFogMode() const
{
    if (fogActive && fogLoaded) {
        BeginShaderMode(fogShader);
    }
}

void FogManager::EndFogMode() const
{
    if (fogActive && fogLoaded) {
        EndShaderMode();
    }
}

// --- Getter und Setter ---
bool FogManager::IsFogActive() const
{
    return fogActive;
}

void FogManager::SetFogStrength(float strength)
{
    fogStrength = strength;
    if (fogLoaded) {
        // Sende den neuen Stärke-Wert sofort an den Shader.
        SetShaderValue(fogShader, fogStrengthLocation, &fogStrength, SHADER_UNIFORM_FLOAT);
    }
}

void FogManager::SetFogMaps(const std::vector<std::string>& maps)
{
    fogMaps = maps;
}

// --- Private Hilfsfunktionen ---

void FogManager::UnloadFog()
{
    if (fogLoaded)
    {
        UnloadShader(fogShader);
        fogLoaded = false;
        std::cout << "Nebel-Shader wurde entladen." << std::endl;
    }
}

bool FogManager::ShouldUseFog(const std::string& mapName) const
{
    // Durchsuche den Vektor `fogMaps` um zu sehen ob der aktuelle mapName darin enthalten ist.
    return std::find(fogMaps.begin(), fogMaps.end(), mapName) != fogMaps.end();
}

void FogManager::LoadFogShader()
{
    if (game::Config::kDebugShowFog)
    {
        fogShader = LoadShader(0, "assets/shaders/fog.fs");
    }

    if (fogShader.id != 0)
    {
        // Hole die "Adressen" der Variablen im Shader-Code.
        playerPosLocation = GetShaderLocation(fogShader, "playerPos");
        resolutionLocation = GetShaderLocation(fogShader, "resolution");
        timeLocation = GetShaderLocation(fogShader, "time");
        fogStrengthLocation = GetShaderLocation(fogShader, "fogStrength");
        fogMaskLocation = GetShaderLocation(fogShader, "fogMask");

        // Setze die Werte die sich nicht ständig ändern.
        SetShaderValue(fogShader, resolutionLocation, &resolution, SHADER_UNIFORM_VEC2);
        SetShaderValue(fogShader, fogStrengthLocation, &fogStrength, SHADER_UNIFORM_FLOAT);

        fogLoaded = true;
        std::cout << "Nebel-Shader erfolgreich geladen." << std::endl;
    }
    else
    {
        std::cerr << "FEHLER: Konnte den Nebel-Shader nicht laden." << std::endl;
        fogActive = false;
    }
}

void FogManager::UpdateShaderUniforms(Vector2 playerPos)
{
    // Sende die Werte die sich jeden Frame ändern an den Shader.
    SetShaderValue(fogShader, playerPosLocation, &playerPos, SHADER_UNIFORM_VEC2);
    SetShaderValue(fogShader, timeLocation, &timeAccumulator, SHADER_UNIFORM_FLOAT);
}

void FogManager::SetFogMaskTexture(RenderTexture2D maskTexture)
{
    if (fogLoaded && fogMaskLocation != -1) {
        // Übergibt die Textur direkt an den Shader-Slot für "fogMask"
        SetShaderValueTexture(fogShader, fogMaskLocation, maskTexture.texture);
    }
}