#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_TESTS 10

char *tests[] = {"B.EQ label1\n","B.NE label2\n","B.HS label1\n",
                 "B.LO label2\n","B.MI my_beautiful_long_label___\n","B.VS label1\n",
                 "B.GE label2\n","B.LT label1\n","B.LE my_beautiful_long_label___\n",
                 "B.GT label2\n"};
char *answers[] = {"0x54000040\n","0x54000081\n","0x54000002\n","0x54000043\n",
                   "0x54000084\n","0x54ffffa6\n","0x54ffffea\n","0x54ffff6b\n",
                   "0x5400000d\n","0x54ffff8c\n"};

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
   FILE *f = fopen("grading_b.txt","w");
   int count;
   for(count = 0; count < NUM_TESTS; count++){
      if(count == 0){
	 fprintf(f,".data\n\tmensaje: .asciz \"Hola Mundo...\"\n.text\nmain:\n");
      }else if(count == 2){
         fprintf(f,"label1:\n");
      }else if(count == 5){
         fprintf(f,"label2:\n");
      }else if(count == 8){
         fprintf(f,"my_beautiful_long_label___:\n");
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
   FILE *f = popen("./ensamblador grading_b.txt","r");
   int i = 0;
   while(1){
      char c = fgetc(f);
      if(feof(f)){
         break;
      }
      buffer[i++]=c;
   }
   char *aux_buffer = ignore_lines(buffer,22);
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
