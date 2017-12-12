// -*- coding: utf-8 -*-

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifndef LG_FLUX
#define LG_FLUX 10
#endif

typedef unsigned char uchar; // Les octets sont non-signés.

int main() 
{
  uchar buffer[16];
  int nb_octets_lus;
  
  uchar state[256];
  int i=0, j=0, w, k, p;    
  
  printf("\n");
  
  FILE* fichier;
  FILE* confidentiel = NULL;
  fichier = fopen("butokuden.jpg","r");
  confidentiel = fopen("pkcs5-butokuden.jpg","w");
  int octets;
  int pad ;
  
  if(fichier == 0) {printf("Pb ouverture"); exit(0);}
  
  nb_octets_lus = fread (buffer, 1, sizeof(buffer), fichier);   // Lecture du premier morceau
  while (nb_octets_lus != 0) {
  	if(nb_octets_lus < 16){
  		pad = (16-nb_octets_lus);
  		printf("pad = %d \n",pad);

  		for (k=0; k < nb_octets_lus; k++)  {
    		
    			
    			fprintf(confidentiel,"%c",buffer[k]);
    		
  		}
  		for(p = nb_octets_lus;p<16;p++){
  			fprintf(confidentiel,"%c",pad);
  		}
  	}
  	else{
  		for (k=0; k < nb_octets_lus; k++)  {
    		
    		
    		fprintf(confidentiel,"%c",buffer[k]);
    		
  	}
  	}
  	nb_octets_lus = fread (buffer, 1, sizeof(buffer), fichier); // Lecture du morceau suivant
  }
  fclose(fichier);
  fclose(confidentiel);
    
  return 0; 
}

/*
> make
gcc -o mon_RC4 -I/usr/local/include -I/usr/include mon_RC4.c -L/usr/local/lib -L/usr/lib -lm -lssl -lcrypto -g -Wall
> ./mon_RC4
0xB2 0x39 0x63 0x05 0xF0 0x3D 0xC0 0x27 0xCC 0xC3 
> gcc -o mon_RC4 -I/usr/local/include -I/usr/include mon_RC4.c -L/usr/lib -lm -g -Wall -DLG_FLUX=20
> ./mon_RC4
0xB2 0x39 0x63 0x05 0xF0 0x3D 0xC0 0x27 0xCC 0xC3 0x52 0x4A 0x0A 0x11 0x18 0xA8 0x69 0x82 0x94 0x4F 
> 
*/
