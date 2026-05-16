/*
    Universidad de las Américas (UDLA)
    Carrera: Ingeniería de Software
    Materia: Programación 1
    Proyecto: Sistema de Optimización de Producción (SOP)
    Autores: Kevin Alexander Egas Pozo,
             Cesar Yunga,
             Christian Fiallos,
             Jardi Caseres.

    HISTORIAL DE CAMBIOS:
    2026-05-15  KEP  Creación del proyecto SOP, constantes y variables locales.
    2026-05-15  CRF  Implementación de paso por referencia y arreglos paralelos.
    2026-05-15  JAC  Desarrollo de lógica de búsqueda y eliminación sin structs.
    2026-05-15  COY  Corrección de interfaz y cálculo de viabilidad.

    EJECUTAR PROYECTO:
    Ingresa los siguientes comandos en la terminal de Visual Studio Code:
    
    gcc main.c -o main.exe
    ./main.exe

    -------------------------------------------------------------------
    INSTRUCCIONES PARA SUBIR A GITHUB:
    Abre una nueva terminal en la carpeta de tu proyecto y ejecuta paso a paso:
    
    1. git init
    2. git add .
    3. git commit -m "Primer commit: Desarrollo completo del sistema SOP en C"
    4. git branch -M main
    5. git remote add origin https://github.com/KevinEgas174/Sistema-de-Optimizaci-n-de-Producci-n-SOP-.git
    6. git push -u origin main
    -------------------------------------------------------------------

    -------------------------------------------------------------------
    INSTRUCCIONES PARA ACTUALIZAR EN GITHUB (Nuevos Cambios):
    Abre tu terminal en la carpeta del proyecto y ejecuta:
    
    1. git add .
    2. git commit -m "Se agregaron validaciones robustas para evitar letras y negativos"
    3. git push origin main
    -------------------------------------------------------------------
*/

#include <stdio.h>
#include <string.h>
#include <windows.h>

#define MAX_PROD 5
#define MAX_LEN 50

// Definimos colores en C
#define CLR_RESET   "\033[0m"
#define CLR_CIAN    "\033[1;36m"
#define CLR_VERDE   "\033[1;32m"
#define CLR_AMARILLO "\033[1;33m"
#define CLR_BLANCO  "\033[1;37m"
#define CLR_ROJO    "\033[1;31m"

// --- PROTOTIPOS DE FUNCIONES ---
void limpiarBuffer();
int leerEnteroPositivo(const char *mensaje);
float leerFlotantePositivo(const char *mensaje);

void ingresarLimites(float *tiempo_max, int *recursos_max);
void ingresarProducto(char nombres[][MAX_LEN], int *demanda, float *tiempo, int *recursos, int *num_prod);
int buscarProducto(char nombres[][MAX_LEN], int num_prod, const char *nombre_buscar);
void editarProducto(char nombres[][MAX_LEN], int *demanda, float *tiempo, int *recursos, int num_prod);
void eliminarProducto(char nombres[][MAX_LEN], int *demanda, float *tiempo, int *recursos, int *num_prod);
void calcularYVerificar(int *demanda, float *tiempo, int *recursos, int num_prod, float tiempo_max, int recursos_max);
void mostrarInventario(char nombres[][MAX_LEN], int *demanda, float *tiempo, int *recursos, int num_prod);

