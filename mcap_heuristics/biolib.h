/*
 * biolib.h
 *
 *  Created on: 07/08/2008
 *      Author: Kishi
 */
#ifndef BIOLIB_H_
#define BIOLIB_H_

/* Valores para os casamentos de bases */
#define MATCH 1
#define MISMATCH -1
#define SPACE -2


/*Distancia de Levenshtein*/
/*
#define MATCH 0
#define MISMATCH -1
#define SPACE -1
*/

/* Função que verifica se ocorre um
 * MATCH ou MISMATCH e retorna o devido valor*/
int score(char a, char b);

/* Valores para mapear pai da célula na matriz */
#define ESQUERDA 0
#define DIAGONAL 1
#define CIMA 2
#define DIAGONAL_BLOCO_ANTERIOR 3
#define CIMA_BLOCO_ANTERIOR 4


#endif /* BIOLIB_H_ */
