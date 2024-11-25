#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <cctype>

using namespace std;



/**
	Declaración de lista simple AUXILIAR, PARA LA VALIDACION DE JUGADORES NO REPETIDOS por nickname
	al momento de cargar los jugadores dentro de los turnos de para la cola.
	De modo que no se acepte que el jugador ya esté en la cola y se lo quiera seleccionar nuevamente.
*/
typedef struct tnodo_cadena *pnodo_cadena;

typedef struct tnodo_cadena
{
    pnodo_cadena sig;
    tcad dato;
};


//	OPERACIONES DE LISTA AUXILIAR DE CADENA ------------------------------------------------------------------------------------------------------------------

void iniciar_lis_nickname(pnodo_cadena &lista)
{
    lista = NULL;
}

pnodo_cadena buscar_nodo_nickname(pnodo_cadena lista, tcad valor)
{
    pnodo_cadena i = NULL;

    if(lista != NULL)
    {
        for(i = lista; i != NULL &&  strcmp(i->dato, valor) != 0; i = i->sig);
    }
    return i;
}

/**
	Para la insercion de jugador en la lista se uso la variante norep que valida que no exista el jugador anteriormente
	Si no existe lo agrega a la lista al inicio, si no, elimina el nodo previamente creado y comunica que ya existe el nick en la lista
*/
void agregar_inicio_nickname_norep(pnodo_cadena &lista, tcad cadena, int &cantidad)
{
    pnodo_cadena nuevo;                         //creamos puntero y inicializamos
    nuevo = new tnodo_cadena;
    if(nuevo != NULL)                           //si se creo correctamente asignamos el nick y el sig en null
    {
        strcpy(nuevo->dato, cadena);
        nuevo->sig = NULL;
        if(lista == NULL)                       //si la lista es vacia asignamos directamente y incrementamos contador
        {
            lista = nuevo;
            cantidad++;
        }
        else
        {
            if(buscar_nodo_nickname(lista, cadena) == NULL)          //si no, buscamos el nick en la lista
            {
                nuevo->sig = lista;                                  //si no existe el nick, es porq no esta repetido
                lista = nuevo;                                       //agregamos al inicio, actualizamos puntero y el contador
                cantidad++;
            }
            else
            {
                cout << "Ya se encuentra agregado el jugador." << endl;
                delete(nuevo);                                      //el nick ya esta repetido, eliminamos nodo y dejamos en null
                nuevo = NULL;
            }
        }
    }
}

/**
	Para la liberacion de la lista vamos recorriendo la lista desde el inicio similar a la liberación de la ruleta
	mientras no sea NULL y se va eliminando los nodos hasta llegar al ultimo nodo que sera NULL
*/

void liberar_lis_nickname(pnodo_cadena &lista)
{
    pnodo_cadena p;
    while(lista != NULL){
        p = lista->sig;
        delete(lista);
        lista = p;
    }
}
// END OPERACIONES DE LISTA AUXILIAR DE CADENA ------------------------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------------------------------------------------------------------

/**
	DECLARACION de una estructura tipo PILA DINAMICA (USANDO LISTAS DOBLEMENTE ENLAZADA)
	Usamos un contador para la pila de modo que usemos el tope de la pila como el MAX_VECTOR_AUX_RULETA (Maximo de la ruleta disponible)
*/

typedef struct tnodo_p *pnodo_p;

typedef struct tnodo_p {
	palabra dato;				//Por cada nodo usamos un registro de tipo PALABRA para ir apilando distintas palabras en cada jugador
	pnodo_p ant;
	pnodo_p sig;
};

typedef struct tpila {
	pnodo_p inicio;
	int cont;
};


//----------------------------------------------------------------------------------------------------------------------------------------------------------------

/**
	Declaramos un registro de tipo TURNO JUGADOR que tendrá dentro el nick del jugador
	su pila de palabras, puntos, vidas, palabras adivinadas y longitud maxima de palabra adivinada.
*/
typedef struct turno_jugador
{
    tcad 	nick;
    tpila 	pila;
    float 	puntos;
    int 	vidas;
	int 	cant_adivinadas;
	int 	longitud_max_palabra;
};

/**
	Declaramos un contenedor de turno_jugador de tamaño MAX_VECTOR_AUX_JUGADORES, QUE PERMITE ingresar hasta el (maximo de jugadores registrados)
*/
typedef turno_jugador tcont_turnos[MAX_VECTOR_AUX_JUGADORES];

