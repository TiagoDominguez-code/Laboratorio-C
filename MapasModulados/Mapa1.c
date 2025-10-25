#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

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

// ---- PROTOTIPOS ----
void cargarMapaCiudad(char mapa[FILAS][COLUMNAS]);
void mostrarMapa(char mapa[FILAS][COLUMNAS], int cursor_y, int cursor_x);
void seleccionarCelda(char mapa[FILAS][COLUMNAS], int *cursor_y, int *cursor_x, const char *mensaje);
void limpiarBufferEntrada();

// ---- MAIN ----
int main() {
    char mapa[FILAS][COLUMNAS];
    int cursor_y = 1, cursor_x = 1;

    cargarMapaCiudad(mapa);

    seleccionarCelda(mapa, &cursor_y, &cursor_x, "Ubica el INICIO (I) en una calle");
    mapa[cursor_y][cursor_x] = INICIO;

    seleccionarCelda(mapa, &cursor_y, &cursor_x, "Ubica el FIN (F)");
    mapa[cursor_y][cursor_x] = FIN;

    system("cls");
    printf("--- MAPA FINAL CONFIGURADO ---\n\n");
    mostrarMapa(mapa, -1, -1);
    printf("\nConfiguracion completada!\n");

    return 0;
}

// ---- FUNCIONES ----

void cargarMapaCiudad(char mapa[FILAS][COLUMNAS]) {
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            if (i == 0 || i == FILAS - 1 || j == 0 || j == COLUMNAS - 1) {
                mapa[i][j] = PAVIMENTO;
            } else if (i % 3 == 0 || j % 3 == 0) {
                mapa[i][j] = PAVIMENTO;
            } else {
                mapa[i][j] = CASA;
            }
        }
    }

    // Barro (B) en columna 6
    for (int i = 4; i < 18; i++) {
        if (mapa[i][6] == PAVIMENTO) {
            mapa[i][6] = BARRO;
        }
    }

    // Pozos (P) en fila 9
    for (int j = 5; j < 17; j++) {
        if (mapa[9][j] == PAVIMENTO) {
            mapa[9][j] = POZO;
        }
    }

    // Cortes (X) en intersecciones
    for (int i = 6; i < 16; i += 3) {
        for (int j = 6; j < 16; j += 3) {
            if (mapa[i][j] == PAVIMENTO) {
                mapa[i][j] = CORTE;
            }
        }
    }
}

void mostrarMapa(char mapa[FILAS][COLUMNAS], int cursor_y, int cursor_x) {
    system("cls");
    printf("                           MAPA DE LA CIUDAD\n");

    printf("   %c", 218);
    for (int i = 0; i < COLUMNAS; i++) printf("%c%c%c", 196, 196, 196);
    printf("%c%c\n", 196, 191);

    for (int i = 0; i < FILAS; i++) {
        printf("   %c ", 179);
        for (int j = 0; j < COLUMNAS; j++) {
            if (i == cursor_y && j == cursor_x) {
                printf("[%c]", mapa[i][j]);
            } else {
                printf(" %c ", mapa[i][j]);
            }
        }
        printf("%c\n", 179);
    }

    printf("   %c", 192);
    for (int i = 0; i < COLUMNAS; i++) printf("%c%c%c", 196, 196, 196);
    printf("%c%c\n", 196, 217);
}

void seleccionarCelda(char mapa[FILAS][COLUMNAS], int *cursor_y, int *cursor_x, const char *mensaje) {
    int tecla;
    while (1) {
        mostrarMapa(mapa, *cursor_y, *cursor_x);
        printf("\n%s\n", mensaje);
        printf("Solo puedes seleccionar celdas de calle ('.')\n");

        tecla = _getch();
        if (tecla == 224 || tecla == 0) {
            tecla = _getch();
            switch (tecla) {
                case TECLA_ARRIBA:    if (*cursor_y > 0) (*cursor_y)--; break;
                case TECLA_ABAJO:     if (*cursor_y < FILAS - 1) (*cursor_y)++; break;
                case TECLA_IZQUIERDA: if (*cursor_x > 0) (*cursor_x)--; break;
                case TECLA_DERECHA:   if (*cursor_x < COLUMNAS - 1) (*cursor_x)++; break;
            }
        } else if (tecla == TECLA_ENTER) {
            if (mapa[*cursor_y][*cursor_x] == PAVIMENTO) {
                break;
            } else {
                printf("\n\aError: No puedes colocar un item sobre una casa u obstáculo. Elige una calle ('.').");
                _getch();
            }
        }
    }
}

void limpiarBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
