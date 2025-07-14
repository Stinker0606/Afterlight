#include "FogManager.h"
#include <iostream>
#include <algorithm>
#include <raymath.h>

FogManager::FogManager()
    : fogLoaded(false), fogActive(true), fogEnabled(true), timeAccumulator(0.0f)
{
    // Dein gewünschtes Standard-Setup
    innerRadius = 90.0f;
    outerRadius = 250.0f;
    fogColor = { 153, 153, 153, 190 }; // Dein ursprüngliches Grau
}

FogManager::~FogManager() {
    UnloadFog();
}

void FogManager::InitializeFog(const std::string& mapName, Vector2 screenResolution) {
    resolution = screenResolution;
    fogActive = ShouldUseFog(mapName);

    if (fogActive && !fogLoaded) {
        LoadFogShader();
    }
    else if (!fogActive && fogLoaded) {
        UnloadFog();
    }
}

void FogManager::LoadFogShader() {
    fogShader = LoadShader(0, "assets/shaders/fog.fs");
    if (fogShader.id != 0) {
        playerPosLocation = GetShaderLocation(fogShader, "playerPos");
        resolutionLocation = GetShaderLocation(fogShader, "resolution");
        timeLocation = GetShaderLocation(fogShader, "time");
        innerRadiusLocation = GetShaderLocation(fogShader, "innerRadius");
        outerRadiusLocation = GetShaderLocation(fogShader, "outerRadius");
        fogColorLocation = GetShaderLocation(fogShader, "fogColorValue");

        SetShaderValue(fogShader, resolutionLocation, &resolution, SHADER_UNIFORM_VEC2);
        fogLoaded = true;
    }
}

void FogManager::Update(Vector2 playerPosition, float deltaTime) {
    if (!fogActive || !fogLoaded) return;
    timeAccumulator += deltaTime;
    UpdateShaderUniforms(playerPosition);
}

void FogManager::UpdateShaderUniforms(Vector2 playerPos) {
    if (!fogLoaded) return;
    SetShaderValue(fogShader, playerPosLocation, &playerPos, SHADER_UNIFORM_VEC2);
    SetShaderValue(fogShader, timeLocation, &timeAccumulator, SHADER_UNIFORM_FLOAT);
    SetShaderValue(fogShader, innerRadiusLocation, &innerRadius, SHADER_UNIFORM_FLOAT);
    SetShaderValue(fogShader, outerRadiusLocation, &outerRadius, SHADER_UNIFORM_FLOAT);
    Vector4 colorNormalized = { (float)fogColor.r/255.0f, (float)fogColor.g/255.0f, (float)fogColor.b/255.0f, (float)fogColor.a/255.0f };
    SetShaderValue(fogShader, fogColorLocation, &colorNormalized, SHADER_UNIFORM_VEC4);
}

void FogManager::BeginFogMode() const {
    if (fogActive && fogEnabled) BeginShaderMode(fogShader);
}
void FogManager::EndFogMode() const {
    if (fogActive && fogEnabled) EndShaderMode();
}

void FogManager::UnloadFog() {
    if (fogLoaded) {
        UnloadShader(fogShader);
        fogLoaded = false;
        fogActive = false;
    }
}

bool FogManager::ShouldUseFog(const std::string& mapName) const
{
    // Diese Funktion kann leer bleiben oder deine Logik enthalten,
    // um zu entscheiden, auf welchen Karten Nebel aktiv sein soll.
    // Fürs Erste ist er immer aktiv.
    return true;
}

// KORREKTUR: Die leeren, fehlerhaften Funktionskörper werden hier entfernt.
// bool FogManager::IsFogActive() const { ... }
// void FogManager::SetFogStrength(float strength) { ... }
// void FogManager::SetFogMaps(const std::vector<std::string>& maps) { ... }