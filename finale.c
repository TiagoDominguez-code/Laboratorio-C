#include <stdio.h>
#include <stdlib.h>
#include <conio.h>  // <-- Necesario para _kbhit()
#include <math.h>     
#include <limits.h>   
#include <inttypes.h> 

// ---- CONSTANTES Y ESTRUCTURA ----
#define FILAS 22
#define COLUMNAS 22
#define MAX_NODOS (FILAS * COLUMNAS) 

#define INICIO 'I'
#define FIN 'F'
#define BARRO 'B'
#define POZO 'P'
#define CORTE 'X'
#define PAVIMENTO '.'
#define CASA 'H'
#define CAMINO '*' 

#define TECLA_ARRIBA 72
#define TECLA_ABAJO 80
#define TECLA_IZQUIERDA 75
#define TECLA_DERECHA 77
#define TECLA_ENTER 13 

// ---- ESTRUCTURA DE CELDA (CON CORRECCION X,Y) ----
typedef struct {
    char tipo;       
    int costo;       
    int g, h, f;     
    int visitado;    
    int padre_x, padre_y;
    int x, y; // Coordenadas propias
} Celda;

// ---- GLOBALES PARA LA LISTA ABIERTA DE A* ----
Celda* listaAbierta[MAX_NODOS];
int numNodosAbiertos = 0;

// ---- PROTOTIPOS (Generacion de Mapa) ----
void inicializarMapaConCiudad(Celda mapa[FILAS][COLUMNAS]);
void mostrarMapa(Celda mapa[FILAS][COLUMNAS], int cursor_y, int cursor_x);
void seleccionarCelda(Celda mapa[FILAS][COLUMNAS], int *cursor_y, int *cursor_x, const char *mensaje);
void limpiarBufferEntrada();
void cargarMapaPredefinido(Celda mapa[FILAS][COLUMNAS], int* inicio_x, int* inicio_y, int* fin_x, int* fin_y);
void personalizarMapa(Celda mapa[FILAS][COLUMNAS], int* inicio_x, int* inicio_y, int* fin_x, int* fin_y);

// ---- PROTOTIPOS (A*) ----
void agregarAListaAbierta(Celda* celda);
Celda* obtenerCeldaConMenorF();
int calcularHeuristica(int x1, int y1, int x2, int y2);
int buscarCaminoAEstrella(Celda mapa[FILAS][COLUMNAS], int inicio_x, int inicio_y, int fin_x, int fin_y);
void reconstruirYMarcarCamino(Celda mapa[FILAS][COLUMNAS], int inicio_x, int inicio_y, int fin_x, int fin_y);


