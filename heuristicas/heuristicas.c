#include "heuristicas.h"
#include "basics.h"
#include <stdlib.h>
#include "pasg.h"
#include <limits.h>
#include <string.h>
#include <time.h>
#include "procrustes.h"

/*função usada para inicializar as variáveis que representam a solução*/
void initSolution() {
    int i;
    gammas = (int**) malloc (nSeqs*sizeof(int*));
    eval(gammas);
    tamGammas = (int*) calloc(nSeqs, sizeof(int));
    eval(tamGammas);

    for(i=0;i<nSeqs;i++) {
        gammas[i] = (int*) malloc(tamSegs[i] * sizeof(int));
        eval(gammas[i]);
    }
}

/*calcula a similaridade de s e t - versão otimizada*/
int similaridade (const char *s, const char *t, int m, int n)
{
    if (m<n) {
        //mais eficiente porque m<n
        return similaridade(t, s, n, m);
    }

    //matriz de alinhamento
    int *M;
	int i,j,old,temp,sim;

    //aloca a matriz de alinhamento
	M = (int*) malloc( sizeof(int) * (n+1));
	eval(M);

	//Inicialização da matriz
	for(j = 0; j <= n; j++)
		M[j] = j * SPACESCORE;

	for(i = 1; i <= m; i++)
	{
	    old=M[0];
        M[0]=i*SPACESCORE;

		for(j = 1; j <= n; j++)
		{
		    temp=M[j];
			M[j] = max(max(M[j]+SPACESCORE, old+scoreFunction(s[i-1],t[j-1])), M[j-1]+SPACESCORE);
			old=temp;
		}
	}

    //define o maior valor
    sim = M[n];

    //libera a memória
	free(M);

	return sim;
}


/*função que calcula o valor da solução encontrada por uma heurística*/
int calcularValorSolucao() {
    char *gammasConcat[nSeqs];
    int tamGammasConcat[nSeqs];
    int i, j, sum;

    //gera os gammas concatenados...
    for(i=0;i<nSeqs;i++) {
        //calcula o tamanho de cada Gamma concatenado...
        tamGammasConcat[i]=0;
        for(j=0;j<tamGammas[i];j++) {
            tamGammasConcat[i]+=segs[i][gammas[i][j]].tam;
        }

        //gera cada Gamma concatenado
        gammasConcat[i] = (char*)malloc((tamGammasConcat[i]+1) * sizeof(char));
        eval(gammasConcat[i]);
        gammasConcat[i][0] = '\0';
        for(j=0;j<tamGammas[i];j++) {
            strncat(gammasConcat[i], seqs[i]+segs[i][gammas[i][j]].first, segs[i][gammas[i][j]].tam);
        }
    }

    //imprime os gammas concatenados
    if (INFO) {
        printf("\nGammas concatenados:\n");
        for(i=0;i<nSeqs;i++)
            printf("\\Gamma_%d^* = %s\n", i, gammasConcat[i]);
        printf("\n");
    }

    //calcula o valor dessa solução
    sum = 0;
	for (i=1;i<nSeqs;i++) {
        for (j=0;j<i;j++){
			sum += similaridade(gammasConcat[i], gammasConcat[j], tamGammasConcat[i], tamGammasConcat[j]);
		}
    }

    //libera a memória alocada
    for(i=0;i<nSeqs;i++) {
        free(gammasConcat[i]);
    }

    //retorna o valor da solução
    return sum;
}

