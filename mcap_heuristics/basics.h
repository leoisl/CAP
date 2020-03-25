#ifndef BASICS_H_
#define BASICS_H_

#include <stdio.h>

/*macros para similaridade*/
#define SIMILARIDADE 1
#define SPACESCORE -2
#define MATCHSCORE 1
#define MISMATCHSCORE -1



/*macros para distância de edição
#define SIMILARIDADE 0
#define SPACESCORE -1
#define MATCHSCORE 0
#define MISMATCHSCORE -1
*/

/*Macros para impressão de dados*/
#define INFO 1
#define DEBUG 0
#define COMPARE 0
#define BENCHMARK 0

int heuristica; //# da heuristica
char **arqEntrada; //arquivos de entrada. Necessário para o gelfand ficar mais eficiente.

/*Variáveis de entrada do PASGM*/
int nSeqs; /*# de sequências*/
char **seqs; /*representa as sequências de entrada*/
int *tamSeqs; /*representa o tamanho das sequências de entrada*/

/*struct que representa um segmento*/
struct segmento
{
	int first;
	int last;
	int number;
	int tam;
};
struct segmento **segs; /*representa os conjuntos de segmentos de entrada*/
int *tamSegs; /*representa o tamanho dos conjuntos de segmentos de entrada*/

/*inicializa variáveis globais de entrada do PASGM*/
void initVariables ();


/*Variáveis de saída do PASGM*/
int **gammas; /*representa as cadeias da saída*/
int *tamGammas; /*representa o tamanho das cadeias da saída*/


/*Variáveis de entrada do PASG interno*/
char *s1; /*representa a sequência s_1*/
int n; /*|s_1|*/
char *s2; /*representa a sequência s_2*/
int m; /*|s_2|*/
struct segmento *B; /*representa o conjunto B de segmentos de s_1*/
int u; /*|B|*/
struct segmento *C; /*representa o conjunto C de segmentos de s_2*/
int v; /*|C|*/

/*Variáveis de saída do PASG interno*/
int PASGSim; /*valor da solução ótima*/
int *gammaB; /*representa a cadeia de segmentos Gamma_B*/
int tamGammaB;
int *gammaC; /*representa a cadeia de segmentos Gamma_C*/
int tamGammaC;


/*estruturas utilizadas no PASG*/
struct cell {
    int value;
    struct cell *before;
    int numSegmentos;
    int k;
    int l;
};
struct tempcell {
    int value;
    struct cell *before;
    int numSegmentos;
};

/*FUNÇÕES UTILIZADAS PELO PASG*/
/*retorna o score de dois caracteres*/
int scoreFunction (char c1, char c2);

/*retorna o caractere b_k[i]*/
char bki(int k, int i);

/*retorna o caractere c_l[j]*/
char clj(int l, int j);

/*verifica se klinha precede k*/
int precedek (int klinha, int k);

/*verifica se llinha precede l*/
int precedel (int llinha, int l);

/*devolve |k'|, usado na equação.*/
int lastLinha(int k);

/*devolve |l'|, usado na equação.*/
int lastColuna(int l);

/*verifica se mem é null ou não...*/
void eval(void* mem);

/*retorna o maior dos dois valores*/
int max (int a, int b);

#endif /* BASICS_H_ */
