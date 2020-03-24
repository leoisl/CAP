/*
 * alglib.c
 *
 *  Created on: 04/03/2010
 *      Author: kishi
 */
#include "alglib.h"

int delta(char a, char b)
{
	if(a == b)
		return MATCH;
	else
		return MISMATCH;
}

int similarity(char *g, char *t, int gsize, int tsize)
{
	int *(M[2]);
	int i,j;

	/*//TODO É possível melhorar a alocação de memória escolhendo a menor das suas sequências p/ alocar a matriz*/
	M[0] = (int*) malloc( sizeof(int) * tsize);
	M[1] = (int*) malloc( sizeof(int) * tsize);

	/*Inicialização da matriz*/
	M[0][0] = 0;
	for(j = 1; j < tsize; j++)
		M[0][j] = M[0][j - 1] + SPACE;
	M[1][0] = M[0][0] + SPACE;

	for(i = 1; i < gsize; i++)
	{
		for(j = 1; j < tsize; j++)
		{
			int spaceint = M[0][j] + SPACE;
			int spaceing = M[1][j - 1] + SPACE;
			int val = M[0][j - 1] + delta(g[i], t[j]);

			if( val < spaceing )
				val = spaceing;
			if( val < spaceint )
				val = spaceint;
			M[1][j] = val;
		}
		for(j = 0; j < tsize; j++)
			M[0][j] = M[1][j];

		M[1][0] = M[0][0] + SPACE;

	}

	free(M[0]);
	free(M[1]);

	return M[1][tsize - 1];
}

int semi_global_alignment_score(char *g, char *t, int gsize, int tsize)
{
	int *(M[2]);
	int i,j;
	int best_score = INT_MIN;

	M[0] = (int*) malloc( sizeof(int) * tsize);
	M[1] = (int*) malloc( sizeof(int) * tsize);

	/*Inicialização da matriz*/
	M[0][0] = 0;
	for(j = 1; j < tsize; j++)
		M[0][j] = M[0][j - 1] + SPACE;
	M[1][0] = 0;

	for(i = 1; i < gsize; i++)
	{
		for(j = 1; j < tsize; j++)
		{
			int spaceint = M[0][j] + SPACE;
			int spaceing = M[1][j - 1] + SPACE;
			int val = M[0][j - 1] + delta(g[i], t[j]);

			if( val < spaceing )
				val = spaceing;
			if( val < spaceint )
				val = spaceint;
			M[1][j] = val;
		}
		for(j = 1; j < tsize; j++)
			M[0][j] = M[1][j];
		if(M[0][tsize - 1] > best_score)
			best_score = M[0][tsize - 1];
	}

	free(M[0]);
	free(M[1]);

	return best_score;
}



struct line** align(char *g, char *t, int gsize, int tsize)
{
	int **M;
	int i,j;
	struct line** A;
	/*int score;*/

	/*Aloca a matriz de alinhamento*/
	M = (int**) malloc( sizeof(int*) * gsize);
	for(i = 0; i < gsize; i++)
		M[i] = (int*) malloc( sizeof(int) * tsize);

	/*Cria as duas linhas do alinhamento*/
	A = (struct line**) malloc(sizeof(struct line*) * 2);
	A[0] = NULL;
	A[1] = NULL;

	/*Inicialização da matriz*/
	M[0][0] = 0;
	for(j = 1; j < tsize; j++)
		M[0][j] = M[0][j - 1] + SPACE;
	for(i = 1; i < gsize; i++)
		M[i][0] = M[i - 1][0] + SPACE;

	for(i = 1; i < gsize; i++)
	{
		for(j = 1; j < tsize; j++)
		{
			int spaceing = M[i][j - 1] + SPACE;
			int spaceint = M[i - 1][j] + SPACE;
			int val = M[i - 1][j - 1] + delta(g[i], t[j]);

			if( val < spaceing )
				val = spaceing;
			if( val < spaceint )
				val = spaceint;
			M[i][j] = val;
		}
	}

