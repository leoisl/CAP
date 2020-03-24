#include "basics.h"

#include <stdlib.h>

/*inicializa variáveis globais do PASGM*/
void initVariables () {
    seqs = (char**) malloc (nSeqs*sizeof(char*));
    eval(seqs);
    tamSeqs = (int*) malloc(nSeqs*sizeof(int));
    eval(tamSeqs);
    segs = (struct segmento**) malloc (nSeqs*sizeof(struct segmento*));
    eval(segs);
    tamSegs = (int*) malloc(nSeqs*sizeof(int));
    eval(tamSegs);
}


/*retorna o score de dois caracteres*/
int scoreFunction (char c1, char c2) {
    if (c1=='-' && c2=='-') {
        printf("Error: cannot align 2 spaces.\n");
        exit(EXIT_FAILURE);
    }

    if (c1=='-' || c2=='-')
        return SPACESCORE;

    if(c1 == c2)
        return MATCHSCORE; /* se iguais retorna valor de match */
    else
       return MISMATCHSCORE; /* se diferentes retorna valor de mismatch */
}

/*retorna o caractere b_k[i]*/
char bki(int k, int i) {
    return s1[B[k].first+i];
}

/*retorna o caractere c_l[j]*/
char clj(int l, int j) {
    return s2[C[l].first+j];
}

/*verifica se klinha precede k*/
int precedek (int klinha, int k) {
    if (B[klinha].last < B[k].first)
        return 1;
    else
        return 0;
}

/*verifica se llinha precede l*/
int precedel (int llinha, int l) {
    if (C[llinha].last < C[l].first)
        return 1;
    else
        return 0;
}

/*devolve |k'|, usado na equação.*/
int lastLinha(int k) {
    return B[k].tam-1;
}

/*devolve |l'|, usado na equação.*/
int lastColuna(int l) {
    return C[l].tam-1;
}

/*verifica se mem щ null ou nуo...*/
void eval(void* mem) {
    if (mem == NULL) {
        printf("Memory error! Terminating program!\n");
        exit(EXIT_FAILURE);
    }
}

/*retorna o maior dos dois valores*/
int max (int a, int b) {
    return (a>b) ? a : b;
}