int main() {
    // Caracteres raros del menu en windows
    SetConsoleOutputCP(CP_UTF8);

    char nombres[MAX_PROD][MAX_LEN];
    int demanda[MAX_PROD];
    float tiempo_unidad[MAX_PROD];
    int recursos_unidad[MAX_PROD];
    
    int num_productos = 0; 
    float tiempo_maximo = 0.0;
    int recursos_maximos = 0;
    int opcion;
    int status;

    printf("\033[H\033[J"); // Limpia la pantalla solo al iniciar
    printf(CLR_CIAN "=================================================================\n");
    printf("   BIENVENIDO AL SISTEMA DE OPTIMIZACIÓN DE PRODUCCIÓN (SOP)     \n");
    printf("=================================================================\n" CLR_RESET);
    ingresarLimites(&tiempo_maximo, &recursos_maximos);

    do {
        printf("\033[H\033[J"); // Limpia la pantalla en cada iteración del menú

        // Marco matemáticamente alineado (56 caracteres internos)
        printf(CLR_CIAN "┌────────────────────────────────────────────────────────┐\n");
        printf("│" CLR_BLANCO "               M E N U   P R I N C I P A L              " CLR_CIAN "│\n");
        printf("├────────────────────────────────────────────────────────┤\n");
        printf("│ " CLR_VERDE "[1]" CLR_RESET " Ingresar un producto                               " CLR_CIAN "│\n");
        printf("│ " CLR_VERDE "[2]" CLR_RESET " Mostrar productos ingresados                       " CLR_CIAN "│\n");
        printf("│ " CLR_VERDE "[3]" CLR_RESET " Editar un producto                                 " CLR_CIAN "│\n");
        printf("│ " CLR_VERDE "[4]" CLR_RESET " Eliminar un producto                               " CLR_CIAN "│\n");
        printf("│ " CLR_VERDE "[5]" CLR_RESET " Calcular totales y verificar viabilidad            " CLR_CIAN "│\n");
        printf("│ " CLR_ROJO "[6]" CLR_RESET " Salir                                              " CLR_CIAN "│\n");
        printf("└────────────────────────────────────────────────────────┘\n" CLR_RESET);
        
        printf(CLR_AMARILLO "\nIngrese una opcion: " CLR_RESET);
        
        status = scanf("%d", &opcion);
        limpiarBuffer();
        if (status != 1) {
            opcion = -1; 
        }

        printf("\n");
        
        switch(opcion) {
            case 1:
                ingresarProducto(nombres, demanda, tiempo_unidad, recursos_unidad, &num_productos);
                break;
            case 2:
                mostrarInventario(nombres, demanda, tiempo_unidad, recursos_unidad, num_productos);
                break;
            case 3:
                editarProducto(nombres, demanda, tiempo_unidad, recursos_unidad, num_productos);
                break;
            case 4:
                eliminarProducto(nombres, demanda, tiempo_unidad, recursos_unidad, &num_productos);
                break;
            case 5:
                calcularYVerificar(demanda, tiempo_unidad, recursos_unidad, num_productos, tiempo_maximo, recursos_maximos);
                break;
            case 6:
                printf(CLR_AMARILLO "\nSaliendo del sistema SOP... ¡Hasta pronto!\n" CLR_RESET);
                break;
            default:
                printf(CLR_ROJO "\n[!] Opcion no valida. Por favor, ingrese un numero del 1 al 6.\n" CLR_RESET);
        }

        if (opcion != 6) {
            printf(CLR_AMARILLO "\nPresione ENTER para continuar..." CLR_RESET);
            getchar();
        }

    } while(opcion != 6);
    
    return 0;
}

// --- DESARROLLO DE FUNCIONES ---

void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int leerEnteroPositivo(const char *mensaje) {
    int valor;
    int status;
    do {
        printf("%s", mensaje);
        status = scanf("%d", &valor);
        limpiarBuffer(); 
        
        if (status != 1 || valor < 0) {
            printf(CLR_ROJO "[!] Entrada invalida. Ingrese un numero entero positivo.\n" CLR_RESET);
        }
    } while (status != 1 || valor < 0);
    return valor;
}

float leerFlotantePositivo(const char *mensaje) {
    float valor;
    int status;
    do {
        printf("%s", mensaje);
        status = scanf("%f", &valor);
        limpiarBuffer();
        
        if (status != 1 || valor < 0.0) {
            printf(CLR_ROJO "[!] Entrada invalida. Ingrese un numero decimal positivo.\n" CLR_RESET);
        }
    } while (status != 1 || valor < 0.0);
    return valor;
}

void ingresarLimites(float *tiempo_max, int *recursos_max) {
    printf(CLR_AMARILLO "\nConfiguracion inicial de la Fabrica:\n" CLR_RESET);
    *tiempo_max = leerFlotantePositivo(CLR_VERDE "Ingrese el tiempo maximo disponible (horas): " CLR_RESET);
    *recursos_max = leerEnteroPositivo(CLR_VERDE "Ingrese los recursos maximos disponibles (unidades): " CLR_RESET);
    printf(CLR_CIAN "[i] Limites configurados exitosamente.\n" CLR_RESET);
}

