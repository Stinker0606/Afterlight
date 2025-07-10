#version 330

// INPUTS
in vec2 fragTexCoord;

// UNIFORMS
uniform sampler2D texture0;
uniform vec2 playerPos; // Erhält jetzt die korrekten Bildschirm-Koordinaten
uniform vec2 resolution;
uniform float time;
uniform float fogStrength;

// OUTPUT
out vec4 finalColor;

// Deine Rausch-Funktionen, um den pixeligen Look zu erzeugen
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
    // 1. Originalfarbe der Szene holen
    vec4 originalColor = texture(texture0, fragTexCoord);

    if (originalColor.a < 0.1)
    {
        // ...gib ihn einfach unverändert aus und beende die Bearbeitung.
        finalColor = originalColor;
        return;
    }

    // 2. Nebelberechnung mit Bildschirm-Koordinaten
    vec2 pixelPos = vec2(gl_FragCoord.x, resolution.y - gl_FragCoord.y);
    float dist = length(pixelPos - playerPos);

    // PASSE DIESE WERTE AN, UM DEN RADIUS ZU ÄNDERN
    // smoothstep(innerer_radius, äußerer_radius, distanz)
    // Der Nebel beginnt bei 80 Pixeln und ist bei 250 voll da.
    float fogFactor = smoothstep(90.0, 250.0, dist);

    // Dein Rausch-Effekt wird wieder hinzugefügt
    float n = noise(pixelPos * 0.3 + vec2(time * 0.23, -time * 0.25));
    fogFactor *= 0.9 + 0.5 * n;

    // Stärke und Begrenzung
    fogFactor *= fogStrength;
    fogFactor = clamp(fogFactor, 0.0, 0.70);

    // 3. Nebelfarbe definieren
    vec4 fogColor = vec4(0.6, 0.6, 0.6, 1.0); // Dunkle, undurchsichtige Nebelfarbe

    // 4. Szene und Nebel mischen, um den "durchsichtigen" Effekt zu erzielen
    finalColor = mix(originalColor, fogColor, fogFactor);
}