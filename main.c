#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define FALSE 0
#define TRUE 1
#define N 100
#define RESULT_ROW 100
#define ENTITY_SPACE_DIM 10000 //10 KB
#define RELATION_SPACE_DIM 10000 //10 KB
#define ENTITY_RELATION_DIM 1000000 //100 KB
#define STRING_EQUALS 0
#define COMMAND_LENGTH 6
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
   char c;
   char command[COMMAND_LENGTH + 1];
   char* commandPointer;

   do{
      commandPointer = fgets(command, COMMAND_LENGTH + 1, stdin);  
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
}

void addRel(){
   char rel[N+1], src[N+1], dst[N+1], c;

   int tmp = scanf(" %s %s %s\n", src, dst, rel);

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
   
   int tmp = scanf(" %s\n", ent);

   char* init = contains(entitySpace, ent);
   
   if(init != NULL){   
      while(*init != '\0'){
	 *init = '*';
	 *init++;
      }
   }
}

void addEnt(){
   char ent[N+1];
   int i;
   
   int tmp = scanf(" %s\n", ent);

   char* init = contains(entitySpace, ent);
   
   if(init == NULL){
      for(i = 0; ent[i] != '\0'; i++)
	 entitySpace[eMarker++] = ent[i];
      eMarker++;
   }
}

void delRel(){
   char rel[N+1], src[N+1], dst[N+1];

   int tmp = scanf(" %s %s %s\n", src, dst, rel);

   char* srcPointer = contains(entitySpace, src);
   if(srcPointer == NULL) return;

   char* dstPointer = contains(entitySpace, dst);
   if(dstPointer == NULL) return;

   char* relPointer = contains(relationSpace, rel);
   if(relPointer == NULL) return;
   
   //Aggiungo alla matrice relations
   int hash = (long)relPointer % ENTITY_RELATION_DIM;
   
   //Scorro fino al primo posto libero
   for(; relations[hash][0] != NULL; hash++){
      if(relations[hash][0] == relPointer && 
	 relations[hash][1] == srcPointer &&
	 relations[hash][2] == dstPointer){
	 for(; relations[hash][0] != NULL; hash++){
	    relations[hash][0] = relations[hash + 1][0];
	    relations[hash][1] = relations[hash + 1][1];
	    relations[hash][2] = relations[hash + 1][2];
	 }
      }
   }
}

void report(){
   int i, k, nMarker = 0, counter = 0, entityCounter = 0, toWriteNum = 0, num;
   long max;
   char* result[RESULT_ROW][2] = { { NULL, NULL } };
   char* toWrite[RESULT_ROW * 10] = { NULL };
   char* entitiesToWrite[RESULT_ROW] = { NULL };
   char numbers[RESULT_ROW] = { '\0' };
   char semiColon = ';';
   char c;

   //Pulisco dopo il report
   int tmp = scanf("\n");

   if(!relEntNum){
      printf("none\n");
      return;
   }

   char* rel = relationSpace;
   while(*rel != '\0'){      

      for(i = 0; i < num; i++){
	 result[i][0] = NULL;
	 result[i][1] = NULL;
      }
      
      num = 0;      
      int hash = (long)(rel) % ENTITY_RELATION_DIM;

      for(; relations[hash][0] != NULL; hash++){
	 if(relations[hash][0] == rel){
	    if(*relations[hash][1] != '*' && *relations[hash][2] != '*'){
	       for(k = 0; result[k][0] != NULL && result[k][0] != relations[hash][2]; k++) ;
	       if(result[k][0] == NULL){
		  num++;
		  result[k][0] = relations[hash][2];
	       }
	       
	       result[k][1]++;
	    }
	 }
      }
      
      if(result[0][0] != NULL){	
	 
	 //Cerco le entità con il massimo che andranno poi stampate
	 max = 0;
	 for(i = 0; i < num; i++) {
	    if((long)result[i][1] > max){
	       max = (long)result[i][1];
	       entitiesToWrite[0] = result[i][0];
	       entityCounter = 1;
	       for(k = 1; entitiesToWrite[k] != NULL; k++) 
		  entitiesToWrite[k] = NULL;
	    } else if((long)result[i][1] == max){
	       k = 0;
	       
	       while(entitiesToWrite[k] != NULL && strcmp(entitiesToWrite[k], result[i][0]) < 0) 
		  k++;
	       
	       int j;
	       for(j = entityCounter; j > k; j--)
		  entitiesToWrite[j] = entitiesToWrite[j-1];
	       entitiesToWrite[j] = result[i][0];
	       entityCounter++;
	    }
	 }

	 //Cerco un posto dove mettere la relazione
	 if(!counter){
	    //Inserisco al primo posto
	    toWrite[0] = rel;
	    counter++;
	    k = 0;
	 } else {
	    i = 0;
	    while(toWrite[i] != NULL && strcmp(toWrite[i], rel) < 0){
	       while(toWrite[i] != NULL && *(toWrite[i]) == '"') 
		  i++;
	       //Se non è NULL sono su un numero, dunque controllo il prossimo
	       if(toWrite[i] != NULL)
		  i++;
	    }
	    
	    if(toWrite[i] != NULL){
	       for(k = toWriteNum + entityCounter + 1; k > i; k--)
		  toWrite[k] = toWrite[k - entityCounter - 2];
	       toWrite[k] = rel;
	    } else {
	       k = i;
	       toWrite[k] = rel;
	    }
	 } 

	 for(i = 0; i < entityCounter; i++){
	    toWrite[k + i + 1] = entitiesToWrite[i];
	 }
	 toWrite[k + entityCounter + 1] = &numbers[nMarker];
	 toWriteNum += entityCounter + 2;

	 int maxTmp = max, tmp = 0;
	 while(maxTmp > 0){
	    maxTmp /= 10;
	    tmp++;
	 }

	 numbers[nMarker + tmp] = semiColon;
	 int nMarkerNext = nMarker + tmp + 2;
	 tmp--; 
	 while(max > 0){
	    numbers[nMarker + tmp] = (char)((max % 10) + '0');
	    max /= 10;
	    tmp--;
	 }
	 nMarker = nMarkerNext;
      }
      
      //Vado alla prossima relazione
      for(; *rel != '\0'; rel++) ;
      rel++;
   
   }

   for(i = 0; toWrite[i] != NULL; i++)
      printf("%s ", toWrite[i]);
   printf("\n");
}

char* contains(char *str, char *obj){
   int i = 0;

   while(i < ENTITY_SPACE_DIM){
      if(!strcmp(&str[i], obj)) 
	 return &str[i];

      for(; str[i] != '\0'; i++) ;
      i++;

      if(str[i] == '\0') 
	 return NULL;
   }

   return NULL;
}
