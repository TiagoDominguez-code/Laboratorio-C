#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

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

// ---- ESTRUCTURA DE CELDA ----
typedef struct {
    char tipo;       // Visualización: '.', 'H', 'B', etc.
    int costo;       // Costo de movimiento
    int g, h, f;     // Costos para A*
    int visitado;    // 0 = no visitado, 1 = visitado
    int padre_x, padre_y;
} Celda;

// ---- PROTOTIPOS ----
void inicializarMapaConCiudad(Celda mapa[FILAS][COLUMNAS]);
void mostrarMapa(Celda mapa[FILAS][COLUMNAS], int cursor_y, int cursor_x);
void seleccionarCelda(Celda mapa[FILAS][COLUMNAS], int *cursor_y, int *cursor_x, const char *mensaje);
void limpiarBufferEntrada();

// ---- MAIN ----
int main() {
    Celda mapa[FILAS][COLUMNAS];
    int cursor_y = 1, cursor_x = 1, cantidad;

    inicializarMapaConCiudad(mapa);

    seleccionarCelda(mapa, &cursor_y, &cursor_x, "Usa las flechas para ubicar el INICIO (I) en una calle");
    mapa[cursor_y][cursor_x].tipo = INICIO;
    mapa[cursor_y][cursor_x].costo = 1;

    seleccionarCelda(mapa, &cursor_y, &cursor_x, "Ahora, ubica la celda de FIN (F)");
    mapa[cursor_y][cursor_x].tipo = FIN;
    mapa[cursor_y][cursor_x].costo = 1;

    system("cls");
    mostrarMapa(mapa, -1, -1);
    printf("\nPresiona cualquier tecla para continuar...");
    _getch();

    // Barro
    printf("\nCuantas celdas con Barro (B) quieres ubicar? ");
    scanf("%d", &cantidad);
    limpiarBufferEntrada();
    for (int i = 0; i < cantidad; i++) {
        char mensaje[100];
        sprintf(mensaje, "Ubicando Barro (%d de %d)", i + 1, cantidad);
        seleccionarCelda(mapa, &cursor_y, &cursor_x, mensaje);
        mapa[cursor_y][cursor_x].tipo = BARRO;
        mapa[cursor_y][cursor_x].costo = 5;
    }

    // Pozos
    printf("\nCuantas celdas con Pozos (P) quieres ubicar? ");
    scanf("%d", &cantidad);
    limpiarBufferEntrada();
    for (int i = 0; i < cantidad; i++) {
        char mensaje[100];
        sprintf(mensaje, "Ubicando Pozo (%d de %d)", i + 1, cantidad);
        seleccionarCelda(mapa, &cursor_y, &cursor_x, mensaje);
        mapa[cursor_y][cursor_x].tipo = POZO;
        mapa[cursor_y][cursor_x].costo = 10;
    }

    // Cortes
    printf("\nCuantas celdas con Corte (X) quieres ubicar? ");
    scanf("%d", &cantidad);
    limpiarBufferEntrada();
    for (int i = 0; i < cantidad; i++) {
        char mensaje[100];
        sprintf(mensaje, "Ubicando Corte (%d de %d)", i + 1, cantidad);
        seleccionarCelda(mapa, &cursor_y, &cursor_x, mensaje);
        mapa[cursor_y][cursor_x].tipo = CORTE;
        mapa[cursor_y][cursor_x].costo = -1; // Intransitable
    }

    system("cls");
    printf("--- MAPA FINAL CONFIGURADO ---\n\n");
    mostrarMapa(mapa, -1, -1);
    printf("\nConfiguracion completada!\n");

    return 0;
}

// ---- FUNCIONES ----

void inicializarMapaConCiudad(Celda mapa[FILAS][COLUMNAS]) {
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            if (i == 0 || i == FILAS - 1 || j == 0 || j == COLUMNAS - 1) {
                mapa[i][j].tipo = PAVIMENTO;
                mapa[i][j].costo = 1;
            } else if (i % 3 == 0 || j % 3 == 0) {
                mapa[i][j].tipo = PAVIMENTO;
                mapa[i][j].costo = 1;
            } else {
                mapa[i][j].tipo = CASA;
                mapa[i][j].costo = -1;
            }

            mapa[i][j].g = mapa[i][j].h = mapa[i][j].f = 0;
            mapa[i][j].visitado = 0;
            mapa[i][j].padre_x = mapa[i][j].padre_y = -1;
        }
    }
}

void mostrarMapa(Celda mapa[FILAS][COLUMNAS], int cursor_y, int cursor_x) {
    system("cls");
    printf("                           MAPA DE LA CIUDAD\n");

    printf("   %c", 218);
    for (int i = 0; i < COLUMNAS; i++) printf("%c%c%c", 196, 196, 196);
    printf("%c%c\n", 196, 191);

    for (int i = 0; i < FILAS; i++) {
        printf("   %c ", 179);
        for (int j = 0; j < COLUMNAS; j++) {
            if (i == cursor_y && j == cursor_x) {
                printf("[%c]", mapa[i][j].tipo);
            } else {
                printf(" %c ", mapa[i][j].tipo);
            }
        }
        printf("%c\n", 179);
    }

    printf("   %c", 192);
    for (int i = 0; i < COLUMNAS; i++) printf("%c%c%c", 196, 196, 196);
    printf("%c%c\n", 196, 217);
}

void seleccionarCelda(Celda mapa[FILAS][COLUMNAS], int *cursor_y, int *cursor_x, const char *mensaje) {
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
            if (mapa[*cursor_y][*cursor_x].tipo == PAVIMENTO) {
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