/*função que imprime a resposta de uma heurística*/
void imprimirResposta() {
    int i, j;

    if (INFO) {
        printf("\n\nSolucao da heuristica:\n");

        if (SIMILARIDADE)
            printf("Valor: %d\n\nCadeias:\n", calcularValorSolucao());
        else
            printf("Valor: %d\n\nCadeias:\n", -calcularValorSolucao());


        for(i=0;i<nSeqs;i++) {
            printf("\\Gamma_%d: ", i);
            for(j=0;j<tamGammas[i];j++) {
                printf("%d: [%d..%d], ", segs[i][gammas[i][j]].number, segs[i][gammas[i][j]].first, segs[i][gammas[i][j]].last);
            }
            printf("\n");
        }
        printf("\n");
    }

    if (COMPARE) {
        char heuristicaName[100];
        sprintf(heuristicaName,"heu%dSolution",heuristica);
        FILE* heuSolution = fopen(heuristicaName,"w");

        if(heuSolution==NULL){
            printf("Error opening file %s! Terminating program!\n", heuristicaName);
            exit(EXIT_FAILURE);
        }

        fprintf(heuSolution, "%d\n", (SIMILARIDADE) ? calcularValorSolucao() : -calcularValorSolucao());
        for(i=0;i<nSeqs;i++) {
            fprintf(heuSolution, "\\Gamma_%d = ", i+1);
            for(j=0;j<tamGammas[i];j++) {
                fprintf(heuSolution, "%d..%d, ", segs[i][gammas[i][j]].first, segs[i][gammas[i][j]].last);
            }
            fprintf(heuSolution, "\n");
        }

        fclose(heuSolution);
    }

    if (BENCHMARK) {
        //cria o nome do arquivo no diretório correto
        char dirName[1000];
        strncpy(dirName, arqEntrada[0], strlen(arqEntrada[0])-2); //pego o nome do diretório
        dirName[strlen(arqEntrada[0])-2]='\0';

        //temp armazena a heu%dsolution
        char temp[1000];
        sprintf(temp,"heu%dSolution",heuristica);

        //heuristicaName armazena o caminho completo do arquivo
        char heuristicaName[1000];
        strcpy(heuristicaName, dirName);
        strcat(heuristicaName, temp);

        FILE* heuSolution = fopen(heuristicaName,"w");

        if(heuSolution==NULL){
            printf("Error opening file %s! Terminating program!\n", heuristicaName);
            exit(EXIT_FAILURE);
        }

        for(i=0;i<nSeqs;i++) {
            fprintf(heuSolution, "\\Gamma_%d = ", i+1);
            for(j=0;j<tamGammas[i];j++) {
                fprintf(heuSolution, "%d..%d, ", segs[i][gammas[i][j]].first, segs[i][gammas[i][j]].last);
            }
            fprintf(heuSolution, "\n");
        }

        fclose(heuSolution);
    }
}


/*função que implementa a primeira heurística*/
void calcularCadeiasDeConsenso() {
    int i, j, k;

    //inicializa as variáveis da solução
    initSolution();

    //declara o vetor que contabilizará as ocorrências de cada segmento
    int **contabilidade;
    contabilidade = (int**) malloc(nSeqs * sizeof(int*));
    eval(contabilidade);

    for(i=0;i<nSeqs;i++) {
        contabilidade[i] = (int*) calloc(tamSegs[i], sizeof(int));
        eval(contabilidade[i]);
    }

    //faz PASG de todos com todos e contabiliza...
    for (i=1;i<nSeqs;i++) {
        for(j=0;j<i;j++) {
            if (DEBUG) printf("\n\n--------------------\ns%d e s%d\n", i, j);
            PASG(i, j);

            if (DEBUG){
                printf("Solucao:\n");
                printf("Gamma_%d = {", i);
                for (k=tamGammaB-1;k>=0;k--) {
                    printf("%d: [%d..%d], ", B[gammaB[k]].number, B[gammaB[k]].first, B[gammaB[k]].last);
                }
                printf("}\nGamma_%d = {", j);
                for (k=tamGammaC-1;k>=0;k--) {
                    printf("%d: [%d..%d], ", C[gammaC[k]].number, C[gammaC[k]].first, C[gammaC[k]].last);
                }
                printf("}\n");
            }

            for (k=0;k<tamGammaB;k++)
                contabilidade[i][gammaB[k]]++;

            for (k=0;k<tamGammaC;k++)
                contabilidade[j][gammaC[k]]++;

            cleanUpPASG();
        }
    }

    //constrói os Gammas - todos os segmentos que aparecem em mais da metade das soluções são colocados na resposta
    for(i=0;i<nSeqs;i++) {
        for(j=0;j<tamSegs[i];j++) {
            if (contabilidade[i][j] > (nSeqs-1)/2) {
                gammas[i][tamGammas[i]]=j;
                tamGammas[i]++;
            }
        }
    }


    //imprime a resposta
    imprimirResposta();
}