	/*Imprime o alinhamento*/
	{
		i = gsize - 1;
		j = tsize - 1;
		while(i > 0 && j > 0)
		{
			int spaceing = M[i][j - 1] + SPACE;
			int val = M[i - 1][j - 1] + delta(g[i], t[j]);

			if(M[i][j] == val)
			{
				insert_char(&(A[0]), g[i]);
				insert_char(&(A[1]), t[j]);
				i--;
				j--;
			}
			else
			{
				if(M[i][j] == spaceing)
				{
					insert_char(&(A[0]), '_');
					insert_char(&(A[1]), t[j]);
					j--;
				}
				else
				{
					insert_char(&(A[0]), g[i]);
					insert_char(&(A[1]), '_');
					i--;
				}
			}
		}
		while(i > 0)
		{
			insert_char(&(A[0]), g[i]);
			insert_char(&(A[1]), '_');
			i--;
		}
		while(j > 0)
		{
			insert_char(&(A[0]), '_');
			insert_char(&(A[1]), t[j]);
			j--;
		}
	}

	for(i = 0; i < gsize; i++)
		free(M[i]);
	free(M);

	return A;
}

void procrustes_otimizado(char *g, int gsize, int **b, int bsize, char *t, int tsize)
{
	int i, j, k;
	int **S;
	/*Comprimento de cada bloco*/
	int *blen;


	/*Alocação da matriz principal*/
	S = (int**) malloc(bsize * sizeof(int*));
	blen = (int *) malloc(bsize * sizeof(int));
	for(k = 0; k < bsize; k++)
	{
		S[k] = (int*) malloc(tsize * sizeof(int));
		S[k][0] = 0;
		blen[k] = b[1][k] - b[0][k] + 2;
	}

	/*Inicialização do bloco zero*/
	for(j = 1; j < tsize; j++)
		S[0][j] = S[0][j - 1] + SPACE;

	for(k = 1; k < bsize; k++)
	{
		for(i = 1; i < blen[k]; i++)
		{
			for(j = 0; j < tsize; j++)
			{

			}
		}
	}


	for(i = 0; i < bsize; i++)
		free(S[i]);
	free(S);
}

