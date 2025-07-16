//
// Created by Manza on 7/7/2025.
//

#include "FogManager.h"
#include <iostream>
#include <algorithm>

FogManager::FogManager()
    : fogLoaded(false), fogActive(false), fogStrength(1.0f), timeAccumulator(0.0f),
      resolution({0, 0}), playerPosLocation(-1), resolutionLocation(-1),
      timeLocation(-1), fogStrengthLocation(-1)
{
    // Default fog maps
    fogMaps = {"Swamp_1", "Swamp_0.json"};
}

FogManager::~FogManager()
{
    UnloadFog();
}

void FogManager::InitializeFog(const std::string& mapName, Vector2 screenResolution)
{
    resolution = screenResolution;
    fogActive = ShouldUseFog(mapName);

    if (fogActive && !fogLoaded)
    {
        LoadFogShader();
    }
    else if (!fogActive && fogLoaded)
    {
        UnloadFog();
    }
}

void FogManager::LoadFogShader()
{
    fogShader = LoadShader(0, "assets/shaders/fog.fs");

    if (fogShader.id != 0)
    {
        // Get uniform locations
        playerPosLocation = GetShaderLocation(fogShader, "playerPos");
        resolutionLocation = GetShaderLocation(fogShader, "resolution");
        timeLocation = GetShaderLocation(fogShader, "time");
        fogStrengthLocation = GetShaderLocation(fogShader, "fogStrength");

        // Set static uniforms
        SetShaderValue(fogShader, resolutionLocation, &resolution, SHADER_UNIFORM_VEC2);
        SetShaderValue(fogShader, fogStrengthLocation, &fogStrength, SHADER_UNIFORM_FLOAT);

        fogLoaded = true;
        std::cout << "Fog shader loaded successfully" << std::endl;
    }
    else
    {
        std::cerr << "Failed to load fog shader" << std::endl;
        fogActive = false;
    }
}

void FogManager::Update(Vector2 playerPosition, float deltaTime)
{
    if (!fogActive || !fogLoaded) return;

    timeAccumulator += deltaTime;
    UpdateShaderUniforms(playerPosition);
}

void FogManager::UpdateShaderUniforms(Vector2 playerPos)
{
    if (!fogLoaded) return;

    // Update dynamic uniforms
    SetShaderValue(fogShader, playerPosLocation, &playerPos, SHADER_UNIFORM_VEC2);
    SetShaderValue(fogShader, timeLocation, &timeAccumulator, SHADER_UNIFORM_FLOAT);
}

void FogManager::BeginFogMode() const
{
    if (fogActive && fogLoaded)
    {
        BeginShaderMode(fogShader);
    }
}

void FogManager::EndFogMode() const
{
    if (fogActive && fogLoaded)
    {
        EndShaderMode();
    }
}

bool FogManager::IsFogActive() const
{
    return fogActive && fogLoaded;
}

void FogManager::SetFogStrength(float strength)
{
    fogStrength = strength;
    if (fogLoaded)
    {
        SetShaderValue(fogShader, fogStrengthLocation, &fogStrength, SHADER_UNIFORM_FLOAT);
    }
}

void FogManager::SetFogMaps(const std::vector<std::string>& maps)
{
    fogMaps = maps;
}

bool FogManager::ShouldUseFog(const std::string& mapName) const
{
    return std::find_if(fogMaps.begin(), fogMaps.end(),
                       [&mapName](const std::string& fogMap) {
                           return mapName.find(fogMap) != std::string::npos;
                       }) != fogMaps.end();
}

void FogManager::UnloadFog()
{
    if (fogLoaded)
    {
        UnloadShader(fogShader);
        fogLoaded = false;
        fogActive = false;
        std::cout << "Fog shader unloaded" << std::endl;
    }
}