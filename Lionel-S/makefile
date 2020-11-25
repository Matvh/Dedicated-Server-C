f1.exe : main.o input.o mssg.o conexion.o checksum.o lista.o file.o cuesmensajes.o paquita.o server.o semaphore_v2.o
	gcc -o Lionel main.o input.o mssg.o conexion.o checksum.o lista.o file.o cuesmensajes.o paquita.o server.o semaphore_v2.o -Wall -Wextra -lpthread -lm

main.o : main.c
	gcc -c main.c -Wall -Wextra -lm
input.o :
	gcc -c ../Aux/input.c -Wall -Wextra
mssg.o :
	gcc -c ../Aux/mssg.c -Wall -Wextra
conexion.o :
	gcc -c ../Aux/conexion.c -Wall -Wextra
checksum.o:
	gcc -c ../Aux/checksum.c -Wall -Wextra
lista.o :
	gcc -c Aux/lista.c -Wall -Wextra
file.o :
	gcc -c Aux/file.c -Wall -Wextra -lm
cuesmensajes.o :
	gcc -c Aux/cuesmensajes.c -Wall -Wextra
paquita.o:
	gcc -c Aux/paquita.c -Wall -Wextra
server.o:
	gcc -c Aux/server.c -Wall -Wextra
semaphore_v2.o :
	gcc -c Aux/semaphore_v2.c -Wall -Wextra
clean :
	rm *.o
