/**********************************************************************************
*
* Fragment-Shader für einen selektiven "Kriegsnebel"-Effekt.
*
* - Behält die komplette Funktionalität des Original-Shaders bei.
* - Fügt der grauen Nebelfläche eine subtile, pixelige Textur hinzu.
* - Die Textur bewegt sich langsam und konstant von oben rechts nach unten links.
*
**********************************************************************************/

#version 330

// --- EINGÄNGE (Vom Vertex-Shader) ---
in vec2 fragTexCoord;
in vec4 fragColor; // <-- NEUE ZEILE: Empfängt die "tint"-Farbe aus C++

// --- UNIFORMS (Parameter von der CPU) ---
uniform sampler2D texture0;
uniform sampler2D fogMask;
uniform vec2 playerPos;
uniform vec2 resolution;
uniform float time;
uniform float fogStrength;

// --- AUSGANG ---
out vec4 finalColor;

// --- RAUSCH-FUNKTIONEN ---
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
    // 1. Hole die Originalfarbe der Szene aus der Textur.
    vec4 originalColor = texture(texture0, fragTexCoord);

    if (originalColor.a < 0.1) {
        discard;
    }

    float maskValue = texture(fogMask, fragTexCoord).r;

    // 2. Berechne die Position und Distanz für den Sichtkreis.
    vec2 pixelPos = vec2(gl_FragCoord.x, resolution.y - gl_FragCoord.y);
    float dist = length(pixelPos - playerPos);

    // 3. Berechne den Nebelfaktor mit weichem Kantenrauschen.
    float fogFactor = smoothstep(90.0, 250.0, dist);
    vec2 movementVector = vec2(time * 0.23, -time * 0.25);
    float n = noise(pixelPos * 0.3 + movementVector);
    fogFactor *= 0.9 + 0.5 * n;
    fogFactor = clamp(fogFactor * fogStrength, 0.0, 0.70);

    // 4. Definiere die Basis-Farbe des Nebels.
    vec4 fogColor = vec4(0.6, 0.6, 0.6, 1.0);

    // --- PIXEL-TEXTUR FÜR DEN NEBEL ---
    float pixelSize = 4.0;
    vec2 pixelGridPos = floor(pixelPos / pixelSize);
    vec2 pixelMovement = vec2(-1.0, 1.0) * time * 0.000005;
    float pixelNoise = rand(pixelGridPos + pixelMovement);
    fogColor.rgb -= pixelNoise * 0.06;

    // 5. Mische die Originalfarbe mit der (jetzt texturierten) Nebelfarbe.
    // WICHTIG: Multipliziere die Originalfarbe mit der Tint-Farbe.
    vec4 finalOriginalColor = originalColor * fragColor;
    vec4 colorWithFog = mix(finalOriginalColor, fogColor, fogFactor);

    // 6. Selektive Transparenz (Fading) basierend auf der Maske.
    // HIER IST DIE ÄNDERUNG:
    // Wir nehmen die Transparenz aus der multiplizierten Farbe,
    // nicht nur aus dem Original.
    float finalAlpha = finalOriginalColor.a;
    if (maskValue > 0.0)
    {
        float startFadeRadius = 50.0;
        float endFadeRadius = 300.0;
        float distanceAlpha = 1.0 - smoothstep(startFadeRadius, endFadeRadius, dist);
        finalAlpha = finalOriginalColor.a * distanceAlpha;
    }

    // 7. Setze die endgültige Farbe zusammen.
    finalColor = vec4(colorWithFog.rgb, finalAlpha);
}