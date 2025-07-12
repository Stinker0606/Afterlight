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

private:
    Shader fogShader;
    bool fogLoaded;
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
};

#endif // FOG_MANAGER_H