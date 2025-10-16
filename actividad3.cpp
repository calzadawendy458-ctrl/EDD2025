#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_DOC_TYPE 110
#define MAX_APELLIDO 20
#define DOC_CC "CC"
#define DOC_TI "TI"
#define DOC_PA "PA"

// Estructura para cada pasajero (nodo de la lista)
typedef struct pasajero {
    char tipo_documento[MAX_DOC_TYPE];
    char apellido[MAX_APELLIDO];
    int genero; // 1: Masculino, 2: Femenino, 3: No binario
    struct pasajero *siguiente;
} Pasajero;

// Estructura para la lista (Cola FIFO)
typedef struct {
    Pasajero *inicio; // Frente (head) - para abordar
    Pasajero *fin;    // Final (tail) - para registrar
    int total_registrados; // Total de pasajeros en la lista de espera
} ColaPasajeros;

// Función auxiliar para limpiar el buffer después de scanf
void limpiar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Inicializar la cola
void inicializar_cola(ColaPasajeros *c) {
    c->inicio = NULL;
    c->fin = NULL;
    c->total_registrados = 0;
}

// Registrar pasajero (Encolar / Agregar al final - FIFO)
// Retorna 1 si es exitoso, 0 si hay error de memoria
int registrar_pasajero(ColaPasajeros *c, const char *tipo_doc, const char *apellido, int genero) {
    Pasajero *nuevo = (Pasajero *)malloc(sizeof(Pasajero));
    if (nuevo == NULL) {
        return 0; // Error de memoria
    }

    // Copiar los datos (usando strncpy para seguridad)
    strncpy(nuevo->tipo_documento, tipo_doc, sizeof(nuevo->tipo_documento) - 1);
    nuevo->tipo_documento[sizeof(nuevo->tipo_documento) - 1] = '\0';
    strncpy(nuevo->apellido, apellido, sizeof(nuevo->apellido) - 1);
    nuevo->apellido[sizeof(nuevo->apellido) - 1] = '\0';
    nuevo->genero = genero;
    nuevo->siguiente = NULL;

    // Enlazar el nodo al final de la cola
    if (c->inicio == NULL) {
        c->inicio = nuevo;
        c->fin = nuevo;
    } else {
        c->fin->siguiente = nuevo;
        c->fin = nuevo;
    }

    c->total_registrados++;
    return 1;
}

// Abordar pasajero (Desencolar / Eliminar del frente - FIFO)
// Retorna el nodo del pasajero abordado o NULL si la cola está vacía
Pasajero *abordar_pasajero(ColaPasajeros *c) {
    if (c->inicio == NULL) {
        return NULL;
    }

    Pasajero *abordado = c->inicio;
    c->inicio = c->inicio->siguiente;

    if (c->inicio == NULL) {
        c->fin = NULL; // La lista quedó vacía
    }

    c->total_registrados--;
    abordado->siguiente = NULL;
    return abordado;
}

// Mostrar la lista de pasajeros registrados (en espera)
void mostrar_lista(ColaPasajeros *c) {
    if (c->inicio == NULL) {
        printf("No hay pasajeros registrados en espera.\n");
        return;
    }

    printf("\n--- LISTA DE PASAJEROS REGISTRADOS (FIFO) ---\n");
    Pasajero *actual = c->inicio;
    int turno = 1;

    while (actual != NULL) {
        char *genero_str = "Desconocido";
        if (actual->genero == 1) genero_str = "Masculino";
        else if (actual->genero == 2) genero_str = "Femenino";
        else if (actual->genero == 3) genero_str = "No binario";

        printf("Turno %3d | Doc: %s | Apellido: %s | Género: %s\n",
               turno, actual->tipo_documento, actual->apellido, genero_str);
        turno++;
        actual = actual->siguiente;
    }
}

// Liberar toda la memoria al salir
void liberar_memoria(ColaPasajeros *c) {
    Pasajero *actual = c->inicio;
    while (actual != NULL) {
        Pasajero *temp = actual;
        actual = actual->siguiente;
        free(temp);
    }
    c->inicio = NULL;
    c->fin = NULL;
    c->total_registrados = 0;
}

