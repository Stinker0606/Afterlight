#include "NoiseManager.h"
#include <iostream>
#include <algorithm>
#include "raylib.h"

// --- Constructor ---
NoiseManager::NoiseManager()
    : noiseLoaded(false), noiseActive(false), timeAccumulator(0.0f), timeLocation(-1)
{
    // IMPORTANT: Define which maps will have the noise effect.
    noiseMaps = {"level_3_7.json"};
}

// --- Destructor ---
NoiseManager::~NoiseManager()
{
    UnloadNoise();
}

// --- Initialization ---
void NoiseManager::InitializeNoise(const std::string& mapName)
{
    noiseActive = ShouldUseNoise(mapName);

    if (noiseActive && !noiseLoaded)
    {
        LoadNoiseShader();
    }
    else if (!noiseActive && noiseLoaded)
    {
        UnloadNoise();
    }
}

// --- Update-Loop ---
void NoiseManager::Update(float deltaTime)
{
    if (!noiseActive || !noiseLoaded) return;

    // Update the time accumulator for the shader animation
    timeAccumulator += deltaTime;
    UpdateShaderUniforms();
}

// --- Draw-Functions ---
void NoiseManager::BeginNoiseMode() const
{
    if (noiseActive && noiseLoaded) {
        BeginShaderMode(noiseShader);
    }
}

void NoiseManager::EndNoiseMode() const
{
    if (noiseActive && noiseLoaded) {
        EndShaderMode();
    }
}

// --- Getters ---
bool NoiseManager::IsNoiseActive() const
{
    return noiseActive;
}

// --- Private Helper Functions ---
void NoiseManager::UnloadNoise()
{
    if (noiseLoaded)
    {
        UnloadShader(noiseShader);
        noiseLoaded = false;
        std::cout << "Noise shader unloaded." << std::endl;
    }
}

bool NoiseManager::ShouldUseNoise(const std::string& mapName) const
{
    // Check if the current map name is in our list of noise maps.
    return std::find(noiseMaps.begin(), noiseMaps.end(), mapName) != noiseMaps.end();
}

void NoiseManager::LoadNoiseShader()
{
    // The vertex shader can be null (0) to use the default one.
    noiseShader = LoadShader(0, "assets/shaders/noise.fs");

    if (noiseShader.id != 0)
    {
        // Get the "address" of the 'time' variable inside the shader code.
        timeLocation = GetShaderLocation(noiseShader, "time");
        noiseLoaded = true;
        std::cout << "Noise shader loaded successfully." << std::endl;
    }
    else
    {
        std::cerr << "ERROR: Could not load the noise shader." << std::endl;
        noiseActive = false;
    }
}

void NoiseManager::UpdateShaderUniforms()
{
    // Send the updated time value to the shader every frame.
    SetShaderValue(noiseShader, timeLocation, &timeAccumulator, SHADER_UNIFORM_FLOAT);
}