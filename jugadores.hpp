#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;


/**
	Declaracion de un registro de tipo jugador para manejar la informacion general de los jugadores
	con campos apellido, nombre, nick, mejor pje, ptje total y cant de ganadas.
*/
typedef struct jugador
{

    tcad apellido;
    tcad nombre;
    tcad nickname;
    float mejor_puntaje;
    float puntaje_total;
    int partidas_ganadas;

};

/**
	Declaramos un ARBOL DE JUGADORES para manejar la insercion de jugadores y mantener en Orden del archivo de jugadores por nickname 
*/
typedef struct tjugador *pjugador; 			//Puntero a un nodo jugador
typedef struct tjugador 					//nodo jugador
{
    pjugador izq; 							//puntero izq
    pjugador der; 							//puntero der
    jugador jug; 							//registro de tipo jugador
};



// OPERACIONES DE ARBOL DE JUGADORES ------------------------------------------------------------------------------------------------------------------
void iniciar_arb_jugadores(pjugador &pj)
{
    pj = NULL;
}

void crear_nodo(pjugador &nuevo, jugador j)
{
    nuevo = new tjugador;

    if(nuevo != NULL)
    {
        nuevo->jug = j;
        nuevo->izq = NULL;
        nuevo->der = NULL;
    }
    else cout << "memoria insuficiente" << endl;
}

/**
	Insercion modificada para insertar en el arbol segun el nickname del jugador
	Nos permite generar un arbol ordenado por nickname
*/
void insertar_arb_jugadores(pjugador &pj, pjugador j)
{
    if(pj == NULL)
        pj = j;
    else
    {
        if(strcmp(j->jug.nickname, pj->jug.nickname) < 0)
        {
            insertar_arb_jugadores(pj->izq, j);
        }
        else
        {
            insertar_arb_jugadores(pj->der, j);
        }
    }
}

/**
	Modulo para liberar el arbol
	Se utilizó el recorrido en posorden para la liberacion,
	lo que permite ir liberando desde el primer izquierdo, final derecho y al ultimo la raiz
*/
void liberar_arbol(pjugador &pj)
{
    if (pj != NULL)
    {
        liberar_arbol(pj->izq);  // liberamos arbol izquierdo
        liberar_arbol(pj->der);  //  liberamos arbol derecho
        delete(pj);				//liberar la raiz
        pj = NULL;				//dejar en null por si acaso
    }
}
// END OPERACIONES DE ARBOL DE JUGADORES ------------------------------------------------------------------------------------------------------------------




// OPERACIONES DE CARGA Y MOSTRAR JUGADORES ------------------------------------------------------------------------------------------------------------------
/**
	La operacion de carga permite cargar manualmente los campos, por cada campo va validando que cumpla min. de caracteres
	Utiliza una validacion de libreria util (verificacion_cadena) para verificar que sea cadena valida
	El flag de 'nuevo', permite reusar tanto para cargar todos campos, como para editar solo campos permitidos.
*/
void cargar_jugador(jugador &j, bool nuevo)  //nuevo true = agregar , false = modificar
{
    bool valido = false;

    system("cls");
    int minimoCaracteres = 4;

    do
    {
        cout << "Ingrese Apellido: ";
        fflush(stdin);
        gets(j.apellido);
        valido = strlen(j.apellido) >= minimoCaracteres;

        if (!valido || !verificacion_cadena(j.apellido))
        {
            cout << "Ingresó un Apellido inválido" << endl;
            system("pause");
        }
    }
    while (!valido || !verificacion_cadena(j.apellido));

    do
    {
        cout << "Ingrese Nombre: ";
        fflush(stdin);
        gets(j.nombre);
        valido = strlen(j.nombre) >= minimoCaracteres;

        if (!valido || !verificacion_cadena(j.nombre))
        {
            cout << "Ingresó un Nombre inválido" << endl;
            system("pause");
        }
    }
    while (!valido || !verificacion_cadena(j.nombre));

    if(nuevo)
    {
        do
        {
            cout << "Ingrese Nickname: ";
            fflush(stdin);
            gets(j.nickname);
            valido = strlen(j.nickname) >= minimoCaracteres;

            if (!valido)
            {
                cout << "Ingresó un Nickname inválido" << endl;
                system("pause");
            }
        }
        while (!valido);

        j.mejor_puntaje = 0;
        j.partidas_ganadas = 0;
        j.puntaje_total = 0;

    }
}