// ---- MAIN (MENÚ) ----
int main() {
    Celda mapa[FILAS][COLUMNAS];
    int inicio_x, inicio_y, fin_x, fin_y;
    int opcion = 0;
    int mapaCargado = 0; 

    while (opcion != 3) {
        system("cls");
        printf("--- PROYECTO ALGORITMO A* ---\n");
        printf("1. Cargar mapa predefinido (con obstaculos fijos)\n");
        printf("2. Personalizar un nuevo mapa (elegir obstaculos)\n");
        printf("3. Salir\n");
        printf("------------------------------\n");
        printf("Seleccione una opcion: ");
        
        if (scanf("%d", &opcion) != 1) {
            opcion = 0; 
        }
        limpiarBufferEntrada();

        mapaCargado = 0; 

        switch (opcion) {
            case 1:
                cargarMapaPredefinido(mapa, &inicio_x, &inicio_y, &fin_x, &fin_y);
                mapaCargado = 1;
                break;
            case 2:
                personalizarMapa(mapa, &inicio_x, &inicio_y, &fin_x, &fin_y);
                mapaCargado = 1;
                break;
            case 3:
                printf("Saliendo...\n");
                continue; 
            default:
                printf("Opcion invalida. Presione una tecla...");
                _getch();
                continue; 
        }

        // ------------------------------------------------------------------
        // ---- EJECUCIÓN DE A* (Solo si se cargó un mapa) ----
        // ------------------------------------------------------------------
        if (mapaCargado) {
            system("cls");
            printf("--- MAPA LISTO PARA EVALUAR ---\n\n");
            mostrarMapa(mapa, -1, -1);
            printf("\nPresiona una tecla para CALCULAR la ruta optima...\n");
            _getch(); 
            system("cls");

            printf("Calculando ruta...\n"); 

            if (buscarCaminoAEstrella(mapa, inicio_x, inicio_y, fin_x, fin_y)) {
                printf("--- RUTA OPTIMA ENCONTRADA ---\n");
                reconstruirYMarcarCamino(mapa, inicio_x, inicio_y, fin_x, fin_y);
                mostrarMapa(mapa, -1, -1);
            } else {
                printf("--- NO SE PUDO ENCONTRAR UNA RUTA ---\n");
                mostrarMapa(mapa, -1, -1); 
            }
            
            // ------------------------------------------------------------------
            // ---- ¡¡CORRECCIÓN!! ----
            // ------------------------------------------------------------------
            printf("\nCalculo finalizado.\n");
            printf("Presione ENTER para SALIR del programa.\n");
            printf("Presione cualquier OTRA TECLA para VOLVER AL MENU...\n");

            // Limpiamos CUALQUIER tecla que haya quedado en el buffer
            // (por ejemplo, de una tecla de flecha que manda 2 chars)
            while (_kbhit()) {
                _getch();
            }

            int tecla_salida = _getch(); // Ahora sí espera
            
            if (tecla_salida == TECLA_ENTER) {
                opcion = 3; // Esto hará que el bucle while(opcion != 3) termine
            }
            // Si no es ENTER, no hacemos nada, y el bucle repetirá el menú
        }
    } // Fin del bucle while

    printf("Programa finalizado.\n");
    return 0;
}

// ------------------------------------------------------------------
// ---- FUNCIONES DE GENERACIÓN DE MAPA ----
// ------------------------------------------------------------------

/**
 * Carga un mapa con la estructura de ciudad y obstaculos fijos.
 * Pide al usuario el INICIO y FIN. (Lógica corregida)
 */
void cargarMapaPredefinido(Celda mapa[FILAS][COLUMNAS], int* inicio_x, int* inicio_y, int* fin_x, int* fin_y) {
    
    // 1. Crear la estructura base de ciudad
    inicializarMapaConCiudad(mapa);
    
    // 2. Añadir obstaculos fijos
    if (mapa[6][4].tipo == PAVIMENTO) { mapa[6][4].tipo = BARRO; mapa[6][4].costo = 5; }
    if (mapa[6][5].tipo == PAVIMENTO) { mapa[6][5].tipo = BARRO; mapa[6][5].costo = 5; }
    if (mapa[7][3].tipo == PAVIMENTO) { mapa[7][3].tipo = BARRO; mapa[7][3].costo = 5; }
    if (mapa[8][3].tipo == PAVIMENTO) { mapa[8][3].tipo = BARRO; mapa[8][3].costo = 5; }
    if (mapa[15][16].tipo == PAVIMENTO) { mapa[15][16].tipo = POZO; mapa[15][16].costo = 10; }
    if (mapa[15][17].tipo == PAVIMENTO) { mapa[15][17].tipo = POZO; mapa[15][17].costo = 10; }
    if (mapa[9][10].tipo == PAVIMENTO) { mapa[9][10].tipo = CORTE; mapa[9][10].costo = -1; }
    if (mapa[9][11].tipo == PAVIMENTO) { mapa[9][11].tipo = CORTE; mapa[9][11].costo = -1; }
    if (mapa[9][12].tipo == PAVIMENTO) { mapa[9][12].tipo = CORTE; mapa[9][12].costo = -1; }
    if (mapa[10][12].tipo == PAVIMENTO) { mapa[10][12].tipo = CORTE; mapa[10][12].costo = -1; }
    if (mapa[11][12].tipo == PAVIMENTO) { mapa[11][12].tipo = CORTE; mapa[11][12].costo = -1; }

    // 3. Mostrar el mapa CON los obstáculos
    system("cls");
    printf("--- MAPA PREDEFINIDO (CON OBSTACULOS) ---\n\n");
    mostrarMapa(mapa, -1, -1);
    printf("\nObstaculos fijos cargados.\n");
    printf("Presiona una tecla para ubicar INICIO y FIN...");
    _getch();

    // 4. Pedir al usuario INICIO y FIN
    int cursor_y = 1;
    int cursor_x = 3; 

    seleccionarCelda(mapa, &cursor_y, &cursor_x, "MAPA PREDEFINIDO: Usa las flechas para ubicar el INICIO (I)");
    mapa[cursor_y][cursor_x].tipo = INICIO;
    mapa[cursor_y][cursor_x].costo = 1;
    *inicio_y = cursor_y; 
    *inicio_x = cursor_x; 

    seleccionarCelda(mapa, &cursor_y, &cursor_x, "MAPA PREDEFINIDO: Ahora, ubica la celda de FIN (F)");
    mapa[cursor_y][cursor_x].tipo = FIN;
    mapa[cursor_y][cursor_x].costo = 1;
    *fin_y = cursor_y; 
    *fin_x = cursor_x; 

    printf("\nInicio y Fin configurados!\n");
}


