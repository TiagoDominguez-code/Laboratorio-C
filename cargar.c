#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
// #include <windows.h> // <-- Ya no es necesario

// ---- CONSTANTES GLOBALES ----
#define FILAS 22
#define COLUMNAS 22

// ... (El resto de las constantes son iguales)
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
void inicializarMapaConCiudad(char mapa[FILAS][COLUMNAS]);
void mostrarMapa(char mapa[FILAS][COLUMNAS], int cursor_y, int cursor_x);
void seleccionarCelda(char mapa[FILAS][COLUMNAS], int *cursor_y, int *cursor_x, const char *mensaje);
void limpiarBufferEntrada();

// ---- FUNCIÓN PRINCIPAL ----
int main() {
    // SetConsoleOutputCP(65001); // <-- Ya no es necesario

    char mapa[FILAS][COLUMNAS];
    int cursor_y = 1;
    int cursor_x = 1;
    int cantidad;

    inicializarMapaConCiudad(mapa);
    
    // ... (El resto de la función main es exactamente igual) ...

    seleccionarCelda(mapa, &cursor_y, &cursor_x, "Usa las flechas para ubicar el INICIO (I) en una calle");
    mapa[cursor_y][cursor_x] = INICIO;

    seleccionarCelda(mapa, &cursor_y, &cursor_x, "Ahora, ubica la celda de FIN (F)");
    mapa[cursor_y][cursor_x] = FIN;

    printf("\nCuantas celdas con Barro (B) quieres ubicar? ");
    scanf("%d", &cantidad);
    limpiarBufferEntrada();
    for (int i = 0; i < cantidad; i++) {
        char mensaje[100];
        sprintf(mensaje, "Ubicando Barro (%d de %d)", i + 1, cantidad);
        seleccionarCelda(mapa, &cursor_y, &cursor_x, mensaje);
        mapa[cursor_y][cursor_x] = BARRO;
    }

    printf("\nCuantas celdas con Pozos (P) quieres ubicar? ");
    scanf("%d", &cantidad);
    limpiarBufferEntrada();
    for (int i = 0; i < cantidad; i++) {
        char mensaje[100];
        sprintf(mensaje, "Ubicando Pozo (%d de %d)", i + 1, cantidad);
        seleccionarCelda(mapa, &cursor_y, &cursor_x, mensaje);
        mapa[cursor_y][cursor_x] = POZO;
    }

    printf("\nCuantas celdas con Corte (X) quieres ubicar? ");
    scanf("%d", &cantidad);
    limpiarBufferEntrada();
    for (int i = 0; i < cantidad; i++) {
        char mensaje[100];
        sprintf(mensaje, "Ubicando Corte (%d de %d)", i + 1, cantidad);
        seleccionarCelda(mapa, &cursor_y, &cursor_x, mensaje);
        mapa[cursor_y][cursor_x] = CORTE;
    }

    system("cls");
    printf("--- MAPA FINAL CONFIGURADO ---\n\n");
    mostrarMapa(mapa, -1, -1);
    printf("\nConfiguracion completada!\n");

    return 0;
}

// ---- IMPLEMENTACIÓN DE FUNCIONES ----

void inicializarMapaConCiudad(char mapa[FILAS][COLUMNAS]) {
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            if (i == 0 || i == FILAS - 1 || j == 0 || j == COLUMNAS - 1) {
                mapa[i][j] = PAVIMENTO;
            } else {
                if (i % 3 == 0 || j % 3 == 0) {
                    mapa[i][j] = PAVIMENTO;
                } else {
                    mapa[i][j] = CASA;
                }
            }
        }
    }
}

// <-- FUNCIÓN MODIFICADA PARA USAR ASCII EXTENDIDO
void mostrarMapa(char mapa[FILAS][COLUMNAS], int cursor_y, int cursor_x) {
    system("cls");
    printf("                           MAPA DE LA CIUDAD\n");
    
    // Borde superior con los nuevos caracteres
    printf("   %c", 218); // Esquina ┌ (Alt + 218)
    for (int i = 0; i < COLUMNAS; i++) printf("%c%c%c", 196, 196, 196); // Línea ───
    printf("%c%c", 196, 191); // Línea extra y esquina ┐ (Alt + 191)
    printf("\n");

    // Contenido y bordes laterales
    for (int i = 0; i < FILAS; i++) {
        printf("   %c ", 179); // Borde lateral │ (Alt + 179)
        for (int j = 0; j < COLUMNAS; j++) {
            if (i == cursor_y && j == cursor_x) {
                printf("[%c]", mapa[i][j]);
            } else {
                printf(" %c ", mapa[i][j]);
            }
        }
        printf("%c\n", 179); // Borde lateral │
    }

    // Borde inferior
    printf("   %c", 192); // Esquina └ (Alt + 192)
    for (int i = 0; i < COLUMNAS; i++) printf("%c%c%c", 196, 196, 196); // Línea ───
    printf("%c%c", 196, 217); // Línea extra y esquina ┘ (Alt + 217)
    printf("\n");
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
                case TECLA_ARRIBA:    if (*cursor_y > 0) (*cursor_y)--;          break;
                case TECLA_ABAJO:     if (*cursor_y < FILAS - 1) (*cursor_y)++;    break;
                case TECLA_IZQUIERDA: if (*cursor_x > 0) (*cursor_x)--;          break;
                case TECLA_DERECHA:   if (*cursor_x < COLUMNAS - 1) (*cursor_x)++;  break;
            }
        } else if (tecla == TECLA_ENTER) {
            if (mapa[*cursor_y][*cursor_x] == PAVIMENTO) {
                break;
            } else {
                printf("\n\aError: No puedes colocar un item sobre una casa (H). Elige una calle ('.').");
                _getch();
            }
        }
    }
}

void limpiarBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}