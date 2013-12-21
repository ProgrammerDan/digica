#
# Ezwindows makefile
# Author: Devon Lockwood
# Modified: Dr Keith Shomper for CS1220
# TO THE STUDENT:  You can make this makefile work for you, by replacing all
# occurances of "yoursourcefile" with your program name.
#
#
# The following directives let make know that you may be using files
# with a cpp extension.
#

.SUFFIXES:
.SUFFIXES: .cpp $(SUFFIXES)

#
# You may have to set the directories so that are appropiate for 
# your account.
#

CC       = g++
EZWINDIR = /home/shomperk/CS1220/public/EzWindows
X11DIR   = /usr/X11R6

#
# The CPPFLAGS macro lets the compiler know where to find include files 
#

CPPFLAGS=-I$(X11DIR)/include -I$(EZWINDIR)/include

#
# The LDFLAGS macro lets the linker know where to find library files
#

LDFLAGS=-L$(X11DIR)/lib -lX11 -L$(EZWINDIR)/lib -lezwin -lXpm

#
# The OBJS macro tells the compiler which object files need to be present 
# to link your program into an executable. 
#

OBJS=logicException.o logic.o gate.o circuit.o gui.o digica.o

#
# The following sequence of commands defines a target for the program 
# executable.
#

digica: $(OBJS)
	$(CC) -o digica $(OBJS) $(LDFLAGS) 

#
# The target below indicates to make how to process files with a cpp 
# extension. Normally this isn't necessary, but the cpp extension isn't 
# defined for make.
#

.cpp.o:
	$(CC) $(CPPFLAGS) -c -g $< 

#
# As a standard practice, a clean target is included in most make files.
# By executing 'make clean', all object files, backup files, and the 
# executable are deleted.
#

clean:
	rm -f *.o *~ digica
