#pragma once

// Ein Enum um die verschiedenen Gegnertypen zu identifizieren.
namespace enemy {
    enum class EnemyType {
        INSECT_MONSTER,
        POISON_FROG, // Beispiel für zukünftigen Gegner
        // ... hier weitere Typen hinzufügen
    };
}

// Diese Datei dient als Sammelstelle für alle Gegner-Header.

#include "InsectMonster.h"

// #include "NeuerGegner.h" // Hier kommen zukünftige Gegner hin