#include <iostream>

using namespace std;

// Estructura para la ficha de domino 
struct Ficha {
    int lado1;
    int lado2;
};

// Nodo para lista enlazada de fichas
struct NodoFicha {
    Ficha ficha;
    NodoFicha* siguiente;
};

// Lista enlazada de fichas
struct ListaFichas {
    NodoFicha* cabeza;
};

// Estructura de un jugador
struct Jugador {
    char nombre[30];
    ListaFichas fichas;
    int puntaje;
};

// Estructura de la mesa 
struct Mesa {
    ListaFichas fichas;
    Jugador jugadores[4];
    Jugador* jugadoresPtr = nullptr;
};

// inicializar lista de fichas
void inicializarLista(ListaFichas& lista) {
    lista.cabeza=NULL; // la lista vacia se representa con cabeza apuntando a null
}

bool estaVacia(const ListaFichas& lista) {
    return lista.cabeza==NULL; // comprueba si la lista esta vacia
}

void agregarFichaFinal(ListaFichas& lista, Ficha ficha) {
    NodoFicha* nuevo=new NodoFicha; // reserva memoria para un nuevo nodo
    nuevo->ficha=ficha;
    nuevo->siguiente = NULL; // el nuevo nodo siempre va al final, su siguiente es null
    if (lista.cabeza==NULL) {
        lista.cabeza=nuevo; // si estaba vacia, el nuevo nodo es la cabeza
    } else {
        NodoFicha* aux=lista.cabeza;
        while (aux->siguiente != NULL) {
            aux=aux->siguiente;
        }
        aux->siguiente=nuevo; // el anterior ultimo nodo apunta al nuevo
    }
}          

void mostrarFichas(const ListaFichas& lista) {
    NodoFicha* aux=lista.cabeza;
    while (aux!=NULL) { // recorre la lista hasta que el puntero sea null
        cout <<"Domino: " << aux->ficha.lado1 << "|" << aux->ficha.lado2 << " ";
        aux = aux->siguiente; // avanza al siguiente nodo
    }
    cout << endl;
}

int contarFichas(ListaFichas& lista) {
    int contador=0;
    NodoFicha* aux=lista.cabeza;
    while (aux != NULL) {
        contador++;
        aux=aux->siguiente;
    }
    return contador;
}

void generarFichas(ListaFichas& lista) {
    inicializarLista(lista);
    for (int i = 0; i <= 6; i++) { // genera el primer lado (0 a 6)
        for (int j = i; j <= 6; j++) { // genera el segundo lado (desde i hasta 6) para evitar duplicados
            Ficha ficha;
            ficha.lado1=i;
            ficha.lado2=j;
            agregarFichaFinal(lista, ficha);
        }
    }
}

void barajarFichas(ListaFichas& lista) {
    Ficha arreglo[28]; // se juegan 28 fichas en total
    int n=0;
    NodoFicha* aux=lista.cabeza;
    while (aux !=NULL && n < 28) { // copiar lista al arreglo
        arreglo[n++]=aux->ficha;
        aux=aux->siguiente;
    }
    // Debo invertir el arreglo ya que si no lo hago se van a generar siempre en el mismo orden y los jugadores van a recibir siempre las mismas piezas
    for (int i = 0; i < n / 2; ++i) {
        Ficha temp=arreglo[i];
        arreglo[i]=arreglo[n - i - 1];
        arreglo[n - i - 1]=temp; //guarda temporalmente el puntero al nodo que esta actualmente en la cabeza, asi puedo extraer la ficha 
                                  // y agregarla a la mano del jugador 
    }
    lista.cabeza = NULL; // vaciar la lista
    for (int i=0; i < n; i++) { // vuelve a meter fichas en la lista
        agregarFichaFinal(lista, arreglo[i]);
    }
}
void repartirFichas(ListaFichas& pozo, Jugador jugadores[], int numeroJugadores) { // se reparten las fichas
    int fichasporJugador=7; // cantidad de fichas por jugador
    for (int i=0; i < numeroJugadores; ++i) {
        jugadores[i].fichas.cabeza = NULL; // inicializando fichas del jugador
        for (int j = 0; j < fichasporJugador; ++j) {
            if (pozo.cabeza!= NULL) { // verifica que haya fichas en el pozo
                NodoFicha* temp=pozo.cabeza; // toma la ficha de la cabeza del pozo
                agregarFichaFinal(jugadores[i].fichas, temp->ficha); // la anade a la mano del jugador
                pozo.cabeza=temp->siguiente; // la cabeza del pozo avanza al siguiente nodo (eliminacion de la ficha del pozo)
                delete temp; // libera la memoria del nodo retirado del pozo
            }
        }
    }
}

