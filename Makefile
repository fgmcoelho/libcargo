CC=gcc
C_FLAGS=-O3
ifdef $(DEBUG)
C_FLAGS+=-g 
endif

all:
	$(CC) $(C_FLAGS) *.c -o container 