/**
	Finalmente declaramos una cola de turnos_jugador con todas las estructuras mencionadas, dicha cola sera de prioridad de almacenamiento.
*/
typedef struct tcola
{
    tcont_turnos datos;
    int frente;
    int final;
    int cantidad;
};


//	OPERACIONES DE PILA DE PALABRAS ------------------------------------------------------------------------------------------------------------------

void iniciar_pila(tpila &pila)
{
	pila.inicio=NULL;
	pila.cont=0;
}

//Creación del nodo
void crear(pnodo_p &nuevo, palabra valor)
{
	nuevo=new tnodo_p;
	if(nuevo!=NULL)
	{
		nuevo->dato=valor;
		nuevo->ant=NULL;
		nuevo->sig=NULL;
	}
}

bool pila_llena(tpila lis) {
	return lis.cont == MAX_VECTOR_AUX_RULETA; // MAX_VECTOR_AUX_RULETA es el límite de la pila del main 
}

bool pila_vacia(tpila pila)
{
    return pila.cont==0;
}

/**
	Por cada palabra nueva creamos un nodo nuevo, si la pila está llena se libera el nodo creado.
	Si no, se agrega el nodo al final de la pila y se va aumentando el cont.
*/
void agregar_pila(tpila &lis, palabra valor){
	pnodo_p i, nuevo;

	crear(nuevo,valor);
	if(pila_llena(lis)){
		cout<<"Pila llena"<<endl;
		delete(nuevo);
		nuevo=NULL;
	}
	else{
		if(lis.inicio==NULL)
			lis.inicio=nuevo;
		else
		{
			for(i=lis.inicio;i->sig!=NULL;i=i->sig);
			i->sig=nuevo;
			nuevo->ant=i;
		}
		lis.cont++;
	}
}

/**
	Modificamos el modulo de quitar pila para quitar una palabra
	Si la pila está vacia se asigna al nick un valor arbitrario @
*/
palabra quitar_pila(tpila &lis){
	pnodo_p extraido, i;
	palabra ext;
	
	if (pila_vacia(lis))//(lis.inicio==NULL)
		strcpy(ext.cad, "@");
	else
	{
		if(lis.inicio->sig==NULL){
			extraido=lis.inicio;
			lis.inicio=NULL;
		}else{   
			for(i=lis.inicio;(i->sig)->sig!=NULL;i=i->sig);
			extraido=i->sig;
			i->sig=NULL;
			extraido->ant=NULL;
		}
		lis.cont--;
		ext=extraido->dato;
		delete(extraido);
		extraido=NULL;
	}
	return ext;
}
	
// END OPERACIONES DE PILA DE PALABRAS ------------------------------------------------------------------------------------------------------------------
	
	
// OPERACIONES DE COLA DE TURNOS ------------------------------------------------------------------------------------------------------------------------

void iniciar_cola(tcola &cola)
{
    cola.frente = 0;
    cola.final = 0;
    cola.cantidad = 0;
}

/**
	Se valida que el siguiente no supere la cantidad MAX_VECTOR_AUX_JUGADORES Que es el tamaño de la cola de turnos.
	Si supera, automaticamente pasa a 0 que es comienzo de cola.
*/
int siguiente(int indice)
{
    indice++;
    if(indice == MAX_VECTOR_AUX_JUGADORES)
        indice = 0;
	
    return indice;
}

/**
	Se valida si la cantidad de la llegó al maximo de tamaño de la cola.
*/
bool cola_llena(tcola cola)
{
    return cola.cantidad == MAX_VECTOR_AUX_JUGADORES;
}

/**
	Usamos un agregar común para manejar la cola, se modificó que permite ahora guardar el registro turno de jugador
*/
void agregar_cola(tcola &cola, turno_jugador nuevo)
{
    if(cola_llena(cola))
        cout << "Cola de turnos llena" << endl;
    else
    {
        cola.final = siguiente(cola.final);
        cola.datos[cola.final] = nuevo;
        cola.cantidad++;
    }
}

bool cola_vacia(tcola cola)
{
    return cola.cantidad == 0;
}

