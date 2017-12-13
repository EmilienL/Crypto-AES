#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/stat.h>

#include "KeyExp.h"
#include "aesC.h"
#include "CBCpkcs.h"
#include "md5sum.h"


int main(int argc, char* argv[]){
	/* Aucun argument: chiffrement du bloc nul, clef nulle */
	if(argc==1){
		Chiffrage(blocNul,clefNulle,16);
	}
	/* 1 argument: -e || -d (de)chiffrement du bloc nul, clef nulle */
	else if(argc==2){
		char* option = argv[1];
		if(option[1] == 'e')
			Chiffrage(blocNul,clefNulle,16);
		if(option[1] == 'd')
			Dechiffrage(blocNul,clefNulle,16);
	}
	/* 2 arguments: -e || -d (de)chiffrement */
	/* d'un fichier(arg 2), clef nulle */
	else if(argc==3){
		char* option = argv[1];
		char fileName[50];
		strcpy(fileName,argv[2]);
		//char* fileName = malloc(strlen(argv[2])*sizeof(char));
		//fileName = argv[2];
		
		printf("2 arguments: %s de %s, clef nulle\n",option,fileName);
		if(option[1] == 'e'){
			Padding_file(fileName);
			
  //printf(" padded file : %s \n",newFileName);
  
			CBC( clefNulle);}
		if(option[1] == 'd')
			Inv_CBC(fileName, clefNulle);
		
	}
	/* 3 arguments: Idem + mot de passe(arg 3) */
	/* clef = md5sum arg3 */
	else if(argc==4){
		char* option = argv[1];
		char* fileName = argv[2];
		char* mdp = argv[3];
		printf("3 arguments: %s de %s, clef %s\n",option,fileName,argv[3]);
		StringToMd5(mdp);
		if(option[1] == 'e'){
			Padding_file(fileName);
			
  //printf(" padded file : %s \n",newFileName);
  
			CBC( clef);}
		if(option[1] == 'd')
			Inv_CBC(fileName, clef);
	}
	else{
		printf("Mauvais nombre d'arguments (0-1-2-3).\n");
		return 0;
	}
	return 0;
}
