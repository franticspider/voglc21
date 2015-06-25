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

#ifndef ROBOFISH_H_
#define ROBOFISH_H_

typedef struct td_model{
	double ZOR;
	double ZOO;
	double ZOA;
	double V;
	double Sigma;
	//
	int color; //used by Vogl to draw the fish.
}rmodel;

/*Fitness measures*/
enum e_fit{coates,fit2};
typedef enum e_fit fit_type;

enum{p_x,p_y,h_x,h_y,view,newh_x,newh_y,fcount};

enum e_boolean {false,true};
typedef enum e_boolean boolean;

void update_fish(const int N, double **distances, double **fish, double arena, rmodel **M, trigLookup * TL);
void initialize_fish(double **fish, double ** distances, const int N, const double arena);

float get_fitness(double **distances,const int N);

//Richard Coates' fitness function:
float coates_fitness(double **distances,const int N,const int t);

//Internal headers.
double distance(double x1, double y1, double x2, double y2, double arena);


void test_landscape(rmodel **modelarray, const int TimeLimit, const int N, double **distances, double **fish,
		const int arena,	trigLookup *TL, const fit_type fitmodel);

#endif /* ROBOFISH_H_ */
