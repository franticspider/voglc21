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
#include <math.h>
#include <string.h>

//todo: define this properly somewhere using extern
#define PI (3.1415926535897932384626433832795028841971693993751)

#include "randutil.h"
#include "trigLookup.h"
#include "robofish.h"



void moveon(double **fish,int number,double deltaT, double velocity, double arena){


	//the birds position is its current one + its heading * timestep(?) * it's velocity
	fish[number][p_x] += fish[number][newh_x]*deltaT*velocity;
	fish[number][p_y] += fish[number][newh_y]*deltaT*velocity;

	//the birds heading is updated
	fish[number][h_x] = fish[number][newh_x];
	fish[number][h_y] = fish[number][newh_y];

	//if for some reason the bird is outside the box it is moved inside
	if(fish[number][p_x] < 0) {
		fish[number][p_x] += arena;
	}
	if(fish[number][p_x] > arena) {
		fish[number][p_x] -= arena;
	}
	if(fish[number][p_y] < 0) {
		fish[number][p_y] += arena;
	}
	if(fish[number][p_y] > arena) {
		fish[number][p_y] -= arena;
	}

}


int cansee(double **fish, int i, int j, double space, trigLookup *TL){

//public boolean cansee(int i,int j) {
	//States whether the distance between two birds is less than the viewing distance of the first bird

	//Recalulate distance? I don't think so!
	double X = fish[j][p_x] - fish[i][p_x];
	double Y = fish[j][p_y] - fish[i][p_y];
	double D;

	if(X + space/2 < 0) {X += space;}
	if(X - space/2 > 0) {X -= space;}
	if(Y + space/2 < 0) {Y += space;}
	if(Y - space/2 > 0) {Y -= space;}

	D = sqrt(X*X + Y*Y);

	//double q = Math.acos((X*bird[i].heading[0] + Y*bird[i].heading[1])/D);
	double q = jacos( (X*fish[i][h_x] + Y*fish[i][h_y])/D, TL);

	if(q < fish[i][view]/2){ //(double) bird[i].view/2) {
	    return 1;
	}
	else {
	    return 0;
	}
 }

void normalise(double * X) {
	 double D = sqrt(X[p_x]*X[p_x] + X[p_y]*X[p_y]);
	 X[p_x] /= D;
	 X[p_y] /= D;
}

void normalise_coords(double *x, double *y){
	double X[2];
	X[p_x] = *x;
	X[p_y] = *y;
	normalise(X);//todo: check this works
	*x = X[p_x];
	*y = X[p_y];
}


void vector(double *vec, double **fish, int i,int j, double arena) {
	//returns a vector that leads from one bird to the other
	//double [] X = new double [2];

	vec[p_x] = fish[j][p_x] - fish[i][p_x];
	vec[p_y] = fish[j][p_y] - fish[i][p_y];

	if(vec[p_x] + arena/2 < 0) {vec[p_x] += arena;}
	if(vec[p_x] - arena/2 > 0) {vec[p_x] -= arena;}
	if(vec[p_y] + arena/2 < 0) {vec[p_y] += arena;}
	if(vec[p_y] - arena/2 > 0) {vec[p_y] -= arena;}

	double D = sqrt(vec[p_x]*vec[p_x] + vec[p_y]*vec[p_y]);
	vec[p_x] /= D;
	vec[p_y] /= D;

	//return X;
}

double distance(double x1, double y1, double x2, double y2, double arena){
	//works out the distance between two birds
	double X = x1 - x2;
	double Y = y1 - y2;
	//makes sure the results are valid (???)
	if(X + arena/2 < 0) {X += arena;}
	if(X - arena/2 > 0) {X -= arena;}
	if(Y + arena/2 < 0) {Y += arena;}
	if(Y - arena/2 > 0) {Y -= arena;}

	return sqrt(X*X + Y*Y);
}