/**
	Se usa quitar cola para retornar el turno_jugador en caso que la cola esté vacia
	retorna un jugador con nick arbitrario '@'
*/
turno_jugador quitar_cola(tcola &cola)
{
    turno_jugador extraido;

    if(cola_vacia(cola))
        strcpy(extraido.nick, "@");
    else
    {
        cola.frente = siguiente(cola.frente);
        extraido = cola.datos[cola.frente];
        cola.cantidad--;
    }
    return extraido;
}

// END OPERACIONES DE COLA DE TURNOS ------------------------------------------------------------------------------------------------------------------------


// OPERACIONES DE CARGA DE JUGADORES E INICIAR JUGADOR ------------------------------------------------------------------------------------------------------
/**
	Este modulo esta diseñado para cargar los jugadores del archivo NO REPITIENDO LOS NICKS, en una lista auxiliar de cadenas, siempre que no este repetido
	permitira cargar el el jugador, e incrementará el contador de inscriptos
*/
void elegir_jugadores(parchivo f, pnodo_cadena &lis_aux_nicks, int &cant)
{
    char res = 's';	//SE INICIA EL CHAR en 's' , lo que permite ingresar al bucle de ingreso de nicks de jugadores
    tcad buscado;
    turno_jugador turno;


    while(tolower(res) == 's') //ingresamos al bucle porque se inicia con 's'
    {
        system("cls");
        cout << "Jugadores nicks disponibles: " << endl;
        view_file_jugador(f, false);

        cout << "Ingrese nick jugador: "; //solicitamos nick
        fflush(stdin);
        gets(buscado);

        if(search_file_jugador(f, buscado)) //buscamos nick en el archivo y si existe lo agregamos a la lista auxiliar
            agregar_inicio_nickname_norep(lis_aux_nicks, buscado, cant);
        else
            cout << "No se encontró el jugador" << endl; //si no existe indicamos

        cout << "Desea seguir ingresando jugadores? (s/n): ";  //solicitamos confirmacion de agregar más jugadores
        cin >> res;
    }
}

/**
	Este modulo esta diseñado para llenar la cola de turno_jugador, para cada nodo de la lista auxiliar
	se inicia un turno_jugador y se le asigna el nombre del nodo de la lista
	se le asigna 5 vidas al jugador y todos sus puntos en 0, se inicia su pila vacia
	y finalmente se agregar_cola a la cola de turnos
*/
void cargar_turnos(pnodo_cadena lis, tcola &q)
{
	pnodo_cadena i;
	turno_jugador turno;
	if(lis != NULL)
	{
		for(i = lis; i != NULL; i = i->sig)
		{
			strcpy(turno.nick, i->dato);
			iniciar_pila(turno.pila);
			turno.puntos = 0;
			turno.vidas = 5;
			turno.cant_adivinadas = 0;
			turno.longitud_max_palabra = 0;
			
			agregar_cola(q, turno);
		}
	}
}

//Se utiliza para cargar un turno jugador vació, todos los campos en 0, su pila inicializada y el nick en un valor arbitrario '@'
void iniciar_jugador(turno_jugador &turno){
	strcpy(turno.nick, "@");
	iniciar_pila(turno.pila);
	turno.puntos = 0;
	turno.vidas = 0;
	turno.cant_adivinadas = 0;
	turno.longitud_max_palabra = 0;
}

//END OPERACIONES DE CARGA DE JUGADORES Y INICIAR JUGADOR ------------------------------------------------------------------------------------------------------
	

//OPERACIONES DE LOGICAS INTERNAS DE LA PARTIDA JUGADA ------------------------------------------------------------------------------------------------------------------
/**
	Buscar letra retorna true si encuentra la letra en la cadena visible,
	reemplazando la letra en la cadena oculta,
	si no encuentra la letra, no se produce ningun cambio por lo que retorna false
*/
bool buscar_letra(tcad &oculta, tcad visible, char letra){
    int i;
    bool cambio = false;

    for (i = 0; i < strlen(visible); i++)
    {
        if(visible[i] == letra)
        {
            oculta[i] = letra;
            if(!cambio)
                cambio = true;
        }
    }
    return cambio;
}

/**
	Oculta una cadena previamente cargada y sustituye todos sus caracteres por guiones bajos.
*/
void guiones(tcad &palabra)
{
    int i;
    for (i = 0; i < strlen(palabra); i++)
    {
        palabra[i] = '_';
    }
}

/**
	Permite visualizar la cadena oculta con un espacio entre medio de cada caracter
*/
void ver_pal_oculta(tcad palabra)
{
    int i;
    cout << "Palabra: ";
    for (i = 0; i < strlen(palabra); i++)
    {
        cout << palabra[i] << " ";
    }
    cout << endl;
}

