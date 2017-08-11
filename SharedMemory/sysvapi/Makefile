all : shmread shmwrite 

shmwrite : shmwrite.o
	gcc -o $@ $^

shmread : shmread.o
	gcc -o $@ $^

*.cc.o : *.c

clean :
	rm -vf shmwrite shmread *.o
