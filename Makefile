build: planificator

planificator: planificator.o fcfs.o operations.o sjf.o roundrobin.o pp.o bonus.o
	gcc planificator.o fcfs.o operations.o sjf.o roundrobin.o pp.o bonus.o -o planificator

planificator.o: planificator.c
	gcc -c planificator.c

fcfs.o: fcfs.c
	gcc -c fcfs.c

operations.o: operations.c
	gcc -c operations.c

sjf.o: sjf.c
	gcc -c sjf.c

roundrobin.o: roundrobin.c
	gcc -c roundrobin.c

pp.o: pp.c
	gcc -c pp.c

bonus.o: bonus.c
	gcc -c bonus.c

clean:
	rm -rf planificator *.o
