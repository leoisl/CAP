/*
 * iolib.h
 *
 *  Created on: 07/08/2008
 *      Author: Kishi
 */

#include "iolib.h"

FILE *openFile(char *filename, char *mode)
{
	FILE *filehandler;
	filehandler = fopen(filename, mode);
	if(!filehandler)
	{
		printf("Error! Cannot open file %s\n", filename);
		exit(0);
	}
	return filehandler;
}

FILE *openFileForWriting(char *filename)
{
	return openFile(filename, "w");
}

FILE *openFileForAppending(char *filename)
{
	return openFile(filename, "a");
}

FILE *openFileForReading(char *filename)
{
	return openFile(filename, "r");
}

int countNucleotides(char *filename)
{
	char c;
	int n = 0;

	FILE *filehandler = openFile(filename, "r");
	while( fscanf(filehandler, "%c", &c) != -1)
	{
		n++;
	}
	fclose(filehandler);
	return n;
}

int countFastaNucleotides(char *filename)
{
	int n = 0;
	char line[FASTA_LINE_MAX_LEN];
	int i = 0;
	char dummy1, dummy2;
	int fscanf_retval;

	FILE *filehandler = openFile(filename, "r");

	fscanf_retval = fscanf(filehandler, "%[^\n]", line);
	while( fscanf_retval > 0 )
	{
		dummy2 = fscanf(filehandler, "%c", &dummy1);
		if( !(line[0] == '>' || line[0] == ';'))
		{
			for(i = 0; i < FASTA_LINE_MAX_LEN && line[i] != '\0'; i++ )
				n++;
		}
		fscanf_retval = fscanf(filehandler, "%[^\n]", line);
	}
	fclose(filehandler);
	return n;
}

char *readSequence(char *filename, int *size)
{
	char *s;
	FILE *filehandler;
	int i;
	int dummy; /* Só serve pra tirar o warning do fscanf */

	(*size) = countNucleotides(filename);
	/* Como o vetor começa em 1, aumentamos o tamanho em 1 */
	(*size)++;
	s = (char*) malloc((*size) * sizeof(char));
	filehandler = openFileForReading(filename);

	/*Insere lixo na primeira posição*/
	s[0] = ' ';

	for(i = 1; i < (*size); i++)
	{
		dummy = fscanf(filehandler, "%c", &(s[i]) );
	}
	fclose(filehandler);
	return s;
}

char *readFastaSequence(char *filename, int *size)
{
	char *s;
	FILE *filehandler;
	int i,j;
	char line[FASTA_LINE_MAX_LEN];
	char dummy1, dummy2;

	(*size) = countFastaNucleotides(filename);
	/* Como o vetor começa em 1, aumentamos o tamanho em 1 */
	(*size)++;
	s = (char*) malloc((*size) * sizeof(char));
	filehandler = openFileForReading(filename);

	/*Insere lixo na primeira posição*/
	s[0] = ' ';

	i = 1;
	while( fscanf(filehandler, "%[^\n]", line) > 0)
	{
		dummy2 = fscanf(filehandler, "%c", &dummy1);
		if( !(line[0] == '>' || line[0] == ';'))
		{
			for(j = 0; j < FASTA_LINE_MAX_LEN && line[j] != '\0'; j++ )
				s[i++] = line[j];
		}
	}
	fclose(filehandler);
	return s;
}

int countBlocks(char *filename)
{
	char c;
	int n = 0;

	FILE *filehandler = openFile(filename, "r");
	while( fscanf(filehandler, "%c", &c) != -1)
	{
		if(c == ',')
			n++;
	}
	fclose(filehandler);
	return n;
}

void sort_two_arrays(int *index_array, int *data_array, int length)
{
	int i, j, aux1, aux2;

	for(i = 1; i < length; i++)
	{
		aux1 = index_array[i];
		aux2 = data_array[i];
		j = i - 1;
		while(j >= 0 && index_array[j] > aux1)
		{
			index_array[j + 1] = index_array[j];
			data_array[j + 1] = data_array[j];
			j--;
		}
		index_array[j + 1] = aux1;
		data_array[j + 1] = aux2;
	}
}

int **readBlocks(char *filename, int *size)
{
	int **s;
	FILE *filehandler;
	int i;
	char c;
	int *last_aux;
	int dummy; /* Só serve pra tirar o warning do fscanf */

	(*size) = countBlocks(filename);
	/* Como o vetor começa em 1, aumentamos o tamanho em 1 */
	(*size)++;
	s = (int**) malloc(3 * sizeof(int*));

	/* Os vetores s[0] e s[1] estão ordenados por first[] */
	/* s[0][k] é o elemento equivalente ao first[k] */
	s[0] = (int*) malloc((*size) * sizeof(int));

	/* s[1][k] é o elemento equivalente ao last[k] */
	s[1] = (int*) malloc((*size) * sizeof(int));

	/* O vetor s[2] mapeará uma ordenação
	 * de s[0] e s[1] por last[] */
	s[2] = (int*) malloc((*size) * sizeof(int));

	filehandler = openFileForReading(filename);

	/* Como o vetor começa em 1, comçamos a contagem em 1 */
	s[0][0] = 0; s[1][0] = 0; s[2][0] = 0;
	for(i = 1; i < (*size); i++)
	{
		dummy = fscanf(filehandler, "%d", &(s[0][i]) );
		dummy = fscanf(filehandler, "%c", &c );
		dummy = fscanf(filehandler, "%c", &c );
		dummy = fscanf(filehandler, "%d", &(s[1][i]) );
		dummy = fscanf(filehandler, "%c", &c );
		s[2][i] = i;
	}
	fclose(filehandler);

	/* Ordena s[0] e s[1] pelos valores de s[0] */
	sort_two_arrays(s[0], s[1], (*size));

	/* Cria uma cópia temporária de s[1]
	 * (vetor last), para ordenar o mapeamento
	 * de s em s[2] */
	last_aux = (int*) malloc((*size) * sizeof(int));
	memcpy(last_aux, s[1], (*size) * sizeof(int));

	/* Ordena s[2] e last_aux pelos valores de s_aux */
	sort_two_arrays(last_aux, s[2], (*size));

	/* Libera a memória de last_aux */
	free(last_aux);
	return s;
}

char **readcDNAFilenames(char *filename, int *size)
{
	int i;
	char **cdnas;
	FILE *filehandler = openFile(filename, "r");
	int dummy; /* Só serve pra tirar o warning do fscanf */

	/*Lê a quantidade de sequências*/
	dummy = fscanf(filehandler, "%d", size);
	cdnas = (char **) malloc (sizeof(char*) * (*size) );

	/*Lê os nomes de sequências*/
	for(i = 0; i < (*size); i++)
	{
		/* //TODO  Ajustar tamanho máx do nome do arquivo*/
		cdnas[i] = (char *) malloc (sizeof(char) * 100 );
		dummy = fscanf(filehandler, "%s", cdnas[i]);
	}

	fclose(filehandler);
	return cdnas;
}

void printBlock(FILE *s, int a, int b)
{
	fprintf(s,"%d..%d,", a, b);
}


