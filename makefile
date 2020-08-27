objs=quality_evaluation.o processing.o
CC=gcc
CFLAGS=-O3 -g
LFLAGS=-I./ -ltiff -pthread -lm 

q_test3 : $(objs) main.c
	$(CC) $(CFLAGS) -o q_test3 $(objs) main.c $(LFLAGS)

processing.o : processing.c quality_evaluation.o
	$(CC) $(CFLAGS) -o processing.o -c processing.c quality_evaluation.o $(LFLAGS)

quality_evaluation.o :quality_evaluation.c
	$(CC) $(CFLAGS) -o quality_evaluation.o  -c quality_evaluation.c $(LFLAGS)
