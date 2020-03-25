#ifndef HEURISTICAS_H_
#define HEURISTICAS_H_

#include "basics.h"

/*fun��o usada para inicializar as vari�veis que representam a solu��o*/
void initSolution();

/*calcula a similaridade de s e t*/
int similaridade (const char *s, const char *t, int m, int n);

/*fun��o que calcula o valor da solu��o encontrada por uma heur�stica*/
int calcularValorSolucao();

/*fun��o que imprime a resposta de uma heur�stica*/
void imprimirResposta();

/*fun��o que implementa a primeira heur�stica*/
void calcularCadeiasDeConsenso();

/*estrutura utilizada na heur�stica gulosa*/
struct PASGReturn {
    int value;
    int tamGammaB, tamGammaC;
    int *gammaB, *gammaC;
};

/*fun��o que faz PASG de todos com todos*/
void PASGTodosComTodos(struct PASGReturn allPASGs[nSeqs][nSeqs]);

/*fun��o que implementa a heur�stica gulosa*/
void calcularHeuristicaGulosa();

/*fun��o que calcula GelfandSim()*/
int gelfandSim(int k, char *gammaij, int tamGammaij);

/*fun��o que implementa a terceira heur�stica, que usa o Gelfand*/
void calcularHeuristicaPASGGelfand();

/*fun��o que concatena \Gamma_i e grava em arquivo*/
void concatenaGammaIEmArq(int i);


#endif
