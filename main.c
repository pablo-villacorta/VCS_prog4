#include <stdio.h>
#include <stdlib.h>
#include "manejo_archivos.h"
#include "graficos.h"

int main() {
    // Grafico de extensiones
    char** n = (char**)malloc(10*sizeof(char*));
    for (int i = 0; i < 10; i++) {
        n[i] = (char*)malloc(10*sizeof(char));
    }

    n[0] = "queso.java";
    n[1] = "queso.png";
    n[2] = "queso.avi";
    n[3] = "queso.mp3";
    n[4] = "queso.";
    n[5] = "queso.jpg";
    n[6] = "queso.avi";
    n[7] = "queso.avi";
    n[8] = "queso.jpg";
    n[9] = "queso";

    graficoExtensiones(n, 10);

    // Arbol de directorios
    printf("Arbol de directorios\n");
    mostrarArbolDirectorios("prueba", 0);

    printf("\n\n");
    // Grafico de tamanyos de archivo
    char** m = (char**)malloc(10*sizeof(char*));
    for (int i = 0; i < 5; i++) {
        m[i] = (char*)malloc(10*sizeof(char));
    }

    m[0] = "prueba/ver1.txt";
    m[1] = "prueba/ver2.txt";
    m[2] = "prueba/ver3.txt";
    m[3] = "prueba/ver4.txt";
    m[4] = "prueba/ver5.txt";

    graficoTamanyoArchivos(m, 5, "Ver.txt");

    // Descomentar para borrar carpeta
    //borrarCarpeta("prueba1", 0);

    // Descomentar para crear carpeta
    //crearCarpeta("prueba1");
    return 0;
}