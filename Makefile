# For Linux or any machines with gcc compiler
CC = gcc
CFLAGS = -std=c99 -Wall -pedantic -g

# For SunOS
#CFLAGS = -Aa

all: lab3

clean:
	/bin/rm *.o lab3

OBJ = tiff.o allocate.o randlib.o subroutine.o

lab3: lab3.o $(OBJ) 
	$(CC) $(CFLAGS) -o lab3 lab3.o $(OBJ) -lm -g

