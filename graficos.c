#include "graficos.h"
#include "manejo_archivos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>

#define ALTURA_MAX 10
#define ELEMENTOS_POR_BLOQUE 20
#define TAM_MAX_EXT 20

/*
int main() {
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
    n[8] = "queso.mp4";
    n[9] = "queso";

    graficoExtensiones(n, 10);

    for(int i = 0; i < 10; i++) {
        free(n[i]);
    }
    free(n);

    return 0;
}
*/

/**
 * Muestra un grafico con la evolucion del tamanyo de una carpeta a lo largo
 * de las distintas versiones del repositorio. Las referencias a las carpetas
 * se pasan como arrays de DIR*.
 *
 * @param carpetas: array de DIR*
 * @param len: tamanyo del array carpetas
 * @param nombreCarpeta: nombre de la carpeta original
 *
 * */

void graficoTamanyoCarpeta(char **pathsCarpetas, int len, char* nombreCarpeta) {
    char** nombres = (char**) malloc(len*sizeof(char*));
    for (int i = 0; i < len; i++) {
        if (nombres)
            nombres[i] = (char*) malloc(sizeof(char)*12);
    }
    long int* tamanyos = (long int*) malloc(len*sizeof(long int));
    for (int i = 0; i < len; i++) {
        if (nombres && tamanyos) {
            snprintf(nombres[i], 12, "Commit #%i",i+1);
            tamanyos[i] = tamanyoCarpeta(pathsCarpetas[i],0);
        }
    }
    printf("Evolucion del tamanyo de la carpeta%s:\n", nombreCarpeta);
    graficoBarras(nombres, tamanyos, len);
    for (int i = 0; i < len; i++) {
        if (nombres && nombres[i])
            free(nombres[i]);
    }
    free(nombres);
    free(tamanyos);
}

/**
 * Muestra un grafico que representa la frecuencia de cada tipo de extension dada
 * una lista de archivos.
 *
 * @param nombresArchivos: array con los nombres de los archivos
 * @param len: tamanyo del array nombresArchivos
 *
 * */
void graficoExtensiones(char** nombresArchivos, int len) {
    int extDistintas = 0;
    char** extensiones = (char**)malloc(len * sizeof(char*));
    for (int i = 0; i < len; i++) {
        extensiones[i] = (char*)malloc(TAM_MAX_EXT * sizeof(char));
    }
    for (int i = 0; i < len; i++) {
        char* ext = getExtension(nombresArchivos[i]);
        int esta = 0;
        for (int j = 0; j < extDistintas; j++) {
            if (strcmp(ext, extensiones[j]) == 0) {
                esta = 1;
                break;
            }
        }
        if (!esta) {
            //extension nueva
            extensiones[extDistintas] = ext;
            extDistintas++;
        }
    }

    long int* frecuencias = malloc(extDistintas*sizeof(long int));
    for (int i = 0; i < extDistintas; i++) {
        frecuencias[i] = 0;
    }

    for (int i = 0; i < len; i++) {
        char* ext = malloc(TAM_MAX_EXT*sizeof(char));
        strcpy(ext, getExtension(nombresArchivos[i]));
        for (int j = 0; j < extDistintas; j++) {
            if (strcmp(ext, extensiones[j]) == 0) {
                frecuencias[j]++;
                break;
            }
        }
    }
    for (int i = 0; i < extDistintas; i++) {
        printf("%s | %i\n", extensiones[i], frecuencias[i]);
    }
    printf("Frecuencias de extensiones en el repositorio:\n");
    graficoBarras(extensiones, frecuencias, extDistintas);
    free(frecuencias);
    for(int i = 0; i < extDistintas; i++) {
        free(extensiones[i]);
    }
    free(extensiones);
}

/**
 * Devuelve la extension (con el punto) de un archivo. Si no tiene
 * extension, se devuelve "Sin extension".
 * */
char* getExtension(char* nombre) {
    char* ret = strrchr(nombre, '.');
    if (!ret) {
        return "Sin extension";
    } else {
        return ret;
    }
}

/**
 * Muestra un grafico con la evolucion del tamanyo de un archivo. Las referencias
 * a los archivos se pasaran como un array de FILE*.
 *
 * @param archivos: array de FILE* con referencias a las diferentes versiones del archivo base
 * @param len: tamanyo del array archivos
 * @param nombreArchivo: nombre del archivo base
 * */
void graficoTamanyoArchivos(char **pathsArchivos, int len, char *nombreArchivo) {
    char** nombres = (char**) malloc(len*sizeof(char*));
    for (int i = 0; i < len; i++) {
        if (nombres)
            nombres[i] = (char*) malloc(sizeof(char)*12);
    }
    long int* tamanyos = (long int*) malloc(len*sizeof(long int));
    for (int i = 0; i < len; i++) {
        if (nombres && tamanyos) {
            snprintf(nombres[i], 12, "Commit #%i",i+1);
            tamanyos[i] = tamanyoArchivo(pathsArchivos[i]);
        }
    }
    printf("Evolucion del tamanyo del archivo %s:\n", nombreArchivo);
    graficoBarras(nombres, tamanyos, len);
    for (int i = 0; i < len; i++) {
        if (nombres && nombres[i])
            free(nombres[i]);
    }
    free(nombres);
    free(tamanyos);
}

