#ifndef CBC__H
#define CBC__H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include "aesC.h"

#ifndef LG_FLUX
#define LG_FLUX 10
#endif

typedef unsigned char uchar; // Les octets sont non-signés.

extern char paddedString[50];

extern char cryptedFileName[50];

void bloc_copy(uchar* bloc, uchar* copy);

uchar* randomBloc();

void Padding_file(char* fileName);

void CBC( uchar* Key);

void Inv_CBC(char* cryptedFileName, uchar* Key);

void Inv_withoutCBC(char* cryptedFileName, uchar* Key);

void withoutCBC( uchar* Key);


#endif
