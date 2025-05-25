#version 330

uniform mat4 MVP; //matriz para posiciones globales de cámara (cómo se ve en pantalla)
uniform mat4 M; //posiciones absolutas para cálculos de luz
attribute vec4 vPos;//atributos de entrada para shader de vértices
attribute vec4 vColor;
attribute vec4 vNorm; //normal a nivel de vértice
attribute vec4 vTextureCoord; //coordenada de textura a nivel de vértice


out vec4 fColor;//variable de salida para shader de fragmentos
out vec4 fNorm; //salida .de datos de normales
out vec4 fPos; //datos de posición absoluto (sin cámara,proyeccion)
out vec4 fTextureCoord; //pasamos coordenadas de textura al shader de fragmentos

void main() {
	//copiar, calcular, etc... variables de salida
	fColor=vColor + vec4(0.0);
	gl_Position = MVP * vPos;
	fPos = M * vPos; //posición para fragmentos absoluta
	fNorm = inverse(transpose(M)) * vNorm;
	fNorm.w = 0;
	fNorm = normalize(fNorm);
	fTextureCoord = vTextureCoord;
}
