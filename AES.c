#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
	/* Aucun argument: chiffrement du bloc nul, clef nulle */
	if(argc==1){
		printf("Chiffrement du bloc nul, clef nulle\n");
	}
	/* 1 argument: -e || -d (de)chiffrement du bloc nul, clef nulle */
	else if(argc==2){
		char* option = argv[1];
		printf("1 argument: %s du bloc nul, clef nulle\n",option);
	}
	/* 2 arguments: -e || -d (de)chiffrement */
	/* d'un fichier(arg 2), clef nulle */
	else if(argc==3){
		char* option = argv[1];
		char* fileName = argv[2];
		printf("2 arguments: %s de %s, clef nulle\n",option,fileName);
	}
	/* 3 arguments: Idem + mot de passe(arg 3)
	/* clef = md5sum arg3 */
	else if(argc==4){
		char* option = argv[1];
		char* fileName = argv[2];
		char* mdp = argv[3];
		printf("3 arguments: %s de %s, clef %s\n",option,fileName,argv[3]);
	}
	else{
		printf("Mauvais nombre d'arguments (0-1-2-3).\n");
		return 0;
	}
	return 0;
}