void ingresarProducto(char nombres[][MAX_LEN], int *demanda, float *tiempo, int *recursos, int *num_prod) {
    if (*num_prod >= MAX_PROD) {
        printf(CLR_ROJO "\n[!] Error: Se alcanzo el limite maximo de %d productos.\n" CLR_RESET, MAX_PROD);
        return;
    }

    int idx = *num_prod;

    printf(CLR_AMARILLO "\n--- Ingreso de Producto %d ---\n" CLR_RESET, idx + 1);
    printf(CLR_VERDE "Nombre del producto: " CLR_RESET);
    fgets(nombres[idx], MAX_LEN, stdin);
    nombres[idx][strcspn(nombres[idx], "\n")] = 0; 

    demanda[idx] = leerEnteroPositivo(CLR_VERDE "Cantidad demandada: " CLR_RESET);
    tiempo[idx] = leerFlotantePositivo(CLR_VERDE "Tiempo de fabricacion por unidad (horas): " CLR_RESET);
    recursos[idx] = leerEnteroPositivo(CLR_VERDE "Recursos requeridos por unidad: " CLR_RESET);

    (*num_prod)++; 
    printf(CLR_CIAN "[i] Producto agregado correctamente.\n" CLR_RESET);
}

int buscarProducto(char nombres[][MAX_LEN], int num_prod, const char *nombre_buscar) {
    for (int i = 0; i < num_prod; i++) {
        if (strcmp(nombres[i], nombre_buscar) == 0) {
            return i; 
        }
    }
    return -1; 
}

void editarProducto(char nombres[][MAX_LEN], int *demanda, float *tiempo, int *recursos, int num_prod) {
    if (num_prod == 0) {
        printf(CLR_ROJO "\n[!] No hay productos ingresados para editar.\n" CLR_RESET);
        return;
    }

    // 1. Mostrar la lista de productos antes de pedir el nombre
    mostrarInventario(nombres, demanda, tiempo, recursos, num_prod);

    char nombre_buscar[MAX_LEN];
    printf(CLR_AMARILLO "\nIngrese el nombre exacto del producto a editar: " CLR_RESET);
    fgets(nombre_buscar, MAX_LEN, stdin);
    nombre_buscar[strcspn(nombre_buscar, "\n")] = 0;

    int indice = buscarProducto(nombres, num_prod, nombre_buscar);

    if (indice != -1) {
        int opcion_edicion;
        int status;
        
        // 2. Submenú de edición con bucle de validación
        do {
            printf(CLR_CIAN "\n[Encontrado] - Seleccione que datos de '%s' le gustaria cambiar:\n" CLR_RESET, nombres[indice]);
            printf("  " CLR_VERDE "[1]" CLR_RESET " Nuevo nombre\n");
            printf("  " CLR_VERDE "[2]" CLR_RESET " Nueva cantidad demandada\n");
            printf("  " CLR_VERDE "[3]" CLR_RESET " Nuevo tiempo (horas)\n");
            printf("  " CLR_VERDE "[4]" CLR_RESET " Nuevos recursos\n");
            printf("  " CLR_ROJO "[5]" CLR_RESET " Terminar edicion (Salir)\n");
            printf(CLR_AMARILLO "Opcion: " CLR_RESET);

            status = scanf("%d", &opcion_edicion);
            limpiarBuffer();
            if (status != 1) {
                opcion_edicion = -1; // Fuerza el default si se ingresan letras
            }

            switch(opcion_edicion) {
                case 1:
                    printf(CLR_VERDE "Nuevo nombre: " CLR_RESET);
                    fgets(nombres[indice], MAX_LEN, stdin);
                    nombres[indice][strcspn(nombres[indice], "\n")] = 0;
                    printf(CLR_CIAN "[i] Nombre actualizado correctamente.\n" CLR_RESET);
                    break;
                case 2:
                    demanda[indice] = leerEnteroPositivo(CLR_VERDE "Nueva cantidad demandada: " CLR_RESET);
                    printf(CLR_CIAN "[i] Demanda actualizada correctamente.\n" CLR_RESET);
                    break;
                case 3:
                    tiempo[indice] = leerFlotantePositivo(CLR_VERDE "Nuevo tiempo (horas): " CLR_RESET);
                    printf(CLR_CIAN "[i] Tiempo actualizado correctamente.\n" CLR_RESET);
                    break;
                case 4:
                    recursos[indice] = leerEnteroPositivo(CLR_VERDE "Nuevos recursos: " CLR_RESET);
                    printf(CLR_CIAN "[i] Recursos actualizados correctamente.\n" CLR_RESET);
                    break;
                case 5:
                    printf(CLR_CIAN "[i] Edicion finalizada regresando al menu principal...\n" CLR_RESET);
                    break;
                default:
                    printf(CLR_ROJO "[!] Opcion invalida. Por favor, ingrese un numero del 1 al 5.\n" CLR_RESET);
            }
        } while (opcion_edicion != 5);

    } else {
        printf(CLR_ROJO "\n[!] Producto no encontrado.\n" CLR_RESET);
    }
}

