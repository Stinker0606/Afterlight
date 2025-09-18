/**********************************************************************************
*
* Fragment-Shader für einen dynamischen, volumetrischen "Kriegsnebel"-Effekt.
*
* - Erzeugt eine sich bewegende, wolkenartige Nebelwand über den ganzen Bildschirm.
* - Behält die pixelige Textur und die grundlegende Farbgebung bei.
* - "Schneidet" einen Sichtbereich um den Spieler in den Nebel, dessen
* Ränder weich und ebenfalls verrauscht sind.
*
**********************************************************************************/

#version 330

// --- EINGÄNGE ---
in vec2 fragTexCoord;
in vec4 fragColor;

// --- UNIFORMS ---
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

float fbm(vec2 p) {
    float value = 0.0;
    float amplitude = 0.5;
    float frequency = 2.0;

    for (int i = 0; i < 4; i++) {
        value += amplitude * noise(p * frequency);
        frequency *= 2.1;
        amplitude *= 0.45;
    }
    return value;
}


void main()
{
    // 1. Hole die Originalfarbe der Szene.
    vec4 originalColor = texture(texture0, fragTexCoord);
    if (originalColor.a < 0.1) {
        discard;
    }
    float maskValue = texture(fogMask, fragTexCoord).r;
    vec2 pixelPos = vec2(gl_FragCoord.x, resolution.y - gl_FragCoord.y);

    // 2. Globale Nebeldichte
    vec2 fogMotion = vec2(time * 0.02, time * 0.01);
    float fogNoise = fbm((pixelPos / resolution) * 3.5 + fogMotion);

    // 3. Sichtkreis des Spielers
    float dist = length(pixelPos - playerPos);

    // NEU: Animiertes Rauschen nur für den Rand des Sichtkreises
    float edgeNoise = noise(pixelPos * 0.015 + time * 0.2) * 15.0;
    // NEU: Wende das Rauschen auf den Sichtkreis an
    float visibilityCircle = 1.0 - smoothstep(100.0, 200.0, dist - edgeNoise);


    // 4. Kombiniere globalen Nebel und Sichtkreis
    float fogFactor = fogNoise * 0.6 + 0.4 - visibilityCircle;
    fogFactor = clamp(fogFactor * fogStrength, 0.0, 0.98);

    // 5. Definiere die Basis-Farbe des Nebels.
    vec4 fogColor = vec4(0.6, 0.6, 0.6, 1.0);

    // --- PIXEL-TEXTUR FÜR DEN NEBEL (bleibt erhalten) ---
    float pixelSize = 4.0;
    vec2 pixelGridPos = floor(pixelPos / pixelSize);
    vec2 pixelMovement = vec2(-1.0, 1.0) * time * 0.000005;
    float pixelNoise = rand(pixelGridPos + pixelMovement);
    fogColor.rgb -= pixelNoise * 0.06;

    // 6. Mische die Farben.
    vec4 finalOriginalColor = originalColor * fragColor;
    vec4 colorWithFog = mix(finalOriginalColor, fogColor, fogFactor);

    // 7. Selektive Transparenz (Fading) basierend auf der Maske.
    float finalAlpha = finalOriginalColor.a;
    if (maskValue > 0.0)
    {
        float startFadeRadius = 50.0;
        float endFadeRadius = 300.0;
        float distanceAlpha = 1.0 - smoothstep(startFadeRadius, endFadeRadius, dist);
        finalAlpha = finalOriginalColor.a * distanceAlpha;
    }

    // 8. Setze die endgültige Farbe zusammen.
    finalColor = vec4(colorWithFog.rgb, finalAlpha);
}