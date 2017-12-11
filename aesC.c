// -*- coding: utf-8 -*-

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>       // log, pow

typedef unsigned char uchar;

uchar Mul_F256[256][256]; // C'est une grande table de multiplication, à initialiser

/* Fonction mystérieuse qui calcule le produit de deux octets */
uchar gmul(uchar a, uchar b) {
  uchar p = 0;
  uchar hi_bit_set;
  int i;
  for(i = 0; i < 8; i++) {
    if((b & 1) == 1) 
      p ^= a;
    hi_bit_set = (a & 0x80);
    a <<= 1;
    if(hi_bit_set == 0x80) 
      a ^= 0x1b;		
    b >>= 1;
  }
  return p;
}

uchar SBox[256] = {
  0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76, 
  0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0, 
  0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15, 
  0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75, 
  0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84, 
  0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF, 
  0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8, 
  0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2, 
  0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73, 
  0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB, 
  0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79, 
  0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08, 
  0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A, 
  0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E, 
  0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF, 
  0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16};
  
uchar InvSBox[256];

int longueur_de_la_clef = 16 ;
uchar K[16] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
} ;
// La clef utilisée est la clef nulle de longueur 128 bits (16 octets)

int Nr = 10, Nk = 4;

int longueur_de_la_clef_etendue = 176;
uchar W[176] = { // Résultat du programme du TP précédent (Diversification)
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x62, 0x63, 0x63, 0x63, 0x62, 0x63, 0x63, 0x63, 0x62, 0x63, 0x63, 0x63, 0x62, 0x63, 0x63, 0x63,
  0x9B, 0x98, 0x98, 0xC9, 0xF9, 0xFB, 0xFB, 0xAA, 0x9B, 0x98, 0x98, 0xC9, 0xF9, 0xFB, 0xFB, 0xAA,
  0x90, 0x97, 0x34, 0x50, 0x69, 0x6C, 0xCF, 0xFA, 0xF2, 0xF4, 0x57, 0x33, 0x0B, 0x0F, 0xAC, 0x99,
  0xEE, 0x06, 0xDA, 0x7B, 0x87, 0x6A, 0x15, 0x81, 0x75, 0x9E, 0x42, 0xB2, 0x7E, 0x91, 0xEE, 0x2B,
  0x7F, 0x2E, 0x2B, 0x88, 0xF8, 0x44, 0x3E, 0x09, 0x8D, 0xDA, 0x7C, 0xBB, 0xF3, 0x4B, 0x92, 0x90, 
  0xEC, 0x61, 0x4B, 0x85, 0x14, 0x25, 0x75, 0x8C, 0x99, 0xFF, 0x09, 0x37, 0x6A, 0xB4, 0x9B, 0xA7, 
  0x21, 0x75, 0x17, 0x87, 0x35, 0x50, 0x62, 0x0B, 0xAC, 0xAF, 0x6B, 0x3C, 0xC6, 0x1B, 0xF0, 0x9B, 
  0x0E, 0xF9, 0x03, 0x33, 0x3B, 0xA9, 0x61, 0x38, 0x97, 0x06, 0x0A, 0x04, 0x51, 0x1D, 0xFA, 0x9F, 
  0xB1, 0xD4, 0xD8, 0xE2, 0x8A, 0x7D, 0xB9, 0xDA, 0x1D, 0x7B, 0xB3, 0xDE, 0x4C, 0x66, 0x49, 0x41, 
  0xB4, 0xEF, 0x5B, 0xCB, 0x3E, 0x92, 0xE2, 0x11, 0x23, 0xE9, 0x51, 0xCF, 0x6F, 0x8F, 0x18, 0x8E
};


void affiche_bloc_matriciel(uchar *M) {
  int i,j;
  for (i=0; i<4; i++) { // lignes 0 à 3
    printf("          ");
    for (j=0; j<4; j++) { // colonnes 0 à 3
      printf ("%02X ", M[4*j+i] & 255); }
    printf("\n");
  }
}

/*uchar State[16] = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00
  } ;*/

uchar State[16] = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00
  } ;


/* Partie à compléter pour ce TP */

void Inv_SubBytes(){
	printf("Test de Inv_SubBytes \n");
	
	/*uchar State[16]  = { 
	0x63, 0x7c,0x77,0x7B,
	0xf2,0x6B,0x6f,0xC5,
	0x30,0x01,0x67,0x2B,
	0xFE,0xD7,0xAB,0x76
	};*/
	
	//printf("Le bloc \"State\" en entrée vaut : \n");
	//affiche_bloc_matriciel(State);
	
	for(int i=0;i<16;i++){
		State[i] = InvSBox[State[i]];
	}
	
	//printf("Le bloc \"State\" en sortie vaut : \n");
	//affiche_bloc_matriciel(State);
}

