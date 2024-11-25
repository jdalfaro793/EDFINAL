#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <cctype>

using namespace std;


/**
	Declaracion de un registro de tipo palabra, compuesto por un registro de sinonimos que tiene un vector de tcad de tamaño MAX_SINONIMOS
	a su vez, contiene la cadena de la palabra y la definicion de tipo tcad
*/
typedef tcad vpalabras[MAX_SINONIMOS]; 	//un vector de tcad para guardar sinonimos

typedef struct tsinonimo{
	vpalabras datos;
	int ocup;
};

typedef struct palabra
{
    tcad cad;                           //cadena
    tcad def;	                        //definicion
	tsinonimo sin;                      //sinonimos 	(REGISTRO CON VECTOR Y OCUP)
};


/**
Declaramos un ARBOL de PALABRAS para manejar la insercion de palabras y mantener en Orden del archivo de palabras por "cadena" (palabra)
*/
typedef struct tpalabra *ppalabra;
typedef struct tpalabra
{
	ppalabra izq;
	ppalabra der;
    palabra pal;
};



/**
	Declaramos Vector auxiliar de palabras de tam. MAX_VECTOR_AUX_RULETA 
	para generar la ruleta con N Palabras registradas del archivo sin limite de palabras excluidas
*/
typedef palabra v_banco_palabras[MAX_VECTOR_AUX_RULETA]; //se crea un vector de MAX_VECTOR_AUX_RULETA y traera palabras del archivo auxiliar para llenar la ruleta

/**
	Declaramos una RULETA CON LISTA DOBLEMENTE ENLAZADA DE PALABRAS 
*/
typedef struct tnodo *pnodo;

typedef struct tnodo{
	palabra dato;
	pnodo ant;
	pnodo sig;
};

typedef struct truleta{
	pnodo inicio;
	pnodo fin;
};


// OPERACIONES DE ARBOL DE PALABRAS ------------------------------------------------------------------------------------------------------------------
void iniciar_arb_palabras(ppalabra &pp)
{
    pp = NULL;
}

void crear_nodo(ppalabra &nuevo, palabra p)
{
    nuevo = new tpalabra;

    if(nuevo != NULL)
    {
        nuevo->pal = p;
        nuevo->izq = NULL;
        nuevo->der = NULL;
    }
    else cout << "memoria insuficiente" << endl;
}

/**
	Insercion modificada para insertar en el arbol segun la palabra ingresada
	Nos permite generar un arbol ordenado por palabra
*/
void insertar_arb_palabras(ppalabra &pp, ppalabra p)
{
    if(pp == NULL)
		pp = p;
    else
    {
		
        if(strcmp(p->pal.cad, pp->pal.cad) < 0)
        {
			insertar_arb_palabras(pp->izq, p);
        }
        else
        {
			insertar_arb_palabras(pp->der, p);
        }
    }
}

/**
	Modulo para liberar el arbol
	Se utilizó el recorrido en posorden para la liberacion,
	lo que permite ir liberando desde el primer izquierdo, final derecho y al ultimo la raiz
*/
void liberar_arbol(ppalabra &pp)
{
    if (pp != NULL)
    {
        liberar_arbol(pp->izq);  // liberamos arbol izquierdo
        liberar_arbol(pp->der);  //  liberamos arbol derecho
        delete(pp);
        pp = NULL;
    }
}
// END OPERACIONES DE ARBOL DE PALABRAS ------------------------------------------------------------------------------------------------------------------


//Operacion de agregar común de un vector con su ocupado.
void agregar_sinonimo(vpalabras &v, int &ocup)
{
	tcad nuevo;
	
	if(ocup == MAX_SINONIMOS - 1)
	{
		cout << "Lista de sinonimos llena.." << endl;
	}
	else
	{
		cout<<"Ingrese sinonimo: ";
		fflush(stdin);
		gets(nuevo);
		minuscula(nuevo);
		if (strlen(nuevo)>=3 && verificacion_cadena(nuevo)) {	//Se verifica que el sinonimo sea valido y se utiliza modulo verificacion_cadena de libreria util.hpp
			ocup++;
			strcpy(v[ocup], nuevo);  //copiar el nuevo sinónimo al vector
		} else {
			cout << "Sinonimo invalido" << endl;
		}
	}
}

// OPERACIONES DE ENTRADA Y SALIDA DE PALABRAS ------------------------------------------------------------------------------------------------------------------

