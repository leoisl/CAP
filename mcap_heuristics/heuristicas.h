#ifndef HEURISTICAS_H_
#define HEURISTICAS_H_

#include "basics.h"

/*função usada para inicializar as variáveis que representam a solução*/
void initSolution();

/*calcula a similaridade de s e t*/
int similaridade (const char *s, const char *t, int m, int n);

/*função que calcula o valor da solução encontrada por uma heurística*/
int calcularValorSolucao();

/*função que imprime a resposta de uma heurística*/
void imprimirResposta();

/*função que implementa a primeira heurística*/
void calcularCadeiasDeConsenso();

/*estrutura utilizada na heurística gulosa*/
struct PASGReturn {
    int value;
    int tamGammaB, tamGammaC;
    int *gammaB, *gammaC;
};

/*função que faz PASG de todos com todos*/
void PASGTodosComTodos(struct PASGReturn allPASGs[nSeqs][nSeqs]);

/*função que implementa a heurística gulosa*/
void calcularHeuristicaGulosa();

/*função que calcula GelfandSim()*/
int gelfandSim(int k, char *gammaij, int tamGammaij);

/*função que implementa a terceira heurística, que usa o Gelfand*/
void calcularHeuristicaPASGGelfand();

/*função que concatena \Gamma_i e grava em arquivo*/
void concatenaGammaIEmArq(int i);


#endif
