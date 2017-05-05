#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_TESTS 4
#define EXTRA_LINES 14

char *tests[] = {"CBZ x1, label1\n","CBZ x2, label3\n",
                 "CBNZ x2, label1\n","CBNZ  x25, label2\n"};

char *answers[] = {"0xb4000021\n","0xb4000042\n",
                   "0xb5ffffe2\n","0xb5fffff9\n"};

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
   FILE *f = fopen("grading_cbz_cbnz.txt","w");
   int count;
   for(count = 0; count < NUM_TESTS; count++){
      if(count == 0){
    fprintf(f,".text\nmain:\n");
      }else if(count == 1){
         fprintf(f,"label1:\n");
      }else if(count == 2){
         fprintf(f,"label2:\n");
      }else if(count == 3){
         fprintf(f,"label3:\n");
      }
      fprintf(f,"\t%s",tests[count]);
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
   /*MODIFIQUEN ESTA LINEA DEPENDIENDO DE SUS ARCHIVOS*/
   FILE *f = popen("./ensamblador grading_cbz_cbnz.txt","r");
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
