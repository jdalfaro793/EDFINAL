#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

/**
Este modulo es una modificacion del modulo ENORDEN, donde solo llamamos al modulo mostrar_registro, para mostrar los datos del jugador
las llamadas son las mismas del modulo original, IZQUIERDA, RAIZ, DERECHA, de esa forma se visualiza en forma Ascendente
*/
void ranking_orden_asc(pjugador a)
{
	if(a != NULL)
	{
		ranking_orden_asc(a->izq);
		
		mostrar_registro(a->jug,true);
		
		ranking_orden_asc(a->der);
	}
}

/**
	Este modulo es una modificacion del modulo ENORDEN, donde solo invertimos la forma de llamar a los nodos hijos
	empezando por la DERECHA, RAIZ, y al ultimo IZQUIERDA, de esa forma se visualiza en forma descendente
*/
void ranking_orden_desc(pjugador a)
{
	if(a != NULL)
	{
		ranking_orden_desc(a->der);
		
		mostrar_registro(a->jug,true);
		
		ranking_orden_desc(a->izq);
	}
}

/**
	Este Modulo de insertar permite insertar el nodo de jugador al arbol, segun el criterio de orden POR PUNTAJE TOTAL
	de esa forma el arbol quedará ordenado con los puntajes menores a la izquierda, y mayores a la derecha, del primer jugador insertado.
*/
void insertar_arb_jugadores_por_puntos(pjugador &pj, pjugador j)
{
	if(pj == NULL)
		pj = j;
	else
	{
		if(j->jug.puntaje_total < pj->jug.puntaje_total)
			insertar_arb_jugadores_por_puntos(pj->izq, j);
		else
			insertar_arb_jugadores_por_puntos(pj->der, j);
	}
}

/**
	Este modulo permite leer el archivo de jugadores. y por cada jugador
	si este tiene puntos > 0, crea el nodo y lo inserta al arbol de jugadores
*/
void llenar_arbol_jugadores_by_puntaje(parchivo fp, pjugador &pj)
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
				if(a.puntaje_total>0){
					crear_nodo(nodo_jug, a);
					insertar_arb_jugadores_por_puntos(pj, nodo_jug);
				}
			}
		}
	}
	fclose(fp);
}

//Sub menu y administracion de ranking ----------------------------------------------------------------------------------------------------------------

void submenu_ranking(int &op)
{
    cout << "===== CUADRO DE HONOR DE JUGADORES =====" << endl;
    cout << "1. Ver ranking ascendente" << endl;
	cout << "2. Ver ranking descendente" << endl;
    cout << "3. Volver al Menú Principal" << endl;
    cout << "Seleccione una opcion: ";
    cin >> op;
}

/**
	Este manager nos muestra un menu con las distintas formas de visualizar el ranking de jugadores
	ascendente, o descendente.
*/
void administrar_ranking(parchivo f)
{
    int op;
	pjugador arboljug; 						//arbol de jugadores para ordenar
	iniciar_arb_jugadores(arboljug); 		//inicializamos el arbol
	llenar_arbol_jugadores_by_puntaje(f, arboljug); 	//llenamos el arbol con los jugadores con insercion segun su PUNTAJE

    do
    {
        system("cls");
        submenu_ranking(op);				//Muestra el menu para elegir las opciones de visualizar
        switch(op)
        {
        case 1:
			ranking_orden_asc(arboljug);		//Muestra el ranking en orden ascendente
			
            system("pause");
            break;
        case 2:
			ranking_orden_desc(arboljug);   //Muestra el ranking en orden descendente
			
            system("pause");
            break;
        case 3: liberar_arbol(arboljug);		//AL FINALIZAR la visualizacion, y volver al menu principal, libera el arbol de jugadores
			break;

        default:
            cout << "Opcion incorrecta" << endl;
            system("pause");
        }
		
	} while(op != 3);
}
