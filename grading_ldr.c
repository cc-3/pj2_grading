#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_TESTS 5
#define EXTRA_LINES 10

char *tests[] = {"LDR x0, [x0, #3]\n","LDR x18, [x15, #11]\n","STR x25, [x29, #4]\n",
                 "LDRB w10, [x10, #8]\n","STRB w4, [x0, #0]\n"};
char *answers[] = {"0xf8403000\n","0xf840b1f2\n","0xf80004b9\n","0x3940214a\n","0x39000004\n"};

char* ignore_lines(char *buffer, int lines){
   if(lines < 0){
      return NULL;
   }
   while(1){
      char c = *(buffer++);
      if(c == '\n'){
         lines--;
      }
      if(lines <= 0){
         break;
      }
   }
   return buffer;
}

void writeTest(){
   FILE *f = fopen("grading_ldr.txt","w");
   int count;
   for(count = 0; count < NUM_TESTS; count++){
      fprintf(f,"%s",tests[count]);
   }
   fclose(f);
}

char* get_line(char *buffer, char **line){
   char c;
   int i = 0;
   while(1){
      c = *(buffer++);
      if(c == '\n' || c == 0){
         line[0][i++] = '\n';
         line[0][i] = 0;
         return buffer;
      }
      line[0][i++] = c;
   }
   return buffer;
}

int main(){
   char *buffer = (char*)malloc(4000*sizeof(char));
   writeTest();
   FILE *f = popen("./ensamblador grading_ldr.txt","r");
   int i = 0;
   while(1){
      char c = fgetc(f);
      if(feof(f)){
         break;
      }
      buffer[i++]=c;
   }
   char *aux_buffer = ignore_lines(buffer,EXTRA_LINES);
   char *line = (char*)malloc(100*sizeof(char));

   i = 0;
   float total = 0.0;
   for(i = 0; i < NUM_TESTS;i++){
      aux_buffer = get_line(aux_buffer,&line);
      printf(".............................\nInst: %sObtenido: %sEsperado: %s",tests[i],line,answers[i]);
      if(!strcmp(line,answers[i])){
	      total+=100.0/NUM_TESTS;
         printf(" (+%.2f)\n",100.0/NUM_TESTS);
      }else{
         puts(" (+0.00)");
      }
   }
   printf(".............................\nTotal: %.2f/100.0\n",total);
   free(line);
   free(buffer);
   return 0;
}
