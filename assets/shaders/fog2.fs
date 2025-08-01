/**********************************************************************************
*
* Fragment-Shader für einen selektiven "Kriegsnebel"-Effekt.
*
* - Nutzt eine "Nebel-Maske", um den Transparenz-Effekt nur auf
* bestimmte Objekte anzuwenden (die mit useFog=true).
* - Objekte ohne useFog werden nur farblich vom Nebel beeinflusst,
* bleiben aber voll sichtbar.
*
* Uniforms:
* - texture0:    Das gerenderte Originalbild der Spielszene.
* - fogMask:     Eine Schwarz-Weiß-Maske. Weiß = Objekt mit useFog=true.
* - playerPos:   Die Bildschirm-Koordinaten des Spielers.
* - resolution:  Die Auflösung des Spielfensters.
* - time:        Eine fortlaufende Zeitvariable.
* - fogStrength: Ein Multiplikator für die Dichte/Stärke des Nebels.
*
**********************************************************************************/

#version 330

// --- EINGÄNGE (Vom Vertex-Shader) ---
in vec2 fragTexCoord;

// --- UNIFORMS (Parameter von der CPU) ---
uniform sampler2D texture0;
uniform sampler2D fogMask;      // NEU: Die Maske für useFog-Objekte
uniform vec2 playerPos;
uniform vec2 resolution;
uniform float time;
uniform float fogStrength;

// --- AUSGANG ---
out vec4 finalColor;

// --- RAUSCH-FUNKTIONEN (bleiben unverändert) ---
float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

float noise(vec2 pos) {
    vec2 i = floor(pos);
    vec2 f = fract(pos);
    float a = rand(i);
    float b = rand(i + vec2(1.0, 0.0));
    float c = rand(i + vec2(0.0, 1.0));
    float d = rand(i + vec2(1.0, 1.0));
    vec2 u = f * f * (3.0 - 2.0 * f);
    return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

void main()
{
    // 1. Originalfarbe der Szene und Masken-Wert holen
    vec4 originalColor = texture(texture0, fragTexCoord);
    float maskValue = texture(fogMask, fragTexCoord).r; // Lese den Rot-Kanal der Maske (weiß=1.0, schwarz=0.0)

    if (originalColor.a < 0.1)
    {
        finalColor = originalColor;
        return;
    }

    // 2. Nebel-FARB-Berechnung (wird immer durchgeführt)
    vec2 pixelPos = vec2(gl_FragCoord.x, resolution.y - gl_FragCoord.y);
    float dist = length(pixelPos - playerPos);
    float fogFactor = smoothstep(90.0, 250.0, dist);
    float n = noise(pixelPos * 0.3 + vec2(time * 0.23, -time * 0.25));
    fogFactor *= 0.9 + 0.5 * n;
    fogFactor *= fogStrength;
    fogFactor = clamp(fogFactor, 0.0, 0.70);

    vec4 fogColor = vec4(0.6, 0.6, 0.6, 1.0);

    // Mische die Originalfarbe mit der Nebelfarbe
    vec4 colorWithFog = mix(originalColor, fogColor, fogFactor);

    // 3. NEU: Selektive Transparenz basierend auf der Maske
    float finalAlpha = originalColor.a; // Standardmäßig ist das Objekt voll sichtbar

    // Wenn der Maskenwert größer als 0 ist (d.h. der Pixel gehört zu einem useFog-Objekt)...
    if (maskValue > 0.0)
    {
        // ...berechne die Transparenz basierend auf der Entfernung.
        // Diese Werte kannst du anpassen, um den Fade-Effekt zu steuern.
        float startFadeRadius = 150.0;
        float endFadeRadius = 250.0;

        // `smoothstep` sorgt für einen weichen Übergang von 1.0 (sichtbar) zu 0.0 (unsichtbar)
        float distanceAlpha = 1.0 - smoothstep(startFadeRadius, endFadeRadius, dist);

        // Kombiniere die berechnete Transparenz mit der originalen Transparenz des Objekts.
        finalAlpha = originalColor.a * distanceAlpha;
    }

    // 4. Setze die endgültige Farbe zusammen
    // Die RGB-Werte kommen von der Farbmischung, der Alpha-Wert von unserer selektiven Berechnung.
    finalColor = vec4(colorWithFog.rgb, finalAlpha);
}