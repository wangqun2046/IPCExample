all : helloworld shmread shmwrite clean_all

clean_all : clean_all.o
	g++ -o $@ $^

helloworld : helloworld.o
	g++ -o $@ $^

shmread : shmread.o
	g++ -o $@ $^

shmwrite : shmwrite.o
	g++ -o $@ $^

*.cpp.o : 

clean:
	rm -f *.o helloworld shmread shmwrite clean_all
