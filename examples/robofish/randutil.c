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
#include <math.h>
#include <time.h>

#include "randutil.h"

#define USING_MT //This means we are using the Mersenne twister


#ifdef USING_MT
#include "mt19937-2.h"
#endif


long initmyrand(long seed){

	if(seed<0)
		seed = time(NULL);
	//seed = 356;//436;		/* choose a seed value */
	//seed = 1225793297; - bug at 000 itn 1990000 - get rule returned error -1 - wasn't trapped

#ifdef USING_MT
	sgenrand(seed);
#else
	srand(seed);
#endif

	return seed;
}


double rand0to1(){
	double x;
#ifdef USING_MT
	x = genrand();
#else
	x = (double) rand() / (double) RAND_MAX;
#endif
	return x;
}

unsigned long randint(){

	unsigned long x;

#ifdef USING_MT
	x = genrandint();
#else
	x = rand();
#endif
	return x;
}




int rand_in_rad(const float rad, float *x, float *y){

	int found=0;
	float tx,ty;
	do{
		  tx =  2 * rad * (rand0to1()-0.5);
		  ty =  2 * rad * (rand0to1()-0.5);

		  if( rad > sqrt( pow(tx,2) + pow(ty,2) ) ){
			  found = 1;
		  }

	}while(!found);

	*x = tx;
	*y = ty;
	return 0;
}
