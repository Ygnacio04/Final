#version 330

uniform mat4 MVP; //matriz para posiciones globales de c�mara (c�mo se ve en pantalla)
uniform mat4 M; //posiciones absolutas para c�lculos de luz

attribute vec4 vPos;            // atributos de entrada para shader de v�rtices
attribute vec4 vColor;
attribute vec4 vNorm;           // normal a nivel de v�rtice
attribute vec4 vTextureCoord;   // coordenada de textura a nivel de v�rtice

out vec4 fColor;           // variable de salida para shader de fragmentos
out vec4 fNorm;            // salida de datos de normales
out vec4 fPos;             // datos de posici�n absoluta (sin c�mara, proyecci�n)
out vec4 fTextureCoord;    // pasamos coordenadas de textura al shader de fragmentos

void main() {
    fColor = vColor + vec4(0.0); // copiamos color
    gl_Position = MVP * vPos;    // posici�n final del v�rtice en pantalla

    fPos = M * vPos; // posici�n absoluta para c�lculos de luz
    fNorm = inverse(transpose(M)) * vNorm;
    fNorm.w = 0;
    fNorm = normalize(fNorm);

    fTextureCoord = vTextureCoord;
}