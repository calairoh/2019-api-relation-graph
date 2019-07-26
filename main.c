#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define FALSE 0
#define TRUE 1
#define N 100
#define ENTITY_SPACE_DIM 100 //10 KB
#define STRING_EQUALS 0
#define COMMAND_LENGTH 6
#define DELETE_CHAR '\0'
#define NULL_CHAR '*'

void read();
void addEnt();
void addRel();
void delEnt();
void delRel();
int searchPlace(char*, char*, int, int, int);

//entity section
char entitySpace[ENTITY_SPACE_DIM + 1] = { '*', [1 ... ENTITY_SPACE_DIM] = '*' };
int eMarker = 0;

int main(int argc, char** argv){
   //setup
   //entitySpace[ENTITY_SPACE_DIM] = '\0';
   //printf("%s\n", entitySpace);
   
   read();

   return 0;
}

void read(){
   int i = 0;
   char c;
   char command[COMMAND_LENGTH];

   do{
      fgets(command, COMMAND_LENGTH + 1, stdin);  

      int hash = command[0] + command[3];

      switch(hash){
	 //addent
	 case 198:
	    addEnt();
	    break;
	 //addrel
	 case 211:
	    addRel();
	    break;
	 //delent
	 case 201:
	    delEnt();
	    break;
	 case 214:
	    delRel();
	    break;
	 default:
	    break;
      }

   }while(!strcmp(command, "end"));

   //Calcolo finale
   printf("%s\n", entitySpace);
}

void addRel(){
   char idRel[N+1], src[N+1], dst[N+1];

   scanf(" \"%[^\"]\" \"%[^\"]\" \"%[^\"]\"", idRel, src, dst);
}

void delEnt(){
   char ent[N+1];
   
   scanf(" \"%[^\"]\"", ent);

   char* init = strstr(entitySpace, ent);

   while(*init != '*'){
      *init = '*';
      *init++;
   }
}

void addEnt(){
   char c;
   int foundPlace = FALSE, equals = TRUE;

   scanf(" %c", &c);
   while(c != ' ') {
      entitySpace[eMarker] = c;
      eMarker++;
      scanf("%c", &c);
   }
   entitySpace[eMarker] = '\0';
}

void delRel(){
}