/**
	El flag ver_todos_datos permite solo mostrar el nick, o mostrar todos los datos del jugador cuando se lo requiera.
*/
void mostrar_registro(jugador a, bool ver_todos_datos)
{
    cout << "\nNickname: " << a.nickname << endl;
	if(ver_todos_datos){
    cout << "Nombre: " << a.nombre << endl;
    cout << "Apellido: " << a.apellido << endl;
    cout << "Mejor puntaje: " << a.mejor_puntaje << endl;
    cout << "Partidas Ganadas: " << a.partidas_ganadas << endl;
    cout << "Puntaje total: " << a.puntaje_total << endl;
	}
}

// END OPERACIONES DE CARGA Y MOSTAR JUGADORES ------------------------------------------------------------------------------------------------------------------




// OPERACIONES DE MANEJO DE ARCHIVO DE JUGADORES  ------------------------------------------------------------------------------------------------------------------

//Permite guardar un jugador al final del archivo
void save_file_jugador(parchivo fp, jugador a)
{
    fp = fopen("jugadores.txt", "ab");
    fwrite(&a, sizeof(a), 1, fp);
    fclose(fp);
}

//Devuelve true si encontró al jugador por su NICKNAME o false si no lo encontró
bool search_file_jugador(parchivo fp, tcad buscado)
{

    bool existe = false;
    jugador a;
    fp = fopen("jugadores.txt", "rb");
    if(fp != NULL)
        while(!feof(fp) && !existe)
        {
            fread(&a, sizeof(a), 1, fp);
            if(strcmp(a.nickname, buscado) == 0)
                existe = true;
        }

    fclose(fp);
    return existe;
}

//Este modulo permite ver todos los jugadores registrados en el archivo, la variable ver_todos_datos indica que datos puede mostrar
//Al finalizar muestra un contador con el total de palabras registradas
void view_file_jugador(parchivo fp, bool ver_todos_datos)
{
	int i = 0;
	jugador a;
	fp = fopen("jugadores.txt", "rb");
	if(fp == NULL)
		cout << "Archivo inexistente" << endl;
	else
	{
		while(!feof(fp))
		{
			fread(&a, sizeof(a), 1, fp);
			if(!feof(fp)){
				i++;
				mostrar_registro(a,ver_todos_datos);
			}
		}
		cout << "\nTotal: " << i << endl;
	}
	fclose(fp);
}

/**
	Este modulo fue diseñado para contabilizar la cantidad de jugadores registrados en el archivo o banco de jugadores
*/
int contar_jugadores_banco(parchivo fp)
{
	int i=0;
	jugador a;
	fp = fopen("jugadores.txt", "rb");
	if(fp == NULL)
		i=-1;
	else
	{
		while(!feof(fp))
		{
			fread(&a, sizeof(a), 1, fp);
			if(!feof(fp)){
				i++; //por cada jugador leido incrementamos el contador
			}
		}
	}
	fclose(fp);
	return i;
}

//Permite editar la informacion de un jugador buscado por nickname
void edit_file_jugador(parchivo fp, tcad buscado)
{
    jugador jug;
    bool existe = false; //crea un flag existe en falso que cambiará si encuentra el jugador
    fp = fopen("jugadores.txt", "rb+");
    if(fp != NULL)
    {
        while(!feof(fp) && !existe)
        {
            fread(&jug, sizeof(jug), 1, fp);
            if(strcmp(jug.nickname, buscado) == 0)
                existe = true;
        }


        if(existe)
        {
            fseek(fp, -sizeof(jug), 1); //ubica el puntero en una posicion especifica del archivo
            cargar_jugador(jug, false);	//acá llamamos al modulo cargar_jugador con el jugador encontrado y le indicamos que NO es nuevo
										//lo que permitirá editar solo nombre y apellido

            fwrite(&jug, sizeof(jug), 1, fp);	//reescribimos el jugador e indicamos que se modifico
            cout << "Se modificó el jugador..." << endl;		
        }
        else
            cout << "Jugador no existe" << endl;
    }
    fclose(fp);
}


