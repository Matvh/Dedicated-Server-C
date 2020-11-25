f1.exe : main.o input.o mssg.o conexion.o file.o
	gcc -o McGruder main.o input.o  mssg.o conexion.o file.o -Wall -Wextra -lpthread -lm

main.o : main.c ../Aux/input.h
	gcc -c main.c -Wall -Wextra -lm
input.o :
	gcc -c ../Aux/input.c -Wall -Wextra
mssg.o :
	gcc -c ../Aux/mssg.c -Wall -Wextra
conexion.o :
	gcc -c ../Aux/conexion.c -Wall -Wextra
file.o :
	gcc -c Aux/file.c -Wall -Wextra -lm
clean :
	rm *.o
