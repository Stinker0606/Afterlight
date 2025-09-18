#version 330

// Input from the vertex shader
in vec2 fragTexCoord;
in vec4 fragColor;

// The texture we are drawing (our game screen)
uniform sampler2D texture0;

// A time uniform to make the noise animate
uniform float time;

// A simple function to generate pseudo-random noise
float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

void main()
{
    // Get the original color of the pixel from the game's texture
    vec4 texColor = texture(texture0, fragTexCoord);

    // Generate a noise value based on pixel position and time
    // The multiplication factor (e.g., 0.15) controls the intensity of the noise
    float noise = (random(fragTexCoord + time * 0.01) - 0.5) * 0.15;

    // Add the noise to the original color
    vec3 noisyColor = texColor.rgb + vec3(noise);

    // Output the final, noisy color
    gl_FragColor = vec4(noisyColor, texColor.a);
}