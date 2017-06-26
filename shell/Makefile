
all:320sh

320sh:320sh.o array_list.o buildProcess.o
	gcc -g 320sh.o array_list.o buildProcess.o main.h array_list.h buildProcess.h -o 320sh

320sh.o:320sh.c main.h
	gcc -g -c 320sh.c main.h 
	
buildProcess.o:buildProcess.c buildProcess.h
	gcc -g -c buildProcess.c buildProcess.h

array_list.o: array_list.c array_list.h
	gcc -g -c array_list.c array_list.h
	
clean:
	rm -f *.o 320sh