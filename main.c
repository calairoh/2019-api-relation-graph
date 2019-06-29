#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define N 100
#define STRING_EQUALS 0
#define COMMAND_LENGTH 6

void read();
//void addEnt(char*);
void addRel();

int main(int argc, char** argv){
   read();

   return 0;
}

void read(){
   int i = 0;
   char c;
   char command[COMMAND_LENGTH];

   do{
      fgets(command, COMMAND_LENGTH + 1, stdin);  

      if(!strcmp(command, "addrel"))
	 addRel();
      else if(!strcmp(command, "addent"))
	 printf("addent\n");

   }while(!strcmp(command, "end"));
}

void addRel(){
   char idRel[N+1], src[N+1], dst[N+1], c;
   int i;

   scanf(" \"%[^\"]\" \"%[^\"]\" \"%[^\"]\"", idRel, src, dst);
}
