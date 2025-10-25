#ifndef MAPA_CIUDAD_H
#define MAPA_CIUDAD_H

#include <stdio.h>

// ---- CONSTANTES GLOBALES ----
#define FILAS 22
#define COLUMNAS 22

#define INICIO 'I'
#define FIN 'F'
#define BARRO 'B'
#define POZO 'P'
#define CORTE 'X'
#define PAVIMENTO '.'
#define CASA 'H'

#define TECLA_ARRIBA 72
#define TECLA_ABAJO 80
#define TECLA_IZQUIERDA 75
#define TECLA_DERECHA 77
#define TECLA_ENTER 13

// ---- PROTOTIPOS DE FUNCIONES ----
void cargarMapaCiudad(char mapa[FILAS][COLUMNAS]);
void mostrarMapa(char mapa[FILAS][COLUMNAS], int cursor_y, int cursor_x);
void seleccionarCelda(char mapa[FILAS][COLUMNAS], int *cursor_y, int *cursor_x, const char *mensaje);
void limpiarBufferEntrada();

#endif