void eliminarProducto(char nombres[][MAX_LEN], int *demanda, float *tiempo, int *recursos, int *num_prod) {
    // 1. PRIMERO verificamos si hay productos (antes de intentar mostrar nada)
    if (*num_prod == 0) {
        printf(CLR_ROJO "\n[!] No hay productos ingresados para eliminar.\n" CLR_RESET);
        return;
    }

    // 2. AHORA mostramos la lista.
    // OJO: Usamos *num_prod para enviar el VALOR (ej. 1, 2, 3), no la dirección de memoria.
    mostrarInventario(nombres, demanda, tiempo, recursos, *num_prod);

    // 3. Pedimos el dato a eliminar
    char nombre_buscar[MAX_LEN];
    printf(CLR_AMARILLO "\nIngrese el nombre exacto del producto a eliminar: " CLR_RESET);
    fgets(nombre_buscar, MAX_LEN, stdin);
    nombre_buscar[strcspn(nombre_buscar, "\n")] = 0;

    int indice = buscarProducto(nombres, *num_prod, nombre_buscar);

    if (indice != -1) {
        // Desplazamiento de memoria para borrar
        for (int i = indice; i < (*num_prod) - 1; i++) {
            strcpy(nombres[i], nombres[i + 1]);
            demanda[i] = demanda[i + 1];
            tiempo[i] = tiempo[i + 1];
            recursos[i] = recursos[i + 1];
        }
        (*num_prod)--; // Reducimos la cantidad de productos
        printf(CLR_CIAN "\n[i] Producto eliminado exitosamente.\n" CLR_RESET);
    } else {
        printf(CLR_ROJO "\n[!] Producto no encontrado.\n" CLR_RESET);
    }
}

void calcularYVerificar(int *demanda, float *tiempo, int *recursos, int num_prod, float tiempo_max, int recursos_max) {
    if (num_prod == 0) {
        printf(CLR_ROJO "\n[!] No hay productos para calcular.\n" CLR_RESET);
        return;
    }

    float tiempo_total_calculado = 0.0;
    int recursos_totales_calculados = 0;

    for (int i = 0; i < num_prod; i++) {
        tiempo_total_calculado += demanda[i] * tiempo[i];
        recursos_totales_calculados += demanda[i] * recursos[i];
    }

    printf(CLR_AMARILLO "\n--- RESULTADOS DE PRODUCCION ---\n" CLR_RESET);
    printf("Tiempo total requerido: " CLR_BLANCO "%.2f horas" CLR_RESET " (Limite: %.2f)\n", tiempo_total_calculado, tiempo_max);
    printf("Recursos totales requeridos: " CLR_BLANCO "%d unidades" CLR_RESET " (Limite: %d)\n", recursos_totales_calculados, recursos_max);

    printf("\n>>> CONCLUSION DE VIABILIDAD: ");
    if (tiempo_total_calculado <= tiempo_max && recursos_totales_calculados <= recursos_max) {
        printf(CLR_VERDE "LA FABRICA CUMPLE CON LA DEMANDA. <<<\n" CLR_RESET);
    } else {
        printf(CLR_ROJO "NO SE CUMPLE CON LA DEMANDA. EXCESO DE LIMITES. <<<\n" CLR_RESET);
    }
}

void mostrarInventario(char nombres[][MAX_LEN], int *demanda, float *tiempo, int *recursos, int num_prod) {
    if (num_prod == 0) {
        printf(CLR_ROJO "\n[!] El inventario esta vacio.\n" CLR_RESET);
        return;
    }

    printf(CLR_AMARILLO "\n--- LISTA DE PRODUCTOS (%d/%d) ---\n" CLR_RESET, num_prod, MAX_PROD);
    printf(CLR_CIAN "%-20s %-10s %-15s %-15s\n" CLR_RESET, "NOMBRE", "DEMANDA", "TIEMPO (h)", "RECURSOS");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < num_prod; i++) {
        printf("%-20s %-10d %-15.2f %-15d\n", nombres[i], demanda[i], tiempo[i], recursos[i]);
    }
}