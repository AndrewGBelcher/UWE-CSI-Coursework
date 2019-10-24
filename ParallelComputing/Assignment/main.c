/************************************************************************************
* Parallel AES-CBC-128 Weak Key Brute forcer
* (note) position 0-3, 4 bytes targeted, adjust to your needs
*
* Author: Andrew Belcher
*************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>

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

void printHelp(void)
{
	printf("Please enter both options with sensible values:\n\t\t-t <thread number>\n\t\t-k <number of bytes to bruteforce in key>\n");
	printf("\n\ni.e. brute -t 3 -k 3\n");
	printf("\nThe key bytes count should be from 1-5\nAnd the thread count should be from 1-max number of threads for the machine\n");
}

int main(int argc, char *argv[])
{
	volatile bool bk = 0; // flow control aka break functionality shared across threads

	int num_of_bytes_missing = 0;
	int num_threads = 0;
	uint8_t replace_bytes[16];

	for(int i = 0; i < argc; i++)
	{
		if(!strcmp(argv[i], "-t") && i != argc-1)
			num_threads = atoi(argv[i+1]);

		if(!strcmp(argv[i], "-k") && i != argc-1)
			num_of_bytes_missing = atoi(argv[i+1]);		

		if(!strcmp(argv[i], "-b") && i != argc-1)
		{
			for (int p = 0; p < (strlen(argv[i+1]) / 2); p++) {
				sscanf(argv[i+1] + 2*p, "%02x", &replace_bytes[p]);
			}			
		}


		if(!strcmp(argv[i],"-h"))
		{
			printHelp();
			exit(0);
		}
	}


	if(num_of_bytes_missing > 5)
	{
		printf("Oh hello mr NSA!\nLet me just super cool the quantum cluster computer for you first...\n");
		exit(1);
	}
	/*
	if(num_threads > omp_get_max_threads()+20)
	{
		printf("You trying to freeze your PC bro?\n");
		exit(2);
	}*/

	if(num_threads == 0 || num_of_bytes_missing == 0)
	{
		printHelp();
		exit(3);
	}

	// Crypto materials
	uint8_t key[0x10], iv[0x10];
	uint8_t ct[0x10];
	uint8_t pt[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // something easy to detect when key is found during decryption


	// Parallel work load variables
	uint64_t index = 0;
	uint64_t index_list[num_threads];
	uint64_t byte_upper = 0x100;
	uint64_t work_div_value = (byte_upper << (uint64_t)((num_of_bytes_missing-1)*8))/num_threads; 

	printf("work divider value is 0x%016llx\n",work_div_value);

	index_list[0] = 0;

	for(int i = 1; i < num_threads; i++)
		index_list[i] = work_div_value + index_list[i-1];

	printf("max num of threads:%d\n",omp_get_max_threads());
	printf("num of threads used:%d\n",num_threads);


	// Setup crypto material and generate cipher text to run against
	srand(time(0));

	for(int i=0; i < 16; i++)
		key[i] = replace_bytes[i];//random()%256;// randomize key to guess

	memset(iv,0,16);

	aes128cbc_enc(key, iv, pt, 16, ct); // encrypt ciphertext


	// Random Key to bruteforce
	printf("\nkey to guess:\n");
	hexDump(&key,16);

	printf("\nDividing the workload...\n");


	// Loops in parallel
	#pragma omp parallel shared(bk,key,ct) private(index) num_threads(num_threads)
	{
		int cur_thread = omp_get_thread_num();

		// Separate buffer for decryption output
		uint8_t* out = malloc(16);
		memcpy(out,ct,16);

		// Load key bytes to guess 
		uint8_t key_guess[0x10];
		memcpy(key_guess,key,16);

		// List how workload is divided
		index = index_list[cur_thread];
		printf("thread:%d index:0x%016llx\n",cur_thread,index);

		while(!bk)
		{

			// Pull bytes from iterating index
			for(int i = 0; i < num_of_bytes_missing; i++)
			{
				key_guess[i] = (uint8_t)(index >> i*8)&0xff; 
			}

			// Decrypt ciphertext
			aes128cbc (key_guess, iv, ct, 16, out);

			// Check if plaintext is 0s where we have bruteforced the correct symmetric key
			if(!memcmp(out, pt, 16))
			{
				uint8_t working_key[0x10];

				memcpy(working_key,key_guess,16);

				printf("working key! solved on thread:%d\n",cur_thread);
				hexDump(working_key, 16);
				bk = 1;

			}

			index++;
		}
	}

  return 0;
}
