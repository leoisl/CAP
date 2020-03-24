/*
 * biolib.c
 *
 *  Created on: 07/08/2008
 *      Author: Kishi
 */

#include "biolib.h"

int score(char a, char b)
{
	if(a == b)
		return MATCH;
	else
		return MISMATCH;
}
