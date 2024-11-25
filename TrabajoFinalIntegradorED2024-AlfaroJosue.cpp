#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <cctype>

using namespace std;

//CONSTANTES CADENA Y CANT SINONIMOS 

const int MAX_CHAR = 50; //maximo de tamaño de cadena, definicion, sinonimos....
const int MAX_SINONIMOS=5; //cantidad sinonimos asignables por palabra

//CONSTANTES DE LIMITES DE ARCHIVOS ES DE LOGICA INTERNA
/**
	Se definieron estos parametros para poder controlar el tamaño del vector auxiliar para la ruleta
	Lo mismo para el contenedor de la tcola de jugadores, se definio un maximo desde donde se podra determinar cuantos jugs se pueden guardar en el archivo
	y que la cola, y ruleta admintan la totalidad de las jugadores/palabras.
*/

const int MAX_VECTOR_AUX_RULETA=150; 	//permitira guardar hasta este valor en el archivo de palabras
const int MAX_VECTOR_AUX_JUGADORES=150; //permitira guardar hasta este valor en el archivo jugadores


typedef char tcad[MAX_CHAR];			//Definido tcad para todos los tipo de dato cadena del proyecto

//PUNTERO A ARCHIVO

typedef FILE *parchivo; 

//LIBRERIAS

#include "util.hpp"
#include "jugadores.hpp"
#include "palabra.hpp"
#include "jugar.hpp"
#include "ranking.hpp"

//MENU 
void menu_principal(int &op);

main(){
	
	bool minimos_jugadores_file=false; //true si existen al menos 2 jugadores en el banco
	bool ruleta_creada=false; //controla si se creo la ruleta se permite una creacion por partida

	srand(time(NULL));
	int op_p;		 //opcion menu principal
	parchivo fp;	//puntero a un archivo, se declara por unica vez y se irá utilizando en cada modulo que lo necesite
	
	truleta ruleta;		//Declaramos la ruleta (LD) y inicializamos
	iniciar_ruleta(ruleta);
	
	minimos_jugadores_file = contar_jugadores_banco(fp)>=2;  //Al iniciar el juego verificamos que existan minimo 2 jugadores SELECCIONABLES en el archivo, 
															//caso contrario debe registrarlos
    do{
        system("cls");
		menu_principal(op_p);
        switch(op_p){
			
			case 1:
				administrar_jugadores(fp);					//Nos lleva al menu de administrar jugadores, solo requiere el puntero al archivo
				minimos_jugadores_file = contar_jugadores_banco(fp)>=2; //Al finalizar cualquier accion controlamos nuevamente el minimo.
				
				break;
			case 2:
				administrar_ruleta(fp, ruleta, ruleta_creada); //Dirige al menu de gestion de ruleta, con la lista vacia, al crear la ruleta marca la variable en true
																//y la lista vuelve cargada.
				break;
			case 3:
				if(minimos_jugadores_file){		//Cumple minimo de jugadores (2)
					if(ruleta_creada){			//Cumple la creacion de la ruleta
						jugar(fp, ruleta);      //Dirige al manager del juego propiamente con el archivo y la ruleta cargada
						
						if(ruleta.inicio==NULL)   //al terminar de jugar. desactiva el flag por que la ruleta está vacia y permite generar otra ruleta NUEVA
							ruleta_creada=false;
						
					} else{
					   cout<<"Debe generar la ruleta antes de ingresar a jugar"<<endl;
					   system("pause");
					}
				}else{
					cout<<"Debe agregar minimo 2 jugadores para jugar."<<endl;
					system("pause");
				}
				
				break;
			case 4:
				administrar_ranking(fp);   //Nos lleva al administrador de ranking, donde podemos ver los puntajes, solo requiere el puntero archivo.
				break;
			case 5:
				cout << "\n- Fin del programa -" << endl;
				break;
			default:
				cout << "Opcion incorrecta" << endl;
				system("pause");
        }
    } while(op_p != 5);
}

/**
	Corresponde al menu principal del sistema.	
*/
void menu_principal(int &op){
	cout<<"****************MENU****************"<<endl;
	cout<<"*                                  *"<<endl;
	cout<<"* 1 - Gestión de jugadores         *"<<endl;
	cout<<"* 2 - Crear ruleta de palabras     *"<<endl;
	cout<<"* 3 - Jugar                        *"<<endl;
	cout<<"* 4 - Cuadro de honor              *"<<endl;
	cout<<"* 5 - Salir                        *"<<endl;
	cout<<"*                                  *"<<endl;
	cout<<"************************************"<<endl;
	cout<<"Elija opcion: ";
	cin>>op;
}
