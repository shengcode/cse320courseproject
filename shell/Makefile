
all:320sh

320sh:320sh.o file2.o file3.o array_list.o
	gcc -g 320sh.o file2.o file3.o array_list.o main.h file2.h file3.h array_list.h -o 320sh

320sh.o:320sh.c main.h
	gcc -g -c 320sh.c main.h 

file2.o:file2.c file2.h
	gcc -g -c file2.c file2.h 
	
file3.o:file3.c file3.h
	gcc -g -c file3.c file3.h 

array_list.o: array_list.c array_list.h
	gcc -g -c array_list.c array_list.h
	
clean:
	rm -f *.o 320sh