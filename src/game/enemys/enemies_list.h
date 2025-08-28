#pragma once

// Ein Enum um die verschiedenen Gegnertypen zu identifizieren.
namespace enemy {
    enum class EnemyType {
        INSECT_MONSTER,
        DROWNED_SNIPER,
        // ... hier weitere Typen hinzufügen
    };
}

// Diese Datei dient als Sammelstelle für alle Gegner-Header.

#include "InsectMonster.h"
#include "DrownedSniper.h"

// #include "NeuerGegner.h" // Hier kommen zukünftige Gegner hin