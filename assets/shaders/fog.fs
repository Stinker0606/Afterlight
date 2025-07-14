#version 330

// uniforms für die Position und Effekte
uniform vec2 playerPos;
uniform vec2 resolution;
uniform float time;

// Steuerbare uniforms für den Nebeleffekt
uniform float innerRadius;
uniform float outerRadius;
uniform vec4 fogColorValue; // Farbe und Deckkraft des Nebels

// Output-Farbe
out vec4 finalColor;

// Funktion zur Erzeugung von Zufallswerten (Rauschen)
float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

// Funktion zur Erzeugung von prozeduralem Rauschen
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
    // Position des aktuellen Pixels berechnen
    vec2 pixelPos = vec2(gl_FragCoord.x, resolution.y - gl_FragCoord.y);

    // Distanz des Pixels zum Spieler berechnen
    float dist = length(pixelPos - playerPos);

    // Deckkraft des Nebels basierend auf der Distanz berechnen (0.0 = klar, 1.0 = voller Nebel)
    float fogOpacity = smoothstep(innerRadius, outerRadius, dist);

    // Rauschen hinzufügen, um den Nebel organischer aussehen zu lassen
    float n = noise(pixelPos * 0.3 + vec2(time * 0.23, -time * 0.25));

    // *** HIER IST DIE ÄNDERUNG: Die Intensität des Rauschens wurde von 0.5 auf 0.1 reduziert ***
    fogOpacity *= 0.9 + 0.1 * n;

    // Die endgültige Farbe ist die Nebelfarbe. Ihre Transparenz wird durch die berechnete Deckkraft bestimmt.
    finalColor = vec4(fogColorValue.rgb, fogColorValue.a * fogOpacity);
}