int procrustes(char *g, int gsize, int **b, int bsize, char *t, int tsize)
{

	/*Matriz*/
	int ***S;

	/*Matriz de pais*/
	char ***P;

	/*Contadores*/
	int i, j, k, z;

	/* Valor máximo de uma montagem */
	int max;

	/* Vetores contendo o pai de cada bloco, em cada posição de first */
	int **p;

	/* Índices da melhor montagem */
	int best_i = 0;
	int best_j = 0;
	int best_k = 0;
	int *print_order_blocks;
	int print_order_blocks_size;

	/* Armazena o alinhamento */
	char *aligned_g;
	char *aligned_t;
	unsigned aligned_g_size, aligned_t_size;

	/*Comprimento de cada bloco*/
	int *blen;

	S = (int***) malloc(bsize * sizeof(int**));
	P = (char***) malloc(bsize * sizeof(char**));
	blen = (int *) malloc(bsize * sizeof(int));

	/*Alocação e inicialização*/
	for(k = 0; k < bsize; k++)
	{
		S[k] = (int**) malloc(tsize * sizeof(int*));
		P[k] = (char**) malloc(tsize * sizeof(char*));
		blen[k] = b[1][k] - b[0][k] + 2;
		for(j = 0; j < tsize; j++)
		{
			S[k][j] = (int*) malloc(blen[k] * sizeof(int));
			P[k][j] = (char*) malloc(blen[k] * sizeof(char));

			if(j == 0)
			{
				S[k][j][0] = 0;
				for(i = 1; i < blen[k]; i++)
					S[k][j][i] = S[k][j][i - 1] + SPACE;
			}
			else
				S[k][j][0] = S[k][j - 1][0] + SPACE;

			for(i = 0; i < blen[k]; i++)
				P[k][j][i] = -1;
		}
	}


	/* Aloca e inicializa o vetor de pais dos blocos */
	p = (int **) malloc(bsize * sizeof(int*));
	for(i = 0; i < bsize; i++)
	{
		p[i] = (int *) malloc(tsize * sizeof(int));
		for(j = 0; j < tsize; j++)
			p[i][j] = -1;
	}


	/* COMPUTA A MATRIZ TRIDIMENSIONAL */
	for(k = 1; k < bsize; k++)
	{
		for(j = 1; j < tsize; j++)
		{
			for(i = 1; i < blen[k]; i++)
			{
				max = SHRT_MIN;
				/* if i != first(k) */
				/*if(i != b[0][k])*/
				/*if(i > 1)*/
				{
					/* Match */
					max = S[k][j - 1][i - 1] + score(g[(i - 1) + b[0][k]], t[j]);
					P[k][j][i] = DIAGONAL;

					/* Space em t */
					if(S[k][j][i - 1] + SPACE > max)
					{
						max = S[k][j][i - 1] + SPACE;
						P[k][j][i] = CIMA;
					}
				}
				/* if i == first(k) */
				/*else*/
				if(i == 1)
				{
					int l;
					z = 1;
					/* b[2][l] contém o indíce do l-ésimo menor last(k)
					 * que é b[1][k],
					 * logo b[1][b[2][l]] contém este l-ésimo valor*/
					l = b[1][ b[2][z] ];
					while(z < bsize && l < b[0][k])
					{
						/* Verifica o max do conjunto de blocos que terminam
						 *  estritamente antes do bloco k*/
						if(S[ b[2][z] ][j - 1][ blen[b[2][z]] - 1 ]  + score(g[(i - 1) + b[0][k]], t[j]) >  max)
						{
							p[k][j] = b[2][z];
							max = S[ b[2][z] ][j - 1][ blen[b[2][z]] - 1 ] + score(g[(i - 1) + b[0][k]], t[j]);
							P[k][j][i] = DIAGONAL_BLOCO_ANTERIOR;
						}

						if(S[ b[2][z] ][j][ blen[b[2][z]] - 1 ] + SPACE >=  max)
						{
							p[k][j] = b[2][z];
							max = S[ b[2][z] ][j][ blen[b[2][z]] - 1 ] + SPACE;
							P[k][j][i] = CIMA_BLOCO_ANTERIOR;
						}

						/* Inicializa próximo passo */
						z++;
						l = b[1][ b[2][z] ];
					}
				}

				/* Space em g */
				/* TODO Se deixar >= ele tende a colocar espaços fora dos blocos */
				if(S[k][j - 1][i] + SPACE > max)
				{
					max = S[k][j - 1][i] + SPACE;
					P[k][j][i] = ESQUERDA;
				}

				/* Se for a primeira posição, ele pode começar do zero */
/*				if(i == 1 && j == 1)
				{
					if(score(g[b[0][k]], t[j]) > max)
					{
						max = score(g[b[0][k]], t[j]);
						P[k][j][i] = DIAGONAL;
					}
				}*/


				/* Atribui a melhor escolha na posição i,j,k */
				S[k][j][i] = max;
			}
		}
	}


	/* Busca a melhor montagem de blocos */
	max = INT_MIN;
	for(k = 1; k < bsize; k++)
	{
		if(S[k][tsize - 1][ blen[k] - 1 ] > max)
		{
			best_i = blen[k] - 1;
			best_j = tsize - 1;
			best_k = k;

			max = S[best_k][best_j][best_i];
		}
	}


	/* Libera a memória */
	for(i = 0; i < bsize; i++)
	{
		for(j = 0; j < tsize; j++)
		{
			free(S[i][j]);
			free(P[i][j]);
		}
		free(S[i]);
		free(P[i]);
	}
	free(S);
	free(P);
	for(i = 0; i < bsize; i++)
		free(p[i]);
	free(p);
	free(blen);


	/*Se a solução encontrada é melhor que uma solução vazia*/
	if(max >= (SPACE * (tsize - 1)) )
		return max;
	else
        return SPACE * (tsize - 1);
}