void SubBytes(){
	printf("Test de SubBytes \n");
	
	/*uchar State[16]= {
	0x00, 0x01,0x02,0x03,
	0x04,0x05,0x06,0x07,
	0x08,0x09,0x0A,0x0B,
	0x0C,0x0D,0x0E,0x0F
	};*/
	
	//printf("Le bloc \"State\" en entrée vaut : \n");
	//affiche_bloc_matriciel(State);

	for(int i=0;i<16;i++){
		State[i] = SBox[State[i]];
	}
	
	printf("Le bloc \"State\" en sortie vaut : \n");
	affiche_bloc_matriciel(State);
	
	/*for(int i =0;i<16;i++){
		State[i]=State[i];
	}
	Inv_SubBytes(State);
	printf("Le bloc \"State\" en sortie vaut : \n");
	affiche_bloc_matriciel(State);*/
}

void Inv_ShiftRows(){
	printf("Test de Inv_ShiftRow \n");
	
	/*uchar State[16]  = { 
	0x00, 0x05,0x0A,0x0F,
	0x04,0x09,0x0E,0x03,
	0x08,0x0D,0x02,0x07,
	0x0C,0x01,0x06,0x0B
	};*/
	
	//printf("Le bloc \"State\" en entrée vaut : \n");
	//affiche_bloc_matriciel(State);
	
	uchar tmp = State[13];
	
	State[13] = State[9];
	State[9] = State[5];
	State[5] = State[1];
	State[1] = tmp;
	
	tmp = State[14];
	uchar tmp2 = State[10];
	
	State[10] = State[2];
	State[14] = State[6];
	State[6] = tmp;
	State[2] = tmp2;
	
	tmp = State[3];

	State[3] = State[7];
	State[7] = State[11];
	State[11] = State[15];
	State[15] = tmp;
	
	//printf("Le bloc \"State\" en sortie vaut : \n");
	//affiche_bloc_matriciel(State);
}

void ShiftRows(){
	printf("Test de ShiftRow \n");
	/*uchar State[16]  = { 
	0x00, 0x01,0x02,0x03,
	0x04,0x05,0x06,0x07,
	0x08,0x09,0x0A,0x0B,
	0x0C,0x0D,0x0E,0x0F
	};*/
	
	//printf("Le bloc \"State\" en entrée vaut : \n");
	//affiche_bloc_matriciel(State);
	
	uchar tmp = State[1];
	
	State[1] = State[5];
	State[5] = State[9];
	State[9] = State[13];
	State[13] = tmp;
	
	tmp = State[2];
	uchar tmp2 = State[6];
	
	State[2] = State[10];
	State[6] = State[14];
	State[10] = tmp;
	State[14] = tmp2;
	
	tmp = State[15];

	State[15] = State[11];
	State[11] = State[7];
	State[7] = State[3];
	State[3] = tmp;
	
	printf("Le bloc \"State\" en sortie vaut : \n");
	affiche_bloc_matriciel(State);
	
	/*for(int i =0;i<16;i++){
		State[i]=State[i];
	}
	Inv_ShiftRows();
	printf("Le bloc \"State\" en sortie vaut : \n");
	affiche_bloc_matriciel(State);*/
}

