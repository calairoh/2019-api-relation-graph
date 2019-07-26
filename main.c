#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define FALSE 0
#define TRUE 1
#define N 100
#define ENTITY_SPACE_DIM 10000 //10 KB
#define ENTITY_RELATION_DIM 10000 //10 KB
#define STRING_EQUALS 0
#define COMMAND_LENGTH 6
#define DELETE_CHAR '\0'
#define NULL_CHAR '*'

void read();
void addEnt();
void addRel();
void delEnt();
void delRel();
void report();
int searchPlace(char*, char*, int, int, int);

//memory section
char entitySpace[ENTITY_SPACE_DIM + 1] = { [0 ... ENTITY_SPACE_DIM] = '\0' };
char relationSpace[ENTITY_RELATION_DIM + 1] = { [0 ... ENTITY_RELATION_DIM] = '\0' };
int eMarker = 0;
int rMarker = 0;

int main(int argc, char** argv){
   //setup
   //entitySpace[ENTITY_SPACE_DIM] = '\0';
   //printf("%s\n", entitySpace);
   //printf("start\n");
   read();
   //printf("end\n");
   return 0;
}

void read(){
   int i = 0;
   char c;
   char command[COMMAND_LENGTH];

   do{
      fgets(command, COMMAND_LENGTH + 1, stdin);  
      printf("%s\n", command);
      int hash = command[0] + command[3];

      //printf("%d\n", hash);
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
	 case 226:
	    report();
	    break;
	 default:
	    break;
      }
      command[3] = '\0';
   }while(strcmp(command, "end"));
   //Calcolo finale
   printf("%s\n%s\n", entitySpace, relationSpace);
}

void addRel(){
   char rel[N+1], src[N+1], dst[N+1];

   scanf(" \"%[^\"]\" \"%[^\"]\" \"%[^\"]\"", src, dst, rel);

   char* srcPointer = strstr(entitySpace, src);
   if(srcPointer == NULL) return;

   char* dstPointer = strstr(entitySpace, dst);
   if(dstPointer == NULL) return;

   char* relPointer = strstr(relationSpace, rel);

   if(relPointer == NULL){
      int i;
      for(i = 0; rel[i] != '\0'; i++){
	 relationSpace[rMarker++] = rel[i];
      }
      relationSpace[rMarker++] = '|';
   }

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

   scanf(" \"%c", &c);
   while(c != '"') {
      entitySpace[eMarker++] = c;
      //eMarker++;
      scanf("%c", &c);
   }
   entitySpace[eMarker++] = '|';

   //Prendo l'ultimo carattere prima del prossimo comando
   //sia che questo sia un \n, uno spazio o qualsiasi altro separatore
   scanf("%c", &c);
   
   //eMarker++;

   //printf("%s\n", entitySpace);
}

void delRel(){
}

void report(){
}
