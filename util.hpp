#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <cctype>

using namespace std;

//MODULOS UTILES-----------------------------------------------------------------------------------------
/**
	Este modulo nos devuelve un valor aleatorio en un rango ingresado cómo: limite inferior y superior
*/
int aleatorio(int inf, int sup)
{
	return rand() % (sup - inf + 1) + inf;
}


/**
	verifica si la cadena tiene todo letras o no
*/
bool verificacion_cadena(tcad cadena)  
{
    int longitud, i;
    bool band = true;

    longitud = strlen(cadena);

    for(i = 0; i < longitud && band == true; i++)
    {
        band =	((cadena[i] >= 'a' && cadena[i] <= 'z') ||
                 (cadena[i] >= 'A' && cadena[i] <= 'Z') ||
                 cadena[i] == ' ');
    }

    return band;
}


/**
	Cambia todas las letras de una cadena a minuscula
*/
void minuscula(tcad &cadena)
{
	int i;
	for (i = 0; i < strlen(cadena); i++)
	{
		cadena[i] = tolower(cadena[i]);
	}
}