void mostrarManos(Jugador jugadores[], int numeroJugadores) { // mostrar manos de todos los jugadores
    for (int i =0; i < numeroJugadores; i++) {
        cout << "La mano del jugador " << jugadores[i].nombre << "es:" << endl;
        mostrarFichas(jugadores[i].fichas);
        cout << endl;
    }
}

void agregarFichaInicio(ListaFichas& lista, Ficha ficha) { // agregar ficha al inicio de la lista
    NodoFicha* nuevo=new NodoFicha;
    nuevo->ficha=ficha;
    nuevo->siguiente=lista.cabeza; // el nuevo nodo apunta a la antigua cabeza
    lista.cabeza=nuevo; // el nuevo nodo se convierte en la nueva cabeza
}

bool obtenerExtremos(const ListaFichas& lista, int& izquierda, int& derecha) { // obtener extremos (lado izquierdo y derecho) de la mesa
    if (lista.cabeza==NULL) return false;
    NodoFicha* aux = lista.cabeza;
    izquierda = aux->ficha.lado1; // el lado izquierdo es el lado1 de la ficha en la cabeza
    while (aux->siguiente != NULL) {
        aux = aux->siguiente; // recorre hasta el ultimo nodo
    }
    derecha = aux->ficha.lado2; // el lado derecho es el lado2 de la ficha en la cola
    return true;
}

bool obtenerFichaPorIndice(const ListaFichas& lista, int indice, Ficha& out) { // obtener ficha por indice (sin eliminar)
    int i=0;
    NodoFicha* aux=lista.cabeza;
    while (aux!=NULL) {
        if (i==indice) {
            out=aux->ficha;
            return true;
        }
        aux=aux->siguiente;
        i++;
    }
    return false;
}

bool eliminarFichaPorIndice(ListaFichas& lista, int indice, Ficha& eliminada) { // eliminar ficha por indice y devolver ficha eliminada /out te permite saber cual ficha se elimino
    if (lista.cabeza==NULL) return false;
    NodoFicha* aux=lista.cabeza;
    NodoFicha* anterior=NULL;
    int i=0;
    while (aux!=NULL && i < indice) { // busca el nodo y guarda el anterior
        anterior=aux;
        aux=aux->siguiente;
        i++;
    }
    if (aux==NULL) return false;
    eliminada=aux->ficha;
    if (anterior==NULL) { // eliminar cabeza
        lista.cabeza=aux->siguiente; // la nueva cabeza es el siguiente del nodo a borrar
    } else {
        anterior->siguiente=aux->siguiente; // salta el nodo a eliminar y enlaza prev con el siguiente
    }
    delete aux; // libera la memoria del nodo eliminado
    return true;
}

void mostrarManoConIndices(const ListaFichas& lista) { // mostrar mano con indices
    NodoFicha* aux = lista.cabeza;
    int i = 0;
    while (aux != NULL) {
        cout << i << ") [" << aux->ficha.lado1 << "|" << aux->ficha.lado2 << "]  ";
        aux = aux->siguiente;
        i++;
    }
    cout << endl;
}
//Ve la ficha como datos: lado1=3 y lado2=5. La funcion ajustarOrientacion es la que "gira" los datos (la convierte en lado1=5 y lado2=3) 
// para que el programa sepa, de forma consistente, que lado conectar y cual es el nuevo extremo libre.
void ajustarOrientacionParaExtremo(Ficha& ficha, int extremo, bool colocarEnCabeza) { // ajustar orientacion para coincidir con extremo deseado
    if (colocarEnCabeza) {
        // queremos que el lado derecho de la ficha (lado2) coincida con el extremo izquierdo
        if (ficha.lado2==extremo){
            return;
        }
        if (ficha.lado1==extremo)  { //si mi lado1 coincide con el extremo de la mesa
            int tmp=ficha.lado1; //guardas la ficha temporalmente para no perder el valor de lado1 cuando lo reemplaces, , aqui guardo el valor original de lado1
            ficha.lado1=ficha.lado2; //tomo el valor del lado2 y ponlo en el lado1
            ficha.lado2=tmp; // realiza el intercambio de lados
        }
    } else {
        // colocar en cola lado izquierdo (lado1) coincida con extremo derecho
        if (ficha.lado1==extremo){
            return; 
        } 
        if (ficha.lado2==extremo) { //si mi lado2 coincide con el extremo de la mesa
            int tmp=ficha.lado1; //guardo el valor original de lado1 en tmp
            ficha.lado1=ficha.lado2;//tomo el valor del lado2 y lo pongo en el lado1
            ficha.lado2=tmp; // realiza el intercambio de lados
        }
    }
}

