/*****************************************************************************************************************
	UNIVERSIDAD NACIONAL AUTONOMA DE MEXICO
	FACULTAD DE ESTUDIOS SUPERIORES -ARAGON-

	Computadoras y programacion. 
	Emiliano Candia Carrillo 322289691
	
 Programa principal de la aplicacion de la distancia de Levenstein.
	
******************************************************************************************************************/

#include "stdafx.h"
#include <string.h>
#include "corrector.h"

/*****************************************************************************************************************
	DICCIONARIO: Esta funcion crea el diccionario completo
	char *	szNombre				:	Nombre del szFile de donde se sacaran las palabras del diccionario	
	char	szPalabras[][TAMTOKEN]	:	Arreglo con las palabras completas del diccionario
	int		iEstadisticas[]			:	Arreglo con el numero de veces que aparecen las palabras en el diccionario
	int &	iNumElementos			:	Numero de elementos en el diccionario
******************************************************************************************************************/
void	Diccionario(char* szNombre, char szPalabras[][TAMTOKEN], int iEstadisticas[], int& iNumElementos)
{
	FILE* szFile;
	char cSzPalabras[NUMPALABRAS][TAMTOKEN];
	int  cChar, i, ii, iFin, iii;
	char cPalabra[TAMTOKEN];

	//Abre Archivo
	fopen_s(&szFile, szNombre, "r");
	if (szFile == NULL)
		return;

	iNumElementos = 0; ii = 0;
	cChar = fgetc(szFile);
	while (feof(szFile) == 0)
	{
		if (!((cChar == 59) || (cChar == 46) || (cChar == 44) || (cChar == 41) || (cChar == 40) || (cChar == 32) || (cChar == 13) || (cChar == 10) || (cChar == 9)))
		{
			cSzPalabras[iNumElementos][ii] = towlower((char)cChar);
			cChar = fgetc(szFile);
			ii++;

		}
		else
		{
			if (ii > 0)
			{
				cSzPalabras[iNumElementos][ii] = '\0';
				iEstadisticas[iNumElementos] = 1;

				if (iNumElementos > 0)
				{
					for (i = 0; i < iNumElementos; i++)
						if (strcmp(cSzPalabras[i], cSzPalabras[iNumElementos]) == 0)
						{
							iEstadisticas[i] = iEstadisticas[i] + 1;
							i = iNumElementos;
							iNumElementos = iNumElementos - 1;
						}
				}

				iNumElementos++;
				ii = 0;
			}
			cChar = fgetc(szFile);
		}
	}
	fclose(szFile);

	cSzPalabras[iNumElementos][ii] = '\0';
	iEstadisticas[iNumElementos] = 1;

	for (i = 0; i < iNumElementos; i++)
		if (strcmp(cSzPalabras[i], cSzPalabras[iNumElementos]) == 0)
		{
			iEstadisticas[i] = iEstadisticas[i] + 1;
			strcpy_s(cSzPalabras[iNumElementos], "");
			i = iNumElementos;
		}

	for (i = 0; i < iNumElementos; i++)
	{
		iFin = 0;
		for (ii = 0; ii < iNumElementos; ii++)
		{
			if (strcmp(cSzPalabras[ii + 1], cSzPalabras[ii]) < 0)
			{
				strcpy_s(cPalabra, cSzPalabras[ii]);
				iii = iEstadisticas[ii];
				strcpy_s(cSzPalabras[ii], cSzPalabras[ii + 1]);
				iEstadisticas[ii] = iEstadisticas[ii + 1];
				strcpy_s(cSzPalabras[ii + 1], cPalabra);
				iEstadisticas[ii + 1] = iii;
			}
			else
				iFin += 1;
		}
		if (iFin == iNumElementos)
			i = iNumElementos;
	}

	if (strcmp(cSzPalabras[0], "") == 0)
	{
		for (i = 0; i < iNumElementos; i++)
		{
			strcpy_s(szPalabras[i], cSzPalabras[i + 1]);
			iEstadisticas[i] = iEstadisticas[i + 1];
		}
		iEstadisticas[i] = '\0';
	}
	else
	{
		iNumElementos++;
		for (i = 0; i < iNumElementos; i++)
			strcpy_s(szPalabras[i], cSzPalabras[i]);
	}
}

