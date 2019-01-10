/*****************************************************************************/
#include <unistd.h>
#include "vogl.h"
#include "vodevice.h"

/******************************************************************************
* This procedure initializes a window for vogl 
graphics                       *
******************************************************************************/
void    initialize_vogl()
{
     /***Initialize a window***/
         vinit("X11");
         winopen("simulation");  /*Window must have a name before 
display appears*/

}

/******************************************************************************
* Main body of 
program                                                        *
******************************************************************************/
int     main()
{
         initialize_vogl();
         //system("sleep 10");
	sleep(10);
}
