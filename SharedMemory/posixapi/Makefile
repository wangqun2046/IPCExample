all : shmread shmwrite 

shmwrite : shmwrite.o
	gcc -g -o $@ $^

shmread : shmread.o
	gcc -g -o $@ $^

*.cc.o : *.c

clean :
	rm -vf shmwrite shmread *.o
