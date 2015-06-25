/* Copyright (C) 2011-2015 Simon Hickinbotham, 				*/
/*			   Jamie Wood, 					*/
/*			   Richard Law					*/
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

#ifndef VOGL_INTERFACE_H_
#define VOGL_INTERFACE_H_

//void	initialize_vogl();
//void	initialize_vogl(double width,double height,double margin,int winw,int winh, char *name);
void	initialize_vogl(double width,double height,double margin, char *name);

//void 	display_vogl();


void drawfish(double **fish, const int N, rmodel **M, double arena,  int clear);

#endif /* VOGL_INTERFACE_H_ */
