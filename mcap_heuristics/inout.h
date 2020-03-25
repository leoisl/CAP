#ifndef INOUT_H_
#define INOUT_H_

#include <stdio.h>
#include <stdlib.h>
#include "basics.h"

#define FASTA_MAXLINE 1024

/*Função que conta o número de caracteres presentes na sequência do arquivo passado por parâmetro, assumindo que o arquivo de entrada é um arquivo FASTA*/
int contaCharFasta (char *arq);

/*Função que recebe um arquivo e seu tamanho, e grava a sequência deste arquivo em seq. Nota: para simplificar, seq conterá um espaço na primeira posição (seq[0]=' '). Seq não termina em '\0'!*/
void leArquivoFasta(char *arq, int tam, char **seq);

/*função que compara dois segmentos para ordenação no qsort()*/
int compare ( const void * elem1, const void * elem2 );

/*Função que lê os segmentos de um arquivo arq e grava em segmentos. Retorna a qtde de segmentos lidos*/
int leSegmentos (char *arq, struct segmento **segmentos);

/*Função que lê uma sequência*/
void readSeq (char *arq, int pos);



#endif /* INOUT_H_ */
