#This sample Makefile allows you to make an OpenGL application
#   whose source is exactly one .c or .cc file.
#
#
# To use this Makefile, you type:
#
#        make xxxx
#                  
# where
#       xxxx.c is the name of the file you wish to compile 
#       
# A binary named xxxx will be produced

CC = gcc
C++ = g++ 
LIBDIRS = -L/usr/lib64
INCDIRS = -I/usr/include
LDLIBS =  -lglut -lGL -lGLU -lX11 -lm

OBJS = poly.o


poly : $(OBJS)
	$(C++) -O -g -o poly $(OBJS) $(INCDIRS) $(LIBDIRS) $(LDLIBS)

poly.o : poly.cpp
	$(C++) -O -g -c poly.cpp