void update_fish(const int N, double **distances, double **fish, double arena, rmodel **M, trigLookup * TL){

	int i,j;
	//double distances[N][N];
	boolean ROR;
	boolean ROO;
	boolean ROA;

	double repulse[2];
	double orient[2];
	double attract[2];
	double temp[2];
	double birdHeading[2];

	const double deltaT = 0.1; //todo: decide if this should be part of rmodel
	const double turn = PI/2.; //from oneBoid.java

	//Loop round all the boids to determine new positions and headings.


	for(i = 0;i< N;i++) {
		distances[i][i]=0;
	    //Calculate distances between all the flock members
	    for(j=i+1;j<N;j++) {
			distances[j][i]=distances[i][j] = distance(fish[i][p_x],fish[i][p_y],fish[j][p_x],fish[j][p_y],arena);
	    }
	}

	//Assign to original boid if visible (weeds out those that shouldn't have been calculated?)
	for(i = 0;i< N;i++) {
		for(j = 0;j<i;j++) {
			if(M[i]->ZOA < distances[i][j]) {
			    distances[i][j] = 0.0;
			}
			if(!cansee(fish,i,j,arena,TL)) {
			    distances[i][j] = 0.0;
			}
		}
	}

	//We now have a matrix which contains all the distances of visible boids for each boid.
	//All that remains is to score them up.
	// Different possible movements

	//Scan round and look where everything is
	//for all prey (?)boids
	// Strategy: figure out what the newheading should be for each,
	// After this, use the moveon function to update the positions
	for(i = 0;i< N;i++) {
	    //if(prey[i]) {
		//if a boid is prey (and alive) set everything to 0 or false
		ROR = false;
		ROO = false;
		ROA = false;
		repulse[0] = repulse[1] = 0.0;
		orient[0]  = orient[1]  = 0.0;
		attract[0] = attract[1] = 0.0;

		//for all prey boids
		for(j = 0;j<N;j++) {
			//if j is prey and not equal to i  and the distance between them isn't 0
		    //if(prey[j]) {
			if(j!=i) {
			    if(distances[i][j] > 0.0) {//TODO: Fix this risky if statement

					if(distances[i][j] < M[i]->ZOR) {
						// Repulsion -- if in here abandon others
						//the repulse values come from the vector between the boids
						//temp = B.vector(i,j);
						vector(temp,fish,i,j,arena);//TODO: Check that double[2] can go in as *double for temp
						repulse[0] -= temp[0];
						repulse[1] -= temp[1];
						ROR = true;

						//System.out.println("REPULSION of " + i + " and " + j + " (" + repulse[0] + "," + repulse[1] + ")");
					}
					else{
						if((distances[i][j] < M[i]->ZOO) && (!ROR)) {
							// Orientation
							//orient taken from j's heading
							orient[p_x] += fish[j][h_x];
							orient[p_y] += fish[j][h_y];
							ROO = true;

						}
						else{
							if((distances[i][j] < M[i]->ZOA) && (!ROR)) {
								// Attraction
								//as repulsion
								vector(temp,fish,i,j,arena);
								attract[0] += temp[0];
								attract[1] += temp[1];
								ROA = true;
							}
						}
					}
			    }
			}
		    //}
		}

		//reset the headings
		birdHeading[0] = 0.0;
		birdHeading[1] = 0.0;

		if(ROR) {
			//if in ROR then normalise repulse and set the birds heading
		    normalise(repulse);
		    birdHeading[0] = repulse[0];
		    birdHeading[1] = repulse[1];
		}
		else {
			//if there are boids in ROO normalise orient
		    if(ROO) {
				normalise(orient);
				if(ROA) {
					//if there are also boids in ROA then normalise attract and the bird heading is a combination of the two
					normalise(attract);
					birdHeading[0] = orient[0]/2.0 + attract[0]/2.0;
					birdHeading[1] = orient[1]/2.0 + attract[1]/2.0;

				}
				else {
					//otherwise it is just the value of orient
					birdHeading[0] = orient[0];
					birdHeading[1] = orient[1];

				}
		    }

		   //if there are no boid in the ROO
		    else {
				if(ROA) {
					//if there are some in the ROA then the heading is just that
					normalise(attract);
					birdHeading[0] = attract[0];
					birdHeading[1] = attract[1];

				}
				else {
				  //otherwise it is the same as the current heading (?)
					birdHeading[0] = fish[i][h_x];
					birdHeading[1] = fish[i][h_y];
				}
		    }
		}

		fish[i][newh_x] = birdHeading[p_x];
		fish[i][newh_y] = birdHeading[p_y];

		// DONE: Add some noise - this currently persuades all to move to the Right
		double q = rand0to1()*2.*PI;

		fish[i][newh_x] += M[i]->Sigma*cos(q);
		fish[i][newh_y] += M[i]->Sigma*sin(q);
		//B.bird[i].newheading = B.normalise(B.bird[i].newheading);
		//todo: check this works!
		normalise_coords(&(fish[i][newh_x]),&(fish[i][newh_y]));

		// Check we don't break the turn rule
		double theta =  jacos((fish[i][newh_x]*fish[i][h_x] + fish[i][newh_y]*fish[i][h_y]),TL);

		double phi = 0.5*turn*deltaT;

		if(phi < theta) {
			fish[i][newh_x] = fish[i][h_x] *sin(theta-phi) + fish[i][newh_x] *sin(phi);
			fish[i][newh_y] = fish[i][h_y] *sin(theta-phi) + fish[i][newh_y] *sin(phi);

			normalise_coords(&(fish[i][newh_x]),&(fish[i][newh_y]));

		    if(fish[i][newh_x] > 1) {
				printf("Yikes -- %f %f %f\n",theta,phi, fish[i][newh_x]);
		    }
		}
	}

	// Now we've done them all move them on.

	for(i = 0;i<N;i++) {
	    //if(prey[i]) {
		//moveon(fish,i);
		moveon(fish,i,deltaT,M[i]->V,arena);
	    //}
	}
}