int *procrustes_blocks(char *g, int gsize, int **b, int bsize, char *t, int tsize, int *retbsize, int *best_assembly_score)
{

	/*Matriz*/
	int ***S;

	/*Matriz de pais*/
	char ***P;

	/*Contadores*/
	int i, j, k, z;

	/* Valor máximo de uma montagem */
	int max;

	/* Vetores contendo o pai de cada bloco, em cada posição de first */
	int **p;

	/* Índices da melhor montagem */
	int best_i = 0;
	int best_j = 0;
	int best_k = 0;
	int *print_order_blocks;
	int print_order_blocks_size;

	/*Comprimento de cada bloco*/
	int *blen;

	/*Vetor de blocos que será retornada*/
	int *retb;

	S = (int***) malloc(bsize * sizeof(int**));
	P = (char***) malloc(bsize * sizeof(char**));
	blen = (int *) malloc(bsize * sizeof(int));

	/*Alocação e inicialização*/
	for(k = 0; k < bsize; k++)
	{
		S[k] = (int**) malloc(tsize * sizeof(int*));
		P[k] = (char**) malloc(tsize * sizeof(char*));
		blen[k] = b[1][k] - b[0][k] + 2;
		for(j = 0; j < tsize; j++)
		{
			S[k][j] = (int*) malloc(blen[k] * sizeof(int));
			P[k][j] = (char*) malloc(blen[k] * sizeof(char));

			if(j == 0)
			{
				S[k][j][0] = 0;
				for(i = 1; i < blen[k]; i++)
					S[k][j][i] = S[k][j][i - 1] + SPACE;
			}
			else
				S[k][j][0] = S[k][j - 1][0] + SPACE;

			for(i = 0; i < blen[k]; i++)
				P[k][j][i] = -1;
		}
	}


	/* Aloca e inicializa o vetor de pais dos blocos */
	p = (int **) malloc(bsize * sizeof(int*));
	for(i = 0; i < bsize; i++)
	{
		p[i] = (int *) malloc(tsize * sizeof(int));
		for(j = 0; j < tsize; j++)
			p[i][j] = -1;
	}


	/* COMPUTA A MATRIZ TRIDIMENSIONAL */
	for(k = 1; k < bsize; k++)
	{
		for(j = 1; j < tsize; j++)
		{
			for(i = 1; i < blen[k]; i++)
			{
				max = SHRT_MIN;
				/* if i != first(k) */
				/*if(i != b[0][k])*/
				/*if(i > 1)*/
				{
					/* Match */
					max = S[k][j - 1][i - 1] + score(g[(i - 1) + b[0][k]], t[j]);
					P[k][j][i] = DIAGONAL;

					/* Space em t */
					if(S[k][j][i - 1] + SPACE > max)
					{
						max = S[k][j][i - 1] + SPACE;
						P[k][j][i] = CIMA;
					}
				}
				/* if i == first(k) */
				if(i == 1)
				/*else*/
				{
					int l;
					z = 1;
					/* b[2][l] contém o indíce do l-ésimo menor last(k)
					 * que é b[1][k],
					 * logo b[1][b[2][l]] contém este l-ésimo valor*/
					l = b[1][ b[2][z] ];
					while(z < bsize && l < b[0][k])
					{
						/* Verifica o max do conjunto de blocos que terminam
						 *  estritamente antes do bloco k*/
						if(S[ b[2][z] ][j - 1][ blen[b[2][z]] - 1 ]  + score(g[(i - 1) + b[0][k]], t[j]) >  max)
						{
							p[k][j] = b[2][z];
							max = S[ b[2][z] ][j - 1][ blen[b[2][z]] - 1 ] + score(g[(i - 1) + b[0][k]], t[j]);
							P[k][j][i] = DIAGONAL_BLOCO_ANTERIOR;
						}

						if(S[ b[2][z] ][j][ blen[b[2][z]] - 1 ] + SPACE >=  max)
						{
							p[k][j] = b[2][z];
							max = S[ b[2][z] ][j][ blen[b[2][z]] - 1 ] + SPACE;
							P[k][j][i] = CIMA_BLOCO_ANTERIOR;
						}

						/* Inicializa próximo passo */
						z++;
						l = b[1][ b[2][z] ];
					}

				}

				/* Space em g */
				/* TODO Se deixar >= ele tende a colocar espaços fora dos blocos */
				if(S[k][j - 1][i] + SPACE > max)
				{
					max = S[k][j - 1][i] + SPACE;
					P[k][j][i] = ESQUERDA;
				}

				/* Se for a primeira posição, ele pode começar do zero */
/*				if(i == 1 && j == 1)
				{
					if(score(g[b[0][k]], t[j]) > max)
					{
						max = score(g[b[0][k]], t[j]);
						P[k][j][i] = DIAGONAL;
					}
				}*/

				/* Atribui a melhor escolha na posição i,j,k */
				S[k][j][i] = max;
			}
		}
	}

	/* Busca a melhor montagem de blocos */
	max = INT_MIN;
	for(k = 1; k < bsize; k++)
	{
		if(S[k][tsize - 1][ blen[k] - 1 ] > max)
		{
			best_i = blen[k] - 1;
			best_j = tsize - 1;
			best_k = k;

			max = S[best_k][best_j][best_i];
		}
	}
	/*Se a solução encontrada é melhor que uma solução vazia*/
	if(max >= (SPACE * (tsize - 1)) )
		(*best_assembly_score) = max;
	else
		(*best_assembly_score) = (SPACE * (tsize - 1));

	/*Imprime G*/
	i = best_i;
	j = best_j;
	k = best_k;

	print_order_blocks = (int *)malloc(sizeof(int) * bsize);
	print_order_blocks_size = 0;

	if(max >= (SPACE * (tsize - 1)) )
		print_order_blocks[ print_order_blocks_size++ ] = k;
	else
	{
		i = 0; j = 0;
	}
	while(j > 0 || i > 0)
	{
		if(j == 0 && i > 0)
		{
			i--;
		}
		else
		{
			/*Preenche o começo*/
			if(i == 0 && j > 0)
			{
				j--;
			}
			else
			{
				if(P[k][j][i] == ESQUERDA)
				{
					j--;
				}
				else
				{
					if(P[k][j][i] == DIAGONAL)
					{
						i--;
						j--;
					}
					else
					{
						if(P[k][j][i] == CIMA)
						{
							i--;
						}
						else
						{
							if(P[k][j][i] == DIAGONAL_BLOCO_ANTERIOR)
							{
								k = p[k][j];
								print_order_blocks[ print_order_blocks_size++ ] = k;
								i = blen[k] - 1;
								j--;
							}
							else /*CIMA_BLOCO_ANTERIOR*/
							{
								k = p[k][j];
								print_order_blocks[ print_order_blocks_size++ ] = k;
								i = blen[k] - 1;
							}
						}
					}
				}
			}
		}
	}

	retb = (int *) malloc(sizeof(int) * print_order_blocks_size);
	i = 0;
	while(print_order_blocks_size > 0)
	{
		print_order_blocks_size--;
		retb[i++] = print_order_blocks[print_order_blocks_size];
	}
	free(print_order_blocks);
	(*retbsize) = i;


	/* Libera a memória */
	for(i = 0; i < bsize; i++)
	{
		for(j = 0; j < tsize; j++)
		{
			free(S[i][j]);
			free(P[i][j]);
		}
		free(S[i]);
		free(P[i]);
	}
	free(S);
	free(P);
	for(i = 0; i < bsize; i++)
		free(p[i]);
	free(p);
	free(blen);

	return retb;
}

