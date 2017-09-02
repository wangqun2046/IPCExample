all : semaphore 

semaphore : semaphore.o
	gcc -o $@ $^

*.cc.o : *.c

clean :
	rm -vf semaphore *.o
