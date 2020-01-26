#version 400 core

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_UV;

out vec3 fN;
out vec3 fE;
out vec3 fL;
out vec2 fTexCoord;

uniform mat4 modelView;
uniform mat3 normal;
uniform mat4 projection;
uniform vec3 lightDirection;

void main(){
    vec4 eyePosition = modelView * vec4(in_Position, 1.0);

    fN = normalize(normal * in_Normal);
    fL = normalize(lightDirection - eyePosition.xyz);
    fE = normalize(-eyePosition.xyz);
    fTexCoord = in_UV;

    gl_Position = projection * eyePosition;
}
