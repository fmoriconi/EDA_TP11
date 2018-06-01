#include "parseCmdLine.h"
#include <stdio.h>
#include <string.h>

int parseCmdLine(int argc, char *argv[], pCallback p, void *userData)
{
	int vacounter = 0; // "Valid argument counter"
	bool abort = false;

	for (int i = 1; i < argc && abort != true; i++)
	{
		if (*argv[i] == OPTION_IDENTIFIER)
		{

			if ((i != argc - 1) && (p(argv[i], argv[i + 1], userData) == true))
			{
				vacounter++;
			}
			else
			{
				if (i == argc - 1) //Si el ultimo argumento es una opción, no tiene clave.
				{
					printf("Error type 1: Option without value\n");

					if (strlen(argv[i]) == 1) //Si la opción tiene un solo caracter, no tiene clave.
					{
						printf("Error type 2: Option without key\n");
					}
				}
				else if (strlen(argv[i]) == 1) //Si la opción tiene un solo caracter, no tiene clave.
				{
					printf("Error type 2: Option without key\n");
				}
				else //Si no es uno de esos errores, la clave y/o el valor son incorrectos.
				{
					printf("Error: Invalid input\n");
				}
				abort = true;
			}
			i++;
		}
		else
		{
			if (p(NULL, argv[i], userData) == 1)
			{
				vacounter++;
			}
			else
			{
				abort = true;
			}
		}
	}

	if (abort == true)
	{
		vacounter = ERRORPARSE; //Si hay error alguno, devuelve -1.
	}
	return vacounter;
}