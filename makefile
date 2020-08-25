objs = quality_evaluation.o processing.o
CC = gcc
CFLAGS = -O3 -g -I./ -ltiff -pthread -lm -lrt -fopenacc -lcuda

q_test3: $(objs) main.c
	$(CC) $(CFLAGS) -o q_test3 $(objs) main.c

processing.o : processing.c quality_evaluation.o
	$(CC) $(CFLAGS) -o processing.o -c processing.c quality_evaluation.o

quality_evaluation.o:quality_evaluation.c
	$(CC) $(CFLAGS) -o quality_evaluation.o  -c quality_evaluation.c
