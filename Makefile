#gcc wallis.c -o wallis -lpthread -lm

TARGET = main


SRC = src
INC = include
OBJ = obj
LIB = lib

CC = gcc
CFLAGS = -c -Wall

all: $(OBJ)/wallis.o $(OBJ)/fourier.o
  

$(OBJ)/wallis.o : $(SRC)/wallis.c
	gcc $(SRC)/wallis.c -o $(OBJ)/wallis.o -lpthread -lm 


$(OBJ)/fourier.o : $(SRC)/fourier.c
	gcc $(SRC)/fourier.c -o $(OBJ)/fourier.o -lpthread -lm


.PHONY : run clean plot imgdel

run : $(OBJ)/wallis.o $(OBJ)/fourier.o
	$(OBJ)/wallis.o $(VAR)
	$(OBJ)/fourier.o $(VAR)


run_wallis : $(OBJ)/wallis.o
	$(OBJ)/wallis.o $(VAR)

run_fourier : $(OBJ)/fourier.o
	$(OBJ)/fourier.o $(VAR)

plot : 
	make all -C gnuplot

open : 
	eog gnuplot/images/output.png

imgdel :
	make clean -C gnuplot


clean :
	rm -f $(OBJ)/wallis.o $(OBJ)/fourier.o

