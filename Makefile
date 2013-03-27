
all : init.o main.o executor.o watcher.o
	gcc -o cage init.o main.o executor.o watcher.o

init.o : init.c
	gcc -c init.c

main.o : init.h main.c
	gcc -c main.c

watcher.o : watcher.c
	gcc -c watcher.c

executor.o : executor.c
	gcc -c executor.c

clean :
	rm *.o cage
