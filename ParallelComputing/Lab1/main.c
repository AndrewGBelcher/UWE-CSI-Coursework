#include <stdio.h>
#include <omp.h>
#include <vector>
#include <stdlib.h>
#include <string.h>

using namespace std;



void process_buffer(char* buffer, size_t size) 
{
	int nums[size];
	printf("size:%d\n");
	int i = 0;
	char *token;

	//printf("%s",buffer);

	while ((token = strtok(buffer, " ")) != NULL) 
	{
	    nums[i] = atoi(token);
	    printf("num %d:%d token:%s\n",i,nums[i],buffer[i]);
	}

}

int main () 
{
  FILE * f;
  long lSize;
  char * buffer;
  size_t result;

  f = fopen ( "ints.txt" , "r" );
  if (f==NULL) {fputs ("File error",stderr); exit (1);}

  // obtain file size:
  fseek (f , 0 , SEEK_END);
  lSize = ftell (f);
  rewind (f);

  // allocate memory to contain the whole file:
  buffer = (char*) malloc (sizeof(char)*lSize);
  if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

  // copy the file into the buffer:
  result = fread (buffer,1,lSize,f);
  if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

  /* the whole file is now loaded in the memory buffer. */
  process_buffer(buffer, result);
  // terminate


  fclose (f);
  free (buffer);
  return 0;
}
