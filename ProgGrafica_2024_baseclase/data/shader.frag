#version 330
in vec4 fColor;
in vec4 fNorm; //dato de vértices
in vec4 fPos;
in vec4 fTextureCoord;

uniform sampler2D textureColor;

//variables material de objeto:
uniform float Kd;
uniform float Ks;
uniform int shinny;

//datos variables de código
//variables de luz
uniform vec4 lightPos;
uniform vec4 lightColor;
uniform float Ik;

#define DIRECTIONAL 0
#define POINT       1
uniform int lightType;
uniform vec4 lightDirection;

//variables de cámara
uniform vec4 cameraPos;


void main() {

	vec4 textureColor = texture(textureColor, fTextureCoord.xy);

	float lightIntensity = 1;
	float ambient = 0.2f;
	float diffuse = 0;
	//IK Intensidad de luz
	//Kd Propiedad difusa del material
	// L Vector de dirección de luz
	// si tipo es DIRECTIONAL, L=lightDirection
	// si es POINt L=lightPos - fPos;

	vec4 L = vec4(0, 0, 0, 0);
	if (lightType == DIRECTIONAL)
		L = lightDirection;
	else if(lightType==POINT)
		L=normalize(lightPos - fPos);
	// N normal de la superfice
	vec4 N = normalize(fNorm);
	//difusa = Ik * Kd * dot(L,N)
	diffuse = Ik * Kd * max(dot(L, N), 0);
	

	float specular = 0;
	// R: Rayo de luz reflejado respecto de la normal
	vec4 R = normalize(reflect(L, N));//(2(N.L)-L)
	// V rayo con la dirección del visor/cámara
	vec4 V = normalize(fPos - cameraPos);

	specular = Ik * Ks * pow(max(dot(R, V), 0), shinny);

	lightIntensity = specular + ambient + diffuse;

	if (textureColor.a < 0.1) discard;
	gl_FragColor = textureColor *lightIntensity*lightColor;
}