//modulo que permite consultar un jugador buscado por su nickname, mostrando todos los datos del jugador consultado
void consultar_file_jugador(parchivo fp, tcad buscado)
{
    jugador jug;
    bool existe = false;
    fp = fopen("jugadores.txt", "rb");
    if(fp != NULL)
    {
        while(!feof(fp) && !existe)
        {
            fread(&jug, sizeof(jug), 1, fp);
            if(strcmp(jug.nickname, buscado) == 0)
                existe = true;
        }


        if(existe)
        {
            mostrar_registro(jug,true);
        }
        else
            cout << "Jugador no existe" << endl;
    }
    fclose(fp);
}


//modulo que permite eliminar un jugador del archivo por su nickname
void delete_file_jugador(parchivo fp, tcad buscado)
{
    jugador a ;
    parchivo aux;	//declaramos un puntero archivo auxiliar

    bool band = false;

	fp = fopen("jugadores.txt", "rb");
    aux = fopen("temporal_j.txt", "wb"); //creamos un archivo temporal para vaciar los jugadores que no coincidan con el nickname buscado

    if(fp != NULL)
    {
        while(!feof(fp))
        {
            fread(&a, sizeof(a), 1, fp);
            if(!feof(fp) &&  strcmp(a.nickname, buscado) != 0)
                fwrite(&a, sizeof(a), 1, aux);
        }
    }

    fclose(aux);
    fclose(fp);
	//al finalizar, eliminamos el archivo original y  renombramos el temporal como el archivo original
    if(remove("jugadores.txt") == 0 && rename("temporal_j.txt", "jugadores.txt") == 0)
        cout << "Eliminado exitosamente!" << endl;
    else
        cout << "Error al eliminar" << endl;
}


/**
	Este modulo fue diseñado para rellenar el arbol de jugadores con todos los jugadores del ARCHIVO
	va recorriendo el archivo, y por cada jugador leido (a) CREA un nodo para el jugador
	y lo inserta en el arbol de jugadores (insercion por nickname)
*/
void llenar_arbol_jugadores(parchivo fp, pjugador &pj)
{
    jugador a;
    pjugador nodo_jug;

    fp = fopen("jugadores.txt", "rb");
    if(fp == NULL)
        cout << "Archivo inexistente" << endl;
    else
    {
        while(!feof(fp))
        {
            fread(&a, sizeof(a), 1, fp);
            if(!feof(fp))
            {
                crear_nodo(nodo_jug, a);
                insertar_arb_jugadores(pj, nodo_jug);
            }
        }
    }
    fclose(fp);
}

/**
	Modulo modificado de una implementacion de recorrido EN-ORDEN
	Permite guardar un jugador del arbol en el archivo segun el orden del recorrido IZQ-RAIZ-DER
	Reutilizamos el modulo save_file_jugador, que recibe el archivo y un registro de tipo jugador.
*/
void guardar_jugador_en_orden(parchivo fp, pjugador a)
{
    if(a != NULL)
    {
        guardar_jugador_en_orden(fp,  a->izq);

        save_file_jugador(fp, a->jug);

        guardar_jugador_en_orden(fp, a->der);
    }
}


/**
	Este modulo esta diseñado solo para editar los puntos de un jugador buscado por su nickname
	Se utiliza al momento de actualizar puntaje del ganador de la partida.
	Acumula sus puntos ganados
	Aumenta cantidad de partidas ganadas
	Si sacó mayor puntaje se reemplaza el puntaje maximo.
*/
void edit_puntaje_jugador(parchivo fp, tcad buscado, int puntos)
{
	jugador jug;
	bool existe = false;
	fp = fopen("jugadores.txt", "rb+");
	if(fp != NULL)
	{
		while(!feof(fp) && !existe)
		{
			fread(&jug, sizeof(jug), 1, fp);
			if(strcmp(jug.nickname, buscado) == 0)
				existe = true;
		}
		
		
		if(existe)
		{
			fseek(fp, -sizeof(jug), 1); //ubica el puntero en una posicion especifica del archivo
			
			
			jug.puntaje_total=jug.puntaje_total+puntos;
			jug.partidas_ganadas++;
			if(jug.mejor_puntaje<puntos)
				jug.mejor_puntaje=puntos;
			
			fwrite(&jug, sizeof(jug), 1, fp);
			cout << "Se Actualizó su puntaje..." << endl;
		}
		else
		   cout << "Jugador no existe" << endl;
	}
	fclose(fp);
}