/*****************************************************************************************************************
	ListaCandidatas: Esta funcion recupera desde el diccionario las palabras validas y su peso
	Regresa las palabras ordenadas por su peso
	char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
	int		iNumSugeridas,						//Lista de palabras clonadas
	char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
	int		iEstadisticas[],					//Lista de las frecuencias de las palabras
	int		iNumElementos,						//Numero de elementos en el diccionario
	char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
	int		iPeso[],							//Peso de las palabras en la lista final
	int &	iNumLista)							//Numero de elementos en la szListaFinal
******************************************************************************************************************/
void	ListaCandidatas		(
	char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
	int		iNumSugeridas,						//Lista de palabras clonadas
	char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
	int		iEstadisticas[],					//Lista de las frecuencias de las palabras
	int		iNumElementos,						//Numero de elementos en el diccionario
	char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
	int		iPeso[],							//Peso de las palabras en la lista final
	int &	iNumLista)							//Numero de elementos en la szListaFinal
{
	int i, c;
	//Hace el array szListaFinal | iPeso
	iNumLista = 0;
	for (i = 0; i < iNumSugeridas; i++)
	{
		for (c = 0; c < iNumElementos; c++)
		{
			if (strcmp(szPalabrasSugeridas[i], szPalabras[c]) == 0)
			{
				strcpy_s(szListaFinal[iNumLista], szPalabras[c]);
				iPeso[iNumLista++] = iEstadisticas[c];
			}
		}
	}


	//Acomoda de mayor a menor
	i = 0;
	char cSzListaFinal1[TAMTOKEN], cSzListaFinal2[TAMTOKEN];
	int cPeso1, cPeso2;
	while (iNumLista > i)
	{
		for (c = i + 1; c < iNumLista; c = c + 1)
		{
			if (iPeso[i] < iPeso[c])
			{
				cPeso1 = iPeso[i];
				cPeso2 = iPeso[c];
				strcpy_s(cSzListaFinal1, szListaFinal[i]);
				strcpy_s(cSzListaFinal2, szListaFinal[c]);
				
				iPeso[i] = cPeso2;
				iPeso[c] = cPeso1;
				strcpy_s(szListaFinal[i], cSzListaFinal2);
				strcpy_s(szListaFinal[c], cSzListaFinal1);
			}
		}
		i++;
	}

	//Borra repe
	int ciNumLista = iNumLista, ii, cNumElementos = iNumElementos;
	i = 0;
	while (ciNumLista > i)
	{
		for (c = i + 1; c <= ciNumLista; c++)
		{
			if (strcmp(szListaFinal[i],szListaFinal[c]) == 0)
			{
				for (ii = c; ii < cNumElementos; ii++)
				{
					strcpy_s(szListaFinal[ii], szListaFinal[ii + 1]);
					iPeso[ii] = iPeso[ii +1];
				}
				ciNumLista--;
				c--;
			}
		}
		i++;
	}
	iNumLista = ciNumLista;
}

