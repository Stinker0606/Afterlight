#pragma once
#ifndef NOISE_MANAGER_H
#define NOISE_MANAGER_H

#include <raylib.h>
#include <string>
#include <vector>

class NoiseManager
{
public:
    NoiseManager();
    ~NoiseManager();

    // Initializes the noise shader for a specific map
    void InitializeNoise(const std::string& mapName);

    // Updates shader uniforms (like time)
    void Update(float deltaTime);

    // Wrappers for Begin/End shader mode
    void BeginNoiseMode() const;
    void EndNoiseMode() const;

    // Checks if noise should be active for the current map
    bool IsNoiseActive() const;

    // Cleans up the shader resource
    void UnloadNoise();

private:
    Shader noiseShader;
    bool noiseLoaded;
    bool noiseActive;
    float timeAccumulator;

    // Shader uniform locations
    int timeLocation;

    // List of maps that should use the noise effect
    std::vector<std::string> noiseMaps;

    // Helper methods
    bool ShouldUseNoise(const std::string& mapName) const;
    void LoadNoiseShader();
    void UpdateShaderUniforms();
};

#endif // NOISE_MANAGER_H