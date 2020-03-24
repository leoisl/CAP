#include "inout.h"
#include <string.h>

/*Função que conta o número de caracteres presentes no arquivo de entrada, assumindo que o arquivo de entrada é um arquivo FASTA*/
int contaCharFasta (char *file)
{
	int size;
	char line[FASTA_MAXLINE];
	int read;
	FILE *f;

	f = fopen(file, "r");
	if(!f)
	{
		printf("Error! Cannot Open File %s.\n", file);
		exit(EXIT_FAILURE);
	}


	size = 0;
	do {
        read = fscanf(f, "%[^\n]", line);
        fscanf(f, "%*c");

        if (read > 0) {
            if(line[0] != '>')
            {
                size += strlen(line);
            }
        }
	}while(!feof(f));

	fclose(f);
	return size;
}

/*Função que recebe um arquivo e seu tamanho, e grava a sequência deste arquivo em seq. Nota: para simplificar, seq conterá um espaço na primeira posição (seq[0]=' '). Seq não termina com \0!*/
void leArquivoFasta(char *arq, int tam, char **seq)
{
	FILE *f;
	char line[FASTA_MAXLINE];
	int read;
	int  i, pos;

	(*seq) = (char *) malloc (sizeof(char) * (tam));
	eval(*seq);

	f = fopen(arq, "r");
	if(!f)
	{
		printf("Error! Cannot Open File %s.\n", arq);
		exit(EXIT_FAILURE);
	}

	(*seq)[0] = ' ';
	pos = 1;


	do {
	    read = fscanf(f, "%[^\n]", line);
	    fscanf(f, "%*c");

        if (read > 0) {
            if(line[0] != '>')
            {
                for(i = 0; line[i] !='\0' && i < FASTA_MAXLINE ; i++)
                {
                    (*seq)[pos++] = line[i];
                }
            }
        }
	}while (!feof(f));

	if(pos!=tam)
	{
		printf("Fatal error: %d chars could not be read in %s.\nTerminating program.\n", tam-pos, arq);
		exit(EXIT_FAILURE);
	}

	fclose(f);
}

/*função que compara dois segmentos para ordenação no qsort()*/
int compare ( const void * elem1, const void * elem2 ) {
    struct segmento *e1 = (struct segmento*) elem1;
    struct segmento *e2 = (struct segmento*) elem2;

    if (e1->first < e2->first)
        return -1;

    if (e1->first > e2->first)
        return 1;

    /*aqui, e1->first == e2->first*/
    if (e1->last < e2->last)
        return -1;

    if (e1->last > e2->last)
        return 1;

    return 0;
}

/*Função que lê os segmentos de um arquivo arq e grava em segmentos. Retorna a qtde de segmentos lidos*/
int leSegmentos (char *arq, struct segmento **segmentos)
{
	int amount;
	FILE *in;
	int i, fst, lst;
	int read;

	in = fopen(arq, "r");
	if (!in)
	{
		printf("Error! Cannot Open File %s.\n", arq);
		exit(EXIT_FAILURE);
	}

	amount = 1;

	do {
	    read = fscanf(in, "%d..%d,", &fst, &lst);

	    if (read>0)
            amount++;
	}while (!feof(in));

    (*segmentos) = (struct segmento*) malloc(sizeof(struct segmento) * amount);
    eval(*segmentos);


	(*segmentos)[0].first = 0;
	(*segmentos)[0].last = 0;
	(*segmentos)[0].tam = 1;

	rewind(in);
	for(i = 1; i < amount; i++)
	{
		fscanf(in, "%d..%d,", &fst, &lst);
		(*segmentos)[i].first = fst;
		(*segmentos)[i].last = lst;
		(*segmentos)[i].tam = lst-fst+1;
	}

	qsort(*segmentos, amount, sizeof(struct segmento), compare);

	for(i = 0; i < amount; i++)
	{
	    (*segmentos)[i].number = i;
	}

	fclose(in);
	return amount;
}

/*Função que lê uma sequência*/
void readSeq (char *arq, int pos) {
    char temp[1024];

    if (INFO) printf("Reading sequence %s.\n", arq);
    tamSeqs[pos] = contaCharFasta(arq)+1; /*+1 por causa do ' '*/
    leArquivoFasta(arq, tamSeqs[pos], &seqs[pos]);
    if (INFO) printf("Sequence read successfully!\n");

    strcpy(temp, arq);
    strcat(temp, ".predex");
    if (INFO) printf("Reading segments %s.\n", temp);
    tamSegs[pos] = leSegmentos(temp, &segs[pos]);
    if (INFO) printf("Segments read successfully!\n");
}
