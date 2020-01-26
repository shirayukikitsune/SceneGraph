#version 400 core

in vec3 fN;
in vec3 fE;
in vec3 fL;
in vec2 fTexCoord;

out vec4 outColor;

uniform sampler2D ambientTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D specularColorTexture;
uniform sampler2D specularHighlightTexture;
uniform sampler2D bumpTexture;
uniform int textureMask;

uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;

uniform vec3 lightAmbientColor;
uniform vec3 lightDiffuseColor;
uniform vec3 lightSpecularColor;

uniform float shininess;

vec3 ambient()
{
    vec3 ambientTerm = ambientColor.rgb;
    if ((textureMask & 1) != 0) {
        ambientTerm *= texture(ambientTexture, fTexCoord).rgb;
    }

    return ambientTerm * lightAmbientColor;
}

vec3 diffuse(in vec3 N, in vec3 L)
{
    float diffuseTerm = clamp(dot(N, L), 0, 1);
    return diffuseColor.rgb * lightDiffuseColor * diffuseTerm;
}

vec3 specular(in vec3 N, in vec3 L, in vec3 V)
{
    float specularTerm = 0.0f;

    if (dot(N, L) > 0) {
        vec3 H = normalize(L + V);
        specularTerm = pow(dot(N, H), shininess);
    }

    return specularColor.rgb * lightSpecularColor * specularTerm;
}

void main(void)
{
    vec3 N = normalize(fN);
    vec3 E = normalize(fE);
    vec3 L = normalize(fL);

    vec3 Iamb = ambient();
    vec3 Idif = diffuse(N, L);
    vec3 Ispe = specular(N, L, E);

    vec3 diffuse = vec3(1.0f);
    if ((textureMask & 2) != 0) {
        diffuse *= texture(diffuseTexture, fTexCoord).rgb;
    }

    outColor = vec4(diffuse * (Iamb + Idif + Ispe), 1.0f);
}
