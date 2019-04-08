#ifndef VCS_PROG4_MANEJO_ARCHIVOS_H
#define VCS_PROG4_MANEJO_ARCHIVOS_H

long int tamanyoArchivo(const char *fichero);
void crearCarpeta(char *nombre);
long int tamanyoCarpeta(char *d, const int root);
void borrarCarpeta(char *directorio, const int root);
int compararArchivos(char *archivoA, char *archivoB);
int copiarArchivo(char *archivoOrigen, char *archivoDestino);

#endif //VCS_PROG4_MANEJO_ARCHIVOS_H