/**
 * Contiene tu código original de personalización.
 */
void personalizarMapa(Celda mapa[FILAS][COLUMNAS], int* inicio_x, int* inicio_y, int* fin_x, int* fin_y) {
    int cursor_y = 1, cursor_x = 3, cantidad; 

    // 1. Crear el mapa base de ciudad
    inicializarMapaConCiudad(mapa);

    // 2. Seleccionar INICIO
    seleccionarCelda(mapa, &cursor_y, &cursor_x, "MAPA PERSONALIZADO: Usa las flechas para ubicar el INICIO (I)");
    mapa[cursor_y][cursor_x].tipo = INICIO;
    mapa[cursor_y][cursor_x].costo = 1;
    *inicio_y = cursor_y; 
    *inicio_x = cursor_x; 

    // 3. Seleccionar FIN
    seleccionarCelda(mapa, &cursor_y, &cursor_x, "MAPA PERSONALIZADO: Ahora, ubica la celda de FIN (F)");
    mapa[cursor_y][cursor_x].tipo = FIN;
    mapa[cursor_y][cursor_x].costo = 1;
    *fin_y = cursor_y; 
    *fin_x = cursor_x; 

    system("cls");
    mostrarMapa(mapa, -1, -1);
    printf("\nPresiona cualquier tecla para anadir obstaculos...");
    _getch();

    // 4. Añadir BARRO
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
    if (cantidad > 0) {
        system("cls");
        printf("--- Celdas de Barro ubicadas ---\n\n");
        mostrarMapa(mapa, -1, -1);
        printf("\nPresiona cualquier tecla para continuar...");
        _getch();
    }

    // 5. Añadir POZOS
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
    if (cantidad > 0) {
        system("cls");
        printf("--- Celdas de pozos ubicadas ---\n\n");
        mostrarMapa(mapa, -1, -1);
        printf("\nPresiona cualquier tecla para continuar...");
        _getch();
    }

    // 6. Añadir CORTES
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
    if (cantidad > 0) {
        system("cls");
        printf("--- Celdas de corte ubicadas ---\n\n");
        mostrarMapa(mapa, -1, -1);
        printf("\nPresiona cualquier tecla para continuar...");
        _getch();
    }
}


/**
 * Función base que crea la ciudad (calles y casas).
 * Guarda X, Y en la celda.
 */
void inicializarMapaConCiudad(Celda mapa[FILAS][COLUMNAS]) {
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            mapa[i][j].y = i;
            mapa[i][j].x = j;

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

            mapa[i][j].g = INT_MAX; // Inicializar g a infinito
            mapa[i][j].h = 0;
            mapa[i][j].f = 0;
            mapa[i][j].visitado = 0;
            mapa[i][j].padre_x = -1;
            mapa[i][j].padre_y = -1;
        }
    }
}

/**
 * Muestra el mapa en pantalla.
 */
