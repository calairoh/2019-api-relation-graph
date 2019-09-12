#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define N 1000
#define RESULT_ROW 1000 
#define ENTITY_SPACE_DIM_ROW 100 
#define ENTITY_SPACE_DIM_COL 1000
#define RELATION_SPACE_DIM_ROW 100
#define RELATION_SPACE_DIM_COL 1000
#define RESULT_SPACE_ROW 1000
#define RESULT_SPACE_COL 1000  
#define ENTITY_RELATION_DIM 2000000 //6 MB
#define STRING_EQUALS 0
#define COMMAND_LENGTH 6
#define NULL_CHAR '*'
#define TRUE 1

void addEnt();
void addRel();
void delEnt();
void delRel();
void report();
void restoreRelation(char*, int);
int findEntitiesToWrite(char*, int, char**, int*);
char* contains(char[][RELATION_SPACE_DIM_COL], char*, int, int);
int hash(char*, int);
 
//memory section
char entitySpace[ENTITY_SPACE_DIM_ROW][ENTITY_SPACE_DIM_COL];
char relationSpace[RELATION_SPACE_DIM_ROW][RELATION_SPACE_DIM_COL];
char* reportSpace[RESULT_SPACE_ROW][RESULT_SPACE_COL] = { { NULL } };
char** reportPosition[RESULT_SPACE_ROW] = { NULL };
char* relationIndexes[RELATION_SPACE_DIM_COL];
int relNum = 0;

char* relations[ENTITY_RELATION_DIM][3] = { { NULL, NULL, NULL } };
int relEntNum = 0;
int rIndex = 0;