/**
	La operacion de carga permite cargar manualmente los campos, por cada campo va validando que cumpla min. de caracteres
	Utiliza una validacion de libreria util (verificacion_cadena) para verificar que sea cadena valida
	Por cada palabra, se declara su ocupado de sinonimos en -1
	Se irá preguntando si desea agregar sinonimos hasta llegar al maximo de sinonimos permitidos.
*/
void cargar_palabra(palabra &p)
{
    bool valido = false;
	int minimoCaracteres = 3;
	char res;

    system("cls");

    do
    {
        cout << "Ingrese Palabra: ";
        fflush(stdin);
        gets(p.cad);
		minuscula(p.cad);
        valido = strlen(p.cad) >= minimoCaracteres;

        if (!valido || !verificacion_cadena(p.cad))
        {
            cout << "Ingresó una palabra invalida" << endl;
            system("pause");
        }
    }
    while (!valido || !verificacion_cadena(p.cad));

    do
    {
        cout << "Ingrese Definicion: ";
        fflush(stdin);
        gets(p.def);
        valido = strlen(p.def) >= minimoCaracteres;

        if (!valido || !verificacion_cadena(p.def))
        {
            cout << "Ingresó una definicion inválida" << endl;
            system("pause");
        }
    }
    while (!valido || !verificacion_cadena(p.def));

	p.sin.ocup = -1;
	
	cout<<"Desea agregar sinónimos? (s/n)"<<endl;
	cin>>res;
	res=tolower(res);
	
	while(p.sin.ocup<MAX_SINONIMOS-1 && res=='s'){
		cout << "--Agregar Sinónimo--" << endl;
		
		agregar_sinonimo(p.sin.datos,p.sin.ocup);
		
		cout << "\nDesea seguir agregando sinónimos? s/n: ";
		cin >> res;
		res=tolower(res);
	}
	
	if(res=='s' && p.sin.ocup==MAX_SINONIMOS-1) 
		cout <<"Lista de sinonimos llena"<<endl;
}

/**
	Este modulo mostrara para cada palabra su contenido, definicion y si tiene sinonimos, los mostrará en el orden guardado
	Si no tiene sinonimos, solo indicará un guión
*/
void mostrar_palabra(palabra a)
{
	int i;
    cout << "\nPalabra: " << a.cad << endl;
    cout << "Definicion: " << a.def << endl;
	cout << "Sinónimos: " << endl;
	if(a.sin.ocup>-1){
		for(i = 0; i <= a.sin.ocup; i++){
			cout << i+1 << ") "<<a.sin.datos[i] << endl;
		}
	}else{
		cout<<"-"<<endl;
	}
}
// END OPERACIONES DE ENTRADA Y SALIDA DE PALABRAS ------------------------------------------------------------------------------------------------------------------


// OPERACIONES DE MANEJO DE ARCHIVO DE PALABRAS  ------------------------------------------------------------------------------------------------------------------

//Permite guardar una palabra al final del archivo
void save_file_pal(parchivo fp, palabra a)
{
    fp = fopen("palabra.txt", "ab");
    fwrite(&a, sizeof(a), 1, fp);
    fclose(fp);
}

//Devuelve true si encontró la palabra por su cadena o false si no lo encontró
bool search_file_pal(parchivo fp, tcad buscado)
{
    bool existe = false;
    palabra a;
    fp = fopen("palabra.txt", "rb");
    if(fp != NULL)
        while(!feof(fp) && !existe)
        {
            fread(&a, sizeof(a), 1, fp);
            if(strcmp(a.cad, buscado) == 0)
                existe = true;
        }

    fclose(fp);
    return existe;
}

//Este modulo permite ver todos las palabras registradas en el archivo
//Al finalizar muestra un contador con el total de palabras registradas
void view_file_pal(parchivo fp)
{
	int i=0;
    palabra a;
    fp = fopen("palabra.txt", "rb");
    if(fp == NULL)
        cout << "Archivo inexistente" << endl;
    else
    {
        while(!feof(fp))
        {
            fread(&a, sizeof(a), 1, fp);
            if(!feof(fp)){
				i++;
                mostrar_palabra(a);
			}
        }
		cout<<"\nTotal: "<<i<<endl;
    }
    fclose(fp);
}

