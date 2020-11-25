f1.exe : main.o input.o mssg.o conexion.o tavish.o
	gcc -o McTavish main.o input.o  mssg.o conexion.o tavish.o -Wall -Wextra -lpthread -lm

main.o : main.c ../Aux/input.h
	gcc -c main.c -Wall -Wextra -lm
input.o :
	gcc -c ../Aux/input.c -Wall -Wextra
mssg.o :
	gcc -c ../Aux/mssg.c -Wall -Wextra
conexion.o :
	gcc -c ../Aux/conexion.c -Wall -Wextra
tavish.o :
	gcc -c Aux/tavish.c -Wall -Wextra
clean :
	rm *.o
