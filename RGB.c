#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "RGB.h"

typedef struct{
	unsigned char r, g, b;
}RGB;

struct PPMFileStruct{
	char * name;
	int nameLength;
	int width;
	int height;
	int max;
	RGB ** imagen;
};

void ppdisplay(PPMFile pf){
	printf("\n\n");
	printf("P3\n");
	printf("%d %d\n",pf->width,pf->height);
	printf("%d\n",pf->max);
	for(int h=0;h<pf->width;h++){
		for(int w=0;w<pf->height;w++){
			printf("%d %d %d\n", pf->imagen[h][w].r,pf->imagen[h][w].g,pf->imagen[h][w].b);
		}
	}
	printf("\n\n");
}


PPMFile ppmFile_create(char * name){
	int i;
	PPMFile pf = (PPMFile)malloc(sizeof(struct PPMFileStruct));
	FILE * f;
	f = fopen(name, "r");
	if(f==NULL){
		free(pf);
		printf("Valio verga");
		return NULL;
	}
	for(i = 0; name[i]!=0 ; i++);
	pf->nameLength = i;
	pf->name = (char*)malloc(i+1);
	for(int j = 0; j<=i; j++){
		pf->name[j] = name[j];
	}
	fseek(f, 4, SEEK_SET);
	char *arr = (char*)malloc(1);
	char *temp;
	int x = 0;
	do{
		arr[x] = fgetc(f);
		temp = (char*)realloc(arr, x+1);
		if(temp!=NULL){
			arr=temp;
		}
		x++;
	}while(arr[x-1] != ' ');
	sscanf(arr, "%d", &pf->width);
	printf("%d\n", pf->width);
	x = 0;
	free(arr);
	arr = (char*)malloc(1);
	do{
		arr[x] = fgetc(f);
		temp = (char*)realloc(arr, x+1);
		if(temp!=NULL){
			arr=temp;
		}
		x++;
	}while(arr[x-1] != '\n');
	sscanf(arr, "%d", &pf->height);
	printf("%d\n", pf->height);
	free(arr);
	arr = (char*)malloc(1);
	x=0;
	do{
		arr[x] = fgetc(f);
		temp = (char*)realloc(arr, x+1);
		if(temp!=NULL){
			arr=temp;
		}
		x++;
	}while(arr[x-1] != '\n');
	sscanf(arr, "%d", &pf->max);
	pf->imagen = (RGB**)malloc(sizeof(RGB*)*pf->width);
	for(int j = 0; j<pf->width; j++){
		pf->imagen[j] = (RGB*)malloc(sizeof(RGB)*pf->height);
	}
	int b;
	free(arr);
	arr=(char*)malloc(3);
	for(int fi = 0; fi<pf->width; fi++){
		for(int c = 0; c<pf->height; c++){
			x = 0;
			do{
				arr[x] = fgetc(f);
				x++;
			}while(arr[x-1] != ' ');
			sscanf(arr, "%d", &b);
			pf->imagen[fi][c].r = (char)b;
			printf("%d ", pf->imagen[fi][c].r);
			x = 0;
			do{
				arr[x] = fgetc(f);
				x++;
			}while(arr[x-1] != ' ');
			sscanf(arr, "%d", &b);
			pf->imagen[fi][c].g = (char)b;
			printf("%d ", pf->imagen[fi][c].g);
			x = 0;
			do{
				arr[x] = fgetc(f);
				x++;
			}while(arr[x-1] != '\n');
			sscanf(arr, "%d", &b);
			pf->imagen[fi][c].b = (char)b;
			printf("%d\n", pf->imagen[fi][c].b);
		}
	}
	fclose(f);
//	ppdisplay(pf);
	return pf;
}



void ppmFile_destroy(PPMFile pf){
	for(int i = 0; i<pf->height; i++){
		free(pf->imagen[i]);
	}
	free(pf->imagen);
	free(pf->name);
	free(pf);
}


void save(PPMFile pf){
	FILE * f;
	f=fopen(pf->name, "w+");
	fputs("P3\n", f);
	fprintf(f, "%d %d\n", pf->width, pf->height);
	fprintf(f,"%d\n",pf->max);
	printf("guardando\n");
	for(int h=0;h<pf->width;h++){
		for(int w=0;w<pf->height;w++){
			fprintf(f, "%d %d %d\n", pf->imagen[h][w].r,pf->imagen[h][w].g,pf->imagen[h][w].b);
		}
	}
	fclose(f);
	printf("%s guardado", pf->name);
}