// END OPERACIONES DE MANEJO DE ARCHIVO DE JUGADORES  ------------------------------------------------------------------------------------------------------------------



/**
	Modulo que permite insertar el nuevo jugador siguiendo una secuencia de pasos para poder hacer la insercion correctamente y mantener el archivo
	ordenado
*/
void insertar_jugador(parchivo fp, jugador j)
{
    pjugador arboljug; 									//Declaramos el arbol de jugadores para ordenar

    if(!search_file_jugador(fp, j.nickname)) 			//buscamos que no exista el nickname en el archivo
    {
        save_file_jugador(fp, j); 						//si no existe se guarda el nuevo nickname

        iniciar_arb_jugadores(arboljug); 				//inicializamos el arbol

        llenar_arbol_jugadores(fp, arboljug); 			//llenamos el arbol con los jugadores con insercion segun el orden del nickname

        if(remove("jugadores.txt") == 0) 				//eliminamos el archivo actual (desordenado)
        {

            guardar_jugador_en_orden(fp, arboljug);  	//recorremos el arbol en orden y vamos rellenando el archivo nuevamente
			cout<<"Se agregó jugador al banco"<<endl;
        }
        else
        {
            cout << "error al eliminar" << endl;
        }
		liberar_arbol(arboljug); 						//una vez se finaliza el proceso se libera el arbol
    }
    else
        cout << "Jugador ya existe" << endl;
}


//Sub menu y administracion de jugadores ----------------------------------------------------------------------------------------------------------------

void submenu_gestion_jugadores(int &op)
{
    cout << "===== GESTION DE JUGADORES =====" << endl;
    cout << "1. Agregar Jugador" << endl;
    cout << "2. Consultar Jugador" << endl;
    cout << "3. Modificar Jugador" << endl;
    cout << "4. Eliminar Jugador" << endl;
    cout << "5. Listar Jugadores" << endl;
    cout << "6. Volver al Menú Principal" << endl;
    cout << "Seleccione una opcion: ";
    cin >> op;
}

/**
	Desde el manager de jugadores se manejarán las distintas opciones del menu de gestion
	Se debe tener en cuenta que antes de insertar un jugador, está controlandose que el total de inscriptos sea MENOR al auxiliar de MAX_VECTOR_AUX_JUGADORES
	Esto se realiza para posteriormente, en la seccion de JUGAR, puedan inscribirse los N jugadores registrados en la COLA DE TURNOS, sin limitar que ingrese algun jugador
	ya que la cola puede contener a TODOS los jugadores del archivo.
*/
void administrar_jugadores(parchivo f)
{
    int op;
    jugador nuevo;
    tcad buscado;

    do
    {
        system("cls");
        submenu_gestion_jugadores(op);
        switch(op)
        {
        case 1:
			if(contar_jugadores_banco(f)<MAX_VECTOR_AUX_JUGADORES){
				cargar_jugador(nuevo, true);
				insertar_jugador(f, nuevo);
			}else{
				cout<<"Se superó el maximo de jugadores insertables"<<endl;
			}
            system("pause");
            break;
        case 2:
            cout << "Ingrese Nickname: ";
            fflush(stdin);
            gets(buscado);
            consultar_file_jugador(f, buscado);
            system("pause");
            break;
        case 3:
            cout << "Ingrese Nickname: ";
            fflush(stdin);
            gets(buscado);
            edit_file_jugador(f, buscado);
            system("pause");
            break;
        case 4:
			cout << "Ingrese Nickname: ";
			fflush(stdin);
			gets(buscado);
			
			if(search_file_jugador(f,buscado))
				delete_file_jugador(f, buscado);
			else
				cout<<"Jugador no existe"<<endl;
				
			system("pause");
			break;
        case 5:
            view_file_jugador(f,true);
            system("pause");
            break;
        case 6:
            break;

        default:
            cout << "Opcion incorrecta" << endl;
            system("pause");
        }
	}
    while(op != 6);
}



