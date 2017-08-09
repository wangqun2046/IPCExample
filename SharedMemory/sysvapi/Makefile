all : shread shwrite 

shwrite : shwrite.o
	gcc -o $@ $^

shread : shread.o
	gcc -o $@ $^

*.cc.o : *.c

clean :
	rm -vf shwrite shread *.o