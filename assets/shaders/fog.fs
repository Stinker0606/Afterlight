#version 330

// --- UNIFORMS (Von C++ steuerbar) ---
uniform vec2 playerPos;
uniform vec2 resolution;
uniform float time;
uniform float innerRadius;
uniform float outerRadius;
uniform vec4 fogColorValue; // Enthält die Farbe (RGB) und die maximale Stärke (A)

// --- AUSGANG ---
out vec4 finalColor;

// --- RAUSCH-FUNKTIONEN (unverändert) ---
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
    // 1. Nebelfaktor berechnen (bestimmt die Form und Textur des Nebels)
    vec2 pixelPos = vec2(gl_FragCoord.x, resolution.y - gl_FragCoord.y);
    float dist = length(pixelPos - playerPos);
    float fogFactor = smoothstep(innerRadius, outerRadius, dist);

    // Rauschen für organische Kanten hinzufügen
    float n = noise(pixelPos * 0.3 + vec2(time * 0.23, -time * 0.25));
    fogFactor *= 0.9 + 0.1 * n; // Subtiles Rauschen, wie du es wolltest

    // 2. Endgültige Farbe für das Overlay berechnen
    // Die Farbe ist die Nebelfarbe aus der UI.
    // Die Transparenz ist das Produkt aus der maximalen Stärke (aus der UI) und dem berechneten fogFactor.
    // Das ist der entscheidende Punkt: Wir geben eine Farbe mit variabler Transparenz aus.
    finalColor = vec4(fogColorValue.rgb, fogColorValue.a * fogFactor);
}