struct line** star(char **t, int tsize, int *tsizes)
{
	/* Vetor que armazena os valores dos somatórios das similaridades entre cada sequência e todas as outras */
	int *sumofscores;

	/*Índice da sequência central e valor da sequência central*/
	int central_index, central_val;

	int i,j,k; /*índices*/

	/*Alinhamento múltiplo*/
	struct line** A;

	/*Aloca o vetor de sequências alinhadas*/
	A = (struct line**) malloc( sizeof(struct line*) * tsize);
	for(i = 0; i < tsize; i++)
		A[i] = NULL;

	/*Aloca o vetor com a soma das similaridades entre cada sequência e todas as outras*/
	sumofscores = (int *) malloc(sizeof(int) * tsize);

	/*Calcula somatório das das similaridades entre cada sequência e todas as outras*/
	for(i = 0; i < tsize; i++)
	{
		sumofscores[i] = 0;
		for(j = 0; j < tsize; j++)
		{
			if(j != i)
				sumofscores[i] += sumofscores[i] + similarity(t[i], t[j], tsizes[i], tsizes[j]);
		}
	}

	/*Encontra a sequência central (centro da estrela)*/
	central_index = 0;
	central_val = sumofscores[0];
	for(i = 1; i < tsize; i++)
	{
		if(sumofscores[i] > central_val)
		{
			central_val = sumofscores[i];
			central_index = i;
		}
	}