/*****************************************************************************************************************
	ClonaPalabras: toma una palabra y obtiene todas las combinaciones y permutaciones requeridas por el metodo
	char *	szPalabraLeida,						// Palabra a clonar
	char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
	int &	iNumSugeridas)						//Numero de elementos en la lista
******************************************************************************************************************/
void	ClonaPalabras(
	char *	szPalabraLeida,						// Palabra a clonar
	char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
	int &	iNumSugeridas)						//Numero de elementos en la lista
{
	//Limpia cSzPalabraLeida
	char cSzPalabraLeida[TAMTOKEN] = "nada";
	strnset(cSzPalabraLeida, '\0', TAMTOKEN);
	int i, ii;
	iNumSugeridas = 0;
	int nSzPalabraLeida;
	nSzPalabraLeida = strlen(szPalabraLeida);
	

	//Letra eliminada
	for (i = 0; i <= nSzPalabraLeida; i++) 
	{
		//Copia array szPalabraLeida a cSzPalabraLeida
		for (ii = 0; ii < nSzPalabraLeida; ii++) //nLetras por nSzPalabraLeida
		{
			cSzPalabraLeida[ii] = szPalabraLeida[ii];
		}

		for (ii = i; ii < nSzPalabraLeida; ii++)
		{
			cSzPalabraLeida[ii] = cSzPalabraLeida[ii + 1];
		}

		if (cSzPalabraLeida[0] != ' ' && cSzPalabraLeida[0] != '\0')
		{
			strcpy_s(szPalabrasSugeridas[iNumSugeridas++], cSzPalabraLeida);
		}
	}
	strnset(cSzPalabraLeida, '\0', TAMTOKEN);

	//Letras intercambiada de izquierda a derecha.
	char cLetraIzq, cLetraDer;
	int iii;
	//strnset(cSzPalabraLeida, '\0', TAMTOKEN);
	i = 0;
	for (iii = 1; iii < nSzPalabraLeida; iii++) 
	{
		//Copia array szPalabraLeida a cSzPalabraLeida(PalabraLeidaEdit)
		for (ii = 0; ii < nSzPalabraLeida; ii++)
		{
			cSzPalabraLeida[ii] = szPalabraLeida[ii];
		}

		cLetraIzq = szPalabraLeida[i];
		cLetraDer = szPalabraLeida[i + 1];

		cSzPalabraLeida[i] = cLetraDer;
		cSzPalabraLeida[i + 1] = cLetraIzq;
		if (cSzPalabraLeida[0] != ' ' && cSzPalabraLeida[0] != '\0')
		{
			strcpy_s(szPalabrasSugeridas[iNumSugeridas++], cSzPalabraLeida);
		}
		i++;
	}
	
	strnset(cSzPalabraLeida, '\0', TAMTOKEN);
	
	//insertar LetrasABC al principio, y final de cada letra de PalLeida
	char LetrasABC[] = "abcdefghijklmn�opqrstuvwxyz�����";
	int numABC = strlen(LetrasABC), c;
	
	for (i = 0; i <= nSzPalabraLeida; i++)
	{
		//Copia array szPalabraLeida a cSzPalabraLeida(PalabraLeidaEdit)
		for (ii = 0; ii < nSzPalabraLeida; ii++)
		{
			cSzPalabraLeida[ii] = szPalabraLeida[ii];
		}

		//Recorrer array
		for (ii = nSzPalabraLeida; ii > i; ii--)
		{
			cSzPalabraLeida[ii] = cSzPalabraLeida[ii - 1];
		}

		for (ii = 0; ii < numABC; ii++) //se cambio < por <=
		{
			cSzPalabraLeida[i] = LetrasABC[ii];
			if (cSzPalabraLeida[0] != ' ' && cSzPalabraLeida[0] != '\0')
			{
				strcpy_s(szPalabrasSugeridas[iNumSugeridas++], cSzPalabraLeida);
			}
		}
	}
	strnset(cSzPalabraLeida, '\0', TAMTOKEN);


	//Letra sustituida por letra del LetrasABC
	for (i = 0; i < nSzPalabraLeida; i++)
	{
		for (ii = 0; ii < numABC; ii++) //no
		{
			//Copia array szPalabraLeida a cSzPalabraLeida(PalabraLeidaEdit)
			for (c = 0; c < nSzPalabraLeida; c++)
			{
				cSzPalabraLeida[c] = szPalabraLeida[c];
			}

			cSzPalabraLeida[i] = LetrasABC[ii];
			strcpy_s(szPalabrasSugeridas[iNumSugeridas++], cSzPalabraLeida);
		}
	}
	strnset(cSzPalabraLeida, '\0', TAMTOKEN);

	//Ordenar por orden alfabetico
	char cSzListaFinal1[TAMTOKEN], cSzListaFinal2[TAMTOKEN];
	//ii = 0;
	//ii = 0;
	i = 0;
	while (iNumSugeridas > i)
	{
		for (ii = i + 1; ii < iNumSugeridas; ii = ii + 1)
		{
			if (strcmp(szPalabrasSugeridas[i], szPalabrasSugeridas[ii]) > 0)
			{
				strcpy_s(cSzListaFinal1, szPalabrasSugeridas[i]);
				strcpy_s(cSzListaFinal2, szPalabrasSugeridas[ii]);
				
				strcpy_s(szPalabrasSugeridas[i], cSzListaFinal2);
				strcpy_s(szPalabrasSugeridas[ii], cSzListaFinal1);
			}
		}
		i++;
	}
	//limpia Palabra Leida
	strnset(szPalabraLeida, '\0', TAMTOKEN);
}
