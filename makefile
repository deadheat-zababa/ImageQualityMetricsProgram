objs=processing.o quality_evaluation.o origin.o
CC=gcc
CFLAGS=-O3 -g
LFLAGS=-I./ -I./origin -ltiff -pthread -lm 

q_test3 : $(objs) main.c
	$(CC) $(CFLAGS) -o q_test3 $(objs) main.c $(LFLAGS)

processing.o : processing.c quality_evaluation.o origin.o
	$(CC) $(CFLAGS) -o processing.o -c processing.c quality_evaluation.o origin.o $(LFLAGS)

quality_evaluation.o :quality_evaluation.c
	$(CC) $(CFLAGS) -o quality_evaluation.o  -c quality_evaluation.c $(LFLAGS)

origin.o :origin/origin_ssim.c
	$(CC) $(CFLAGS) -o origin.o  -c origin/origin_ssim.c $(LFLAGS)