	/*Monta o alinhamento múltiplo, inserindo as arestas da estrela, uma a uma*/
	insert_sequence(A, central_index, t[central_index], tsizes[central_index]);
	for(i = 0; i < tsize; i++)
	{
		/*printf("Inseriu a %d\n",i);
		print_alignment(A,tsize);
		printf("\n");*/
		if(i != central_index)
		{
			struct line** a = align(t[central_index], t[i], tsizes[central_index], tsizes[i]);
			struct line* aux1 = A[central_index];
			struct line* aux2 = a[0];

			j = 0;

			while(aux1 != NULL || aux2 != NULL)
			{
				if( aux1->element != aux2->element)
				{
					if(aux1->element == '_')
					{
						insert_char_in_pos(&(a[0]), '_', j);
						insert_char_in_pos(&(a[1]), '_', j);
						aux1 = aux1->next;
						j++;
					}
					else
					{
						if(aux2->element == '_')
							for(k = 0; k < tsize; k++)
							{
								if(A[k] != NULL)
								{
									insert_char_in_pos(&(A[k]), '_', j);
								}
							}
						aux2 = aux2->next;
						j++;
					}
				}
				else
				{
					aux1 = aux1->next;
					aux2 = aux2->next;
					j++;
				}

				if(aux1 == NULL)
				{
					while(aux2 != NULL)
					{
							for(k = 0; k < tsize; k++)
							{
								if(A[k] != NULL)
								{
									insert_char_in_pos(&(A[k]), '_', j);
								}
							}
							aux2 = aux2->next;
							j++;
					}
				}
				if(aux2 == NULL)
				{
					while(aux1 != NULL)
					{
						insert_char_in_pos(&(a[0]), '_', j);
						insert_char_in_pos(&(a[1]), '_', j);
						aux1 = aux1->next;
						j++;
					}
				}
			}
			A[i] = a[1];
			a[1] = NULL;
			free_alignment(&a, 2);
		}
	}

	free(sumofscores);
	return A;
}

void insert_char(struct line** l, char c)
{
	struct line* newelement;

	newelement = (struct line*) malloc(sizeof(struct line));
	newelement->next = (*l);
	newelement->element = c;

	(*l) = newelement;
}

void insert_char_in_pos(struct line** l, char c, int pos)
{
	struct line* aux1 = *l;
	struct line* aux2;
	int i = 0;
	struct line* new;

	while(i < pos)
	{
		aux2 = aux1;
		aux1 = aux1->next;
		i++;
	}

	new = (struct line*) malloc(sizeof(struct line));

	new->element = c;
	if(i > 0)
	{
		new->next = aux1;
		aux2->next = new;
	}
	else
	{
		new->next = (*l);
		(*l) = new;
	}
}

void insert_sequence(struct line** A, int pos, char *s, int ssize)
{
	int i;
	for(i = ssize - 1; i > 0; i--)
		insert_char( &(A[pos]), s[i]);
}

void free_alignment(struct line*** A, int size)
{
	int i;
	struct line* aux;

	for(i = 0; i < size; i++)
	{
		while( (*A)[i] != NULL)
		{
			aux = (*A)[i];
			(*A)[i] = ((*A)[i])->next;
			free(aux);
		}
	}

	free(*A);
}

void print_alignment(struct line** A, int size)
{
	int i;
	struct line* aux;
	for(i = 0; i < size; i++)
	{
		aux = A[i];
		while(aux != NULL)
		{
			printf("%c", aux->element);
			aux = aux->next;
		}
		printf("\n");
	}
}

void print_alignment_to_file(struct line** A, int size, FILE *out)
{
	int i;
	struct line* aux;
	for(i = 0; i < size; i++)
	{
		aux = A[i];
		while(aux != NULL)
		{
			fprintf(out, "%c", aux->element);
			aux = aux->next;
		}
		fprintf(out, "\n");
	}
}

void free_all_blocks(struct block ***A, int size)
{
	int i;

	for(i = 0; i < size; i++)
		free_blocks( &((*A)[i]) );

	free(*A);
}

