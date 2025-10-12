
// Librerías necesarias para C++
#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;

// Estructura para cada pasajero (nodo de la lista)
struct Pasajero {
    char tipo_documento[10];
    char apellido[30];
    Pasajero *siguiente;
};
    // ...existing code...

    int main() {
        int capacidad, opcion;
        int total = 0;
        Pasajero *inicio = NULL, *fin = NULL, *nuevo = NULL, *aux = NULL; // Solo punteros

        cout << "Ingrese la capacidad maxima del avion" << endl;
        cin >> capacidad;

        int limite = capacidad + (capacidad * 0.10);

        do {
            cout << endl << "MENU AEROLINEA" << endl;
            cout << "1 Registrar pasajero FIFO" << endl;
            cout << "2 Mostrar lista de pasajeros registrados" << endl;
            cout << "3 Mostrar orden de abordaje y lista de espera" << endl;
            cout << "4 Salir" << endl;
            cout << "Seleccione una opcion" << endl;
            cin >> opcion;

            switch (opcion) {
                case 1:
                    if (total >= limite) {
                        cout << "Ya se alcanzo el limite maximo de pasajeros " << limite << endl;
                    } else {
                        nuevo = (Pasajero*) malloc(sizeof(Pasajero));
                        if (nuevo == NULL) {
                            cout << "Error de memoria" << endl;
                            break;
                        }
                        cout << "Ingrese tipo de documento CC TI PA" << endl;
                        cin >> nuevo->tipo_documento;
                        cout << "Ingrese primer apellido" << endl;
                        cin >> nuevo->apellido;
                        nuevo->siguiente = NULL;

                        // Agregar el nuevo pasajero al final de la lista
                        if (inicio == NULL) {
                            inicio = nuevo;
                            fin = nuevo;
                        } else {
                            fin->siguiente = nuevo;
                            fin = nuevo;
                        }
                        total++;
                        cout << "Pasajero registrado exitosamente " << total << " de " << limite << endl;
                        cout << "Turno asignado FIFO " << total << endl;
                    }
                    break;

                case 2:
                    if (total == 0) {
                        cout << "No hay pasajeros registrados" << endl;
                    } else {
                        cout << "LISTA DE PASAJEROS REGISTRADOS FIFO" << endl;
                        int turno = 1;
                        aux = inicio; // Usar auxiliar para recorrer
                        while (aux != NULL) {
                            cout << "Turno " << turno << " " << aux->tipo_documento << " " << aux->apellido << endl;
                            turno++;
                            aux = aux->siguiente;
                        }
                    }
                    break;

                case 3:
                    if (total == 0) {
                        cout << "No hay pasajeros registrados" << endl;
                    } else {
                        cout << "PASAJEROS QUE ABORDAN FIFO" << endl;
                        int turno = 1;
                        aux = inicio; // Usar auxiliar para recorrer
                        for (int i = 0; i < capacidad && aux != NULL; i++) {
                            cout << "Turno " << turno << " " << aux->tipo_documento << " " << aux->apellido << endl;
                            turno++;
                            aux = aux->siguiente;
                        }
                        if (total > capacidad) {
                            cout << "LISTA DE ESPERA FIFO" << endl;
                            for (int i = capacidad; i < total && aux != NULL; i++) {
                                cout << "Turno " << turno << " " << aux->tipo_documento << " " << aux->apellido << endl;
                                turno++;
                                aux = aux->siguiente;
                            }
                        }
                    }
                    break;

                case 4:
                    cout << "Saliendo del programa" << endl;
                    break;

                default:
                    cout << "Opcion invalida" << endl;
                    break;
            }

        } while (opcion != 4);

    // Liberar memoria
    // Liberar memoria usando auxiliar
    aux = inicio;
    while (aux != NULL) {
        Pasajero* temp = aux;
        aux = aux->siguiente;
        free(temp);
    }

    return 0;
}