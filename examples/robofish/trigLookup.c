/* Copyright (C) 2011-2015 Simon Hickinbotham, 				*/
/*			   Jamie Wood, 					*/
/*			   Richard Coates 				*/
/* When you use this, send an email to: sjh518@york.ac.uk               */
/* with an appropriate reference to your work.                          */

/* This file is part of ROBOFISH					*/

/* ROBOFISH is free software: you can redistribute it and/or modify    */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or    */
/* (at your option) any later version.                                  */

/* This program is distributed in the hope that it will be useful,      */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of       */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        */
/* GNU General Public License for more details.                         */

/* You should have received a copy of the GNU General Public License    */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "trigLookup.h"

#define PI (3.1415926535897932384626433832795028841971693993751)


int divideLemma(trigLookup *TL, double number, int counting,int choice) {

	if(number < TL->dividers[choice+counting])
		return 1;
	else
		return 0;
}

trigLookup  * init_trigLookup(int power) {
	int i;
	trigLookup *TL;

	//todo: ensure that this memory is freed somewhere
	TL = (trigLookup *) malloc(sizeof(trigLookup));

	TL->fractions = pow(2,power);
	TL->dividers = (double *) malloc(TL->fractions * sizeof(double));
	TL->answers = (double *) malloc(TL->fractions * sizeof(double));



	for(i = 0;i < TL->fractions;i++) {
		TL->answers[i] = ((double) (1.0+2.0*i)*PI)/(4*TL->fractions);
		TL->dividers[i] = cos((double) ((i)*PI)/(2*TL->fractions));
	    //System.out.println(i + " " + answers[i] + " " + dividers[i]);
	    if(TL->answers[i]>PI) {
			printf("Fuck you %d\n",i);
	    }
	    if(TL->answers[i]<0) {
			printf("Fuck you %d\n",i);
	    }
	}
	return TL;
}


double jacos(double x, trigLookup *TL) {
	int sector = 0;
	int decrement = TL->fractions;
	int choice = 0;

	if(x < 0) {
	    sector +=1;
	    x *= -1;
	}


	while(decrement > 1) {
	    decrement /=2;
	    if(divideLemma(TL,x,decrement,choice)) {
		choice += decrement;
	    }
	}

	//System.out.println(choice);

	if(sector == 0) {
	    return TL->answers[choice];
	}
	else {
	    return PI - TL->answers[choice];
	}
}
