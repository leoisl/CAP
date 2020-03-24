/*
 * iolib.h
 *
 *  Created on: 07/08/2008
 *      Author: Kishi
 */
#ifndef IOLIB_H_
#define IOLIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FASTA_LINE_MAX_LEN 1000

/* Abre um arquivo e imprime mensagem de erro
 * em stdout caso não consiga abrir*/
FILE *openFile(char *filename, char *mode);

/* Abre um arquivo para escrita
 * utilizando openFile */
FILE *openFileForWriting(char *filename);

/* Abre um arquivo para leitura
 * utilizando openFile */
FILE *openFileForReading(char *filename);

/* Abre um arquivo para escrita no final
 * utilizando openFile */
FILE *openFileForAppending(char *filename);

/* Conta a quantidade de bases de um arquivo apontado por filename */
int countNucleotides(char *filename);

/* Conta a quantidade de bases de um arquivo em formato FASTA apontado por filename */
int countFastaNucleotides(char *filename);

/* Lê uma seqüência de um arquivo apontado por filename */
char *readSequence(char *filename, int *size);

/* Lê uma seqüência de um arquivo em formato FASTA apontado por filename */
char *readFastaSequence(char *filename, int *size);

/* Conta os blocos de um arquivo apontado por filename */
int countBlocks(char *filename);

/* Lê os blocos de um arquivo apontado por filename */
int **readBlocks(char *filename, int *size);

/* Lê os nomes dos arquivos das sequências de cDNA */
char **readcDNAFilenames(char *filename, int *size);

/* Imprime um bloco em um arquivo */
void printBlock(FILE *s, int a, int b);

/* Função que implementa o algoritmo
 * Insertion Sort para ordenar dois vetores
 * de inteiros ao mesmo tempo
 * OBS: Está comentada porque só é utilizada
 * dentro da própria biblioteca. */
/*void sort_two_arrays(int *index_array, int *data_array, int length);*/



#endif /* IOLIB_H_ */