// verificar si el jugador tiene algun movimiento valido en la mesa, es decir si tiene al menos una ficha que pueda jugar o si debe pasar
bool jugadorTieneMovimiento(const Jugador &jugador, const ListaFichas &mesa) {
    int izquierda, derecha;
    if (!obtenerExtremos(mesa, izquierda, derecha)){
        //si la mesa esta vacia cualquier ficha es valida (se asume que el jugador tiene fichas)
        return true; 

    }
    //SI LA MESA NO ESTA VACIA RECORRER LA MANO DEL JUGADOR
        NodoFicha *aux=jugador.fichas.cabeza;
        while (aux!=NULL) {
            if(aux->ficha.lado1==izquierda or aux->ficha.lado2==izquierda or aux->ficha.lado1==derecha or aux->ficha.lado2 or derecha){
                return true; //si tengo al menos un aldo que coincida juego si no paso
 
            }
            aux=aux->siguiente;
    }
    return false;

    }
    

// sumar puntos de la mano del jugador
int sumarPuntosMano(const ListaFichas &lista) {
    int suma=0;
    NodoFicha* aux = lista.cabeza;
    while (aux != NULL) {
        suma+=aux->ficha.lado1 + aux->ficha.lado2; // suma los valores de ambos lados
        aux = aux->siguiente;
    }
    return suma;
}

// esta funcion sirve para encontrar cual de los jugadores tiene una ficha especifica en su mano 
//su uso en domino es saber quien empieza la partida, el jugador que tiene el doble seis [6|6] (o el doble mas alto) es el que comienza.
int encontrarJugadorConFicha(Jugador jugadores[], int numeroJugadores, Ficha objetivo) {
    for (int i=0; i < numeroJugadores; ++i) { // itera sobre todos los jugadores
        NodoFicha* aux=jugadores[i].fichas.cabeza;
        while (aux!=NULL) {
            if (aux->ficha.lado1==objetivo.lado1 && aux->ficha.lado2==objetivo.lado2) { // compara ficha actual con objetivo
                return i;
            }
            aux=aux->siguiente;
        }
    }
    return -1;
}

// quita la ficha de la mano del jugador cuando ya la jugo y la puso en la mesa, basicamente el programa la elimina para que no la pueda usar otra vez
bool eliminarFichaExacta(ListaFichas &lista, Ficha objetivo, Ficha &eliminada) {
    if (lista.cabeza==NULL) return false;
    NodoFicha* aux=lista.cabeza;
    NodoFicha* anterior= NULL;
    while (aux != NULL) {
        if (aux->ficha.lado1 == objetivo.lado1 && aux->ficha.lado2 == objetivo.lado2) { // busca la ficha exacta
            eliminada= aux->ficha;
            if (anterior== NULL){
                lista.cabeza = aux->siguiente; //elimino la cabeza

            }
            else{
                anterior->siguiente = aux->siguiente; // eliminacion (nodo intermedio/final)
            } 
            delete aux;
            return true;
        }
        anterior= aux;
        aux=aux->siguiente;
    }
    return false;
}

