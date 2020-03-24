#ifndef SOLUTION_H_
#define SOLUTION_H_

/*matriz de alinhamento*/
struct cell ****matrix;

/*matriz temporária para cálculo da matriz de alinhamento*/
struct tempcell **tempmatrix;

/*inicializa a matriz*/
void initMatrix();

/*calcula a solução*/
void calculate();

/*determina Gamma_B e Gamma_C baseado na matriz calculada*/
void getSolution();

/*libera a memória utilizada*/
void cleanUpPASG();

/*faz o PASG das duas sequências passadas por parâmetro*/
void PASG(int seq1, int seq2);

#endif
