#include <stdio.h>
#include <stdlib.h>
#include "inout.h"
#include "pasg.h"
#include "heuristicas.h"

int main(int argc, char *argv[])
{
    int i;

    if (COMPARE) {
        //faz com que os erros sejam impressos em um arquivo!
        freopen("errors", "a", stdout);
    }

    //verifica os argumentos
    if (argc<=4) {
        printf("Error on arguments. For two or less input sequences, use PASG.\n\nSyntax: ./pasgm [# of the heuristic] [sequence 1] [sequence 2] ... [sequence n]\n\nHeuristic #1: Heuristica da cadeia de segmentos consenso.\nHeuristic #2: Heuristica gulosa.\nHeuristic #3: Heuristica da cadeia de segmentos central.\n");
        exit(EXIT_FAILURE);
    }

    //arquivos de entrada. Necessário para o gelfand ficar mais eficiente.
    arqEntrada=argv+2;

    //verifica o argumento heurística
    if (sscanf(argv[1], "%d", &heuristica)==0 || heuristica<1 || heuristica>3) {
        printf("Error on [# of the heuristic] argument.\n");
        exit(EXIT_FAILURE);
    }

    //inicializa as variáveis globais...
    nSeqs=argc-2;
    initVariables();

    /*lê as sequências e seus segmentos*/
    for(i=2;i<argc;i++)
        readSeq(argv[i], i-2);

    switch (heuristica){
        case 1:
            calcularCadeiasDeConsenso();
            break;

        case 2:
            calcularHeuristicaGulosa();
            break;

        case 3:
            calcularHeuristicaPASGGelfand();
            break;
    }

    return 0;
}