int main() {
    int numJugadores;
    cout << "Bienvenidos al juego de domino" << endl;
    cout << "Ingrese el numero de jugadores a participar entre (2-4): ";
    cin >> numJugadores;
    if (numJugadores < 2 or numJugadores > 4) { // validacion del numero de jugadores
        cout << "El numero de jugadores no es valido" << endl;
        cout<<"Intente nuevamente..."<<endl; 
        return 0;
    }
    // inicializar jugadores (ahora dinámicamente según numJugadores)
    Jugador* jugadores = new Jugador[numJugadores];
    cin.ignore(); // limpiar buffer antes de getline

    for (int i = 0; i < numJugadores; ++i) {
        cout << "Ingrese el nombre del jugador: " << (i + 1) << endl;
        cin.getline(jugadores[i].nombre, 30); // lee nombre como arreglo de char
        jugadores[i].puntaje = 0;
        jugadores[i].fichas.cabeza = NULL;
    }

    const int RONDAS = 3; //cantidad de rondas a jugar

    for (int ronda = 1; ronda <= RONDAS; ++ronda) {
        cout << "INICIA LA RONDA " << ronda << " !!!" << endl;
        // generar y barajar 
        ListaFichas pozo;
        pozo.cabeza = NULL;
        generarFichas(pozo);
        barajarFichas(pozo);

        // repartir fichas 
        repartirFichas(pozo, jugadores, numJugadores);

        // inicializar mesa
        Mesa mesa;
        mesa.fichas.cabeza = NULL;
        // enlazar la mesa con el arreglo de jugadores actual (referencia dinámica)
        mesa.jugadoresPtr = jugadores;

        // determinar quien inicia (igual que antes)
        Ficha doble66; doble66.lado1 = 6; doble66.lado2 = 6;
        int jugadorCon66 = -1; //-1 significa nadie lo tiene o aun no se ha buscado
        int jugadorActual = 0; // por defecto inicia jugador 1 (indice 0)
        if (numJugadores==4) {
            jugadorCon66=encontrarJugadorConFicha(jugadores, numJugadores, doble66); // busca quien tiene el 6|6
            if (jugadorCon66!=-1) {
                Ficha inicial;
                if (eliminarFichaExacta(jugadores[jugadorCon66].fichas, doble66, inicial)) { // quita el 6|6 de la mano
                    agregarFichaFinal(mesa.fichas, inicial); // lo pone en la mesa
                    cout << "El jugador " << jugadores[jugadorCon66].nombre << " coloca [6|6] como primera ficha en la mesa." << endl;
                    jugadorActual=(jugadorCon66 + 1) % numJugadores;
                } else {
                    cout << "No se pudo extraer [6|6] de la mano. La mesa queda vacia y comienza " << jugadores[0].nombre << "." << endl;
                    jugadorActual=0;
                }
            } else {
                cout << "Ningun jugador tiene [6|6]. La mesa queda vacia y comienza " << jugadores[0].nombre << "." << endl;
                jugadorActual=0;
            }
        } else {
            cout << "Modo " << numJugadores << " jugadores: la mesa queda vacia " << jugadores[0].nombre << " comienza." << endl;
            jugadorActual=0;
        }

        int pasesConsecutivos = 0; // contador para detectar bloqueo
        bool juegoTerminado = false;

        while (!juegoTerminado) { // bucle principal de la ronda
            Jugador& actual = jugadores[jugadorActual]; // referencia al jugador actual
            cout << "------------------------------------------" << endl;
            cout << "turno de: " << actual.nombre << endl;
            cout << "mesa: ";
            mostrarFichas(mesa.fichas);
            cout << "su mano: ";
            mostrarManoConIndices(actual.fichas);

            // extremos actuales
            int izquierda, derecha;
            bool mesaVacia = !obtenerExtremos(mesa.fichas, izquierda, derecha);

            bool turnoFinalizado = false;
            while (!turnoFinalizado) {
                int opcion = -99;
                cout << "elija un numero para jugar, o -1 para robar/pasar: ";
                cin >> opcion;
                if (opcion >= 0) { // el jugador intenta jugar una ficha
                    Ficha candidata;
                    if (!obtenerFichaPorIndice(actual.fichas, opcion, candidata)) {
                        cout << "opcion invalida." << endl;
                        // NO termina turno, vuelve a preguntar!
                    } else {
                        bool puedeColocarCabeza = false, puedeColocarCola = false;
                        if (mesaVacia) {
                            puedeColocarCabeza = true;
                        } else {
                            if (candidata.lado1==izquierda or candidata.lado2==izquierda){
                                puedeColocarCabeza=true;
                            } 
                            if (candidata.lado1 ==derecha or candidata.lado2 == derecha){
                                puedeColocarCola=true;
                            } 
                        }
                        if (!puedeColocarCabeza && !puedeColocarCola) {
                            cout << "La ficha seleccionada no puede colocarse en la mesa." << endl;
                        } else {
                            bool colocarEnCabeza = false;
                            if (puedeColocarCabeza && puedeColocarCola) {
                                cout << "La ficha puede colocarse en ambos extremos.Ingrese 0 para cabeza o 1 para cola: ";
                                int extremo;
                                cin >> extremo;
                                colocarEnCabeza=(extremo==0);
                            } else colocarEnCabeza=puedeColocarCabeza;

                            Ficha jugada;
                            if (eliminarFichaPorIndice(actual.fichas, opcion, jugada)) {
                                if (!mesaVacia) {
                                int extremo;
                                if (colocarEnCabeza) {
                                    extremo=izquierda;
                                } else {
                                    extremo=derecha;
                                }
                                ajustarOrientacionParaExtremo(jugada, extremo, colocarEnCabeza);
                            }
                                if (colocarEnCabeza) agregarFichaInicio(mesa.fichas, jugada);
                                else agregarFichaFinal(mesa.fichas, jugada);
                                cout << actual.nombre << " juega: [" << jugada.lado1 << "|" << jugada.lado2 << "]" << endl;
                                pasesConsecutivos = 0;
                                turnoFinalizado = true; // Solo termina turno cuando verdaderamente juega
                            }
                        }
                    }
                } else if (opcion == -1) {
                    // robar o pasar
                    if (pozo.cabeza != NULL) { // si hay fichas en el pozo, roba una
                        NodoFicha* robo=pozo.cabeza;
                        pozo.cabeza=robo->siguiente; // elimina de la cabeza del pozo
                        robo->siguiente=NULL;
                        agregarFichaFinal(actual.fichas, robo->ficha); // la anade al jugador
                        delete robo;
                        cout << actual.nombre << " Roba una ficha del pozo." << endl;
                        pasesConsecutivos=0;
                        turnoFinalizado=true;
                    } else {
                        cout << "No hay fichas en el pozo. Pasar el turno." << endl;
                        pasesConsecutivos++;
                        turnoFinalizado = true;
                    }
                } else {
                    cout << "Opcion no valida." << endl;
                    // NO termina turno, vuelve a preguntar!
                }
            }

            // verificar si el jugador se quedo sin fichas
            if (contarFichas(actual.fichas) == 0) { // condicion de victoria por dominó
                cout << actual.nombre << " Se ha quedado sin fichas y gana la ronda" << endl;
                // suma de fichas de los otros jugadores
                int sumaOponentes = 0;
                for (int k=0; k < numJugadores; ++k) { //el ++k Primero incrementa k, luego usa el nuevo valor.
                    if (k==jugadorActual) continue;
                    sumaOponentes+=sumarPuntosMano(jugadores[k].fichas); // suma los puntos de las manos restantes
                }
                jugadores[jugadorActual].puntaje += sumaOponentes;
                cout << "Puntos obtenidos por " << actual.nombre << ": " << sumaOponentes << endl;
                juegoTerminado = true;
                break;
            }

            // si todos pasan consecutivamente -> bloqueo
            if (pasesConsecutivos >= numJugadores) { // condicion de fin de juego por bloqueo
                cout << "La partida ha sido bloqueada... Calculando ganador por menor puntaje en mano." << endl;
                int ganador=0;
                int menor=sumarPuntosMano(jugadores[0].fichas);
                for (int k =1; k < numJugadores; ++k) { // busca el jugador con menor puntuacion en mano
                    int suma=sumarPuntosMano(jugadores[k].fichas);
                    if (suma < menor) {
                        menor=suma;
                        ganador=k;
                    }
                }
                int total=0;
                for (int k =0; k < numJugadores; ++k){
                    total += sumarPuntosMano(jugadores[k].fichas);
                } 
                int puntosGanador = total - menor; // el ganador suma la diferencia 
                jugadores[ganador].puntaje += puntosGanador;
                cout << "Gana " << jugadores[ganador].nombre << " por menor puntuacion en mano (" << menor << ") y obtiene " << puntosGanador << " puntos." << endl;
                juegoTerminado = true;
                break;
            }

            // siguiente jugador
            jugadorActual = (jugadorActual + 1) % numJugadores; // avanza al siguiente jugador de forma circular
        }
        // Mostrar resultados despues de cada ronda
        cout << "Puntajes luego de la ronda " << ronda << ":" << endl;
        for (int i = 0; i < numJugadores; ++i) {
            cout << jugadores[i].nombre << ": " << jugadores[i].puntaje << " puntos." << endl;
        }
    }
    // mostrar puntajes finales
    cout << "****PUNTAJES FINALES ****" << endl;
    for (int i = 0; i < numJugadores; ++i) {
        cout << jugadores[i].nombre << ": " << jugadores[i].puntaje << " puntos." << endl;
    }

    // liberar memoria dinámica de jugadores
    delete[] jugadores;

    return 0;
}