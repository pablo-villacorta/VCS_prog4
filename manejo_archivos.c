#include "manejo_archivos.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

/**
 * @return 0 si son iguales, -1 si son diferentes, -2 si ha habido algun error
 * */
int compararArchivos(char *archivoA, char *archivoB) {
    if(tamanyoArchivo(archivoA) != tamanyoArchivo(archivoB)) return -1;

    FILE *fileA, *fileB;
    fileA = fopen(archivoA, "r");
    fileB = fopen(archivoB, "r");

    char ca, cb;
    ca = fgetc(fileA);
    cb = fgetc(fileB);

    while(ca != EOF && cb != EOF) {
        if(ca != cb) return -1;
        ca = fgetc(fileA);
        cb = fgetc(fileB);
    }

    return 0;
}

/**
 * @return 0 si todo ha ido bien, -1 si ha habido algún error
 * */
int copiarArchivo(char *archivoOrigen, char *archivoDestino) {
    FILE *source, *target;
    source = fopen(archivoOrigen, "r");
    target = fopen(archivoDestino, "w");

    if (!source || !target) {
        return -1; //error
    }

    char ch;
    while ((ch = fgetc(source)) != EOF) {
        fputc(ch, target);
    }

    fclose(source);
    fclose(target);

    return 0; //todo ha ido bien
}

long int tamanyoArchivo(const char *fichero){
    long int tamanyo; // declaro la variable que recibira el tamanio.
    FILE* arch; // declaro un puntero de tipo FILE.

    arch=fopen(fichero, "rb"); // abro el archivo de solo lectura.
    fseek(arch, SEEK_CUR, SEEK_END); // me ubico en el final del archivo.
    tamanyo=ftell(arch);                     // obtengo su tamanyo en BYTES.
    fclose(arch);                               // cierro el archivo.

    return tamanyo;
}

void crearCarpeta(char *nombre){
    mkdir(nombre);
}

long int tamanyoCarpeta(char *directorio, const int root) {
    long int tamanyo = 0;
    int i;
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(directorio);

    if (!dir) {
        return -1; //error
    }

    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            tamanyo += tamanyoArchivo(dp->d_name);
            strcpy(path, directorio);
            strcat(path, "/");
            strcat(path, dp->d_name);
            tamanyo = tamanyo + tamanyoCarpeta(path, root+2);
        }
    }
    closedir(dir);
    return tamanyo;
}

void borrarCarpeta(char *directorio, const int root) {
    int i;
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(directorio);

    if (!dir) {
        return;
    }

    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            strcpy(path, directorio);
            strcat(path, "/");
            strcat(path, dp->d_name);
            remove(path);
            int a = rmdir(directorio);
            printf("%i", a);
            borrarCarpeta(path, root+2);
        }
    }
    rmdir(directorio);

    closedir(dir);
}