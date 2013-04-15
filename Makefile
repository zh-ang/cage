
all : init.o main.o executor.o tracer.o
	gcc -o cage init.o main.o executor.o tracer.o

init.o : init.c
	gcc -c init.c

main.o : init.h main.c
	gcc -c main.c

tracer.o : tracer.c
	gcc -c tracer.c

executor.o : executor.c
	gcc -c executor.c

clean :
	rm *.o cage
