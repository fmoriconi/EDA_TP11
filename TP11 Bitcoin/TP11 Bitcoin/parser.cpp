#include "parser.h"
#include "parameters.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_PARAM_SIZE 7

#define MAXNODES 20
#define MINNODES 5

#define MAXMINERS (MAXNODES-1)
#define MINMINERS 2

bool parseCallback(char *key, char *value, void * parameters)
{

	class parameters_t * data = (class parameters_t*) parameters ;  //Esto es redundante, pero visual studio no me dejaba desreferenciarlo bien si no lo hacía.


	bool validez = false; //Respuesta del callback
	char casehandler[MAX_PARAM_SIZE]; //String para trabajar con las mayusculas/minusculas.

	if (key[0] == '-') // Analizo parámetros
	{
		int i;
		int cerrarBucle = 0;

		if (strlen(&(key[1])) <= MAX_PARAM_SIZE) {
			strcpy(casehandler, &(key[1])); // para no tomar con el strcmp el OPTION_IDENTIFIER
		}

		for (i = 0; (i < OPTION_QTY) && cerrarBucle == 0; i++) //ciclo el arreglo de opciones hasta hallar la clave igual
		{
			if (!(strcmp(strlower(casehandler), data->options[i])))
			{
				if(checkValues(key,value,parameters)){
					validez = true;
					cerrarBucle = 1;
				}
			}
			else  //Ocurre en el caso en el que la clave no es valida.
			{
				validez = false;
			}
		}

	}
	return validez;
}

/***********************************
Función strlwr
************************************/

char* strlower(char str[])
{
	int i;
	for (i = 0; str[i] != '\0'; i++) {
		str[i] = tolower(str[i]); //Todo caracter en mayuscula se pasa a minuscula, logrando que el programa sea case insensitive.
	}
	return str;
}

bool checkValues(char * key, char * value, void * parameters) {

	class parameters_t * param = (class parameters_t *)parameters;
	bool validez = false;

	char casehandler[MAX_PARAM_SIZE]; //String para trabajar con las mayusculas/minusculas.

	if (strlen(&(key[1])) <= MAX_PARAM_SIZE)
	{
		strcpy(casehandler, &(key[1])); // Para no tomar con el strcmp el OPTION_IDENTIFIER, además de asegurarnos que podamos modificar el string.
		if (!(strcmp(strlower(casehandler), param->options[0])) && (atoi(value) <= MAXNODES) && (atoi(value) >= MINNODES)) { //Corroboro cual parámetro es y si cumple los valores en los que está acotado.
			if (param->flags[0] == false) {
				param->nodes = atoi(value);
				validez = true;
				param->flags[0] = true;
			}
		}
		else if (!(strcmp(strlower(casehandler), param->options[1])) && (atoi(value) < param->nodes) && (atoi(value) >= MINMINERS)) {
			if (param->flags[1] == false) {
				{
					param->miners = atoi(value);
					validez = true;
					param->flags[1] = true;
				}
			}
		}
	}

	return validez;

}