/**
 * Muestra un grafico de barras cualquiera. Si el numero de elementos
 * del grafico supera la constante ELEMENTOS_POR_BLOQUE, el grafico se dividira
 * en diferentes bloques para que pueda ser visualizado correctamente por pantalla.
 *
 * @param nombresX: array de cadenas de caracteres que conformaran el eje X
 * @param valores: array de enteros que representa los valores (eje Y) del grafico.
 * 				   Cada elemento de nombresX tendra un valor asociado. Ambos arrays
 * 				   deben seguir el mismo orden
 * @param len: tamanyo de los arrays
 * */
void graficoBarras(char* nombresX[], long int *valores, int len) {
    int max;
    max = getMax(valores, len);
    // reescalar valores
    for (int i = 0; i < len; i++) {
        valores[i] = (valores[i] * 1.0 / max) * ALTURA_MAX;
    }

    int bloques = len/ELEMENTOS_POR_BLOQUE;
    if (len % ELEMENTOS_POR_BLOQUE != 0) {
        bloques++;
    }

    for (int i = 0; i < bloques; i++) {
        pintarBloqueGraficoBarras(valores, max, i*ELEMENTOS_POR_BLOQUE, len);
        printf("\n");
        imprimirLeyenda(nombresX, i*ELEMENTOS_POR_BLOQUE, len);
        printf("\n");
    }
}

/**
 * Muestra la leyenda de un grafico de barras determinado.
 *
 * @param nombres: array de nombres que conforman el eje X del grafico
 * @param inicio: posicion del array de nombres a partir de la cual se mostrara la leyenda
 * @param len: tamanyo del array de nombres
 * */
void imprimirLeyenda(char *nombres[], int inicio, int len) {
    int fin = inicio+ELEMENTOS_POR_BLOQUE;
    if (fin >= len) {
        fin = len;
    }
    for (int i = inicio; i < fin; i++) {
        printf("%i: %s\n", i+1, nombres[i]);
    }
}

/**
 * Muestra por pantalla un bloque determinado de un grafico de barras.
 *
 * @param valores: array con los valores (eje Y) del grafico
 * @param max: valor mas alto del array de valores
 * @param inicio: posicion del array de valores a partir de la cual comienza el bloque
 * @param len: tamanyo del array de valores
 * */
void pintarBloqueGraficoBarras(long int *valores, long int max, int inicio, int len) {
    int fin;
    if (inicio + ELEMENTOS_POR_BLOQUE > len) {
        fin = len;
    } else {
        fin = inicio + ELEMENTOS_POR_BLOQUE;
    }
    printf("Rango del eje vertical: [0-%i]\n", max);
    printf("-");
    for(int i = inicio; i < fin; i++) {
        printf("---");
    }
    printf("\n");
    for (int i = ALTURA_MAX; i >= 1; i--) {
        printf(" ");
        for (int j = inicio; j < fin; j++) {
            if (valores[j] >= i) {
                printf("%c  ", '='); // 219 para imprimir bloque en blanco (puede descuadrarse)
            }
            else {
                printf("   ");
            }
        }
        printf("\n");
    }
    printf(" ");
    for (int i = inicio+1; i <= fin; i++) {
        if (i < 10) {
            printf("%i  ", i);
        }
        else {
            printf("%i ", i);
        }
    }
    printf("\n");
}

/**
 * Devuelve el valor mas alto de un array de enteros
 *
 * @param nums: array de enteros
 * @param len: tamanyo del array de enteros
 * */
long int getMax(long int *nums, int len) {
    long int max = nums[0];
    for (int i = 1; i < len; i++) {
        if (nums[i] > max) {
            max = nums[i];
        }
    }
    return max;
}

/*
 * @param directorio Base path to traverse directory
 * @param root     Integer representing indention for current directory
 */
void mostrarArbolDirectorios(char *directorio, const int root) {
    int i;
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(directorio);

    if (!dir) return;

    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            for (i=0; i<root; i++) {
                if (i%2 == 0 || i == 0) {
                    // printf("%c", 179);
                    printf("|");
                } else {
                    printf(" ");
                }
            }
            //printf("%c%c%s\n", 195, 196, dp->d_name);

            printf("|--%s\n", dp->d_name);

            strcpy(path, directorio);
            strcat(path, "/");
            strcat(path, dp->d_name);
            mostrarArbolDirectorios(path, root + 2);
        }
    }
    closedir(dir);
}
