/* Copyright (C) 2011-2015 Simon Hickinbotham, 				*/
/*			   Jamie Wood, 					*/
/*			   Verena Fischer 				*/
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


#include "microbial_ga.h"
#include "randutil.h"

/* GA parameters */
//const int POPSIZE = 100;
//const int RUNS = 100;
//const int PARAMETERS = 25; //25 rules in the metabolism

const double REC = 0.5;
const double MUT = 0.1;
double MDIST = 0.5; //0.05;
/* GA parameters END*/


/*	 COMMENTED THESE OUT - NOW PASSED INTO THE PROGRAM
double pop[POPSIZE][PARAMETERS];
double eval[POPSIZE];
*/

/*
 * "Mutates" a double value, i.e. adds or subtracts MDIST to/from the value.
 */
double mutate (double f) {
	if (randint()%2 == 0){
		f = (f + MDIST);
	}
	else{
		f = (f - MDIST);
	}
	return f;
}

/**
 * Microbial GA step.
 */
//void step() {
int ga_step(double **pop, double *eval, const int POPSIZE, const int PARAMETERS){
	int w;
	int l;
	int i;
	int a=randint()%POPSIZE;
	int b=randint()%POPSIZE;
	while (a==b) {
		b=randint()%POPSIZE;
	}
	if (eval[a]<eval[b]) {//note the lower the value the fitter the individual!
		w=a;
		l=b;
	}
	else {
		w=b;
		l=a;
	}
	for (i=0; i<PARAMETERS; i++) {
		//recombine
		if (((randint()%1000)/1000.0)<REC) {
			pop[l][i]=pop[w][i];
		}
		//mutate
		if ((randint()%1000)/1000.0<MUT) {
			pop[l][i]=mutate(pop[l][i]);
		}
	}
	//eval[l] = evaluate(l);
	return l;
}

/**
 * (returns euclidian distance to goal point)
 */
double evaluate(int i) {
	//do things with Player Stage
	return 0.;
}