void Inv_MixColumns(){
	printf("Test de Inv_MixColumns \n");
	
	/*uchar State[16]  = { 
	0x01, 0x00,0x00,0x00,
	0x00,0x01,0x00,0x00,
	0x00,0x00,0x01,0x00,
	0x00,0x00,0x00,0x01
	};*/
	
	uchar TampState[16];
	for(int i=0;i<16;i++){
		TampState[i] = State[i];
	}
	
	uchar Inv[16] = {
	0x0E,0x0B,0x0D,0x09,0x09,0x0E,0x0B,0x0D,0x0D,0x09,0x0E,0x0B,0x0B,0x0D,0x09,0x0E
	};
	
	//printf("Le bloc \"State\" en entrée vaut : \n");
	//affiche_bloc_matriciel(State);
	
	//printf ("%02X + %02X + %02X + %02X \n", gmul(0x02,TampState[0]),gmul(0x03,TampState[1]),gmul(0x01,TampState[2]),gmul(0x01,TampState[3]));
	
	State[0] = gmul(Inv[0],TampState[0])^gmul(Inv[1],TampState[1])^gmul(Inv[2],TampState[2])^gmul(Inv[3],TampState[3]);
	State[1] = gmul(Inv[4],TampState[0])^gmul(Inv[5],TampState[1])^gmul(Inv[6],TampState[2])^gmul(Inv[7],TampState[3]);
	State[2] = gmul(Inv[8],TampState[0])^gmul(Inv[9],TampState[1])^gmul(Inv[10],TampState[2])^gmul(Inv[11],TampState[3]);
	State[3] = gmul(Inv[12],TampState[0])^gmul(Inv[13],TampState[1])^gmul(Inv[14],TampState[2])^gmul(Inv[15],TampState[3]);
	
	State[4] = gmul(Inv[0],TampState[4])^gmul(Inv[1],TampState[5])^gmul(Inv[2],TampState[6])^gmul(Inv[3],TampState[7]);
	State[5] = gmul(Inv[4],TampState[4])^gmul(Inv[5],TampState[5])^gmul(Inv[6],TampState[6])^gmul(Inv[7],TampState[7]);
	State[6] = gmul(Inv[8],TampState[4])^gmul(Inv[9],TampState[5])^gmul(Inv[10],TampState[6])^gmul(Inv[11],TampState[7]);
	State[7] = gmul(Inv[12],TampState[4])^gmul(Inv[13],TampState[5])^gmul(Inv[14],TampState[6])^gmul(Inv[15],TampState[7]);
	
	State[8] = gmul(Inv[0],TampState[8])^gmul(Inv[1],TampState[9])^gmul(Inv[2],TampState[10])^gmul(Inv[3],TampState[11]);
	State[9] = gmul(Inv[4],TampState[8])^gmul(Inv[5],TampState[9])^gmul(Inv[6],TampState[10])^gmul(Inv[7],TampState[11]);
	State[10] = gmul(Inv[8],TampState[8])^gmul(Inv[9],TampState[9])^gmul(Inv[10],TampState[10])^gmul(Inv[11],TampState[11]);
	State[11] = gmul(Inv[12],TampState[8])^gmul(Inv[13],TampState[9])^gmul(Inv[14],TampState[10])^gmul(Inv[15],TampState[11]);
	
	State[12] = gmul(Inv[0],TampState[12])^gmul(Inv[1],TampState[13])^gmul(Inv[2],TampState[14])^gmul(Inv[3],TampState[15]);
	State[13] = gmul(Inv[4],TampState[12])^gmul(Inv[5],TampState[13])^gmul(Inv[6],TampState[14])^gmul(Inv[7],TampState[15]);
	State[14] = gmul(Inv[8],TampState[12])^gmul(Inv[9],TampState[13])^gmul(Inv[10],TampState[14])^gmul(Inv[11],TampState[15]);
	State[15] = gmul(Inv[12],TampState[12])^gmul(Inv[13],TampState[13])^gmul(Inv[14],TampState[14])^gmul(Inv[15],TampState[15]);
	
	printf("Le bloc \"State\" en sortie vaut : \n");
	affiche_bloc_matriciel(State);
}