//modulo que permite eliminar una palabra del archivo por su cadena buscada
void delete_file_pal(parchivo fp, tcad buscado)
{
    palabra a;
    parchivo aux; //declaramos un puntero archivo auxiliar

    bool band = false;

	fp = fopen("palabra.txt", "rb");
    aux = fopen("temporal_p.txt", "wb"); //creamos un archivo temporal para vaciar las palabras que no coincidan con la cadena buscado

    if(fp != NULL)
    {
        while(!feof(fp))
        {
            fread(&a, sizeof(a), 1, fp);
            if(!feof(fp) &&  strcmp(a.cad, buscado) != 0)
                fwrite(&a, sizeof(a), 1, aux);
        }
    }

    fclose(aux);
    fclose(fp);
	
	//al finalizar, eliminamos el archivo original y  renombramos el temporal como el archivo original
    if(remove("palabra.txt") == 0 && rename("temporal_p.txt", "palabra.txt") == 0)
        cout << "Eliminado exitosamente!" << endl;
    else
        cout << "Error al eliminar" << endl;
}


/**
	Este modulo fue diseñado para rellenar el arbol de palabras con todas las palabras del ARCHIVO
	va recorriendo el archivo, y por cada palabra leiao (a) CREA un nodo para la palabra
	y lo inserta en el arbol de palabras (insercion por orden de palabra)
*/
void llenar_arbol_pal(parchivo fp, ppalabra &pp)
{
    palabra a;
    ppalabra nodo_pal;

    fp = fopen("palabra.txt", "rb");
    if(fp == NULL)
        cout << "Archivo inexistente" << endl;
    else
    {
        while(!feof(fp))
        {
            fread(&a, sizeof(a), 1, fp);
            if(!feof(fp))
            {
                crear_nodo(nodo_pal, a);
                insertar_arb_palabras(pp, nodo_pal);
            }
        }
    }
    fclose(fp);
}


/**
	Modulo modificado de una implementacion de recorrido EN-ORDEN
	Permite guardar una palabra del arbol en el archivo segun el orden del recorrido IZQ-RAIZ-DER
	Reutilizamos el modulo save_file_pal, que recibe el archivo y un registro de tipo palabra.
*/
void guardar_palabra_en_orden(parchivo fp, ppalabra a)
{
    if(a != NULL)
    {
		guardar_palabra_en_orden(fp,  a->izq);

        save_file_pal(fp, a->pal);

		guardar_palabra_en_orden(fp, a->der);
    }
}

/**
	Este modulo fue diseñado para contabilizar la cantidad de palabras registradas en el archivo o banco de palabras
*/
int  contar_palabras_banco(parchivo fp)
{
	int i=0;
	palabra a;
	fp = fopen("palabra.txt", "rb");
	if(fp == NULL)
		i=-1;
	else
	{
		while(!feof(fp))
		{
			fread(&a, sizeof(a), 1, fp);
			if(!feof(fp)){
				i++;
			}
		}
	}
	fclose(fp);
	return i;
}

/**
	Operacion de insercion de registros basica, va recorriendo el archivo de palabras y rellenando el vector auxiliar
	por cada palabra incrementa el contador y guarda la palabra leída hasta terminar de leer todas.
*/
void llenar_vector_palabras(parchivo fp, v_banco_palabras &vect, int &ocup)
{
	palabra a;
	fp = fopen("palabra.txt", "rb");
	if(fp == NULL)
		ocup=-1;
	else
	{
		while(!feof(fp))
		{
			fread(&a, sizeof(a), 1, fp);
			if(!feof(fp)){
				ocup++;
				vect[ocup]=a;
			}
		}
	}
	fclose(fp);
}


//END OPERACIONES DE MANEJO DE ARCHIVO DE PALABRAS  ------------------------------------------------------------------------------------------------------------------


/**	
	Modulo que permite insertar nueva palabra siguiendo ciertos pasos para poder hacer la insercion correctamente y mantener el archivo
	ordenado
*/
void insertar_palabra(parchivo fp, palabra p)
{
    ppalabra arbolpal; 									//arbol de palabras para ordenar

    if(!search_file_pal(fp, p.cad)) 					//buscamos que no exista palabra en el archivo
    {
        save_file_pal(fp, p); 							//si no existe se guarda nueva palabra

        iniciar_arb_palabras(arbolpal); 				//inicializamos el arbol

        llenar_arbol_pal(fp, arbolpal); 				//llenamos el arbol con los palabras con insercion segun el orden de la palabra

        if(remove("palabra.txt") == 0) 					//eliminamos el archivo actual (desordenado)
        {

            guardar_palabra_en_orden(fp, arbolpal);  	//recorremos el arbol en orden y vamos rellenando el archivo nuevamente
			cout<<"Se agregó palabra al banco"<<endl;
        }
        else
        {
            cout << "error al eliminar" << endl;
        }
		liberar_arbol(arbolpal); 						//una vez se finaliza el proceso se libera el arbol
    }
    else
        cout << "Palabra ya existe" << endl;
}




