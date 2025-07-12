/**********************************************************************************
*
* Fragment-Shader für einen kreisförmigen "Kriegsnebel"-Effekt (War of Fog).
*
* - Erzeugt einen weichen, kreisförmigen, klaren Bereich um eine Spielerposition.
* - Füllt den Rest des Bildschirms mit einem statischen, grauen Nebel.
* - Nutzt einen prozeduralen Rausch-Algorithmus, um dem Nebelrand eine
* organische, "pixelige" und animierte Textur zu verleihen.
* - Die Stärke des Nebeleffekts kann von außen gesteuert werden.
*
* Uniforms:
* - texture0:    Das gerenderte Originalbild der Spielszene.
* - playerPos:   Die Bildschirm-Koordinaten des Spielers (Mittelpunkt des klaren Bereichs).
* - resolution:  Die Auflösung des Spielfensters (Breite und Höhe).
* - time:        Eine fortlaufende Zeitvariable, um das Rauschen zu animieren.
* - fogStrength: Ein Multiplikator, um die Dichte/Stärke des Nebels zu steuern.
*
**********************************************************************************/

#version 330

// --- EINGÄNGE (Vom Vertex-Shader) ---
in vec2 fragTexCoord; // Die Texturkoordinaten für den aktuellen Pixel.

// --- UNIFORMS (Parameter von der CPU) ---
uniform sampler2D texture0;    // Der Framebuffer mit der gerenderten Szene.
uniform vec2 playerPos;        // Die Position des Spielers in Bildschirm-Koordinaten.
uniform vec2 resolution;       // Die Auflösung des Bildschirms.
uniform float time;            // Laufende Zeit für Animationen.
uniform float fogStrength;     // Globale Stärke des Nebeleffekts.

// --- AUSGANG ---
out vec4 finalColor; // Die endgültige Farbe des Pixels nach der Nebelberechnung.


// --- RAUSCH-FUNKTIONEN ---
// Diese Funktionen erzeugen eine prozedurale, "zufällige" Textur.
// Sie wird verwendet, um dem Nebelrand ein ungleichmäßiges Aussehen zu verleihen.

// Erzeugt einen pseudo-zufälligen Wert (Hash) basierend auf einer 2D-Koordinate.
float rand(vec2 co) {
    // Eine mathematische Formel, die für einen gegebenen Input immer denselben,
    // aber scheinbar zufälligen, Output zwischen 0.0 und 1.0 erzeugt.
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

// Erzeugt ein weiches "Value Noise" durch Interpolation der zufälligen Werte.
// Dies verhindert harte Kanten und erzeugt ein wolkigeres Muster.
float noise(vec2 pos) {
    vec2 i = floor(pos); // Ganzzahliger Teil der Position (Gitterzelle)
    vec2 f = fract(pos); // Nachkommaanteil (Position innerhalb der Zelle)

    // Hole Zufallswerte für die vier Ecken der Gitterzelle
    float a = rand(i);
    float b = rand(i + vec2(1.0, 0.0));
    float c = rand(i + vec2(0.0, 1.0));
    float d = rand(i + vec2(1.0, 1.0));

    // Erzeuge weiche Übergänge mit einer Smoothstep-Kurve
    vec2 u = f * f * (3.0 - 2.0 * f);

    // Interpoliere (mische) die Werte der Eckpunkte, um den finalen Rauschwert zu erhalten.
    return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}


void main()
{
    // 1. Originalfarbe der Szene aus der Textur holen
    // Liest die Farbe des aktuellen Pixels aus dem gerenderten Spielbild.
    vec4 originalColor = texture(texture0, fragTexCoord);

    // Optimierung: Wenn ein Pixel fast vollständig durchsichtig ist (z.B. außerhalb der gerenderten Welt),
    // wird er ignoriert, um unnötige Berechnungen für den Nebeleffekt zu sparen.
    if (originalColor.a < 0.1)
    {
        // Gib die transparente Farbe direkt aus und beende den Shader für diesen Pixel.
        finalColor = originalColor;
        return;
    }

    // 2. Nebelberechnung basierend auf der Distanz zum Spieler
    // Ermittle die exakte Bildschirm-Koordinate des aktuellen Pixels.
    // Die Y-Koordinate wird invertiert (resolution.y - ...), da gl_FragCoord den Ursprung unten links hat.
    vec2 pixelPos = vec2(gl_FragCoord.x, resolution.y - gl_FragCoord.y);

    // Berechne die euklidische Distanz zwischen dem Pixel und der Spielerposition.
    float dist = length(pixelPos - playerPos);

    // Erzeuge einen weichen Übergang von klar zu neblig mit smoothstep.
    // - Unter 90 Pixeln Distanz ist der Nebelfaktor 0.0 (komplett klar).
    // - Über 250 Pixeln Distanz ist der Nebelfaktor 1.0 (potenziell voller Nebel).
    // - Dazwischen wird der Wert weich interpoliert.
    float fogFactor = smoothstep(90.0, 250.0, dist);

    // Füge den animierten Rausch-Effekt hinzu, um die Kanten aufzubrechen.
    // Die Position wird skaliert und die Zeit addiert, um eine langsam wogende Bewegung zu erzeugen.
    float n = noise(pixelPos * 0.3 + vec2(time * 0.23, -time * 0.25));
    // Das Ergebnis des Rauschens modifiziert den Nebelfaktor.
    fogFactor *= 0.9 + 0.5 * n;

    // Wende die globale Nebelstärke an und begrenze das Ergebnis.
    fogFactor *= fogStrength;
    // `clamp` stellt sicher, dass der Nebel nie stärker als 70% wird,
    // um zu verhindern, dass die Szene komplett verdeckt wird.
    fogFactor = clamp(fogFactor, 0.0, 0.70);

    // 3. Definiere die Farbe des Nebels
    // Ein mittleres, neutrales Grau mit voller Deckkraft.
    vec4 fogColor = vec4(0.6, 0.6, 0.6, 1.0);

    // 4. Mische die Originalfarbe mit der Nebelfarbe
    // `mix` interpoliert linear zwischen der Originalfarbe und der Nebelfarbe.
    // Der `fogFactor` bestimmt das Mischverhältnis:
    // - fogFactor = 0.0 -> 100% originalColor
    // - fogFactor = 0.7 -> 30% originalColor und 70% fogColor
    finalColor = mix(originalColor, fogColor, fogFactor);
}