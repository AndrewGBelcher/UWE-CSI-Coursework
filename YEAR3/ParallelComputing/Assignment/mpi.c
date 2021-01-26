/************************************************************************************
*				Parallel AES-CBC-128 Weak Key Brute forcer
*			(position 0-3, 4 bytes targeted, adjust to your needs)
*								 MPI Version
*							Author: Andrew Belcher
*************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>
#include<mpi.h>

#include "aes.h"

#define uint128_t __uint128_t

// AES Cipher Block Chain Decryption wrapper
void aes128cbc(uint8_t *key, uint8_t *iv_in, uint8_t *in, uint64_t len, uint8_t *out)
{
	AES_KEY k;
	uint32_t i;
	uint8_t tmp[16];
	uint8_t iv[16];

	memcpy(iv, iv_in, 16);
	memset(&k, 0, sizeof k);
	AES_set_decrypt_key(key, 128, &k);

	while (len > 0) {
		memcpy(tmp, in, 16);
		AES_decrypt(in, out, &k);

		for (i = 0; i < 16; i++)
			out[i] ^= iv[i];

		memcpy(iv, tmp, 16);

		out += 16;
		in += 16;
		len -= 16;

	}
}

// AES Cipher Block Chain Encryption wrapper
void aes128cbc_enc(uint8_t *key, uint8_t *iv, uint8_t *in, uint64_t len, uint8_t *out)
{
	AES_KEY k;
	uint32_t i;
	uint8_t tmp[16];

	memcpy(tmp, iv, 16);
	memset(&k, 0, sizeof k);
	AES_set_encrypt_key(key, 128, &k);

	while (len > 0) {
		for (i = 0; i < 16; i++)
			tmp[i] ^= *in++;

		AES_encrypt(tmp, out, &k);
		memcpy(tmp, out, 16);

		out += 16;
		len -= 16;
	}
} 

// A nice hexdump function taking from pieces of code here and there
int hexDump(const void *data, size_t size)
{
	unsigned char *d = (unsigned char *)data;
	size_t consoleSize = 16;
	char b[consoleSize + 3];
	size_t i;

	if(data == NULL){
		return -1;
		}
	b[0] = '|';
	b[consoleSize + 1] = '|';
	b[consoleSize + 2] = '\0';
	
	printf("\n-------HEX DUMP------\n");
	for (i = 0; i < size; i++)
	{
		if ((i % consoleSize) == 0)
		{
			if (i != 0){
				printf("  %s\n", b);
				}
			printf("%016lx ", (unsigned char *)data + i);
		}

		if(i % consoleSize == 8)
			printf(" ");
		printf(" %02x", d[i]);

		if (d[i] >= ' ' && d[i] <= '~')
			b[i % consoleSize + 1] = d[i];
		else
			b[i % consoleSize + 1] = '.';
	}
	while((i % consoleSize) != 0)
	{
		if(i % consoleSize == 8)
			printf("    ");
		else
			printf("   ");
		b[i % consoleSize + 1] = '.';
		i++;
	}
	printf("  %s\n", b);
	return 0;
}

// display a help message on command prompt
void printHelp(void)
{
			printf("Please enter both options with sensible values:\n\t\t-k <number of bytes to bruteforce in key>\n");
			printf("\n\ni.e. mpibrute -k 3\n");
			printf("\nThe key bytes count should be from 1-8 for the machine to be of any use\n");
}


int main(int argc, char *argv[])
{

	volatile bool bk = 0; // flow control aka break functionality shared across threads
	uint64_t m_index;
	int num_of_bytes_missing;

	// Crypto materials
	uint8_t key[0x10], iv[0x10] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	uint8_t ct[0x10];
	uint8_t pt[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // something easy to detect when key is found during decryption

	// Parallel work load variables
	uint64_t byte_upper = 0x100;


	// read in and parse command line arguments
	for(int i = 0; i < argc; i++)
	{
		// number of bytes missing argument
		if(!strcmp(argv[i], "-k") && i != argc-1)
			num_of_bytes_missing = atoi(argv[i+1]);		

		// print help message
		if(!strcmp(argv[i],"-h"))
		{
			printHelp();
			exit(0);
		}
	}


	// evaluation of command line argument data
	if(num_of_bytes_missing > 8)
	{
		printf("Oh hello mr NSA!\nLet me just super cool the quantum cluster computer for you first...\n");
		exit(1);
	}

	// invalids
	if(num_of_bytes_missing == 0)
	{
		printHelp();
		exit(3);
	}


	// Initialize the MPI environment
	int world_size, world_rank;
	MPI_Init(NULL, NULL);
	MPI_Status status; 
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	// divide the workload
	uint64_t work_div_value = (byte_upper << (uint64_t)((num_of_bytes_missing-1)*8))/world_size; 

	// 1st process sets up variables for all the others
	if(world_rank == 0)
	{

		// Setup crypto material and generate cipher text to run against
		srand(time(0));

		for(int i=0; i < 16; i++)
			key[i] = random()%256;// randomize key to guess

		memset(iv,0,16);

		aes128cbc_enc(key, iv, pt, 16, ct); // encrypt plaintext to ciphertext for generated random key


		// Random Key to bruteforce
		printf("\nkey to guess:");
		hexDump(&key,16);

		// send new ciphertext to all other worlds
		for(int i = 1; i < world_size; i++)
			MPI_Send(ct, 16, MPI_CHAR, i, 0, MPI_COMM_WORLD);
		
		// send randomly selected key to all other worlds
		for(int i = 1; i < world_size; i++)
			MPI_Send(key, 16, MPI_CHAR, i, 0, MPI_COMM_WORLD);

		printf("num of processes used:%d\n",world_size);
		printf("work divider value is 0x%016llx\n",work_div_value);
		printf("Dividing the workload...\n");
	}

	// keep other processes from progressing until the 1st one has setup the variables
	MPI_Barrier(MPI_COMM_WORLD);


	// determine each index according to process number
	m_index = world_rank * work_div_value;

	// pull in generated data
	if(world_rank > 0)
	{
		MPI_Recv(ct, 16, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
		MPI_Recv(key, 16, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
	}

	// Separate buffer for decryption output
	uint8_t* out = malloc(16);
	memcpy(out,ct,16);

	// Load key bytes to guess 
	uint8_t key_guess[0x10];
	memcpy(key_guess,key,16);

	// List how workload is divided
	printf("process:%d index:0x%016llx\n",world_rank,m_index);

	// wait till all processes now have everything before attempting to crack the key
	MPI_Barrier(MPI_COMM_WORLD);

	// no need for control variable cause we are using an abort signal
	while(1)
	{

		// Pull bytes from iterating index
		for(int i = 0; i < num_of_bytes_missing; i++)
		{
			key_guess[i] = (uint8_t)(m_index >> i*8)&0xff; 
		}

		// Decrypt ciphertext
		aes128cbc (key_guess, iv, ct, 16, out);


		// Check if plaintext is 0s where we have bruteforced the correct symmetric key
		if(!memcmp(out, pt, 16))
		{
			uint8_t working_key[0x10];

			memcpy(working_key,key_guess,16);

			// print the key to the command prompt now that its found
			printf("working key! solved on node:%d\n",world_rank);
			hexDump(working_key, 16);

			// terminate the process thus signaling all others to stop
			MPI_Abort(MPI_COMM_WORLD, 1);
		}
		
		// keep moving through the search space
		m_index++;
			
	}
	
	MPI_Finalize(); //MPI cleanup

  return 0;
}