void MixColumns(){
	printf("Test de MixColumns \n");
	
	/*uchar State[16] = { 
	0x0E, 0x09,0x0D,0x0B,
	0x0B,0x0E,0x09,0x0D,
	0x0D,0x0B,0x0E,0x09,
	0x09,0x0D,0x0B,0x0E
	};*/
	
	/*uchar TampState[16] = { 
	0x0E, 0x09,0x0D,0x0B,
	0x0B,0x0E,0x09,0x0D,
	0x0D,0x0B,0x0E,0x09,
	0x09,0x0D,0x0B,0x0E
	};*/
	
	uchar TampState[16];
	for(int i=0;i<16;i++){
		TampState[i] = State[i];
	}
	
	printf("Le bloc \"State\" en entrée vaut : \n");
	affiche_bloc_matriciel(State);
	
	//printf ("%02X + %02X + %02X + %02X \n", gmul(0x02,TampState[0]),gmul(0x03,TampState[1]),gmul(0x01,TampState[2]),gmul(0x01,TampState[3]));
	
	State[0] = gmul(0x02,TampState[0])^gmul(0x03,TampState[1])^gmul(0x01,TampState[2])^gmul(0x01,TampState[3]);
	State[1] = gmul(0x01,TampState[0])^gmul(0x02,TampState[1])^gmul(0x03,TampState[2])^gmul(0x01,TampState[3]);
	State[2] = gmul(0x01,TampState[0])^gmul(0x01,TampState[1])^gmul(0x02,TampState[2])^gmul(0x03,TampState[3]);
	State[3] = gmul(0x03,TampState[0])^gmul(0x01,TampState[1])^gmul(0x01,TampState[2])^gmul(0x02,TampState[3]);
	
	State[4] = gmul(0x02,TampState[4])^gmul(0x03,TampState[5])^gmul(0x01,TampState[6])^gmul(0x01,TampState[7]);
	State[5] = gmul(0x01,TampState[4])^gmul(0x02,TampState[5])^gmul(0x03,TampState[6])^gmul(0x01,TampState[7]);
	State[6] = gmul(0x01,TampState[4])^gmul(0x01,TampState[5])^gmul(0x02,TampState[6])^gmul(0x03,TampState[7]);
	State[7] = gmul(0x03,TampState[4])^gmul(0x01,TampState[5])^gmul(0x01,TampState[6])^gmul(0x02,TampState[7]);
	
	State[8] = gmul(0x02,TampState[8])^gmul(0x03,TampState[9])^gmul(0x01,TampState[10])^gmul(0x01,TampState[11]);
	State[9] = gmul(0x01,TampState[8])^gmul(0x02,TampState[9])^gmul(0x03,TampState[10])^gmul(0x01,TampState[11]);
	State[10] = gmul(0x01,TampState[8])^gmul(0x01,TampState[9])^gmul(0x02,TampState[10])^gmul(0x03,TampState[11]);
	State[11] = gmul(0x03,TampState[8])^gmul(0x01,TampState[9])^gmul(0x01,TampState[10])^gmul(0x02,TampState[11]);
	
	State[12] = gmul(0x02,TampState[12])^gmul(0x03,TampState[13])^gmul(0x01,TampState[14])^gmul(0x01,TampState[15]);
	State[13] = gmul(0x01,TampState[12])^gmul(0x02,TampState[13])^gmul(0x03,TampState[14])^gmul(0x01,TampState[15]);
	State[14] = gmul(0x01,TampState[12])^gmul(0x01,TampState[13])^gmul(0x02,TampState[14])^gmul(0x03,TampState[15]);
	State[15] = gmul(0x03,TampState[12])^gmul(0x01,TampState[13])^gmul(0x01,TampState[14])^gmul(0x02,TampState[15]);
	
	printf("Le bloc \"State\" en sortie vaut : \n");
	affiche_bloc_matriciel(State);
	
	/*for(int i =0;i<16;i++){
		State[i]=State[i];
	}
	Inv_MixColumns(State);
	printf("Le bloc \"State\" en sortie vaut : \n");
	affiche_bloc_matriciel(State);*/
	
}

void AddStateRoundKey(int r){
int index = r*4*4;
	
	State[0] = State[0]^W[index];
	State[1] = State[1]^W[index+1];
	State[2] = State[2]^W[index+2];
	State[3] = State[3]^W[index+3];
	State[4] = State[4]^W[index+4];
	State[5] = State[5]^W[index+5];
	State[6] = State[6]^W[index+6];
	State[7] = State[7]^W[index+7];
	State[8] = State[8]^W[index+8];
	State[9] = State[9]^W[index+9];
	State[10] = State[10]^W[index+10];
	State[11] = State[11]^W[index+11];
	State[12] = State[12]^W[index+12];
	State[13] = State[13]^W[index+13];
	State[14] = State[14]^W[index+14];
	State[15] = State[15]^W[index+15];
	
}

void AddRoundKey(int r){
	printf("Test de AddRoundKey \n");
	
	/*uchar State[16]  = { 
	0xFF, 0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF
	};*/
	
	
	//printf("Le bloc \"State\" en entrée vaut : \n");
	//affiche_bloc_matriciel(State);
	
	int index = r*4*4;
	
	State[0] = State[0]^W[index];
	State[1] = State[1]^W[index+1];
	State[2] = State[2]^W[index+2];
	State[3] = State[3]^W[index+3];
	State[4] = State[4]^W[index+4];
	State[5] = State[5]^W[index+5];
	State[6] = State[6]^W[index+6];
	State[7] = State[7]^W[index+7];
	State[8] = State[8]^W[index+8];
	State[9] = State[9]^W[index+9];
	State[10] = State[10]^W[index+10];
	State[11] = State[11]^W[index+11];
	State[12] = State[12]^W[index+12];
	State[13] = State[13]^W[index+13];
	State[14] = State[14]^W[index+14];
	State[15] = State[15]^W[index+15];
	
	
	printf("Le bloc \"State\" en sortie vaut : \n");
	affiche_bloc_matriciel(State);
	
	/*for(int i =0;i<16;i++){
		State[i]=State[i];
	}
	AddStateRoundKey(r);
	printf("Le bloc \"State\" en sortie vaut : \n");
	affiche_bloc_matriciel(State);*/
	

}
void chiffrer(){
  int i;
  AddRoundKey(0);
  for (i = 1; i < Nr; i++) {
    SubBytes();
    ShiftRows();
    MixColumns();
    AddRoundKey(i);
  }
  SubBytes();
  ShiftRows();
  AddRoundKey(Nr);
}

