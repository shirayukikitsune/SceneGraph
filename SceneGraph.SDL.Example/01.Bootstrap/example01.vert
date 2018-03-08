#version 400 core

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_UV;
 
// We output vertex_Color to the next shader in chain. In this example, it is the fragment shader.
out vec4 vertex_Color;

void main(void) {
    // No transformations here...
    gl_Position = vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
 
    // Pass the color on to the fragment shader
    vertex_Color.rgb = (in_Position + vec3(1.0f)) * 0.5f;
    vertex_Color.a = 1.0;
}
