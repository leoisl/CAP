#ifndef INOUT_H_
#define INOUT_H_

#include <stdio.h>
#include <stdlib.h>
#include "basics.h"

#define FASTA_MAXLINE 1024

/*Fun��o que conta o n�mero de caracteres presentes na sequ�ncia do arquivo passado por par�metro, assumindo que o arquivo de entrada � um arquivo FASTA*/
int contaCharFasta (char *arq);

/*Fun��o que recebe um arquivo e seu tamanho, e grava a sequ�ncia deste arquivo em seq. Nota: para simplificar, seq conter� um espa�o na primeira posi��o (seq[0]=' '). Seq n�o termina em '\0'!*/
void leArquivoFasta(char *arq, int tam, char **seq);

/*fun��o que compara dois segmentos para ordena��o no qsort()*/
int compare ( const void * elem1, const void * elem2 );

/*Fun��o que l� os segmentos de um arquivo arq e grava em segmentos. Retorna a qtde de segmentos lidos*/
int leSegmentos (char *arq, struct segmento **segmentos);

/*Fun��o que l� uma sequ�ncia*/
void readSeq (char *arq, int pos);



#endif /* INOUT_H_ */
