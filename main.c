#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define FALSE 0
#define TRUE 1
#define N 100
#define RESULT_ROW 100
#define ENTITY_SPACE_DIM 10000 //10 KB
#define RELATION_SPACE_DIM 10000 //10 KB
#define ENTITY_RELATION_DIM 100000 //100 KB
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
char* contains(char*, char*);
int searchPlace(char*, char*, int, int, int);

//memory section
char entitySpace[ENTITY_SPACE_DIM + 1] = { [0 ... ENTITY_SPACE_DIM] = '\0' };
char relationSpace[RELATION_SPACE_DIM + 1] = { [0 ... RELATION_SPACE_DIM] = '\0' };
int eMarker = 0;
int rMarker = 0;
int relNum = 0;


char* relations[ENTITY_RELATION_DIM][3] = { { NULL, NULL, NULL } };
int relEntNum = 0;

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
      //printf("%s\n", command);
      int hash = command[0] + command[3];

      //printf("%s\t%d\n", command, hash);
      switch(hash){
	 case 198:
	    addEnt();
	    break;
	 case 211:
	    addRel();
	    break;
	 case 201:
	    delEnt();
	    break;
	 case 214:
	    delRel();
	    break;
	 case 225:
	    report();
	    break;
	 default:
	    break;
      }
      
      command[3] = '\0';
   
   }while(strcmp(command, "end"));
   //Calcolo finale
   //printf("%s\n%s\n", entitySpace, relationSpace);
   //printf("%d\n", relEntNum);
   //printf("\n");
}

void addRel(){
   char rel[N+1], src[N+1], dst[N+1];

   scanf(" \"%[^\"]\" \"%[^\"]\" \"%[^\"]\"", src, dst, rel);

   char* srcPointer = contains(entitySpace, src);
   if(srcPointer == NULL) return;

   char* dstPointer = contains(entitySpace, dst);
   if(dstPointer == NULL) return;

   char* relPointer = contains(relationSpace, rel);

   if(relPointer == NULL){
      relPointer = &relationSpace[rMarker];
      int i;
      for(i = 0; rel[i] != '\0'; i++){
	 relationSpace[rMarker++] = rel[i];
      }
      relationSpace[rMarker++] = '\0';
      relNum++;
   }
   
   //Aggiungo alla matrice relations
   int hash = (long)relPointer % ENTITY_RELATION_DIM;
   
   //Scorro fino al primo posto libero
   for(; relations[hash][0] != NULL; hash++){
      if(relations[hash][0] == relPointer && 
	 relations[hash][1] == srcPointer &&
	 relations[hash][2] == dstPointer)
	 return;
   }

   relations[hash][0] = relPointer;
   relations[hash][1] = srcPointer;
   relations[hash][2] = dstPointer;
   relEntNum++;
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
      scanf("%c", &c);
   }
   entitySpace[eMarker++] = '\0';

   //Prendo l'ultimo carattere prima del prossimo comando
   //sia che questo sia un \n, uno spazio o qualsiasi altro separatore
   scanf("%c", &c);

   //printf("%s\n", entitySpace);
}

void delRel(){
}

void report(){
   int i, k;
   long max;
   char* result[RESULT_ROW][2] = { { NULL, NULL } };
   
   if(!relEntNum){
      printf("none\n");
      return;
   }

   /*for(i = 0; i < ENTITY_RELATION_DIM; i++){
      if(relations[i][0] != NULL)
	 printf("%s | %s | %s\n", relations[i][0], relations[i][1], relations[i][2]);
   }
   return;*/

   char* rel = relationSpace;
   int num;
   while(*rel != '\0'){
      
      for(i = 0; i < num; i++){
	 result[i][0] = NULL;
	 result[i][1] = NULL;
      }
      
      num = 0;      
      int hash = (long)(rel) % ENTITY_RELATION_DIM;

      for(; relations[hash][0] != NULL; hash++){
	 if(relations[hash][0] == rel){
	    if(*relations[hash][1] != '*'){
	       for(k = 0; result[k][0] != NULL && result[k][0] != relations[hash][2]; k++) ;
	       if(result[k][0] == NULL){
		  num++;
		  result[k][0] = relations[hash][2];
	       }
	       
	       result[k][1]++;
	    }
	 }
      }
      
      //Cerco il numero più alto (da ottimizzare)
      max = (long)result[0][1];
      for(i = 1; i < num; i++)
	 if((long)result[i][1] > max)
	    max = (long)result[i][1];

      //Stampo i risultati di questa relazione
      printf("\"%s\" ", rel);
      for(i = 0; i < RESULT_ROW; i++)
	 if((long)result[i][1] == max)
	    printf("\"%s\" ", result[i][0]);
      printf("%ld; ", max); 

      //Vado alla prossima relazione
      for(; *rel != '\0'; rel++) ;
      rel++;
   }
   printf("\n");
}

char* contains(char *str, char *obj){
   int i = 0;

   while(i < ENTITY_SPACE_DIM){
      if(!strcmp(&str[i], obj)) 
	 return &str[i];

      for(; str[i] != '\0'; i++) ;
      i++;

      if(str[i] == '\0') return NULL;
   }

   return NULL;
}
