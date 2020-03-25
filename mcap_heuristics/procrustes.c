/*
 ============================================================================
 Name        : procrustes.c
 Author      : Rodrigo Kishi
 Version     : 1.0
 Copyright   :
 Description : spliced sequence alignment tool (Procrustes)
 ============================================================================
 */
#include "procrustes.h"
#include <stdlib.h>
#include <limits.h>
#include "iolib.h"
#include "biolib.h"
#include "alglib.h"


//Entrada:
//gelfand.exe <seq g> <blocos b de g> <seq t>
//g e t em Fasta
//b = não sei se é igual do genscan e do q eu faço...
int gelfand(char *seqG, char* blocksB, char* seqT)
{

	/*Seqüência G*/
	char *g;
	int gsize;

	/*Seqüência T*/
	char *t;
	int tsize;

	int score;

	/*Blocos*/
	/* b[0][k] é o elemento equivalente ao first[k] */
	/* b[1][k] é o elemento equivalente ao last[k] */
	/* Os vetores b[0] e b[1] estão ordenados por first[] */
	/* O vetor s[2] mapeará uma ordenação
		 * de s[0] e s[1] por last[] */
	int **b;
	int bsize;

	/* Realiza a leitura das seqüências */
	g = readFastaSequence(seqG, &gsize);
	t = readFastaSequence(seqT, &tsize);

	/* Realiza a leitura dos blocos */
	/* Assumiremos que os índices dos
	 * blocos na seqüência começam em 0.
	 * Caso isso não seja verdadeiro, modificar a readBlocks */
	b = readBlocks(blocksB, &bsize);

	score = procrustes(g, gsize, b, bsize, t, tsize);

	free(g);
	free(t);
	free(b[0]);
	free(b[1]);
	free(b[2]);
	free(b);

	return score;
}