/**
	Este modulo esta diseñado para solicitar al jugador que ingrese la cadena que puede coincidir con la palabra oculta.
	esta cadena se convierte a minusculas y se compara con la pabra real.
	Si el jugador acierta, se carga la palabra EN SU PILA, y se le asigna 3 vidas nuevas.
	Finalmente se cambia el flag de palabra adivinada a true
*/
void arriesgar_palabra(turno_jugador &jugador, palabra palabra, bool &palabraok)
{

    tcad cadena;
	
    cout << "Ingrese palabra: ";
    fflush(stdin);
    gets(cadena);
    minuscula(cadena);
    if(strcmp(palabra.cad, cadena) == 0)
    {

        cout << "Adivinaste la palabra!!!..." << endl;
        jugador.vidas = jugador.vidas + 3;
		
        agregar_pila(jugador.pila, palabra);
        palabraok = true;
    }
    else
    {
        cout << "No acertaste...buen intento..." << endl;
    }

}

/**
	Este modulo se encarga de calcular el puntaje del jugador
	Mientras la pila de palabras tenga palabras adivinadas, va contando cada una,
	y guardando en longitud_max_palabra la más larga,
	finalmente multiplica cant adivinadas por la longitud de la palabra más larga
*/
void calcular_puntaje(turno_jugador &ganador) {
	
	int longitud_palabra=0;
	
	while (!pila_vacia(ganador.pila)) {
		ganador.cant_adivinadas++;
		
		// Calcular la longitud máxima de palabra
		longitud_palabra = strlen(quitar_pila(ganador.pila).cad);
		if (longitud_palabra > ganador.longitud_max_palabra) {
			ganador.longitud_max_palabra = longitud_palabra;
		}
		
	}
	ganador.puntos = ganador.cant_adivinadas * ganador.longitud_max_palabra;
}

/**
	Este modulo se utiliza para verificar el ganador con el puntaje mas alto en caso de que la ruleta se quede sin palabras
	y hayan aún jugadores en la partida, toma al primer jugador como ganador y en base a este, va verificando el resto
	se usa una bandera que controlarán si al menos hubo un cambio entre cada jugador (todos_igual_puntaje)
	si el jugador analizado tiene puntaje distinto al primero analizado es por que al menos no son iguales, caso contrario la bandera siempre será true
	en este caso el juego determina que nadie GANO y se reinicia el puntaje del ganador a 0.
	en caso se encuentre ganador se triplican sus puntos en esta instancia
*/
void verificar_ganador(tcola &turnos_jugadores, turno_jugador &ganador) {
	bool todos_igual_puntaje=true;
	turno_jugador jugador;	

	cout << "\nSe jugaron todas las palabras, verificando mayor puntaje.\n";
	ganador = quitar_cola(turnos_jugadores); //damos por ganador al primer jugador de la cola y calculamos su puntaje
	calcular_puntaje(ganador);
	
	while (!cola_vacia(turnos_jugadores)) { //analizamos el resto de los jugadores
		jugador = quitar_cola(turnos_jugadores);
		calcular_puntaje(jugador);			//vamos calculando puntaje de los jugadores (operacion limpia pila de palabras)
		
		if (jugador.puntos > ganador.puntos) { // si algún jugador supera al ganador actual
			ganador = jugador;                 // actualizamos el ganador
			todos_igual_puntaje = false;       // uno tiene un puntaje diferente
		} else if (jugador.puntos != ganador.puntos) { 
			todos_igual_puntaje = false;       //se detecta que el jugador analizado no es mayor ni igual, por lo que es distinto al primero.
		}
	}
	
	//evaluamos el caso final 
	if (todos_igual_puntaje) { 											//si todos tienen el mismo puntaje obtenido nadie ganó
		cout << "Todos sacaron el mismo puntaje, nadie gano." << endl;
		iniciar_jugador(ganador);                                       //como nadie gano inicializamos el ganador con puntos 0 lo que no actualizara el archivo
	} else { 
		ganador.puntos = ganador.puntos * 3; // si hubo un cambio o el ganador inicial tiene el mayor puntaje, multiplicamos su puntaje por 3
		cout << "El ganador del juego es: " << ganador.nick << " con " << ganador.puntos << " puntos, sus puntos fueron TRIPLICADOS" << endl;
	}
}

