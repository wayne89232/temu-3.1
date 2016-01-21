#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int fname2sector(char* filename){
  char buf[30];  
  FILE *fp;

  // bash ./fname2sector.sh filename
  char* bash = "bash ../../fname2sector_singl.sh ";  
  char* file = filename;

  char *s = malloc(strlen(bash)+strlen(file)+1);
  strcpy(s, bash);
  strcat(s, file);

  if ((fp = popen(s, "r")) == NULL) {  
      printf("popen() error!\n");  
      exit(1);  
  }   
  // run the shell script with popen
  fgets(buf, sizeof buf, fp);
  // fgets the first sector info
  //printf("%s", buf);
  pclose(fp);  
  free(s);
  int sector = atoi(buf);
  return sector; 
}

int main(int argc, char *argv[]) {  

  int sector = fname2sector(argv[1]);
  printf("%d\n", sector);
  return 0;  
} 