//MODULOS DE GESTIONAR RULETA DE PALABRAS -------------------------------------------------------------------------------------------------------------------------

void iniciar_ruleta(truleta &lis)
{
	lis.inicio=NULL;
	lis.fin=NULL;
}

/**
	Se modifica creacion de nodo para guardar el registro de tipo palabra en cada nodo
*/
void crear_nodo_ruleta(pnodo &nuevo, palabra p){
	nuevo = new tnodo;
	
	if(nuevo!=NULL){
		nuevo->sig=NULL;
		nuevo->ant=NULL;
		nuevo->dato=p;
	}else{
		cout<<"Memoria insuficiente"<<endl;
	}
}
/**
	Se modifica modulo de busqueda de lista para buscar una cadena en la ruleta
	Si encuentra devuelve el puntero con la palabra buscada
	Si no encuentra la palabra devuelve el nodo NULL
*/
pnodo buscar_palabra_ruleta(truleta lista, tcad buscado){
	pnodo i, ext=NULL;
	bool encontrado=false;
	
	if(lista.inicio!=NULL){
		for(i=lista.inicio; i!=NULL && !encontrado; i=i->sig){
			if(strcmp(i->dato.cad, buscado)==0){
				encontrado=true;
				ext=i;
			}
		}
	}
	
	return ext;
}

/**
	Agregar inicio comun
*/
void agregar_inicio_ruleta(truleta &li, pnodo nuevo){
	if(li.inicio==NULL){
		li.inicio=nuevo;
		li.fin=nuevo;
	}else{
		nuevo->sig = li.inicio;
		li.inicio->ant = nuevo;
		li.inicio = nuevo;
	}
}

/**
	Quitar inicio comun
*/
pnodo quitar_inicio_ruleta(truleta &lista){
	pnodo extraido;
	
	if(lista.inicio==NULL)
		extraido=NULL;
	else{
		if(lista.inicio==lista.fin){
			extraido=lista.inicio;
			lista.inicio=NULL;
			lista.fin=NULL;
		}else{
			extraido = lista.inicio;
			(lista.inicio->sig)->ant = NULL;
			lista.inicio = lista.inicio->sig;
			extraido->sig = NULL;
		}
	}

	return extraido;
}

/**
	Diseñamos el modulo que permite generar la ruleta de palabras con N palabras indicadas por parametro
	Se utiliza para la ruleta el vector auxiliar de palabras para agregar todas las palabras del banco
	Al finalizar tendremos la ruleta cargada con distintas palabras seleccionadas aleatoriamente y SIN REPETIR
*/
void generar_ruleta(parchivo fp, int cant_palabras_ruleta, truleta &ruleta){
																			//definimos el vector auxiliar para guardar las palabras del ARCHIVO
	int i,ocup_aux=-1;
	v_banco_palabras v_aux;
	pnodo nuevo;
	
	llenar_vector_palabras(fp,v_aux,ocup_aux);              				//leer el archivo y llenar el vector aux y su ocupado
	
	while(cant_palabras_ruleta>0){											//Ejecuta acciones si N palabras es mayor a 0
		i=aleatorio(0,ocup_aux);											//obtenemos un valor aleatorio entre 0 y el ocupado del vector aux.
		
		while(buscar_palabra_ruleta(ruleta,v_aux[i].cad)!=NULL)				//Se realiza la accion de Consultar si el vector en esta posicion ya existe en LA RULETA
			i=aleatorio(0,ocup_aux);                        				//si YA EXISTE, ELIJE otro aleatorio y vuelve a ejecutar bucle
		
		crear_nodo_ruleta(nuevo, v_aux[i]);                 				//SE CREA UN NODO QUE agrega la cadena tcad, a la ruleta
		
		if(nuevo!=NULL){
			agregar_inicio_ruleta(ruleta,nuevo); 							//Si se generó el nodo correctamente se agrega al inicio de la ruleta
			cant_palabras_ruleta--;                            				//decrementar cant palabras hasta llegar a 0
		}
	}																		//Finalmente repite el bucle
}