//END OPERACIONES DE LOGICAS INTERNAS DE LA PARTIDA JUGADA ------------------------------------------------------------------------------------------------------------------



// INICIO DEL JUEGO -----------------------------------------------------------------------------------------------------------------------------------------------
void menu_turno(int &op)
{
	cout << "\n1. Arriesgar palabra" << endl;
	cout << "2. Probar una letra" << endl;
	cout << "3. Solicitar primera letra" << endl;
	cout << "4. Solicitar Sinonimos" << endl;
	cout << "5. Solicitar Definicion" << endl;
	cout << "Ingrese opcion: ";
	cin >> op;
}

/** 
	El modulo iniciar_juego recibe todos los atributos previamente cargados, ruleta de palabras, la cola de turnos, jugares inscriptos, y una variable de juego_terminado en FALSO
	Donde por cada palabra de la ruleta, deben ir jugando los distintos jugadores hasta que se obtenga el ganador, ya sea por que adivinó todas las palabras, o por que los otros jugadores
	se quedaron sin vidas.
	
	se determina el ganador basado en los puntajes obtenidos, con reglas adicionales en caso de empate cuya reglas seran explicadas en cada seccion.
	Al final, libera los ruleta de palabras. y si se obtuvo un ganador, devuelve el ganador en la variable 'ganador'.
*/
void iniciar_juego(parchivo f, truleta &ruleta, tcola &turnos_jugadores, int jugadores_inscriptos, bool &juego_terminado, turno_jugador &ganador)
{
    pnodo palabra; 				//Puntero a la palabra trabajada de la ruleta
    turno_jugador jugador;		//Registro de un jugador
    int op;						//Variable del menu de opciones
    char letra;					//Variable para almacenar la letra pedida al jugador
    tcad palabra_trabajada;		//En esta variable ocultamos la cadena para mostrar al jugador
    bool pal_adivinada = false;	//Se declaran banderas para controlar cuando se adivine la palabra

    while (ruleta.inicio != NULL && !juego_terminado)	//Se ejecutara el bucle mientras hayan palabras y el juego NO haya terminado
    {
        pal_adivinada = false;							//Para la primer palabra se coloca el flag de 'adivinada' en false

        palabra = quitar_inicio_ruleta(ruleta);			//Quitamos la palabra de la ruleta y llenamos palabra_trabajada con el contenido de esta palabra
        strcpy(palabra_trabajada, palabra->dato.cad);
        guiones(palabra_trabajada);						//Ocultamos la palabra trabajada y la sustituimos por guiones bajos

        while (!pal_adivinada && !juego_terminado)		//Mientras aun no haya adivinado la palabra, y el juego siga activo se ejecuta el bucle
        {
            system("cls");
            //cout << "************ Nueva Palabra a Adivinar: "<<  palabra->dato.cad <<" ************" << endl;
            ver_pal_oculta(palabra_trabajada);			//Mostramos la palabra con espacios y guiones: "perro" -> "_ _ _ _ _"

            jugador = quitar_cola(turnos_jugadores);	//Obtenemos el primer jugador de la cola y mostramos su nick y vidas disponibles y un MENU DE OPCIONES

            cout << "\nTurno de: " << jugador.nick << endl;
            cout << "Vidas restantes: " << jugador.vidas << endl;

            menu_turno(op);

            switch (op)									//Luego de mostrar el menu de opciones el jugador podra acceder a distintas secciones, arriesgar, probar letra, sinonimos, etc.
            {
            case 1:
                arriesgar_palabra(jugador, palabra->dato, pal_adivinada);          //Esta seccion Permite arriesgar la palabra sin descontarle vidas al jugador
                system("pause");
                break;

            case 2:
                cout << "Ingrese una letra: ";										//Esta seccion permite probar una LETRA al jugador , al final pide ARRIESGAR la palabra
                cin >> letra;																																																

                if (buscar_letra(palabra_trabajada, palabra->dato.cad, letra))		//Busca la letra en la palabra original, si existe la sustituye en la palabra oculta y retorna true
                {
                    cout << "\nLetra acertada...!" << endl;
                    ver_pal_oculta(palabra_trabajada);								//Muestra la palabra oculta con las letras nuevas ADIVINADAS
                }
                else
                {
                    cout << "\nLetra incorrecta. Perdiste 1 vida." << endl;			//De no adivinar, le quita una vida al jugador
                    jugador.vidas--;	
                }
                arriesgar_palabra(jugador, palabra->dato, pal_adivinada);
                system("pause");
                break;

            case 3:
                palabra_trabajada[0] = palabra->dato.cad[0];						//Esta seccion le muestra al jugador la primera letra de la palabra OCULTA, al final debe ARRIESGAR 

                cout << "La primera letra es: " << palabra_trabajada[0] << endl;
                cout << "Perdiste 1 vida." << endl;

                ver_pal_oculta(palabra_trabajada);									//Le muestra la palabra oculta con la primer letra revelada
                jugador.vidas--;													//Le quita una vida por mostrarle la primer letra
                arriesgar_palabra(jugador, palabra->dato, pal_adivinada);			//Le pide Arriesgar
                system("pause");
                break;

            case 4:																	//La seccion SINONIMOS solo se habilita si el jugador tiene por lo menos 2 vidas, caso contrario debe ARRIESGAR 
                if (jugador.vidas >= 2)
                {
                    if (palabra->dato.sin.ocup > -1)								//Si la palabra oculta tiene sinonimos se le quitarán 2 vidas al jugador y se mostrarán los sinonimos 
                    {
                        cout << "Sinonimos: " << endl;
						jugador.vidas =jugador.vidas-2;											//Descontamos dos vidas

                        for (int i = 0; i <= palabra->dato.sin.ocup; i++)
                            cout << i + 1 << ") " << palabra->dato.sin.datos[i] << endl;

                        cout << "Perdiste 2 vidas." << endl;
                        arriesgar_palabra(jugador, palabra->dato, pal_adivinada);		//Se le pide arriesgar la palabra
                    }
                    else{
                        cout << "No hay sinonimos disponibles para esta palabra. Debes arriesgar." << endl;			//Si la palabra no tiene sinonimos el jugador debe ARRIESGAR de todas formas
						arriesgar_palabra(jugador, palabra->dato, pal_adivinada);
					}
                }
                else{
                    cout << "No tienes suficientes vidas para solicitar sinonimos. Debes arriesgar." << endl; 
					arriesgar_palabra(jugador, palabra->dato, pal_adivinada);  		//Si el jugador no tiene 2 vidas debe ARRIESGAR de todas formas.
				}
				
                system("pause");
                break;

            case 5:
                if (jugador.vidas >= 3)													//La definicion solo se habilita si el jugador tiene al menos 3 vidas
                {
                    cout << "Definicion de la palabra: " << palabra->dato.def << endl;	//Mostramos la definicion de la palabra 
                    cout << "Perdiste 3 vidas." << endl;

					jugador.vidas= jugador.vidas-3;													//y descontamos 3 vidas 
                    arriesgar_palabra(jugador, palabra->dato, pal_adivinada);			//Finalmente debe ARRIESGAR
                }
                else{
						cout << "No tienes suficientes vidas para solicitar la definicion. Debes arriesgar." << endl;	//Si no tiene 3 vidas, debe ARRIESGAR De todas formas
						arriesgar_palabra(jugador, palabra->dato, pal_adivinada);
				}
				
                system("pause");
                break;

            default:
                cout << "Opción incorrecta" << endl;
                system("pause");
            }

            if (jugador.vidas > 0){													//Al finalizar las opciones, si el jugador tiene vidas aún, se lo agrega NUEVAMENTE A LA COLA
                agregar_cola(turnos_jugadores, jugador);
			}else
            {
				system("cls");

				calcular_puntaje(jugador);											//SI EL JUGADOR no tiene vidas, se calcula su puntaje (Se libera la pila de palabras del jugador)
				
				cout << jugador.nick << " quedo fuera del juego(ATRAPADO POR LA PARCA !!!) con "<<jugador.puntos<<" puntos obtenidos"<< endl; //Mostramos su puntaje
				
                jugadores_inscriptos--;					//Descontamos los inscriptos, y si al finalizar solo QUEDO UN JUGADOR, El flag de juego terminado pasa A TRUE
                if (jugadores_inscriptos == 1)
                {
                    juego_terminado = true;
					
                    ganador = quitar_cola(turnos_jugadores);					//Quitamos de la cola al unico jugador y calculamos su puntaje
					
					calcular_puntaje(ganador);								
					
					if(ganador.puntos>0){										//Si dicho ganador tiene puntos, se lo dá por ganador 
						cout << "El ganador del juego es: " << ganador.nick << " con: "<< ganador.puntos<<" puntos !!" << endl;
						
						
					}else
					   cout << "\nNingún jugador adivino las palabras." << endl;  //Caso contrario retornará el modulo con ganador con 0 puntos lo cual no actualizará el archivo
					
                }
            }
        }			
        delete palabra;				//Al finalizar el bucle por que haya ganador, o por que se haya adivinado la palabra, se elimina la palabra jugada
        palabra = NULL;
    }

	if (ruleta.inicio == NULL && !juego_terminado)			//Si por algun motivo la ruleta quedo vacia pero aún hay jugadores PROCEDEMOS A DESENPATAR
	{
		juego_terminado = true;								//Damos por terminado el juego y verificamos el puntaje más alto
		verificar_ganador(turnos_jugadores, ganador);
	}
	
    
	liberar_ruleta(ruleta);							//Al finalizar el juego liberamos la lista de la ruleta por posible nodos que hayan quedado aun en memoria
	cout << "\nJuego finalizado :D" << endl;	
	
}