void mostrarMapa(Celda mapa[FILAS][COLUMNAS], int cursor_y, int cursor_x) {
    system("cls");
    printf("                  MAPA DE LA CIUDAD\n"); 

    printf("   %c", 218); // Esquina sup-izq
    for (int i = 0; i < COLUMNAS; i++) printf("%c%c%c", 196, 196, 196); // Linea hor
    printf("%c%c\n", 196, 191); // Esquina sup-der

    for (int i = 0; i < FILAS; i++) {
        printf("   %c ", 179); // Linea ver
        for (int j = 0; j < COLUMNAS; j++) {
            if (i == cursor_y && j == cursor_x) {
                printf("[%c]", mapa[i][j].tipo);
            } else {
                printf(" %c ", mapa[i][j].tipo);
            }
        }
        printf("%c\n", 179); // Linea ver
    }

    printf("   %c", 192); // Esquina inf-izq
    for (int i = 0; i < COLUMNAS; i++) printf("%c%c%c", 196, 196, 196); // Linea hor
    printf("%c%c\n", 196, 217); // Esquina inf-der
}

/**
 * Permite al usuario mover un cursor y seleccionar una celda.
 */
void seleccionarCelda(Celda mapa[FILAS][COLUMNAS], int *cursor_y, int *cursor_x, const char *mensaje) {
    int tecla;
    while (1) {
        mostrarMapa(mapa, *cursor_y, *cursor_x);
        printf("\n%s\n", mensaje);
        printf("Solo puedes seleccionar celdas transitables ('.', 'B', 'P', 'I', 'F')\n");
        printf("Tipo de celda actual: '%c' (Costo: %d)\n", mapa[*cursor_y][*cursor_x].tipo, mapa[*cursor_y][*cursor_x].costo);

        tecla = _getch();
        if (tecla == 224 || tecla == 0) { // Flechas
            tecla = _getch();
            switch (tecla) {
                case TECLA_ARRIBA:    if (*cursor_y > 0) (*cursor_y)--; break;
                case TECLA_ABAJO:     if (*cursor_y < FILAS - 1) (*cursor_y)++; break;
                case TECLA_IZQUIERDA: if (*cursor_x > 0) (*cursor_x)--; break;
                case TECLA_DERECHA:   if (*cursor_x < COLUMNAS - 1) (*cursor_x)++; break;
            }
        } else if (tecla == TECLA_ENTER) {
            if (mapa[*cursor_y][*cursor_x].costo != -1) {
                if (mapa[*cursor_y][*cursor_x].tipo == INICIO) {
                    printf("\n\aError: No puedes seleccionar la celda de INICIO.");
                    _getch();
                } else {
                    break;
                }
            } else {
                printf("\n\aError: No puedes seleccionar una casa ('H') o un corte ('X').");
                _getch();
            }
        }
    }
}

/**
 * Limpia el buffer de entrada (stdin).
 */
void limpiarBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


// ------------------------------------------------------------------
// ---- FUNCIONES DE A* (Robustas) ----
// ------------------------------------------------------------------

void agregarAListaAbierta(Celda* celda) {
    for (int i = 0; i < numNodosAbiertos; i++) {
        if (listaAbierta[i] == celda) return; 
    }
    if (numNodosAbiertos < MAX_NODOS) {
        listaAbierta[numNodosAbiertos++] = celda;
    }
}

Celda* obtenerCeldaConMenorF() {
    if (numNodosAbiertos == 0) return NULL; 
    int mejorIndice = 0;
    for (int i = 1; i < numNodosAbiertos; i++) {
        if (listaAbierta[i]->f < listaAbierta[mejorIndice]->f) {
            mejorIndice = i;
        }
    }
    Celda* mejorCelda = listaAbierta[mejorIndice];
    listaAbierta[mejorIndice] = listaAbierta[numNodosAbiertos - 1];
    numNodosAbiertos--;
    return mejorCelda;
}