/*função que faz PASG de todos com todos*/
void PASGTodosComTodos(struct PASGReturn allPASGs[nSeqs][nSeqs]) {
    int i,j,k;

    //faz PASG de todos com todos e armazena cada solução...
    for (i=1;i<nSeqs;i++) {
        for(j=0;j<i;j++) {
            //*detalhe: gammaB sempre terá a cadeia do primeiro índice!
            PASG(i, j);
            allPASGs[i][j].value=allPASGs[j][i].value=PASGSim;
            allPASGs[i][j].tamGammaB=allPASGs[j][i].tamGammaC=tamGammaB;
            allPASGs[i][j].gammaB=allPASGs[j][i].gammaC=(int*) malloc(tamGammaB * sizeof(int));
            for(k=0;k<tamGammaB;k++) {
                allPASGs[i][j].gammaB[tamGammaB-1-k]=gammaB[k];
            }
            allPASGs[i][j].tamGammaC=allPASGs[j][i].tamGammaB=tamGammaC;
            allPASGs[i][j].gammaC=allPASGs[j][i].gammaB=(int*) malloc(tamGammaC * sizeof(int));
            for(k=0;k<tamGammaC;k++) {
                allPASGs[i][j].gammaC[tamGammaC-1-k]=gammaC[k];
            }
            cleanUpPASG();
        }
    }

    //imprime a matriz allPASGs
    if (DEBUG) {
        for (i=0;i<nSeqs;i++) {
            for(j=0;j<nSeqs;j++) {
                if (i!=j) {
                    printf("Printing allPASGs[%d][%d]:\nallPASGs[%d][%d].value: %d\nallPASGs[%d][%d].tamGammaB: %d\nallPASGs[%d][%d].tamGammaC: %d\n",
                           i,j,i,j,allPASGs[i][j].value,i,j,allPASGs[i][j].tamGammaB,i,j,allPASGs[i][j].tamGammaC);
                    printf("gammaB: ");
                    for(k=0;k<allPASGs[i][j].tamGammaB;k++)
                        printf("%d ", allPASGs[i][j].gammaB[k]);
                    printf("\ngammaC: ");
                    for(k=0;k<allPASGs[i][j].tamGammaC;k++)
                        printf("%d ", allPASGs[i][j].gammaC[k]);
                    printf("\nEND...\n========================================\n");
                }
            }
        }
    }
}

/*função que implementa a heurística gulosa*/
/*versão otimizada*/
void calcularHeuristicaGulosa() {
    int i,j,k,max;
    struct PASGReturn allPASGs[nSeqs][nSeqs];

    //inicializa as variáveis da solução
    initSolution();

    //faz PASG de todos com todos e armazena cada solução...
    PASGTodosComTodos(allPASGs);

    //para cada sequência, escolhe aquela cadeia cujo PASG é máximo com outra sequência
    int best[nSeqs];
    for(i=0;i<nSeqs;i++) {
        max = INT_MIN;
        for(j=0;j<nSeqs;j++) {
            if (i!=j) {
                if (allPASGs[i][j].value > max) {
                    max = allPASGs[i][j].value;
                    best[i]=j;
                }
            }
        }
    }

    //define a resposta (o vetor tamGammas e gammas)
    for (i=0;i<nSeqs;i++) {
        j = best[i];
        tamGammas[i]=allPASGs[i][j].tamGammaB;
        for(k=0;k<tamGammas[i];k++) {
            gammas[i][k]=allPASGs[i][j].gammaB[k];
        }
    }

    //libera a memória usada
    for (i=1;i<nSeqs;i++) {
        for(j=0;j<i;j++) {
            free(allPASGs[i][j].gammaB);
            free(allPASGs[i][j].gammaC);
        }
    }

    //imprime a resposta
    imprimirResposta();
}


