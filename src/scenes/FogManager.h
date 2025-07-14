#pragma once
#ifndef FOG_MANAGER_H
#define FOG_MANAGER_H

#include <raylib.h>
#include <string>
#include <vector>

class FogManager
{
public:
    // Öffentliche, steuerbare Parameter
    float innerRadius;
    float outerRadius;
    Color fogColor;
    bool fogEnabled;

    FogManager();
    ~FogManager();

    void InitializeFog(const std::string& mapName, Vector2 screenResolution);
    void Update(Vector2 playerPosition, float deltaTime);
    void BeginFogMode() const;
    void EndFogMode() const;
    void UnloadFog();

    // KORREKTUR: Diese alten Funktionen werden entfernt, da sie nicht mehr benötigt werden.
    // bool IsFogActive() const;
    // void SetFogStrength(float strength);
    // void SetFogMaps(const std::vector<std::string>& maps);

private:
    Shader fogShader;
    bool fogLoaded;
    bool fogActive; // Dieser wird intern verwendet
    float timeAccumulator;
    Vector2 resolution;

    int playerPosLocation;
    int resolutionLocation;
    int timeLocation;
    int innerRadiusLocation;
    int outerRadiusLocation;
    int fogColorLocation;

    void LoadFogShader();
    void UpdateShaderUniforms(Vector2 playerPos);
    bool ShouldUseFog(const std::string& mapName) const;
};

#endif // FOG_MANAGER_H