void dechiffrer(){
  printf("Dechiffrage \n");
  int i;
  AddRoundKey(Nr);
  Inv_ShiftRows();
  Inv_SubBytes();
  for(i = Nr-1;i> 0;i--){
  	AddRoundKey(i);
  	Inv_MixColumns();
  	Inv_ShiftRows();
  	Inv_SubBytes();
  }
  AddRoundKey(0);
}

int main (int argc, char * argv[]) {
  /* Il faut tout d'abord initialiser la table de multiplication */
  for (int i=0 ; i<256; i++) {
    for (int j=0 ; j<256; j++) {
      Mul_F256[i][j] = gmul(i,j) ;
    }
  }
  
  /* Puis initialiser la table de dechiffrage pour Sub */
  for (int i=0; i<256;i++){
  	InvSBox[SBox[i]] = i;
  }
  
  printf("Le bloc \"State\" en entrée vaut : \n");
  
  /*uchar State[16] = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00
  } ;*/
  
  affiche_bloc_matriciel(State);
  chiffrer();
  //SubBytes();
  //ShiftRows();
  //MixColumns();
  //AddRoundKey(0);
  
  //Inv_SubBytes();
  //Inv_ShiftRows();
  //Inv_MixColumns();
  printf("!!!!!!!!!!!!!!!!!!!FIN CHIFFRAGE!!!!!!!!!!!!!!!!!\n");
  printf("Le bloc \"State\" en sortie vaut : \n");
  affiche_bloc_matriciel(State);
  printf("!!!!!!!!!!!!!!!!!!!DEBUT DECHIFFRAGE!!!!!!!!!!!!!!!!!\n");
  dechiffrer();
  printf("!!!!!!!!!!!!!!!!!!!FIN DECHIFFRAGE!!!!!!!!!!!!!!!!!\n");
  printf("Le bloc \"State\" en sortie vaut : \n");
  affiche_bloc_matriciel(State);
  
  exit(EXIT_SUCCESS);
}


/* Pour compiler:
> make
gcc aes.c -o aes -lm
> ./aes

*/


/* Quelques tests pour les fonctions à introduire 
Test de SubBytes():
Le bloc "State" en entrée vaut : 
          00 04 08 0C 
          01 05 09 0D 
          02 06 0A 0E 
          03 07 0B 0F 
Le bloc "State" en sortie vaut : 
          63 F2 30 FE 
          7C 6B 01 D7 
          77 6F 67 AB 
          7B C5 2B 76 

Test de ShiftRows():
Le bloc "State" en entrée vaut : 
          00 04 08 0C 
          01 05 09 0D 
          02 06 0A 0E 
          03 07 0B 0F 
Le bloc "State" en sortie vaut : 
          00 04 08 0C 
          05 09 0D 01 
          0A 0E 02 06 
          0F 03 07 0B 

Test de MixColumns():
Le bloc "State" en entrée vaut : 
          0E 0B 0D 09 
          09 0E 0B 0D 
          0D 09 0E 0B 
          0B 0D 09 0E 
Le bloc "State" en sortie vaut : 
          01 00 00 00 
          00 01 00 00 
          00 00 01 00 
          00 00 00 01 

Test de AddRoundKey():
La clef de ronde 1 vaut : 
          62 62 62 62 
          63 63 63 63 
          63 63 63 63 
          63 63 63 63 
Le bloc "State" en entrée vaut : 
          FF FF FF FF 
          FF FF FF FF 
          FF FF FF FF 
          FF FF FF FF 
Le bloc "State" en sortie vaut : 
          9D 9D 9D 9D 
          9C 9C 9C 9C 
          9C 9C 9C 9C 
          9C 9C 9C 9C 

Test final: chiffrement du bloc nul:
Le bloc "State" en entrée vaut : 
          00 00 00 00 
          00 00 00 00 
          00 00 00 00 
          00 00 00 00 
Le bloc "State" en sortie vaut : 
          66 EF 88 CA 
          E9 8A 4C 34 
          4B 2C FA 2B 
          D4 3B 59 2E 
*/

