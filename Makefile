esqueleto: esqueleto.c 
	gcc -c esqueleto.c -o esqueleto.o 
	gcc -o esqueleto esqueleto.o 

pruebas: pruebas.c 
	gcc -c pruebas.c -o pruebas.o 
	gcc -o pruebas pruebas.o 
