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


#include <math.h>
#include "vogl.h"
#include "vodevice.h"

#include "trigLookup.h" //TODO: remove the need for this include - robofish.h depends on it and it shouldn't
#include "robofish.h"

#define		color_code 1	/*0: white to black	1: black to red		*/
#define		txt_margin 0.3	/*Gives a margin around edge for text		*/
#define		sep_margin 0.1	/*Gives a margin between grid displays		*/
#define		window     150.0/*Size of window				*/



/****************************************************************************************************************
* This procedure carries out colour mixing.  Colouring depends on color_code:					*
*	0:	white to black	(for postscript)								*
*	1:	black to red	(for vogl)									*
* Procedure supplied by Ulf.											*
****************************************************************************************************************/

void fraction2rgb(fraction, rP, gP, bP)
double	fraction, *rP, *gP, *bP;
{
	int	huesector;
	double	hue, huetune, mix_up, mix_do, r, g, b;

	if(color_code==1)
	{
		hue=1.0-fraction;
		if(hue<0.0)	hue=0.0;
		if(hue>1.0)	hue=1.0;
		huesector=(int)floor(hue*5.0);
		huetune=hue*5.0-huesector;
		mix_up=huetune;
		mix_do=1.0-huetune;
		mix_up=pow(mix_up,1.0/2.5);
		mix_do=pow(mix_do,1.0/2.5);
		switch(huesector)
		{
			case 0 : r=1.0   ;g=mix_up;b=0.0   ;break; /* red    to yellow */
			case 1 : r=mix_do;g=1.0   ;b=0.0   ;break; /* yellow to green  */
			case 2 : r=0.0   ;g=1.0   ;b=mix_up;break; /* green  to cyan   */
			case 3 : r=0.0   ;g=mix_do;b=1.0   ;break; /* cyan   to blue   */
			case 4 : r=0.0   ;g=0.0   ;b=mix_do;break; /* blue   to black  */
			default: r=0.0   ;g=0.0   ;b=0.0   ;break;
		}
	}

	else
	{
		r=1.0-fraction;
		g=1.0-fraction;
		b=1.0-fraction;
	}

	*rP=r;*gP=g;*bP=b;
}

/****************************************************************************************************************
* This procedure generates a color scale from black to red for coding numbers.					*
* Procedure supplied by Ulf.											*
****************************************************************************************************************/

void colors_init()
{
	int	index;
	double	r, g, b;

    /***Colours 0 to 7 are predefined***/

    /***colors 8 to 15 are less bright versions of predefined colours***/
	mapcolor( 8,  0,  0,  0);
	mapcolor( 9,100,  0,  0);
	mapcolor(10,  0,100,  0);
	mapcolor(11, 50, 50,  0);
	mapcolor(12,  0,  0,100);
	mapcolor(13, 50,  0, 50);
	mapcolor(14,  0, 50, 50);
	mapcolor(15,153,153,153);

    /***colors 16 to 79 are mixed as below (64 colors)***/
	for(index=16; index<=79; index++)
	{	fraction2rgb((double)(index-16)/(double)(80-16),&r,&g,&b);
		mapcolor(index,(int)(r*255.0),(int)(g*255.0),(int)(b*255.0));
	}
}


/****************************************************************************************************************
* This procedure initializes a window for vogl graphics								*
*
* This is naive - places a rect in the middle, with a margin, and a 1-1 coordinate mapping.
****************************************************************************************************************/

void	initialize_vogl(double width,double height,double margin, char *name)
{
	//double	width, height;
	float	winw = width+(2.*margin),
			winh = height+(2.*margin);

    /***Initialize a window***/
	prefposition(0, (int)(winw), 0, (int)(winh));
	prefsize((int)(winw), (int)(winh));
	//width  = txt_margin + 4.0 * (1.0 + sep_margin);
	//height = txt_margin + 4.0 * (1.0 + sep_margin) + txt_margin;
	//prefposition(0, (int)(width*window), 0, (int)(height*window));
	vinit("X11");

	winopen(name);
	//winopen("simulation");	/*Window must have a name before display appears*/

    /***Set background colour***/
	color(BLACK);
	clear();

    /***Initialize colour scheme***/
	colors_init();

    /***Set up a coordinate frame***/
	//ortho2(0., winw, 0., winh);
	ortho2(-margin, width+margin, -margin, height + margin);

	//SET UP DRAWING TO THE BACK BUFFER (COULD PUT THIS IN INITIALIZE)
	backbuffer(1);

}


