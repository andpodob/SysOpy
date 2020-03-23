all:
	gcc -c matrixutils.c
	gcc -c filesmanagement.c
	gcc -c proccessmanagment.c
	gcc -o macierz macierz.c matrixutils.o filesmanagement.o proccessmanagment.o
	gcc -o generator matrixgenerator.c filesmanagement.o
	cp macierz /usr/bin/macierz
	rm matrixutils.o filesmanagement.o proccessmanagment.o