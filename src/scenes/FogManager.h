#pragma once
#ifndef FOG_MANAGER_H
#define FOG_MANAGER_H

#include <raylib.h>
#include <string>
#include <vector>

class FogManager
{
public:
    // Steuerbare Parameter
    float innerRadius;
    float outerRadius;
    Color fogColor;
    float fogStrength; // Stärke als separater Wert
    bool fogEnabled;

    FogManager();
    ~FogManager();

    void InitializeFog(const std::string& mapName, Vector2 screenResolution);
    void Update(Vector2 playerPosition, float deltaTime);
    void BeginFogMode() const;
    void EndFogMode() const;
    void UnloadFog();

private:
    Shader fogShader;
    bool fogLoaded;
    float timeAccumulator;
    Vector2 resolution;

    // KORREKTUR: Fehlende Variablen wieder hinzugefügt
    bool fogActive;
    std::vector<std::string> fogMaps;

    // Shader uniform locations
    int playerPosLocation;
    int resolutionLocation;
    int timeLocation;
    int innerRadiusLocation;
    int outerRadiusLocation;
    int fogColorLocation;
    int fogStrengthLocation;

    void LoadFogShader();
    void UpdateShaderUniforms(Vector2 playerPos);
    bool ShouldUseFog(const std::string& mapName) const;
};

#endif // FOG_MANAGER_H