#version 330

// INPUTS
in vec2 fragTexCoord;

// UNIFORMS
uniform sampler2D texture0;
uniform vec2 playerPos;
uniform vec2 resolution;
uniform float time;

// NEUE STEUERBARE UNIFORMS
uniform float innerRadius;
uniform float outerRadius;
uniform vec4 fogColorValue; // Neuer Name, um Konflikte zu vermeiden

// OUTPUT
out vec4 finalColor;

// Rausch-Funktionen (unverändert)
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
    vec4 originalColor = texture(texture0, fragTexCoord);

    if (originalColor.a < 0.1) {
        finalColor = originalColor;
        return;
    }

    vec2 pixelPos = vec2(gl_FragCoord.x, resolution.y - gl_FragCoord.y);
    float dist = length(pixelPos - playerPos);

    // Verwendet jetzt die neuen, steuerbaren Radius-Werte
    float fogFactor = smoothstep(innerRadius, outerRadius, dist);

    float n = noise(pixelPos * 0.3 + vec2(time * 0.23, -time * 0.25));
    fogFactor *= 0.9 + 0.5 * n;

    // Der Alpha-Wert der Farbe steuert jetzt die Stärke
    fogFactor *= fogColorValue.a;
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    // Mischt mit der neuen, steuerbaren Nebelfarbe
    finalColor = mix(originalColor, fogColorValue, fogFactor);
}