void free_blocks(struct block **A)
{
	struct block* aux;
	while( (*A) != NULL)
	{
		aux = (*A);
		(*A) = (*A)->next;
		free(aux);
	}
}

void insert_block(struct block **l, int v)
{
	struct block* aux1 = *l;
	struct block* aux2 = NULL;
	struct block* new;

	while(aux1 != NULL && aux1->block < v)
	{
		aux2 = aux1;
		aux1 = aux1->next;
	}

	new = (struct block *) malloc(sizeof(struct block));

	new->block = v;
	new->next = NULL;
	if(aux2 != NULL)
	{
		new->next = aux1;
		aux2->next = new;
	}
	else
	{
		new->next = (*l);
		(*l) = new;
	}
}

void print_blocks(struct block **A, int size)
{
	int i;
	struct block *aux;
	for(i = 0; i < size; i++)
	{
		aux = A[i];
		printf("b%d:", i);
		while(aux != NULL)
		{
			printf("%d ", aux->block);
			aux = aux->next;
		}
		printf("\n");
	}
}

void print_blocks_to_file(struct block **A, int size, FILE *out)
{
	int i;
	struct block *aux;
	for(i = 0; i < size; i++)
	{
		aux = A[i];
		fprintf(out,"b%d:", i);
		while(aux != NULL)
		{
			fprintf(out,"%d ", aux->block);
			aux = aux->next;
		}
		fprintf(out,"\n");
	}
}

void sort_blocks(struct block **b, int *score, int n, int *tam, int *qtd)
{
	int i, j, key;
	struct block *aux1;
	int aux2, aux3;

	for(i = 1; i < n; i++)
	{
		key = score[i];
		aux1 = b[i];
		aux2 = tam[i];
		aux3 = qtd[i];

		j = i - 1;
		while( j >= 0 && score[j] < key )
		{
			score[j + 1] = score[j];
			b[j + 1] = b[j];
			tam[j + 1] = tam[j];
			qtd[j + 1] = qtd[j];

			j--;
		}
		score[j + 1] = key;
		b[j + 1] = aux1;
		tam[j + 1] = aux2;
		qtd[j + 1] = aux3;
	}
}


void print_blocks_with_info(struct block **A, int size, int *score, int *tam, int *qtd)
{
	int i;
	struct block *aux;
	for(i = 0; i < size; i++)
	{
		aux = A[i];
		printf("Individuo %d: ***",i);
		while(aux != NULL)
		{
			printf("%d ", aux->block);
			aux = aux->next;
		}
		printf("*** Qtd: %d Comp: %d Score: %d\n", qtd[i], tam[i] - 1, score[i]);
	}
	printf("\n");
}

void print_blocks_with_info_to_file(struct block **A, int size, int *score, int *tam, int *qtd, int **b, FILE *out)
{
	int i;
	struct block *aux;
	for(i = 0; i < size; i++)
	{
		aux = A[i];
		fprintf(out, "SOLUTION%d ***",i);
		while(aux != NULL)
		{
			fprintf(out,"[%d][%d] ", b[0][aux->block], b[1][aux->block]);
			aux = aux->next;
		}
		fprintf(out,"*** QTD %d LEN %d SCORE %d\n", qtd[i], tam[i] - 1, score[i]);
	}
	fprintf(out,"\n");
}

char overlap(struct block *b1, struct block **b, int b2)
{
	struct block *aux1 = b1;
	struct block *aux2;

	while(aux1 != NULL)
	{
		aux2 = b[aux1->block];
		while(aux2 != NULL)
		{
			if(aux2->block == b2)
				return YES;
			aux2 = aux2->next;
		}
		aux1 = aux1->next;
	}
	return NO;
}

int delete_block(struct block **l, int index)
{
	struct block *aux1 = (*l);
	struct block *aux2 = NULL;
	int block;
	int i;

	for(i = 0; i < index; i++)
	{
		aux2 = aux1;
		aux1 = aux1->next;
	}

	if(aux2 == NULL)
		(*l) = aux1->next;
	else
		aux2->next = aux1->next;

	block = aux1->block;
	free(aux1);
	return block;
}