void initialize_fish(double **fish, double ** distances, const int N, const double arena){
	int i;
	double q;

	for(i=0;i<N;i++){

		fish[i][p_x]=arena*rand0to1();
		fish[i][p_y]=arena*rand0to1();
		q = rand0to1()*2.*PI;
		fish[i][h_x]=cos(q);
		fish[i][h_y]=sin(q);
		fish[i][view]=0;
		fish[i][newh_x]=cos(q);
		fish[i][newh_y]=sin(q);

		memset(distances[i],0,N*sizeof(double));
	}
}


float get_fitness(double **distances,const int N){

	int i=0,cr=0,cm=0,j;
	double sr=0,sm=0;
	for(i=1;i<N;i++){
		sr += distances[0][i];
		cr ++;
	}

	for(i=1;i<N;i++){
		for(j=i+1;j<N;j++){
			sm += distances[i][j];
			cm ++;
		}
	}

	return fabs((sr/cr)-(sm/cm));
}


float coates_fitness(double **distances,const int N,const int t){

	int i=0,cr=0;
	double sr=0;

	if(t>5000 &&!(t%50)){
		for(i=1;i<N;i++){
			sr += distances[0][i];
			cr ++;
		}
		return fabs((sr/cr));
	}
	return 0;
}



void test_landscape(rmodel **modelarray, const int TimeLimit, const int N, double **distances, double **fish,
		const int arena,	trigLookup *TL, const fit_type fitmodel){

	int param,val,i,t;
	const int nTrials=5;
	double fitness[nTrials];
	FILE *fout;

	fout = fopen("landscape.txt","w");


	for(param=0;param<3;param++){//Todo: make this less fragile!
		modelarray[0]->ZOR = modelarray[1]->ZOR;
		modelarray[0]->ZOO = modelarray[1]->ZOO;
			modelarray[0]->ZOA = modelarray[1]->ZOA;
		for(val=0;val<50;val++){
			switch(param){
			case 0:
				modelarray[0]->ZOR = val;
				modelarray[0]->ZOO = val +  modelarray[1]->ZOO - modelarray[1]->ZOR;
				modelarray[0]->ZOA = val +  modelarray[1]->ZOA - modelarray[1]->ZOR;
				break;
			case 1:
				modelarray[0]->ZOR = modelarray[1]->ZOR;
				modelarray[0]->ZOO = val +  modelarray[1]->ZOR;
				modelarray[0]->ZOA = val +  modelarray[1]->ZOA - modelarray[1]->ZOO + modelarray[1]->ZOR;
				break;
			case 2:
				modelarray[0]->ZOR = modelarray[1]->ZOR;
				modelarray[0]->ZOO = modelarray[1]->ZOO;
				modelarray[0]->ZOA = modelarray[1]->ZOO + val;
				break;
			}
			for(t=0;t<nTrials;t++){

				fitness[t] = 0;
				initialize_fish(fish,distances,N,arena);
				for(i=0;i<TimeLimit;i++){

					/* UPDATE THE MODEL	 */
					update_fish(N,distances,fish,arena,modelarray,TL);

					switch(fitmodel){

					case fit2://fit2:
						fitness[t] += get_fitness(distances,N);
						break;

					case coates://coates
						fitness[t] += coates_fitness(distances,N,i);
						break;
					}

					/* DRAW */
					if(!(i%25)){
						//ARENA
						color(7);
						rect(0.,0.,(float) arena, (float) arena);

						//FISH
						drawfish(fish,N,modelarray,(double) arena,0);

						//Move backbuffer to become the front buffer
						swapbuffers();

						drawfish(fish,N,modelarray,(double) arena, 1);
					}

					/* CONSOLE */
					if(!(i%500)){
						//printf("i=%d, xpos = %f, ypos = %f\n",i,fish[0][p_x],fish[0][p_y]);
						printf(".");fflush(stdout);
					}

				}
			}
			printf("\nZOR %02d, ZOO %02d, ZOA %02d, fitness:",(int)modelarray[0]->ZOR, (int) modelarray[0]->ZOO, (int) modelarray[0]->ZOA,fitness);

			fprintf(fout,"ZOR %02d, ZOO %02d, ZOA %02d, fitness:",(int)modelarray[0]->ZOR, (int) modelarray[0]->ZOO, (int) modelarray[0]->ZOA,fitness);

			for(t=0;t<nTrials;t++){
				printf("\t%f",fitness[t]);
				fprintf(fout,"\t%f",fitness[t]);
			}
			printf("\n");
			fprintf(fout,"\n");

		}
	}

	fclose(fout);
}

int count_in_rad(double **distances, rmodel **modelarray, const int N){




	return 0;
}