void ppmFile_blackAndWhite(PPMFile pf){
	PPMFile bw_pf = (PPMFile)malloc(sizeof(struct PPMFileStruct));
	int largo = pf->nameLength + 3;
	char nombre[largo];
	nombre[0] = 'b';
	nombre[1] = 'w';
	nombre[2] = '_';
	int j = 0;
	for(int i = 3; i<=largo;i++, j++){
		nombre[i] = pf->name[j];
	}
	printf("%s\n", nombre);
	bw_pf->name = nombre;
	printf("%s\n", bw_pf->name);
	bw_pf->height = pf->height;
	bw_pf->width = pf->width;
	bw_pf->max = 1;
	bw_pf->nameLength = largo;
	bw_pf->imagen = (RGB**)malloc(sizeof(RGB*)*bw_pf->height);
	for(int i = 0; i<bw_pf->width; i++){
		bw_pf->imagen[i] = (RGB*)malloc(sizeof(RGB)*bw_pf->width);
	}
	int bw;
	for(int f = 0; f<pf->width; f++){
		for(int c = 0; c<pf->height; c++){
			bw = (int)round((pf->imagen[f][c].r + pf->imagen[f][c].g + pf->imagen[f][c].b)/3.0/255);
			bw_pf->imagen[f][c].r = (char)bw;
			bw_pf->imagen[f][c].g = (char)bw;
			bw_pf->imagen[f][c].b = (char)bw;
		}
	}
	save(bw_pf);
}

void ppmFile_grayscale(PPMFile pf);
void ppmFile_compress(PPMFile pf){
	PPMFile cmp_pf = (PPMFile)malloc(sizeof(struct PPMFileStruct));
	int largo = pf->nameLength + 4;
	char nombre[largo];
	nombre[0] = 'c';
	nombre[1] = 'm';
	nombre[2] = 'p';
	nombre[3] = '_';
	int j = 0;
	for(int i = 4; i<=largo;i++, j++){
		nombre[i] = pf->name[j];
	}
	printf("%s\n", nombre);
	cmp_pf->name = nombre;
	printf("%s\n", cmp_pf->name);
	cmp_pf->height = pf->height;
	cmp_pf->width = pf->width;
	cmp_pf->max = 1;
	cmp_pf->nameLength = largo;
	cmp_pf->imagen = (RGB**)malloc(sizeof(RGB*)*cmp_pf->height);
	for(int i = 0; i<cmp_pf->width; i++){
		cmp_pf->imagen[i] = (RGB*)malloc(sizeof(RGB)*cmp_pf->width);
	}
	
	int r,g,b;
	for(int f = 0; f<pf->width; f++){
		for(int c = 0; c<pf->height; c++){
			
			r = (int) round(pf->imagen[f][c].r /255.0);
			g = (int) round( pf->imagen[f][c].g/255.0);
			b = (int) round( pf->imagen[f][c].b/255.0);

			cmp_pf->imagen[f][c].r = (char)r;
			cmp_pf->imagen[f][c].g = (char)g;
			cmp_pf->imagen[f][c].b = (char)b;
		}
	}
	save(cmp_pf);
}
void ppmFile_dithering(PPMFile pf){
	PPMFile dth_pf = (PPMFile)malloc(sizeof(struct PPMFileStruct));
	int largo = pf->nameLength + 4;
	char nombre[largo];
	nombre[0] = 'd';
	nombre[1] = 't';
	nombre[2] = 'h';
	nombre[3] = '_';
	int j = 0;
	for(int i = 4; i<=largo;i++, j++){
		nombre[i] = pf->name[j];
	}
	printf("%s\n", nombre);
	dth_pf->name = nombre;
	printf("%s\n", dth_pf->name);
	dth_pf->height = pf->height;
	dth_pf->width = pf->width;
	dth_pf->max = 1;
	dth_pf->nameLength = largo;
	dth_pf->imagen = (RGB**)malloc(sizeof(RGB*)*dth_pf->height);
	for(int i = 0; i<dth_pf->width; i++){
		dth_pf->imagen[i] = (RGB*)malloc(sizeof(RGB)*dth_pf->width);
	}
	
	int r,g,b;
	for(int f = 0; f<(pf->width/2); f+=2){
		for(int c = 0; c<(pf->height/2); c+=2){
			r=
		}
	}
	save(cmp_pf);
}





