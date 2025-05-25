#version 330 core
in vec4 fColor;
in vec4 fNorm;
in vec4 fPos;
in vec4 fTextureCoord;

out vec4 FragColor;
uniform sampler2D textureColor;

// material
uniform float Kd;
uniform float Ks;

// luz 1 (azul)
uniform vec4 lightPos1;
uniform vec4 lightColor1;
uniform float Ik1;

// luz 2 (roja)
uniform vec4 lightPos2;
uniform vec4 lightColor2;
uniform float Ik2;

// ambas son POINT en tu caso
#define POINT 1
uniform int lightType1;
uniform int lightType2;
uniform vec4 lightDirection1;
uniform vec4 lightDirection2;

// cámara
uniform vec4 cameraPos;

void main() {
    vec4 texCol = texture(textureColor, fTextureCoord.xy);
    if (texCol.a < 0.1) discard;

    // normal y visor
    vec3 N = normalize(fNorm.xyz);
    vec3 V = normalize((cameraPos - fPos).xyz);

    // ===== luz 1 =====
    vec3 L1 = normalize((lightPos1 - fPos).xyz);
    float diff1 = Ik1 * Kd * max(dot(N, L1), 0.0);
    vec3 R1 = reflect(-L1, N);
    float spec1 = Ik1 * Ks * pow(max(dot(R1, V), 0.0), 1.0);

    // ===== luz 2 =====
    vec3 L2 = normalize((lightPos2 - fPos).xyz);
    float diff2 = Ik2 * Kd * max(dot(N, L2), 0.0);
    vec3 R2 = reflect(-L2, N);
    float spec2 = Ik2 * Ks * pow(max(dot(R2, V), 0.0), 1.0);

    // ===== combinar =====
    // un término ambiental compartido
    vec3 ambient = 0.2 * (lightColor1.rgb + lightColor2.rgb);

    // difusa y especular por separado
    vec3 diffuse  = diff1 * lightColor1.rgb + diff2 * lightColor2.rgb;
    vec3 specular = spec1 * lightColor1.rgb + spec2 * lightColor2.rgb;

    // color final
    vec3 outColor = ambient + diffuse + specular;
    FragColor = texCol * fColor * vec4(outColor, 1.0);
}