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

uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform vec3 lightColor;

uniform float shininess;

void main(void)
{
    vec3 N = normalize(fN);
    vec3 E = normalize(fE);
    vec3 L = normalize(fL);
    vec3 R = normalize(2.0 * dot(L, N) * N - L);

    float NdotL = dot(N, L);
    float Kd = max(NdotL, 0.0);
    float Ks = (NdotL < 0.0) ? 0.0 : pow(max(dot(R, E), 0.0), shininess);

    vec4 diffuse = Kd * vec4(texture(diffuseTexture, fTexCoord).rgb, 1.0) * diffuseColor;
    vec3 ambient = texture(ambientTexture, fTexCoord).rgb * ambientColor.rgb;
    vec4 specular = Ks * specularColor;

    //outColor = vec4((ambient + diffuse.rgb + specular.rgb) * lightColor, diffuse.a);
    outColor = Kd * diffuse * vec4(lightColor, 1.0);
}
