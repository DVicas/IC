CC=g++
CFLAGS=-g -Wall 
AUDIOFLAG= -lsndfile

all: ex1 ex2 ex3 ex3b

ex1: ex1.cpp
	$(CC) $(CFLAGS) $< -o $@.o

ex2: ex2.cpp
	$(CC) $(CFLAGS) $(AUDIOFLAG) $< -o $@.o

ex3: ex3.cpp
	$(CC) $(CFLAGS) $(AUDIOFLAG) $< -o $@.o

ex3b: ex3_b.cpp 
	$(CC) $(CFLAGS) $(AUDIOFLAG) $< -o $@.o

clean: 
	rm *.o
