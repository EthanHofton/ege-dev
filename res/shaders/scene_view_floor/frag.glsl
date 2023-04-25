#version 410 core

in vec2 vUv;
out vec4 fragColor;

uniform sampler2D floorTex;

void main()
{
    // fragColor = texture(floorTex, vUv);

    vec2 uv = vUv;
    float size = 1.0/8.0;   // size of the tile
    float edge = size/32.0; // size of the edge
    vec4 face_tone = vec4(0.0, 0.0, 0.0, 0.0); // 0.9 for the face of the tile
    vec4 edge_tone = vec4(0.5, 0.5, 0.5, 0.3); // 0.5 for the edge
    uv =  mod(uv, size);
    fragColor = (uv.x< edge || uv.y< edge) ? edge_tone : face_tone;
}