// FIN DEL JUEGO -----------------------------------------------------------------------------------------------------------------------------------------------



//Sub menu y administracion jugar ----------------------------------------------------------------------------------------------------------------

void submenu_jugar(int &op)
{
    cout << "===== * JUGAR * =====" << endl;
    cout << "1. Elegir Jugadores" << endl;
    cout << "2. Iniciar el Juego >:)" << endl;
    cout << "Seleccione una opcion: ";
    cin >> op;
}

/**
	El modulo jugar administra la seccion de JUGAR, donde nos permite elegir una opcion para elegir jugadores, o iniciar el juego propiamente.
	Recibe la ruleta cargada con las palabras y el puntero al archivo
*/
void jugar(parchivo f, truleta &ruleta)
{
    int op, jugadores_inscriptos = 0; //Permite contabilizar los jugadores a jugar 

    bool juego_terminado = false; //controlar juego terminado, indica cuando finaliza el juego y retorna al menu principal
    bool jugadores_elegidos = false;	//bandera para saber si se eligieron jugadores 

    char res;

    pnodo_cadena lis_aux_nicks;			//Declaramos e Inicializamos una lista auxiliar de cadena
    iniciar_lis_nickname(lis_aux_nicks);	
	turno_jugador ganador;
	
    tcola turnos_jugadores;				//Declaramos la cola de turnos e inicializamos la misma
    iniciar_cola(turnos_jugadores);

	iniciar_jugador(ganador);			//Inicializamos el jugador que indicará el ganador del juego

    do
    {
        system("cls");
        submenu_jugar(op);
        switch(op)
        {
        case 1:

            elegir_jugadores(f, lis_aux_nicks, jugadores_inscriptos);

            if(jugadores_inscriptos >= 2)				//Permite saber si al menos se inscribieron 2 jugadores
            {
                jugadores_elegidos = true;				//Actualiza el flag de jugadores elegidos a true
                cout << "Jugadores elegidos!!" << endl;
            }
            else													//Caso contrario indica que aun falta ingresar jugadores
                cout << "Aún le falta ingresar jugadores" << endl;

            break;
        case 2:
            if(jugadores_elegidos)
            {
				cargar_turnos(lis_aux_nicks, turnos_jugadores);   //Una vez se da confirmacion, cargamos la tcola con la lista de nicks de jugadores
				liberar_lis_nickname(lis_aux_nicks);			//Liberamos la lista auxiliar de nicks previamente cargada
				iniciar_juego(f, ruleta, turnos_jugadores, jugadores_inscriptos, juego_terminado, ganador); //Procedemos a iniciar el juego con la tcola, la ruleta, el ganador vacío, la cantidad de inscriptos
				if(ganador.puntos>0)								//Al finalizar el juego si vuelve al menos un ganador con puntaje > 0, se guarda el puntaje
					edit_puntaje_jugador(f,ganador.nick,ganador.puntos);
            }
            else
                cout << "Debe elegir jugadores para esta partida!..." << endl;
           
			break;
        default:
            cout << "Opcion incorrecta" << endl;
        }
        system("pause");
    }
    while(!juego_terminado);				//Este manager se ejecutará siempre que el juego este ACTIVO, al finalizar retorna al menu principal
}

