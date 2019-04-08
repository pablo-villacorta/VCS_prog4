#ifndef VCS_PROG4_GRAFICOS_H
#define VCS_PROG4_GRAFICOS_H

#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>

void graficoBarras(char *nombresX[], long int *valores, int num);
void graficoTamanyoArchivos(char **pathsArchivos, int len, char *nombreArchivo);
long int getMax(long int *nums, int len);
void pintarBloqueGraficoBarras(long int* valores, long int max, int inicio, int fin);
void imprimirLeyenda(char* nombres[], int inicio, int len);
void graficoExtensiones(char** nombresArchivos, int len);
char* getExtension(char* nombre);
void graficoTamanyoCarpeta(char **pathsCarpetas, int len, char* nombreCarpeta);
void mostrarArbolDirectorios(char *directorio, const int root);

#endif //VCS_PROG4_GRAFICOS_H