/*função que implementa a terceira heurística, que usa o Gelfand*/
void calcularHeuristicaPASGGelfand() {
    int i,j,k,max;
    struct PASGReturn allPASGs[nSeqs][nSeqs];

    //inicializa as variáveis da solução
    initSolution();

    //faz PASG de todos com todos e armazena cada solução...
    PASGTodosComTodos(allPASGs);

    //define cada \Gamma_i
    for(i=0;i<nSeqs;i++) {
        //aqui define um \Gamma_i em particular
        max = INT_MIN;

        for(j=0;j<nSeqs;j++) {
            if (i!=j) {
                //verifica se \Gamma_i(j) é o melhor...

                //gera \Gamma_i(j) concatenado
                int tamGammaij=0;
                for(k=0;k<allPASGs[i][j].tamGammaB;k++) {
                    tamGammaij+=segs[i][allPASGs[i][j].gammaB[k]].tam;
                }

                //gera cada \Gamma_i(j) concatenado
                char *gammaij = (char*)malloc((tamGammaij+1) * sizeof(char));
                eval(gammaij);
                gammaij[0] = '\0';
                for(k=0;k<allPASGs[i][j].tamGammaB;k++) {
                    strncat(gammaij, seqs[i]+segs[i][allPASGs[i][j].gammaB[k]].first, segs[i][allPASGs[i][j].gammaB[k]].tam);
                }

                //gammaij = \Gamma_i(j)^*
                //faz o somatório utilizando GelfandSim...
                int sum=0;
                for (k=0;k<nSeqs;k++) {
                    if (k!=i) {
                        sum += gelfandSim(k, gammaij, tamGammaij);
                    }
                }

                //se gammaij for o melhor \Gamma_i até agora, define ele como \Gamma_i
                if (sum>max) {
                    max=sum;
                    tamGammas[i] = allPASGs[i][j].tamGammaB;
                    gammas[i] = allPASGs[i][j].gammaB;
                }
                free(gammaij);
            }
        }
    }

    //imprime a resposta
    imprimirResposta();
}

/*função que calcula GelfandSim()*/
int gelfandSim(int k, char *gammaij, int tamGammaij){
    FILE *gelseq2;
    int i;

    //pega o nome do primeiro arquivo de entrada pro gelfand - o k-ésimo arquivo
    char* gelseq1 = arqEntrada[k];

    //gera o nome do arquivo de blocos deste arquivo de entrada
    char gelblocks1[200];
    strcpy(gelblocks1, gelseq1);
    strcat(gelblocks1, ".predex");

    //escreve gammaij no arquivo gelseq2
    gelseq2 = fopen("gelseq2", "w");
    if(gelseq2==NULL){
        printf("Error opening file %s! Terminating program!\n", "gelseq2");
        exit(EXIT_FAILURE);
    }

    fprintf(gelseq2, ">artificial test\n");
    for(i=0;i<tamGammaij;i++) {
        fputc(gammaij[i], gelseq2);

        if (i!=0 && i%70==0)
            fputc('\n', gelseq2);
    }
    fclose(gelseq2);

    //chama o programa do Kishi
    int gelfandResult = gelfand(gelseq1, gelblocks1, "gelseq2");

    //deleta o arquivo gelseq2
    if (remove("gelseq2") != 0) {
        printf("Error deleting file %s! Terminating program!\n", "gelseq2");
        exit(EXIT_FAILURE);
    }

    return gelfandResult;
}


/*função que concatena \Gamma_i e grava em arquivo*/
void concatenaGammaIEmArq(int i) {
    int j,k;

    //gera o nome do arquivo
    char arq[100];
    sprintf(arq, "pasm%d", i);

    //abre o arquivo
    FILE *f = fopen(arq, "w");
    if (f==NULL) {
        printf("Error opening file %s! Terminating program!\n", arq);
        exit(EXIT_FAILURE);
    }

    //escreve no arquivo - concatena \Gamma_i e escreve-o no arquivo
    fprintf(f, ">artificial test\n");
    for(k=0;k<tamGammas[i];k++) {
        for (j=0;j<segs[i][gammas[i][k]].tam;j++) {
            if (j!=0 && j%70==0)
                fputc('\n', f);
            fputc(seqs[i][segs[i][gammas[i][k]].first+j],f);
        }
    }

    fclose(f);
}

