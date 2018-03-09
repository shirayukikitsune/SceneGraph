#version 400 core

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_UV;
 
// We output vertex_Color to the next shader in chain. In this example, it is the fragment shader.
out vec4 vertex_Color;
 
// Values that stay constant for the whole mesh.
uniform mat4 MVP;
  
void main(){
    gl_Position = MVP * vec4(in_Position, 1);

    // Pass the color on to the fragment shader
    vertex_Color.rgb = in_Position + vec3(0.5f);
    vertex_Color.a = 1.0;
}
