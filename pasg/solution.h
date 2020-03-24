#ifndef SOLUTION_H_
#define SOLUTION_H_

/*matriz de alinhamento*/
struct cell ****matrix;

/*matriz tempor�ria para c�lculo da matriz de alinhamento*/
struct tempcell **tempmatrix;

/*inicializa a matriz*/
void initMatrix();

/*calcula a solu��o*/
void calculate();

/*determina Gamma_B e Gamma_C baseado na matriz calculada*/
void getSolution();

#endif
