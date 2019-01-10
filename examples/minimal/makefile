#/* Copyright (C) 2011-2015 Simon Hickinbotham, 			*/
#/*			   Jamie Wood, 					*/
#/*			   Richard Coates 				*/
#/* When you use this, send an email to: sjh518@york.ac.uk              */
#/* with an appropriate reference to your work.                         */

#/* This file is part of ROBOFISH					*/

#/* ROBOFISH is free software: you can redistribute it and/or modify    */
#/* it under the terms of the GNU General Public License as published by */
#/* the Free Software Foundation, either version 3 of the License, or    */
#/* (at your option) any later version.                                  */

#/* This program is distributed in the hope that it will be useful,      */
#/* but WITHOUT ANY WARRANTY; without even the implied warranty of       */
#/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        */
#/* GNU General Public License for more details.                         */

#/* You should have received a copy of the GNU General Public License    */
#/* along with this program.  If not, see <http://www.gnu.org/licenses/>.*/


# alias for compiler
CC = gcc

## alias for compiling commands
## use this one for debugging
#CCOMP = $(CC) -c -g
## use this one for speed
CCOMP = $(CC) -c -O3

# alias for linking commands
CLINK = -g -o

INCLUDE = -I/usr/local/include/

#TODO: reintegrate hershey when it's working
#LIBS =  -lvogl -lhershey -lX11 -lm 
LIBS =  -lvogl -lX11 -lm

# name of object file(s)
OBJS =   minimal.o

# the first argument is an object you wish to create, i.e. integrate.
# Then follow the dependencies.  To make integrate you need integrate.o .
# Next follows information on how to create integrate with integrate.o.

minimal: $(OBJS)
	  $(CC) $(CLINK) minimal $(OBJS) $(LIBS)


# the creation of main.o is dependent upon the existence of 
# main.c .  $(CCOMP) integrate.c creates integrate.o from integrate.c 

minimal.o: minimal.c
	    $(CCOMP) $(INCLUDE) minimal.c


#just get rid of intermediate files:
clean: 
	rm -f *.o

purge:
	rm -f *.o
	rm -f rfish
	# also remove these unwanted files:
	rm -f *~
	rm -f *.txt