int main(int argc, char** argv){

   char c;
   char command[COMMAND_LENGTH + 1];
   char* commandPointer;

   do{
      commandPointer = fgets(command, COMMAND_LENGTH + 1, stdin);  
      int hash = command[0] + command[3];

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
   
   return 0;
}

void addRel(){
   char rel[N+1], src[N+1], dst[N+1];

   int tmp = scanf(" %s %s %s\n", src, dst, rel);

   char* srcPointer = contains(entitySpace, src, ENTITY_SPACE_DIM_ROW,
   ENTITY_SPACE_DIM_COL);
   if(srcPointer == NULL) return;

   char* dstPointer = contains(entitySpace, dst, ENTITY_SPACE_DIM_ROW,
   ENTITY_SPACE_DIM_COL);
   if(dstPointer == NULL) return;

   char* relPointer = contains(relationSpace, rel, RELATION_SPACE_DIM_ROW,
   RELATION_SPACE_DIM_COL);

   if(relPointer == NULL){
      int i;
      int finish = 0;
      int row = hash(rel, RELATION_SPACE_DIM_ROW);
      while(!finish){
	 char* str = relationSpace[row];
	 for(i = 0; str[i] != '\0' && str[i + 1] != '\0'; i = (i + 1) %
	    RELATION_SPACE_DIM_COL) ;
      
	 if((RELATION_SPACE_DIM_COL - i) < (tmp + 1)){
	    row++;
	 }
	 else{
	    if(i > 0) i+=2;

	    strcpy(&str[i], rel);
      
	    relPointer = &str[i];
	    relationIndexes[rIndex] = relPointer;
	    rIndex++;
	    finish = 1;
	 }
      }
   }
   
   //Aggiungo alla matrice relations
   int hash = (long)relPointer % ENTITY_RELATION_DIM;
   
   //Scorro fino al primo posto libero
   for(; relations[hash][0] != NULL; hash = (hash + 1) % ENTITY_RELATION_DIM){
      if(relations[hash][0] == relPointer && 
	 relations[hash][1] == srcPointer &&
	 relations[hash][2] == dstPointer)
	 return;
   }

   relations[hash][0] = relPointer;
   relations[hash][1] = srcPointer;
   relations[hash][2] = dstPointer;
   relEntNum++;

   //report part

   //search relation
   int found = 0, foundDst = 0, i;
   for(i = 0; !found && reportSpace[i][1] != NULL && i < RESULT_SPACE_ROW; i++)
      if(!strcmp(reportSpace[i][1], relPointer))
	 found = 1;
	 
   if(found){
      //search the dst
      i--;
      int j;
      for(j = 2; !foundDst && reportSpace[i][j] != NULL && j < RESULT_SPACE_COL; j++)
	 if(!strcmp(reportSpace[i][j], dstPointer))
	    foundDst = 1;

      if(foundDst){
	 reportSpace[i][2] = dstPointer;
	 reportSpace[i][0]++;

	 for(j = 3; reportSpace[i][j] != NULL && j < RESULT_SPACE_COL; j++)
	    reportSpace[i][j] = NULL;
      } else {
	 int count = 0;

	 hash = (long)relPointer % ENTITY_RELATION_DIM;
	 for(; relations[hash][0] != NULL; hash = (hash + 1) % ENTITY_RELATION_DIM)
	    if(relations[hash][2] == dstPointer &&
	       *relations[hash][1] != '*' && 
	       relations[hash][0] == relPointer)
	    count++;

	 if(count == (long)reportSpace[i][0]){
	    //Cerco un posto dove metterlo
	    int foundPlace = 0, k = 2;
	    while(!foundPlace && reportSpace[i][k] != NULL && k < RESULT_SPACE_COL){
	       if(strcmp(reportSpace[i][k], dstPointer) > 0)
		  foundPlace = 1;
	       k++;
	    }
	    
	    if(foundPlace)
	       k--;

	    char* tmp1;
	    char* tmp2 = dstPointer;
	    do{
	       tmp1 = reportSpace[i][k];
	       reportSpace[i][k] = tmp2;
	       tmp2 = tmp1;
	       k++;
	    }
	    while(tmp1 != NULL && k < RESULT_SPACE_COL);	    
	 }
      }
   } else {
      //Se non ho trovato la relazione, la aggiungo
      reportSpace[i][0] = (char*)1;
      reportSpace[i][1] = relPointer;
      reportSpace[i][2] = dstPointer;

      //Cerco un posto nell'ordine delle relazioni
      int j, foundPlace = 0;
      for(j = 0; !foundPlace && reportPosition[j] != NULL && j < RESULT_SPACE_ROW; j++)
	 if(strcmp(reportPosition[j][1], relPointer) > 0)
	       foundPlace = 1;

      if(foundPlace) 
	 j--;

      char** tmp1;
      char** tmp2 = reportSpace[i];
      do{
	 tmp1 = reportPosition[j];
	 reportPosition[j] = tmp2;
	 tmp2 = tmp1;
	 j++;
      } while(tmp1 != NULL && j < RESULT_SPACE_ROW);
   }
}

void delEnt(){
   char ent[N+1];
   
   int tmp = scanf(" %s\n", ent);

   char* init = contains(entitySpace, ent, ENTITY_SPACE_DIM_ROW, ENTITY_SPACE_DIM_COL);
   char* word = init;
   
   if(init != NULL){   
      while(*init != '\0'){
	 *init = '*';
	 *init++;
      }
   
      //Controllo nel report
      int i;
      for(i = 0; reportSpace[i][1] != NULL && i < RESULT_SPACE_ROW; i++){
	 int hash = (long)reportSpace[i][1] % ENTITY_RELATION_DIM;

	 for(; relations[hash][0] != NULL; hash++){
	    if(relations[hash][1] == word){
	       //Se è sorgente di qualcosa vado a vedere nel report se c'è la sua
	       //destinazione e nel caso lo tolgo
	       int j, k, found = 0;
	       for(j = 0; reportSpace[j][1] != NULL; j++){
		  for(k = 2; !found && reportSpace[j][k] != NULL; k++){
		     if(relations[hash][2] == reportSpace[j][k]){
			found = 1;
			for(; reportSpace[j][k] != NULL; k++)
			   reportSpace[j][k] = reportSpace[j][k + 1];
		     }
		  }
	       }
	    }
	 }

	 int s = 0;
      }

      for(i = 0; reportSpace[i][1] != NULL; i++){
	 int j;
	 for(j = 0; reportSpace[i][j] != NULL; j++)
	    if(reportSpace[i][j] == word)
	       for(; reportSpace[i][j] != NULL; j++)
		  reportSpace[i][j] = reportSpace[i][j + 1];

	 if(reportSpace[i][2] == NULL)
	    restoreRelation(reportSpace[i][1], i);
      }
   }
}

void addEnt(){
   char ent[N+1];
   int i;
   int finish = 0;
   
   int tmp = scanf(" %s\n", ent);

   char* init = contains(entitySpace, ent, ENTITY_SPACE_DIM_ROW,
   ENTITY_SPACE_DIM_COL);

   if(init == NULL){
      int row = hash(ent, ENTITY_SPACE_DIM_ROW);
      while(!finish){
	 char* str = entitySpace[row];
	 for(i = 0; str[i] != '\0' || str[i + 1] != '\0'; i = (i + 1) %
	 (ENTITY_SPACE_DIM_COL - 1)) ;
	 
	 if((ENTITY_SPACE_DIM_COL - 1) < tmp){
	    row = (row + 1) % ENTITY_SPACE_DIM_ROW;
	 }
	 else {
	    if(i > 0) i++;
	    strcpy(&str[i], ent);
	    finish = 1;
	 }
      }
   }
}

void delRel(){
   char rel[N+1], src[N+1], dst[N+1];

   int tmp = scanf(" %s %s %s\n", src, dst, rel);

   char* srcPointer = contains(entitySpace, src, ENTITY_SPACE_DIM_ROW,
   ENTITY_SPACE_DIM_COL);
   if(srcPointer == NULL) return;

   char* dstPointer = contains(entitySpace, dst, ENTITY_SPACE_DIM_ROW,
   ENTITY_SPACE_DIM_COL);
   if(dstPointer == NULL) return;

   char* relPointer = contains(relationSpace, rel, RELATION_SPACE_DIM_ROW,
   RELATION_SPACE_DIM_COL);
   if(relPointer == NULL) return;
   
   //Aggiungo alla matrice relations
   int hash = (long)relPointer % ENTITY_RELATION_DIM;
   int foundRel = 0; 
   //Scorro fino al primo posto libero
   for(; relations[hash][0] != NULL; hash = (hash + 1) % ENTITY_RELATION_DIM){
      if(relations[hash][0] == relPointer && 
	 relations[hash][1] == srcPointer &&
	 relations[hash][2] == dstPointer){
	 for(; relations[hash][0] != NULL; hash++){
	    relations[hash][0] = relations[hash + 1][0];
	    relations[hash][1] = relations[hash + 1][1];
	    relations[hash][2] = relations[hash + 1][2];
	 }
	 foundRel = 1;
      }
   }

   //Metto a posto il report
   if(foundRel){
      int found = 0, foundDst = 0, i, j;
      for(i = 0; !found && reportSpace[i][1] != NULL && i < RESULT_SPACE_ROW; i++)
	 if(!strcmp(reportSpace[i][1], relPointer))
	    found = 1;
   
      if(found){
	 i--;
	 //Controllo se esiste il dstPointer
	 for(j = 2; !foundDst && reportSpace[i][j] != NULL && j < RESULT_SPACE_COL; j++)
	    if(!strcmp(reportSpace[i][j], dstPointer))
	       foundDst = 1;
   
	 if(foundDst){
	    for(j = j - 1; reportSpace[i][j] != NULL && j < RESULT_SPACE_COL - 1; j++){
	       reportSpace[i][j] = reportSpace[i][j + 1];
	    }

	    //Controllo se esiste un altro destinatario
	    if(reportSpace[i][2] == NULL){
	       restoreRelation(reportSpace[i][1], i);
	    }
	 }
      }
   }
}

void restoreRelation(char* relPointer, int i){
   char* entities[RESULT_ROW] = { NULL };
   int indexes[RESULT_ROW] = { [ 0 ... RESULT_ROW - 1 ] = -1 };
   int k, j;
	    
   int hash = (long)relPointer % ENTITY_RELATION_DIM;
   long max = findEntitiesToWrite(relPointer, hash, entities, indexes);
   for(k = 0; indexes[k] != -1; k++){
      int index = indexes[k];
      int foundPlace = 0;
      for(j = 2; !foundPlace && reportSpace[i][j] != NULL; j++)
        if(strcmp(reportSpace[i][j], entities[index]) > 0)
           foundPlace = 1;
      
      if(foundPlace)
	 j--;   
	        
      char* tmp1;
      char* tmp2 = entities[index];
      do{
	 tmp1 = reportSpace[i][j];
	 reportSpace[i][j] = tmp2;
	 tmp2 = tmp1;
	 j++;
      }
      while(tmp1 != NULL && k < RESULT_SPACE_COL);
   }

   if(k == 0){
      reportSpace[i][0] = NULL;
      //reportSpace[i][1] = NULL;
      
      /*int i;
      for(i = 0; reportPosition[i] != NULL; i++)
	 if(reportPosition[i] == reportSpace[i])
	    for(; reportPosition[i] != NULL; i++)
	       reportPosition[i] = reportPosition[i + 1];*/
   } else {
      reportSpace[i][0] = (char*)max;
   }
}

int findEntitiesToWrite(char* relPointer, int hash, char** entities, int* indexes){
   int entitiesMax[RESULT_ROW] = { 0 };
   int i;

   for(; relations[hash][0] != NULL; hash++){
      if(relations[hash][0] == relPointer){
	 if(*relations[hash][1] != '*' && *relations[hash][2] != '*'){
	    int found = 0;
	    for(i = 0; !found && entities[i] != NULL && i < RESULT_ROW; i++)
	       if(entities[i] == relations[hash][2]){
		  entitiesMax[i]++;
		  found = 1;
	       }

	    if(!found){
	       entities[i] = relations[hash][2];
	       entitiesMax[i]++;
	    }
	 }
      }
   }

   int max = 0, k = 0; 
   for(i = 0; entitiesMax[i] > 0 && i < RESULT_ROW; i++)
      if(entitiesMax[i] > max)
	 max = entitiesMax[i];

   for(i = 0; entitiesMax[i] > 0 && i < RESULT_ROW; i++)
      if(entitiesMax[i] == max)
	 indexes[k++] = i;
	 
   return max;   
}

void report(){
   int i, k;   

   //Pulisco dopo il report
   int tmp = scanf("\n");

   if(!relEntNum){
      printf("none\n");
      return;
   }

   for(i = 0; reportPosition[i] != NULL && i < RESULT_SPACE_ROW; i++){
      if(reportPosition[i][0] != NULL){
	 for(k = 1; reportPosition[i][k] != NULL && k < RESULT_SPACE_COL; k++) 
	    if(*reportPosition[i][k] != '*')
	       printf("%s ", reportPosition[i][k]);

	 printf("%ld; ", (long)reportPosition[i][0]);
      }
   }
   printf("\n");
}

char* contains(char matrix[][RELATION_SPACE_DIM_COL], char *obj, int rowDim, int colDim){
   int row = hash(obj, rowDim);
   int len = strlen(obj);

   char* str = matrix[row];

   int i = 0;
   if(str[i] == '\0')
      return NULL;

   while(TRUE){
      i = 0;
      while(i < colDim){
	 if(!strcmp(&str[i], obj)) 
	    return &str[i];

	 for(; str[i] != '\0'; i++) ;
	 i++;

	 if(str[i] == '\0')
	    if((colDim - i) < (len + 1))
	       break;
	    else
	       return NULL;
      }

      row++;
   }

   return NULL;
}

int hash(char *str, int mod){
   return (str[1] * str[2] * strlen(str)) % mod;
}
