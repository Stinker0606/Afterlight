#pragma once
#include <map>
#include <string>
#include "raylib.h"

// Eine Klasse, die das Laden und Speichern von Texturen verwaltet.
class AssetManager
{
public:
    // Gibt die einzige globale Instanz des Managers zurück.
    static AssetManager& GetInstance();

    // Lädt eine Textur aus einer Datei. Wenn die Textur bereits geladen wurde,
    // wird sie direkt aus dem Cache zurückgegeben, um doppeltes Laden zu vermeiden.
    Texture2D Load(const std::string& path);

    // Entlädt alle Texturen, die vom Manager geladen wurden.
    // Sollte am Ende des Spiels aufgerufen werden.
    void UnloadAll();

    // Verhindere das Kopieren und Zuweisen, um das Singleton-Muster sicherzustellen.
    AssetManager(AssetManager const&) = delete;
    void operator=(AssetManager const&) = delete;

private:
    AssetManager() = default; // Privater Konstruktor
    ~AssetManager() = default; // Privater Destruktor

    // Das "Lager" (Cache) für alle bereits geladenen Texturen.
    std::map<std::string, Texture2D> texture_cache_;
};