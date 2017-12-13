#include "CBCpkcs.h"

char paddedString[50];

char cryptedFileName[50];

void bloc_copy(uchar* bloc, uchar* copy){
	for(int i=0;i<16;i++)
		copy[i] = bloc[i];
}

uchar* randomBloc(){
	srand(time(0));	
	
	uchar* randomBloc = malloc(16*sizeof*randomBloc);
	for(int i=0;i<16;i++){
		//randomBloc[i] = (rand() & 0xfff);
		//printf(" randomBloc[%d]=%x",i,randomBloc[i] & 0xfff);
		randomBloc[i] = 0x00;
	}
	
	return randomBloc;
}

void Padding_file(char* fileName){
  uchar buffer[16];
  int nb_octets_lus;
  
  int k;    
  
  printf("\n");
  printf(" file name : %s \n", fileName);
  
  FILE* fichier;
  FILE* paddedFile = NULL;
  
  fichier = fopen(fileName,"r");
  if(fichier == NULL) {printf("aie\n");exit(1);}
  
  char newFileName[50];
  strcpy(newFileName,"padded-");
  strcat(newFileName,fileName);
  
  strcpy(paddedString,newFileName);
  
  paddedFile = fopen(newFileName,"w");
  
  int pad ;
  
  if(fichier == 0) {printf("Pb ouverture"); exit(0);}
  
  nb_octets_lus = fread (buffer, 1, sizeof(buffer), fichier);   // Lecture du premier morceau
  while (nb_octets_lus != 0) {
  	if(nb_octets_lus < 16){
  		pad = (16-nb_octets_lus);
  		printf("pad = %d \n",pad);

  		for (k=0; k < nb_octets_lus; k++)  {
    		
    			
    			fprintf(paddedFile,"%c",buffer[k]);
    		
  		}
  		for(int p = nb_octets_lus;p<16;p++){
  			fprintf(paddedFile,"%c",pad & 0xfff);
  		}
  	}
  	else{
  		for (k=0; k < nb_octets_lus; k++)  {
    		
    		
    		fprintf(paddedFile,"%c",buffer[k]);
    		
  	}
  	}
  	nb_octets_lus = fread (buffer, 1, sizeof(buffer), fichier); // Lecture du morceau suivant
  }
  fclose(fichier);
  fclose(paddedFile);
  
}

void CBC( uchar* Key){
  uchar buffer[16];
  int nb_octets_lus;
  
  int k;    
  
  
  FILE* cryptedFile = NULL;
  FILE* paddedFile;
  
  printf(" padded file : %s \n",paddedString);
  
  paddedFile = fopen(paddedString,"r");
  if(paddedFile == NULL) {printf("aie\n");exit(1);}
  
  
  strcpy(cryptedFileName,"aes-");
  strcat(cryptedFileName,paddedString);
  
  printf("crypted file name : %s \n",cryptedFileName);
  
  cryptedFile = fopen(cryptedFileName,"w");
  
  
  uchar* initBloc = randomBloc();
  for (k=0; k < 16; k++)  {
  		fprintf(cryptedFile,"%c",initBloc[k]);
	}
  
  
  nb_octets_lus = fread (buffer, 1, sizeof(buffer), paddedFile);   // Lecture du premier morceau
  while (nb_octets_lus != 0) {
  	
  	uchar* currentBloc = malloc(16*sizeof*currentBloc);
  	
  	for(k=0 ; k<16 ; k++){
  		currentBloc[k] = initBloc[k] ^ buffer[k];
  	}
  	
  	Chiffrage(currentBloc,Key,16);
  	
  	
  	for (k=0; k < nb_octets_lus; k++)  {
  		fprintf(cryptedFile,"%c",currentBloc[k]);
	}
	
	bloc_copy(currentBloc,initBloc);
 	
  	nb_octets_lus = fread (buffer, 1, sizeof(buffer), paddedFile); // Lecture du morceau suivant
  }
  
  free(initBloc);
  
  fclose(cryptedFile);
  fclose(paddedFile);
}