void circtor(double x, double y, double rad, double arena, int clear){

	double a1,a2,tmp;
	int drawn=0;
	int cs,cf;

	int tl,t,tr,r,c,l,bl,b,br;

	tl=t=tr=r=c=l=bl=b=br = 0;

	if(y+rad>arena){
		t=1;
		if(x+rad>arena)
			tr=1;
		if(x<rad)
			tl=1;
	}

	if(y<rad){
		b=1;
		if(x+rad>arena)
			br=1;
		if(x<rad)
			bl=1;
	}

	if(x+rad>arena)
		r=1;
	if(x<rad)
		l=1;


	////////////////
	//let's go round, anticlockwise from r.

	if(!clear)color(GREEN);

	if(r){
		if(br)
			a1 = 3600 -(asin((y)/rad)*  1800./PI);
		else
			a1 = 3600- (acos((arena-x)/rad)* 1800./PI);

		if(tr)
			a2 = (asin((arena-y)/rad)*  1800./PI);
		else
			a2 = (acos((arena-x)/rad)* 1800./PI);

		arc(x-arena,y,rad,(int) a1,(int)a2+3600);
		drawn=1;
	}


	if(tr){
		a1 = (asin((arena-y)/rad)*  1800./PI);
		a2 = (acos((arena-x)/rad)* 1800./PI);
		arc(x-arena,y-arena,rad,(int) a1,(int)a2);
		drawn=1;
	}

	if(t){
		if(tr)
			a1 = acos((arena-x)/rad) * 1800./PI;
		else
			a1 = asin((arena-y)/rad)*  1800./PI;

		if(tl)
			a2 = 1800. - (acos((x)/rad) * 1800./PI);
		else
			a2 = 1800. - (asin((arena-y)/rad)*  1800./PI);

		arc(x,y-arena,rad,(int)a1,(int)a2);
		drawn=1;
	}

	if(tl){
		a1 = 1800. - (acos((x)/rad) * 1800./PI);
		a2 = 1800. - (asin((arena-y)/rad)*  1800./PI);

		arc(x+arena,y-arena,rad,(int)a1,(int)a2);
		drawn=1;
	}


	if(l){
		if(tl)
			a1 = 1800. - (asin((arena-y)/rad)*  1800./PI);
		else
			a1 = 1800. - (acos((x)/rad) * 1800./PI);

		if(bl)
			a2 = 1800. + (asin((y)/rad)*  1800./PI);
		else
			a2 = 1800. + (acos((x)/rad)* 1800./PI);

		arc(x+arena,y,rad,(int) a1,(int)a2);
		drawn=1;
	}


	if(bl){
		a1 = 1800. + (asin((y)/rad)*  1800./PI);
		a2 = 1800. + (acos((x)/rad) * 1800./PI);

		arc(x+arena,y+arena,rad,(int)a1,(int)a2);
		drawn=1;

	}


	if(!clear)color(RED);
	if(b){
		if(bl)
			a1 = 1800. + (acos((x)/rad) * 1800./PI);
		else
			a1 = 1800. + (asin((y)/rad)*  1800./PI);

		if(br)
			a2 = acos((arena-x)/rad) * 1800./PI;
		else
			a2 = asin((arena-y)/rad)*  1800./PI;


		arc(x,y+arena,rad,(int)a1,(int)a2);
		drawn=1;
	}


	if(tr){
		a1 = (asin((y)/rad)*  1800./PI);
		a2 = (acos((arena-x)/rad)* 1800./PI);
		arc(x-arena,y-arena,rad,(int) a1,(int)a2);
		drawn=1;
	}




	if(!clear)color(GREEN);
	if(!drawn)
		circ(x,y,rad);


}



void drawfish(double **fish, const int N, rmodel **M, double arena, int clear){

	int i;


	if(clear)
		color(BLACK);
	for(i=(N-1);i>=0;i--){//reverse order so robofish is drawn last
		if(!i){//for the radii:
			if(!clear)color(GREEN);
			circ(fish[i][p_x],fish[i][p_y],M[i]->ZOA);
			/*arc(fish[i][p_x],fish[i][p_y],M[i]->ZOA,0,2700);*/
			/*circtor(fish[i][p_x],fish[i][p_y],M[i]->ZOA,arena,clear);*/
			if(!clear)color(YELLOW);
			circ(fish[i][p_x],fish[i][p_y],M[i]->ZOO);
			if(!clear)color(RED);
			circ(fish[i][p_x],fish[i][p_y],M[i]->ZOR);
		}
		if(!clear)
			color(M[i]->color);
		circf(fish[i][p_x],fish[i][p_y],1.);
		move2(fish[i][p_x],fish[i][p_y]);
		draw2(fish[i][p_x]-(4.*fish[i][h_x]),fish[i][p_y]-(4.*fish[i][h_y]));
	}
}
