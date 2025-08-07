//
// Created by Manza on 7/7/2025.
//

#pragma once
#ifndef FOG_MANAGER_H
#define FOG_MANAGER_H

#include <raylib.h>
#include <string>
#include <vector>

class FogManager
{
public:
    FogManager();
    ~FogManager();

    // Initialize fog for a specific map
    void InitializeFog(const std::string& mapName, Vector2 screenResolution);

    // Update fog parameters (call every frame)
    void Update(Vector2 playerPosition, float deltaTime);

    // Begin/End fog rendering
    void BeginFogMode() const;
    void EndFogMode() const;

    // Check if fog should be active for current map
    bool IsFogActive() const;

    // Setters for fog parameters
    void SetFogStrength(float strength);
    void SetFogMaps(const std::vector<std::string>& maps);

    // Cleanup
    void UnloadFog();

    // Eine Getter-Methode für den Shader.
    Shader GetShader() const { return fogShader; }

private:
    Shader fogShader;
    bool fogLoaded;
    bool fogActive;
    float fogStrength;
    float timeAccumulator;
    Vector2 resolution;

    // Shader uniform locations
    int playerPosLocation;
    int resolutionLocation;
    int timeLocation;
    int fogStrengthLocation;

    // Maps that should use fog
    std::vector<std::string> fogMaps;

    // Helper methods
    bool ShouldUseFog(const std::string& mapName) const;
    void LoadFogShader();
    void UpdateShaderUniforms(Vector2 playerPos);
};

#endif // FOG_MANAGER_H