// Función para solicitar el género al usuario
int solicitar_genero() {
    int genero;
    do {
        printf("Ingrese genero (1: Masculino, 2: Femenino, 3: No binario): ");
        if (scanf("%d", &genero) != 1) {
            limpiar_buffer();
            printf("Entrada inválida. Intente de nuevo.\n");
            continue;
        }
        limpiar_buffer();
        if (genero < 1 || genero > 3) {
            printf("Genero no válido. Intente de nuevo.\n");
        }
    } while (genero < 1 || genero > 3);
    return genero;
}

// Función de informe de género con tres recorridos separados
void informe_genero(ColaPasajeros *c) {
    int masc = 0, fem = 0, nb = 0;
    Pasajero *actual;

    // Primer recorrido: Masculino
    actual = c->inicio;
    while (actual != NULL) {
        if (actual->genero == 1) masc++;
        actual = actual->siguiente;
    }

    // Segundo recorrido: Femenino
    actual = c->inicio;
    while (actual != NULL) {
        if (actual->genero == 2) fem++;
        actual = actual->siguiente;
    }

    // Tercer recorrido: No binario
    actual = c->inicio;
    while (actual != NULL) {
        if (actual->genero == 3) nb++;
        actual = actual->siguiente;
    }

    // Mostrar resultados
    printf("\n--- INFORME DE GÉNERO DE PASAJEROS EN ESPERA ---\n");
    printf("Masculino : %d\n", masc);
    printf("Femenino  : %d\n", fem);
    printf("No binario: %d\n", nb);
}

int main() {
    ColaPasajeros cola;
    inicializar_cola(&cola);

    int capacidad;
    int opcion;
    int abordados = 0; // Contador de pasajeros que ya subieron al avión

    printf(" SISTEMA DE GESTIÓN DE VUELO (LENGUAJE C) \n");
    printf("Ingrese la capacidad maxima del avion: ");

    if (scanf("%d", &capacidad) != 1 || capacidad <= 0) {
        printf("Error: Capacidad no valida.\n");
        return 1;
    }
    limpiar_buffer(); // Limpiar el buffer

    int limite = capacidad + (int)(capacidad * 0.10); // Límite con overbooking

    printf("Capacidad real: %d asientos.\n", capacidad);
    printf("Limite de registro (con 10%% overbooking): %d\n", limite);

    do {
        printf("\n------ MENU AEROLINEA ------\n");
        printf("1) Registrar pasajero (FIFO)\n");
        printf("2) Mostrar pasajeros registrados (Lista de espera)\n");
        printf("3) Abordar siguiente pasajero (FIFO)\n");
        printf("4) Abordar a todos hasta llenar el avion\n");
        printf("5) Mostrar estado del vuelo\n");
        printf("6) Informe de genero de pasajeros registrados\n");
        printf("7) Salir\n");
        printf("Seleccione una opcion: ");

        if (scanf("%d", &opcion) != 1) {
            printf("Error: Entrada no valida. Intente de nuevo.\n");
            limpiar_buffer();
            continue;
        }
        limpiar_buffer();

        switch (opcion) {
            case 1: { // Registrar Pasajero
                if (cola.total_registrados >= limite) {
                    printf(" Ya se alcanzo el limite maximo de pasajeros (%d).\n", limite);
                } else {
                    char tipo_doc[MAX_DOC_TYPE];
                    char apellido[MAX_APELLIDO];
                    int genero;

                    printf("Ingrese tipo de documento (%s, %s, %s): ", DOC_CC, DOC_TI, DOC_PA);
                    scanf("%s", tipo_doc);
                    limpiar_buffer();

                    printf("Ingrese primer apellido: ");
                    scanf("%s", apellido);
                    limpiar_buffer();

                    genero = solicitar_genero();

                    if (registrar_pasajero(&cola, tipo_doc, apellido, genero)) {
                        printf(" Pasajero registrado exitosamente. Turno asignado: %d.\n", cola.total_registrados);
                    } else {
                        printf(" Error de memoria al registrar.\n");
                    }
                }
                break;
            }

            case 2: // Mostrar Lista de Espera
                mostrar_lista(&cola);
                break;

            case 3: { // Abordar Siguiente Pasajero
                if (abordados >= capacidad) {
                    printf(" El avion ya esta lleno (%d/%d).\n", abordados, capacidad);
                } else {
                    Pasajero *p = abordar_pasajero(&cola);
                    if (p != NULL) {
                        abordados++;
                        printf(" Abordando: %s | %s. Asientos ocupados: %d/%d\n",
                               p->tipo_documento, p->apellido, abordados, capacidad);
                        free(p);
                    } else {
                        printf("No hay pasajeros en espera para abordar.\n");
                    }
                }
                break;
            }

            case 4: { // Abordar a todos hasta llenar
                int count = 0;
                while (abordados < capacidad) {
                    Pasajero *p = abordar_pasajero(&cola);
                    if (p == NULL)
                        break;

                    abordados++;
                    count++;
                    printf(" Abordando: %s  %s. Asientos ocupados: %d/%d\n",
                           p->tipo_documento, p->apellido, abordados, capacidad);
                    free(p);
                }

                if (count == 0 && abordados < capacidad)
                    printf("No hay pasajeros en espera para abordar.\n");
                else if (count > 0)
                    printf(" %d pasajeros abordaron. Total en el avion: %d/%d\n", count, abordados, capacidad);
                else if (abordados == capacidad)
                    printf("El avion ya esta lleno (%d/%d).\n", capacidad, capacidad);

                break;
            }

            case 5: // Mostrar estado del vuelo
                printf("\n--- ESTADO ACTUAL DEL VUELO ---\n");
                printf("Capacidad: %d asientos\n", capacidad);
                printf("Maximo de tickets (Overbooking): %d\n", limite);
                printf("Tickets vendidos (Registrados total): %d\n", cola.total_registrados + abordados);
                printf("Pasajeros Abordados (En el avion): %d\n", abordados);
                printf("Pasajeros En Espera (Lista FIFO): %d\n", cola.total_registrados);
                break;

            case 6: // Informe de género
                informe_genero(&cola);
                break;

            case 7:
                printf("Saliendo del programa...\n");
                break;

            default:
                printf("Opcion invalida. Intente de nuevo.\n");
                break;
        }

    } while (opcion != 7);

    liberar_memoria(&cola);
    printf("Memoria liberada. ¡Adios! \n");

    return 0;
}// Retorna el nodo del pasajero abordado o NULL si la cola está vacía
Pasajero *abordar_pasajero(ColaPasajeros *c) {
    if (c->inicio == NULL) {
        return NULL;
    }

    Pasajero *abordado = c->inicio;
    c->inicio = c->inicio->siguiente;

    if (c->inicio == NULL) {
        c->fin = NULL; // La lista quedó vacía
    }

    c->total_registrados--;
    abordado->siguiente = NULL;
    return abordado;
}