int calcularHeuristica(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

void reconstruirYMarcarCamino(Celda mapa[FILAS][COLUMNAS], int inicio_x, int inicio_y, int fin_x, int fin_y) {
    int camino_x[MAX_NODOS], camino_y[MAX_NODOS];
    int contador = 0;
    int actual_x = fin_x, actual_y = fin_y;

    if (actual_x == inicio_x && actual_y == inicio_y) {
        printf("El inicio y el fin son la misma celda.\n");
        printf("(%d, %d)\n\n", inicio_x, inicio_y);
        return;
    }
    if (mapa[actual_y][actual_x].padre_x == -1) {
         printf("Error: El nodo final no tiene padre. No se puede reconstruir.\n");
         return;
    }

    while (actual_x != inicio_x || actual_y != inicio_y) {
        if (contador >= MAX_NODOS) {
             printf("Error: Camino demasiado largo (bucle infinito?).\n");
             return;
        }
        camino_x[contador] = actual_x;
        camino_y[contador] = actual_y;
        contador++;
        int temp_x = mapa[actual_y][actual_x].padre_x;
        int temp_y = mapa[actual_y][actual_x].padre_y;
        if (temp_x == -1 && temp_y == -1) {
             printf("Error: Se perdio la ruta del padre en (%d, %d).\n", actual_x, actual_y);
             return; 
        }
        actual_x = temp_x;
        actual_y = temp_y;
    }
    
    camino_x[contador] = inicio_x;
    camino_y[contador] = inicio_y;

    printf("Camino (Inicio -> Fin):\n");
    for (int i = contador; i >= 0; i--) {
        printf("(%d, %d)", camino_x[i], camino_y[i]);
        if (mapa[camino_y[i]][camino_x[i]].tipo != INICIO && 
            mapa[camino_y[i]][camino_x[i]].tipo != FIN) {
            mapa[camino_y[i]][camino_x[i]].tipo = CAMINO;
        }
        if (i > 0) printf(" -> ");
    }
    printf("\n\n");
}


int buscarCaminoAEstrella(Celda mapa[FILAS][COLUMNAS], int inicio_x, int inicio_y, int fin_x, int fin_y) {
    numNodosAbiertos = 0;
    
    // Resetear los valores de A* para cada búsqueda
    for (int y = 0; y < FILAS; y++) {
        for (int x = 0; x < COLUMNAS; x++) {
            mapa[y][x].g = INT_MAX;
            mapa[y][x].h = 0;
            mapa[y][x].f = 0;
            mapa[y][x].visitado = 0;
            mapa[y][x].padre_x = -1;
            mapa[y][x].padre_y = -1;
        }
    }

    if (inicio_x == fin_x && inicio_y == fin_y) {
        return 1; 
    }

    Celda* inicio = &mapa[inicio_y][inicio_x];
    Celda* fin = &mapa[fin_y][fin_x];
    inicio->g = 0;
    inicio->h = calcularHeuristica(inicio_x, inicio_y, fin_x, fin_y);
    inicio->f = inicio->g + inicio->h;
    agregarAListaAbierta(inicio);

    while (numNodosAbiertos > 0) {
        Celda* actual = obtenerCeldaConMenorF();
        if (actual == NULL) { 
            break;
        }
        if (actual == fin) {
            return 1; // ¡Camino encontrado!
        }
        actual->visitado = 1;

        // Usamos los valores guardados en la celda
        int actual_x = actual->x;
        int actual_y = actual->y;

        int vecinos_xy[4][2] = {
            {actual_x, actual_y - 1}, {actual_x, actual_y + 1},
            {actual_x - 1, actual_y}, {actual_x + 1, actual_y}
        };

        for (int i = 0; i < 4; i++) {
            int v_x = vecinos_xy[i][0], v_y = vecinos_xy[i][1];
            if (v_x < 0 || v_x >= COLUMNAS || v_y < 0 || v_y >= FILAS) continue;
            Celda* vecino = &mapa[v_y][v_x];
            if (vecino->costo == -1 || vecino->visitado == 1) continue;

            int nuevoCostoG = actual->g + vecino->costo;
            if (nuevoCostoG < vecino->g) {
                vecino->padre_x = actual_x;
                vecino->padre_y = actual_y;
                vecino->g = nuevoCostoG;
                vecino->h = calcularHeuristica(v_x, v_y, fin_x, fin_y);
                vecino->f = vecino->g + vecino->h;
                agregarAListaAbierta(vecino);
            }
        }
    }
    return 0; // No se encontró camino
}