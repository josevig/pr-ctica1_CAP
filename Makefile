esqueleto: esqueleto.c 
	gcc -c funciones_matriz.c -o funciones_matriz.o
	gcc -c esqueleto.c -o esqueleto.o
	gcc -o esqueleto esqueleto.o funciones_matriz.o

pruebas: pruebas.c
	gcc -c funciones_matriz.c -o funciones_matriz.o
	gcc -c pruebas.c -o pruebas.o 
	gcc -o pruebas pruebas.o funciones_matriz.o