void Inv_CBC(char* cryptedFileName, uchar* Key){
  uchar buffer[16];
  int nb_octets_lus;
  
  int k;    
  
  uchar padlingBuffer[16];
  uchar initBloc[16];
  uchar tampBloc[16];
  
  FILE* unCryptedFile = NULL;
  FILE* cryptedFile;
  
  printf("crypted file name : %s \n",cryptedFileName);
  
  cryptedFile = fopen(cryptedFileName,"r");
  if(cryptedFile == NULL) {printf("aie\n");exit(1);}
  
  char unCryptedFileName[50];
  strcpy(unCryptedFileName,"aes-");
  strcat(unCryptedFileName,cryptedFileName);
  
  unCryptedFile = fopen(unCryptedFileName,"w");
  
  nb_octets_lus = fread (buffer, 1, sizeof(buffer), cryptedFile);   // Lecture du premier morceau
  
  bloc_copy(buffer,initBloc);
  
  nb_octets_lus = fread (buffer, 1, sizeof(buffer), cryptedFile);
  bloc_copy(buffer,tampBloc);
  
  while (nb_octets_lus != 0) {
  	Dechiffrage(buffer,Key,16);
  	
  	for(k=0 ; k<16 ; k++){
  		buffer[k] = initBloc[k] ^ buffer[k];
  	}
  	
  	bloc_copy(tampBloc,initBloc);
  	bloc_copy(buffer,padlingBuffer);
  	
  	int stop = -1;
  	
  	nb_octets_lus = fread (buffer, 1, sizeof(buffer), cryptedFile); 
  	if(nb_octets_lus == 0){
  		printf("bloc de fin trouvé\n");
  		for(int i = 15;i>0;i--){
  			printf(" %d : %d \n", (int)padlingBuffer[i], 16-i);
  			if((int)padlingBuffer[i] == 16-i){
  				printf("je rentre\n");
  				for(int j=i+1;j<16;j++){
  					if(padlingBuffer[i]!=padlingBuffer[j]){
  						stop = -1;
  						break;
  					}
  					else
  						stop = i;
  				}
  				printf("le pad est a %d \n",stop);
  			}
  		}
  		
  		for (k=0; k < stop; k++)  {
  			
  				fprintf(unCryptedFile,"%c",padlingBuffer[k]);
			
		}
  	}
  	else{
  		for (k=0; k < nb_octets_lus; k++)  {
  			fprintf(unCryptedFile,"%c",padlingBuffer[k]);
		}
 	}
 	bloc_copy(buffer,tampBloc);
  }
  
  /*int index = 0;
  
  uchar** BigBuffer;
  BigBuffer = malloc(100000*sizeof*BigBuffer);
  for(int i = 0; i<100000;i++){
  	BigBuffer[i] = malloc(16*sizeof*BigBuffer[i]);  
  }
  
  nb_octets_lus = fread (buffer, 1, sizeof(buffer), cryptedFile);   // Lecture du premier morceau
  printf("apres premiere lecture de %d octets \n",nb_octets_lus);
  bloc_copy(buffer,BigBuffer[index]);
  nb_octets_lus = fread (buffer, 1, sizeof(buffer), cryptedFile);   // Lecture du premier morceau
  
  printf("apres deuxieme lecture de %d octets \n",nb_octets_lus);
  
  while (nb_octets_lus != 0) {
  	index++;
  	printf("index : %d\n",index);
  	//BigBuffer = realloc(BigBuffer,nbBloc*sizeof*BigBuffer);
  	//BigBuffer[index] = malloc(16*sizeof*BigBuffer[index]);
	
	bloc_copy(buffer,BigBuffer[index]);
 	
 	
  	nb_octets_lus = fread (buffer, 1, sizeof(buffer), cryptedFile); // Lecture du morceau suivant
  	printf("apres %d lecture de %d octets\n",index+2,nb_octets_lus);
  	
  	
  	
  }
  printf("avant le for \n");
  for (int s = index ; s > 0 ; s--){
  	printf("dechiffrage de BigBuffer[%d] \n",s);
  	Dechiffrage(BigBuffer[s],Key,16);
  	for(k=0 ; k<16 ; k++){
  		BigBuffer[s][k] = BigBuffer[s-1][k] ^ BigBuffer[s][k];
  	}
  }
  
  for(int i = 1;i<=index;i++){
  	
  	for(int j = 0;j<16;j++){
  		fprintf(unCryptedFile,"%c",BigBuffer[i][j]);
  	}
  }
  
 
  for(int i = 0; i<100000;i++){
  	free(BigBuffer[i]);  
  }
  free(BigBuffer);*/
  
  fclose(cryptedFile);
  fclose(unCryptedFile);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void withoutCBC( uchar* Key){
  uchar buffer[16];
  int nb_octets_lus;
  
  int k;    
  
  
  FILE* cryptedFile = NULL;
  FILE* paddedFile;
  
  printf(" padded file : %s \n",paddedString);
  
  paddedFile = fopen(paddedString,"r");
  if(paddedFile == NULL) {printf("aie\n");exit(1);}
  
  
  
  strcpy(cryptedFileName,"aes-");
  strcat(cryptedFileName,paddedString);
  
  printf("crypted file name : %s \n",cryptedFileName);
  
  cryptedFile = fopen(cryptedFileName,"w");
  
  nb_octets_lus = fread (buffer, 1, sizeof(buffer), paddedFile);   // Lecture du premier morceau
  while (nb_octets_lus != 0) {
  	
  	Chiffrage(buffer,Key,16);
  	
  	
  	for (k=0; k < nb_octets_lus; k++)  {
  		fprintf(cryptedFile,"%c",buffer[k]);
	}
	
	
 	
  	nb_octets_lus = fread (buffer, 1, sizeof(buffer), paddedFile); // Lecture du morceau suivant
  }
  
  
  fclose(cryptedFile);
  fclose(paddedFile);
}

void Inv_withoutCBC(char* cryptedFileName, uchar* Key){
  uchar buffer[16];
  int nb_octets_lus;
  
  int k;    
  
  uchar padlingBuffer[16];
  
  FILE* unCryptedFile = NULL;
  FILE* cryptedFile;
  
  printf("crypted file name : %s \n",cryptedFileName);
  
  cryptedFile = fopen(cryptedFileName,"r");
  if(cryptedFile == NULL) {printf("aie\n");exit(1);}
  
  char unCryptedFileName[50];
  strcpy(unCryptedFileName,"aes-");
  strcat(unCryptedFileName,cryptedFileName);
  
  unCryptedFile = fopen(unCryptedFileName,"w");
  
 
  
  nb_octets_lus = fread (buffer, 1, sizeof(buffer), cryptedFile);   // Lecture du premier morceau
  
  
  while (nb_octets_lus != 0) {
  	Dechiffrage(buffer,Key,16);
  	
  	bloc_copy(buffer,padlingBuffer);
  	
  	int stop = -1;
  	
  	nb_octets_lus = fread (buffer, 1, sizeof(buffer), cryptedFile); 
  	if(nb_octets_lus == 0){
  		printf("bloc de fin trouvé\n");
  		for(int i = 15;i>0;i--){
  			printf(" %d : %d \n", (int)padlingBuffer[i], 16-i);
  			if((int)padlingBuffer[i] == 16-i){
  				printf("je rentre\n");
  				for(int j=i+1;j<16;j++){
  					if(padlingBuffer[i]!=padlingBuffer[j]){
  						stop = -1;
  						break;
  					}
  					else
  						stop = i;
  				}
  				printf("le pad est a %d \n",stop);
  			}
  		}
  		
  		for (k=0; k < stop; k++)  {
  			
  				fprintf(unCryptedFile,"%c",padlingBuffer[k]);
			
		}
  	}
  	else{
  		for (k=0; k < nb_octets_lus; k++)  {
  			fprintf(unCryptedFile,"%c",padlingBuffer[k]);
		}
 	}
 	
  }
  
  fclose(cryptedFile);
  fclose(unCryptedFile);
}
