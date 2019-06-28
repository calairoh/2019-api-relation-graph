#include<stdio.h>
#include<string.h>

#define N 100
#define STRING_EQUALS 0
#define COMMAND_LENGTH 6

void read();

int main(int argc, char** argv){
   read();

   return 0;
}

void read(){
   int i = 0;
   char c;
   char command[COMMAND_LENGTH+1];

   do{
      scanf("%s ", command);

      if(!strcmp(command, "addrel"))
	 printf("addrel\n");
      else if(!strcmp(command, "addent"))
	 printf("addent\n");

   }while(!strcmp(command, "end"));
}
