# For Linux or any machines with gcc compiler
CFLAGS = -std=c99 -g -Wall -Wshadow --pedantic -Wvla -Werror
GCC = gcc $(CFLAGS)
SRCS = allocate.c subroutine.c tiff.c Lab3.c

OBJ = $(SRCS:%.c=%.o)

Lab3: $(OBJ) 
	$(GCC) $(CFLAGS) $(OBJ) -o Lab3


.c.o:
	$(GCC) $(FLAGS) -c $*.c



clean:
	/bin/rm *.o Lab3
