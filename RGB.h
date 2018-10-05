#ifndef RGB_H_
#define RGB_H_

typedef struct PPMFileStruct * PPMFile;

PPMFile ppmFile_create(char * name);
void ppmFile_destroy(PPMFile pf);
void ppmFile_blackAndWhite(PPMFile pf);
void ppmFile_grayscale(PPMFile pf);
void ppmFile_compress(PPMFile pf);
void ppmFile_dithering(PPMFile pf);


#endif /* RGB_H_ */
