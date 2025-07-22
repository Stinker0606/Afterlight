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