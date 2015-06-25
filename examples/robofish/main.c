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



#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "vogl.h"
#include "vodevice.h"

#include "trigLookup.h"
#include "randutil.h"
#include "robofish.h"
#include "vogl_interface.h"

#include "microbial_ga.h"


//todo: define this properly somewhere using extern
#define PI (3.1415926535897932384626433832795028841971693993751)









void	main()
{


	/*EXPERIMENTAL VOGL PARAMETERS*/

	int i=0,rr,gg,g1,g2,t;
	const float pi = 3.14159;
	float xpos,ypos,xside,yside;
	const int nTrials = 5;

	double	x1, y1, x2, y2, xshift, yshift;

	const int margin=30;

	/* PARAMS FOR "RUN" FROM encounter.java */
	const int N = 100; 			/* swarm size*/

	const int arena = 240;		/* The arena size */
	const int TimeLimit = 10000;/* How long to run the trial */
	const int Nrobots = 20;			/* size of the robot population */
	const int Ngens = 50;			/* Number of generations */

	rmodel modelfish;
	rmodel eval_robofish;
	rmodel **modelarray;
	double **robofish_gene;
	robofish_gene = (double **)malloc(Nrobots * sizeof(double *));
	for(i=0;i<Nrobots;i++)
		robofish_gene[i] = (double *) malloc(3*sizeof(double));

	double q;

	trigLookup *TL;

	double **fish;
	double **distances;
	double fitness[Nrobots];

	const fit_type fitmodel=coates;

	fish = (double **) malloc(N*sizeof(double *));
	distances = (double **) malloc(N*sizeof(double *));

	for(i=0;i<N;i++){
		fish[i]=(double *) malloc(fcount*sizeof(double));
		distances[i]=(double *) malloc(N*sizeof(double));
	}

	FILE *fout;
	fout = fopen("robofish_ga.txt","w");


	/*NOW WE START THE PROGRAM PROPER*/

	//Set up rand
	initmyrand(436);

	//Set up the trigLookup
	TL = init_trigLookup(8);

	//todo: sort out the summing of zone widths
	//Set up the model/robot values;
	modelfish.ZOR 	= 1;
	modelfish.ZOO 	= 13;
	modelfish.ZOA 	= 26;
	modelfish.V   	= 2.25;
	modelfish.Sigma = 	0.1;
	modelfish.color = 4;

	eval_robofish.V   	= 2.25;
	eval_robofish.Sigma = 0.1;
	eval_robofish.color = 3;


	modelarray = (rmodel **) malloc(N*sizeof(rmodel *));
	modelarray[0] = &eval_robofish;
	for(i=1;i<N;i++)
		modelarray[i] = &modelfish;

	initialize_vogl(arena,arena,margin,"Robofish");
	color(BLACK);
	clear();



	test_landscape(modelarray,TimeLimit,N,distances,fish,arena,TL,fitmodel);


	for(gg=0;gg<Ngens*Nrobots;gg++){//microbial ga counts no. of tournaments, not generations...
		if(!gg){
			for(rr=0;rr<Nrobots;rr++){
				robofish_gene[rr][0] = 50.*rand0to1();
				robofish_gene[rr][1] = 50.*rand0to1();
				robofish_gene[rr][2] = 50.*rand0to1();
				g1=0;
				g2=Nrobots;
			}
		}
		else{
			g1 = ga_step(robofish_gene,fitness,Nrobots,3);
			g2 = g1+1;
		}//TODO: we are re-evaluating things that havent been recombined - in fact we only need to reevaluate the individual that has been changed!
		for(rr=g1;rr<g2;rr++){


			eval_robofish.ZOR 	= robofish_gene[rr][0];//0;
			eval_robofish.ZOO 	= robofish_gene[rr][0] + robofish_gene[rr][1];//13;
			eval_robofish.ZOA 	= robofish_gene[rr][0] + robofish_gene[rr][1] + robofish_gene[rr][2];//26;



			printf("evaluating %d, ZOR = %f, ZOO = %f, ZOA = %f\n", rr, eval_robofish.ZOR, eval_robofish.ZOO, eval_robofish.ZOA );

			fitness[rr]=0;

			for(t=0;t<nTrials;t++){
				initialize_fish(fish,distances,N,arena);
				for(i=0;i<TimeLimit;i++){

					/*
					 * UPDATE THE MODEL
					 */


					update_fish(N,distances,fish,arena,modelarray,TL);

					switch(fitmodel){

					case fit2://fit2:
						fitness[rr] += get_fitness(distances,N);
						break;

					case coates://coates
						fitness[rr] += coates_fitness(distances,N,i);
						break;
					}

					/*
					 * DRAW
					 */



					if(!(i%25)){
						//ARENA
						color(7);
						rect(0.,0.,(float) arena, (float) arena);

						//FISH
						drawfish(fish,N,modelarray,(double) arena,0);

						//Move backbuffer to become the front buffer
						swapbuffers();


						// (backbuffer only overwrites -- it doesn't start from blank screen)
						//color(BLACK);
						//clear();
						drawfish(fish,N,modelarray,(double) arena, 1);


						/*Let's have some console messages too:
						 */
					}


					if(!(i%500)){
						//printf("i=%d, xpos = %f, ypos = %f\n",i,fish[0][p_x],fish[0][p_y]);
						printf(".");fflush(stdout);
					}

				}
			}
			printf("\nGen %02d: FITNESS for robot %d is %f\n---------------------------------------------\n",gg,rr,fitness[rr]);

			fprintf(fout,"%d\t%d\t%f\t%f\t%f\t%f\n",gg,rr,fitness[rr],robofish_gene[rr][0],robofish_gene[rr][1],robofish_gene[rr][2]);
			fflush(fout);

		}
	}
	fclose(fout);
}
