#voglc21

pronounced "*vogulseetoowon*", is a "very ordinary GL-like" library for graphics, based on vogl, a free software library that was available in the 1990s, which I've updated so it compiles on Ubuntu. If you've not used it before, then **don't**: you'll be much better of starting from scratch using openGL. However, if you've got old code which uses the vogl libraries, then this project might help you port it to a modern Linux system like Ubuntu.

##Prerequisites

You'll need to have the git software installed on your system. (If you're not sure, you may as well just try to install it - the system will then tell you it it is already installed.) Enter the following commands in a terminal window:

~~~bash
sudo apt-get update
sudo apt-get install git
~~~


##Installation

Open a terminal window in Ubuntu and type the following:

~~~bash
git clone https://github.com/franticspider/voglc21
cd volgc
make -f Makefile.gcc
sudo make -f Makefile.gcc install
~~~
To test the installation, we have included an example project that uses vogl. To try it, do:

~~~bash 
cd examples/robofish
make
./rfish

This runs a simulated fish shoal, as described in the paper 

**Recovering Hidden Swarm Parameters Using a Simulated Robofish.**
Richard Coates and Simon Hickinbotham.
*ECAL 2011, Paris, France, August 2011. LNCS. Springer, 2011, Pages 153-160*


~~~

..and you should see a working visualisation. 



---

##versioning

I'm going to follow the version numbering that vogl was using, so the first increment will be 1.2.9. When I'm happy that this is all working, we'll issue a stable release as 1.3.0. Someday!

---
#Notes on the first build
##Sources

The places to get hold of vogl on the internet are dwindling. I got the source from here this time: 

http://www.filewatcher.com/m/vogl-1.2.8-1.src.rpm.411559-0.html

##Notes on first compile

I'm running Ubuntu 14.04. Originally I planned to build a .deb package from the .rpm package, but there were issues with the conversion and I ended up needing to modify the source anyway. Here's how I did it, for future reference:

1. **Create a working directory**

    1. Get hold of the vogl rpm package from the website above
    2. Install alien: 
    "``sudo apt-get install alien``"
    3. generate a directory from the package: 
    "``alien -g vogl-1.2.9-1.src.rpm``"
    4. Unzip the tarball in vogl-1.2.8.orig: 
    "``tar -xzvf vogl.tar.gz``"

2. **Make a working library**

    *NB:* There's a couple of things that need doing that I haven't done yet. Firstly, we aren't concerning ourselves with the hershey library, so we ignore any compile errors in there. Secondly, we aren't bothered for interfacing with Fortran, so we've commented out the build instructions in the makefile. 

    1. Enter the vogl subdirectory
    2. In file ``drivers/tek.c``, change line 9 from ``#include <sys\temio.h>`` to ``#include <termio.h>`` 
    3. In file ``src/drivers.c``, change line 9 from ``static FILE *fp = stdout;`` to ``static FILE *fp = NULL;`` 
    4. Comment out references to fortran in the makefile (see Makefile.gcc in the vogl subdirectory of this project)
    3. Run make as root: ``sudo make -f Makefile.gcc`` (NB: you need the sudo because it does something with the hershey fonts
    
3. **Configure ``make install`` **
    The original make install didn't copy the header files to a system-level path. So I fixed that too.
    
---
#TODO List:

1. make the hershey libraries work
2. make the fortran interface work
3. make the changed Makefile.gcc work via the makefile










