#include <stdio.h>
#include <stdlib.h>
#include "RGB.h"

int main(void) {
	char nombre[20];
	printf("ingrese el nombre del archivo a convertir: ");
	scanf("%s",nombre);
	PPMFile a = ppmFile_create(nombre);
	//ppmFile_blackAndWhite(a);
	ppmFile_compress(a);
	return EXIT_SUCCESS;
}
