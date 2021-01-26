#include <stdio.h>
#include <omp.h>
#include <vector>
#include <stdlib.h>
#include <string.h>

// find highest number in file by parallel processing parsed data

using namespace std;



void process_buffer(char* buffer, size_t size) 
{

	int high;
	int nums[size];
	int i = 0;
	char *token;
	char tmp[50];
	int var;
	int numcnt;
  int th_id, nthreads;

	memset(nums,0,size);

	token = strtok (buffer, " ");
	while (token != NULL)
	{
		sscanf (token, "%d ", &var);
		nums[i] = var;
		
		i++;
		numcnt++;
		token = strtok (NULL, " ");
	}

	int d;
	int numchk;

	#pragma omp parallel private(th_id)
	{
    th_id = omp_get_thread_num();
    #pragma omp parallel for private(d,numchk) shared(nums, high) schedule(static,1)
		for(d = 0; d < numcnt; d++)
		{

			#pragma omp critical 
			{
				numchk = nums[d];

				if(numchk > high)
					high = numchk;
			}
		}
		printf("high: %d from thread %d\n", high, th_id);
		#pragma omp barrier    //    <----------- master waits until all threads finish before printing 
		if(th_id == 0)
		{
		  nthreads = omp_get_num_threads();
		  printf("There are %d threads\n",nthreads);
		}

   	}

printf("high is:%d\n",high);
	


}

int main () 
{
  FILE * f;
  long lSize;
  char * buffer;
  size_t result;

  f = fopen ( "ints.txt" , "r" );
  if (f==NULL) 
  {
	fputs ("File error",stderr); 
	exit (1);
	}

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