// Mostrar la lista de pasajeros registrados (en espera)
void mostrar_lista(ColaPasajeros *c) {
    if (c->inicio == NULL) {
        printf("No hay pasajeros registrados en espera.\n");
        return;
    }

    printf("\n--- LISTA DE PASAJEROS REGISTRADOS (FIFO) ---\n");
    Pasajero *actual = c->inicio;
    int turno = 1;

    while (actual != NULL) {
        printf("Turno %3d | Doc: %s | Apellido: %s\n",
               turno, actual->tipo_documento, actual->apellido);
        turno++;
        actual = actual->siguiente;
    }
}

// Liberar toda la memoria al salir
void liberar_memoria(ColaPasajeros *c) {
    Pasajero *actual = c->inicio;
    while (actual != NULL) {
        Pasajero *temp = actual;
        actual = actual->siguiente;
        free(temp);
    }
    c->inicio = NULL;
    c->fin = NULL;
    c->total_registrados = 0;
}


// Función Principal (Main)


int main() {
    ColaPasajeros cola;
    inicializar_cola(&cola);

    int capacidad;
    int opcion;
    int abordados = 0; // Contador de pasajeros que ya subieron al avión

    printf(" SISTEMA DE GESTIÓN DE VUELO (LENGUAJE C) \n");
    printf("Ingrese la capacidad maxima del avion: ");

    if (scanf("%d", &capacidad) != 1 || capacidad <= 0) {
        printf("Error: Capacidad no valida.\n");
        return 1;
    }
    limpiar_buffer(); // Limpiar el buffer

    int limite = capacidad + (int)(capacidad * 0.10); // Límite con overbooking

    printf("Capacidad real: %d asientos.\n", capacidad);
    printf("Limite de registro (con 10%% overbooking): %d\n", limite);

    do {
        printf("\n------ MENU AEROLINEA ------\n");
        printf("1) Registrar pasajero (FIFO)\n");
        printf("2) Mostrar pasajeros registrados (Lista de espera)\n");
        printf("3) Abordar siguiente pasajero (FIFO)\n");
        printf("4) Abordar a todos hasta llenar el avion\n");
        printf("5) Mostrar estado del vuelo\n");
        printf("6) Salir\n");
        printf("Seleccione una opcion: ");

        if (scanf("%d", &opcion) != 1) {
            printf("Error: Entrada no valida. Intente de nuevo.\n");
            limpiar_buffer();
            continue;
        }
        limpiar_buffer();

        switch (opcion) {
            case 1: { // Registrar Pasajero
                if (cola.total_registrados >= limite) {
                    printf(" Ya se alcanzo el limite maximo de pasajeros (%d).\n", limite);
                } else {
                    char tipo_doc[MAX_DOC_TYPE];
                    char apellido[MAX_APELLIDO];
                    
                    // Nota: Se usa scanf("%s") que es simple, pero no maneja espacios.
                    printf("Ingrese tipo de documento (%s, %s, %s): ", DOC_CC, DOC_TI, DOC_PA); // Usando las constantes
                    scanf("%s", tipo_doc);
                    limpiar_buffer ();

                    printf("Ingrese primer apellido: ");
                    scanf("%s", apellido);
                    limpiar_buffer();

                    if (registrar_pasajero(&cola, tipo_doc, apellido)) {
                        printf(" Pasajero registrado exitosamente. Turno asignado: %d.\n", cola.total_registrados);
                    } else {
                        printf(" Error de memoria al registrar.\n");
                    }
                }
                break;
            }

            case 2: // Mostrar Lista de Espera
                mostrar_lista(&cola);
                break;

            case 3: { // Abordar Siguiente Pasajero
                if (abordados >= capacidad) {
                    printf(" El avion ya esta lleno (%d/%d).\n", abordados, capacidad);
                } else {
                    Pasajero *p = abordar_pasajero(&cola);
                    if (p != NULL) {
                        abordados++;
                        printf(" Abordando: %s | %s. Asientos ocupados: %d/%d\n", 
                               p->tipo_documento, p->apellido, abordados, capacidad);
                        free(p); // Liberar la memoria del nodo
                    } else {
                        printf("No hay pasajeros en espera para abordar.\n");
                    }
                }
                break;
            }

            case 4: { // Abordar a todos hasta llenar
                int count = 0;
                while (abordados < capacidad) {
                    Pasajero *p = abordar_pasajero(&cola);
                    if (p == NULL)
                        break;
                    
                    abordados++;
                    count++;
                    printf(" Abordando: %s  %s. Asientos ocupados: %d/%d\n", 
                           p->tipo_documento, p->apellido, abordados, capacidad);
                    free(p);
                }
                
                if (count == 0 && abordados < capacidad)
                    printf("No hay pasajeros en espera para abordar.\n");
                else if (count > 0)
                    printf(" %d pasajeros abordaron. Total en el avion: %d/%d\n", count, abordados, capacidad);
                else if (abordados == capacidad)
                    printf("El avion ya esta lleno (%d/%d).\n", capacidad, capacidad);
                
                break;
            }
            
            case 5: // Mostrar estado del vuelo
                printf("\n--- ESTADO ACTUAL DEL VUELO ---\n");
                printf("Capacidad: %d asientos\n", capacidad);
                printf("Maximo de tickets (Overbooking): %d\n", limite);
                printf("Tickets vendidos (Registrados total): %d\n", cola.total_registrados + abordados);
                printf("Pasajeros Abordados (En el avion): %d\n", abordados);
                printf("Pasajeros En Espera (Lista FIFO): %d\n", cola.total_registrados);
                break;

            case 6:
                printf("Saliendo del programa...\n");
                break;

            default:
                printf("Opcion invalida. Intente de nuevo.\n");
                break;
        }

    } while (opcion != 6);

    // Liberar memoria antes de salir
    liberar_memoria(&cola);
    printf("Memoria liberada. ¡Adios! \n");

    return 0;
}