/**
	Se recorre la ruleta mientras no sea NULL y se va eliminando los nodos hasta llegar al ultimo nodo que sera NULL
*/
void liberar_ruleta(truleta &lista) {
	pnodo p;
	while(lista.inicio!=NULL){	
		p=lista.inicio->sig;	//en P, obtenemos el siguiente al puntero inicio
		delete(lista.inicio);	//eliminamos el puntero inicio
		lista.inicio=p;			//Asignamos al puntero de inicio, el siguiente guardado que es P
	}
}

/**
	Recorremos la ruleta para mostrar por cada nodo, el contenido de la ruleta.
*/
void mostrar_ruleta(truleta lis){
	pnodo i;
	if(lis.inicio!=NULL)
	{
		for(i=lis.inicio;i!=NULL;i=i->sig)
			mostrar_palabra(i->dato);
	}
	else
	 cout<<"Debe generar ruleta primero."<<endl;
}

//END MODULOS DE GESTIONAR RULETA DE PALABRAS -------------------------------------------------------------------------------------------------------------------------
	
	
	
//Sub menu y administracion de ruleta ----------------------------------------------------------------------------------------------------------------

void submenu_gestion_ruleta(int &op)
{
    cout << "===== GESTION DE RULETA =====" << endl;
    cout << "1. Registrar Palabra" << endl;
	cout << "2. Ver Banco de Palabras" << endl;
	cout << "3. Eliminar Palabra" << endl;
    cout << "4. Generar Ruleta de Palabras" << endl;
    cout << "5. Mostrar Ruleta Generada" << endl;
    cout << "6. Volver al Menú Principal" << endl;
    cout << "Seleccione una opcion: ";
    cin >> op;
}


/**
Desde el manager de ruleta se manejarán las distintas opciones del menu de gestion de ruleta
Se debe tener en cuenta que antes de insertar una palabra, está controlandose que el total de palabras sea MENOR al auxiliar de MAX_VECTOR_AUX_RULETA
Al momento de crear la ruleta se verifica que no se haya creado previamente, y que en el banco existan minimo 5 palabras
Al finalizar la creacion de la ruleta se cambia el flag a ruleta_creada=TRUE lo que indica que no se puede generar MAS ruletas en esta partida.
*/
void administrar_ruleta(parchivo f, truleta &ruleta, bool &ruleta_creada)
{
    int op;
    palabra nuevo;
    tcad buscado;
	int totalbanco=0,cant_palabras_ruleta=0;
	
    do
    {
        system("cls");
        submenu_gestion_ruleta(op);
        switch(op)
        {
        case 1:
			if(contar_palabras_banco(f)<MAX_VECTOR_AUX_RULETA){
				cargar_palabra(nuevo);
				insertar_palabra(f, nuevo);
			}else{
				cout<<"Se superó el maximo de palabras insertables"<<endl;
			}
            system("pause");
            break;
        case 2:
			view_file_pal(f);
            system("pause");
            break;
        case 3:
			cout << "Ingrese Palabra: ";
			fflush(stdin);
			gets(buscado);
			
			if(search_file_pal(f,buscado))
				delete_file_pal(f, buscado);
			else
				cout<<"Palabra no existe"<<endl;
			
			
			system("pause");
            break;
        case 4:
			if(!ruleta_creada){
				
				totalbanco = contar_palabras_banco(f);
				
				if(totalbanco<5){
					cout<<"Deben haber minimo 5 palabras para generar la ruleta!!!..."<<endl;
				}else{
					
					do{
						cout<<"ingrese cantidad de palabras de la ruleta min. (5), max. ("<< totalbanco<<"): ";
						cin>>cant_palabras_ruleta;
						
					} while(cant_palabras_ruleta<5 || cant_palabras_ruleta> totalbanco);
					
					generar_ruleta(f, cant_palabras_ruleta, ruleta);
					cout<<"Ruleta generada!!!..."<<endl;                      	//es todo. ruleta generada
					ruleta_creada=true;
				}
			}else{
				cout<<"La ruleta ya a sido generada!!...."<<endl;
			}
			
			system("pause");
			break;		
        case 5: 
			mostrar_ruleta(ruleta);
			system("pause");
			break;
		case 6: break;

        default:
            cout << "Opcion incorrecta" << endl;
            system("pause");
        }
    }
    while(op != 6);
}

