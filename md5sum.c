// -*- coding: utf-8 -*-


#include "md5sum.h"

uchar clef[16];

uchar* StringToMd5(char* mdp)
{
  int i;
  uchar resume_md5[MD5_DIGEST_LENGTH];
  
  
  
  MD5_CTX contexte;
  MD5_Init(&contexte); // Initialisation de la fonction de hachage
  //nb_octets_lus = fread (buffer, 1, sizeof(buffer), fichier);   // Lecture du premier morceau
  //while (nb_octets_lus != 0) {
    MD5_Update(&contexte, mdp, 16);                    // Digestion du morceau
    //nb_octets_lus = fread (buffer, 1, sizeof(buffer), fichier); // Lecture du morceau suivant
  //}
  //fclose (fichier);
  MD5_Final(resume_md5, &contexte);
  printf("La clef utilisé est: 0x");
  for(i = 0; i < MD5_DIGEST_LENGTH; i++){
  	 printf("%02x", resume_md5[i]);
  	 clef[i] = resume_md5[i] ;
  }
  printf("\n");
  
  
  return resume_md5;
}

/*
> make
> ./resumes
Le résumé MD5 du fichier "butokuden.jpg" vaut: 0xaeef572459c1bec5f94b8d62d5d134b5
> cat butokuden.jpg | md5
aeef572459c1bec5f94b8d62d5d134b5
*/
