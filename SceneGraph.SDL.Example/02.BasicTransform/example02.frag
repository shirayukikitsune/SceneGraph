#version 400 core

// This comes from the vertex shader
in vec4 vertex_Color;

// The final color of the fragment
out vec4 frag_color;

void main(void) {
    // Just output the input color...
    frag_color = vertex_Color;
}
