/*
 * alglib.h
 *
 *  Created on: 04/03/2010
 *      Author: kishi
 */

#ifndef ALGLIB_H_
#define ALGLIB_H_

#include "biolib.h"
#include "iolib.h"
#include <stdlib.h>
#include <limits.h> /*INT_MIN*/

struct line
{
	char element;
	struct line *next;
};

struct block
{
	int block;
	struct block *next;
};

#define YES 0
#define NO 1

/*Calcula a pontuação de um casamento de caracteres*/
int delta(char a, char b);

/*Calcula o valor da similaridade entre duas sequências*/
int similarity(char *g, char *t, int gsize, int tsize);

/*Calcula o valor do melhor alinhamento semi-global entre duas sequências
 * deixando de penalizar as abas da sequencia g */
int semi_global_alignment_score(char *g, char *t, int gsize, int tsize);

/*Alinha duas sequências*/
struct line** align(char *g, char *t, int gsize, int tsize);

/*Aplica o algoritmo do alinhamento spliced, alocando SOMENTE as colunas de blocos, com saída em arquivo */
int procrustes(char *g, int gsize, int **b, int bsize, char *t, int tsize);

/*Aplica o algoritmo do alinhamento spliced, alocando SOMENTE as colunas de blocos, e retorna um vetor com os blocos */
int *procrustes_blocks(char *g, int gsize, int **b, int bsize, char *t, int tsize, int *retbsize, int *best_assembly_score);

/*2-aproximação para o problema do alinhamento múltiplo*/
struct line** star(char **t, int tsize, int *tsizes);

/*Insere um caracter numa lista ligada de caracteres, na primeira posição*/
void insert_char(struct line** l, char c);

/*Insere um caracter numa lista ligada de caracteres, numa determinada posição*/
void insert_char_in_pos(struct line** l, char c, int pos);

/*Insere uma sequência em um alinhamento*/
void insert_sequence(struct line** alinhamento, int pos, char *s, int ssize);

/*Libera a memória de um vetor de alinhamentos*/
void free_alignment(struct line*** A, int size);

/*Imprime um alinhamento na saída padrão*/
void print_alignment(struct line** A, int size);

/*Imprime um alinhamento em um arquivo*/
void print_alignment_to_file(struct line** A, int size, FILE *out);

/*Insere um bloco numa lista ligada de blocos, de maneira ordenada*/
void insert_block(struct block **l, int v);

/*Libera a memória de um vetor de listas de blocos*/
void free_all_blocks(struct block ***A, int size);

/*Libera a memória de uma lista de blocos*/
void free_blocks(struct block **A);

/*Imprime uma população de blocos*/
void print_blocks(struct block **A, int size);

/*Imprime uma população de blocos, em um arquivo apontado por out*/
void print_blocks_to_file(struct block **A, int size, FILE *out);

/*Ordena blocos pelo score*/
void sort_blocks(struct block **b, int *score, int n, int *tam, int *qtd);

/*Imprime os blocos, seus tamanhos, scores e quantidade*/
void print_blocks_with_info(struct block **A, int size, int *score, int *tam, int *qtd);

/*Imprime os blocos, seus tamanhos, scores e quantidade, em um arquivo apontado por out*/
void print_blocks_with_info_to_file(struct block **A, int size, int *score, int *tam, int *qtd, int **b, FILE *out);

/*Verifica se b2 está na lista de b1*/
char overlap(struct block *b1, struct block **b, int b2);

/*Remove o bloco na posição index*/
int delete_block(struct block **l, int index);


#endif /* ALGLIB_H_ */
