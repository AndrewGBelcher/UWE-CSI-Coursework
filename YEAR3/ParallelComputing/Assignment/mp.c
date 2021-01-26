/************************************************************************************
*				Parallel AES-CBC-128 Weak Key Brute forcer
*			(position 0-3, 4 bytes targeted, adjust to your needs)
*								OpenMP Version
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
#include <omp.h>

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
			printf("Please enter both options with sensible values:\n\t\t-t <thread number>\n\t\t-k <number of bytes to bruteforce in key>\n\t\t-p performance mode\n\t\t-d attack mode\n\t\t-pt <plaintext binary file>\n\t\t-ct <ciphertext binary file>\n\n");
			printf("\ni.e. mpbrute -t 3 -k 3 -p\n");
			printf("\ni.e. mpbrute -t 3 -k 3 -d -pt plain.bin -ct cipher.bin\n");
			printf("\nThe key bytes count should be from 1-8\nAnd the thread count should be from 1-max number of threads for the machine to be of any use\n");
}


// function that takes key string from arguments and finds how many intended missing bytes are in it
int count_missing_bytes(char* target_key)
{
	int count = 0; // number of x/X found

	// for length of the key string, will be bound checked before here
	for(int i = 0; i<32; i++)
	{
		// either x or X is valid
		if(target_key[i]=='x' || target_key[i]=='X')
		{
			if(target_key[i+1]=='x' || target_key[i+1]=='X')
			{
				count++;
				i++;
			}
		}
	}
	return count;
}

// convert hex value to 8bit number
uint8_t hex_to_int(char val)
{
		// 0-9 used in hex
	    if(val >='0' && val <='9')
        {
            return (val - 48);
        }
		// lower case
        else if( val >='a' && val <='f')
        {
            return (val - 97 + 10);
        }
		// upper case
        else if( val >='A' && val <='F')
        {
            return (val - 65 + 10);
        }
		return 0;
}

// take key string from command line and load both the key and missing byte position
void parse_key_string(char* target_key, uint8_t key[16], bool byte_check[16])
{
	int d=0;

	for(int i=0; i<32; )
	{
		// its a missing byte
		if(target_key[i]=='x' || target_key[i]=='X')
		{
			// must follow XX or xx format for full byte
			if(target_key[i+1]=='x' || target_key[i+1]=='X')
			{
				key[d] = 0; // just 0 for now
				byte_check[d]=true; // we have a target byte to attack
				i=i+2; // iterate on key string by 2
				d++; // iterate on key by 1
			}
		}
		// check if its a valid hex val for upper nibble
		else if( (target_key[i]>='a' && target_key[i]<='f') ||
				(target_key[i]>='A' && target_key[i]<='F') || 
				(target_key[i]>='0' && target_key[i]<='9'))
		{
			// now check if lower nibble is valid
			if((target_key[i+1]>='a' && target_key[i+1]<='f') ||
					(target_key[i+1]>='A' && target_key[i+1]<='F') || 
					(target_key[i+1]>='0' && target_key[i+1]<='9'))
			{
		
				uint8_t upper = hex_to_int(target_key[i]);
				uint8_t lower = hex_to_int(target_key[i+1]);
					
				key[d] = (upper << 4) | lower; // align nibbles and set
				i = i+2; // iterate on key string by 2
				d++; // iterate on key by 1
			}			
		}
		else
			i=i+2; // skip index always to complete the loop, for edgecase if string isnt hex values itll do nothing
	}
}

// reads in file from filename into buffer
int read_crypto_file(char* buffer, char* fname)
{

	FILE *file;
	int flen;

	// read as binary data and determine file length
	file = fopen(fname, "rb"); 
	fseek(file, 0, SEEK_END);         
	flen = ftell(file);             
	rewind(file); // reset position to start

	// must be 128bits long atleast to provide us with some plaintext
	if(flen < 16 || file == -1)
	{
		printHelp();
		exit(8);
	}

	// we only need 1st 16 bytes to check
	fread(buffer, 16, 1, file);
	fclose(file);
	
	return 0;
}


int main(int argc, char *argv[])
{
	volatile bool bk = 0; // flow control aka break functionality shared across threads

	// command line argument variables
	int num_of_bytes_missing = 0; 		
	int num_threads = 0;				
	int performance_eval_mode = false;
	int attack_mode = false;
	char* target_key = "";
	uint8_t key[0x10], iv[0x10];
	bool byte_check[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	char* plaintext_filename = "";
	char* ciphertext_filename = "";

	// Crypto materials
	uint8_t ct[0x10];
	uint8_t pt[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // something easy to detect when key is found during decryption


	// read in and parse command line arguments
	for(int i = 0; i < argc; i++)
	{
		// thread number argument
		if(!strcmp(argv[i], "-t") && i != argc-1)
			num_threads = atoi(argv[i+1]);

		// number of bytes missing argument
		if(!strcmp(argv[i], "-k") && i != argc-1)
			num_of_bytes_missing = atoi(argv[i+1]);		

		// performance mode setting
		if(!strcmp(argv[i], "-p"))
		{
			attack_mode = false;
			performance_eval_mode = true;
		}

		// attack mode setting
		if(!strcmp(argv[i], "-d") && i != argc-1)
		{
			performance_eval_mode = false;
			attack_mode = true;
			target_key = argv[i+1];
		}

		// plaintext file supplied
		if(!strcmp(argv[i], "-pt") && i != argc-1)
		{
			plaintext_filename = argv[i+1];
		}

		// ciphertext file supplied
		if(!strcmp(argv[i], "-ct") && i != argc-1)
		{
			ciphertext_filename = argv[i+1];
		}

		// display help message
		if(!strcmp(argv[i],"-h"))
		{
			printHelp();
			exit(0);
		}
	}

	// evaluation of command line argument data

	// if not in performance evaluation mode we need to crack a supplied key with some ct/pt sample
	if(!performance_eval_mode)
	{
		// check if key was entered
		if(!strcmp(target_key,""))
		{
			printf("bad target key\n\n");
			printHelp();
			exit(4);
		}
		// check if plaintext file name was entered
		if(!strcmp(plaintext_filename,""))
		{
			printf("bad plain text file\n\n");
			printHelp();
			exit(6);
		}
		// check if ciphertext file name was entered
		if(!strcmp(ciphertext_filename,""))
		{
			printf("bad cipher text file\n\n");
			printHelp();
			exit(7);
		}
		// key has been entered after -d
		else	
		{
			// check length of key string
			if(strlen(target_key)!=32)
			{
				printf("key is not 128bit\n\n");
				printHelp();
				exit(5);
			}
			// key string is valid length
			else
			{

				read_crypto_file(pt,plaintext_filename);
				read_crypto_file(ct,ciphertext_filename);	

				// find out how many missing bytes are in the string via x/X chars
				int count = count_missing_bytes(target_key);

				// verify if key string was correctly representing the missing bytes
				if(count != num_of_bytes_missing)
				{
					printf("target key does not reflect missing bytes, use xx/XX to tell which bytes of key are missing\n\n");
					printHelp();
					exit(6);	
				}
				// key can be parsed with no issue
				else
				{
					parse_key_string(target_key, key, byte_check);	
				}
			}
		}
	}
	
	// bound check on threads
	if(num_of_bytes_missing > 8)
	{
		printf("Oh hello mr NSA!\nLet me just super cool the quantum computer for you first...\n");
		exit(1);
	}
	// invalids
	if(num_threads == 0 || num_of_bytes_missing == 0)
	{
		printHelp();
		exit(3);
	}


	// Parallel work load variables
	uint64_t index = 0;
	uint64_t index_list[num_threads];
	uint64_t byte_upper = 0x100;
	uint64_t work_div_value = (byte_upper << (uint64_t)((num_of_bytes_missing-1)*8))/num_threads; 

	printf("work divider value is 0x%016llx\n",work_div_value);
	printf("max num of threads:%d\n",omp_get_max_threads());
	printf("num of threads used:%d\n",num_threads);

	// Setup crypto material and generate cipher text to run against
	srand(time(0));

	// always clear iv, even tho it would be nice to have iv set at somepoint from cmd line args
	memset(iv,0,16);

	// performance mode should generate a random key to guess
	if(performance_eval_mode)
	{
		for(int i=0; i < 16; i++)
			key[i] = random()%256;// randomize key to guess

		// Random Key to bruteforce
		printf("\nkey to guess:\n");
		hexDump(&key,16);
		
		aes128cbc_enc(key, iv, pt, 16, ct); // encrypt ciphertext
	}

	// normal mode we have a supplied key with target bytes along with pt/ct files to check
	else	
	{
		// Random Key to bruteforce
		printf("\nPartial key to crack:\n");
		printf("%s\n",target_key);
		hexDump(&key,16);
	}

	printf("\nDividing the workload...\n");

	// Loops in parallel
	#pragma omp parallel shared(bk,key,ct,pt,work_div_value) private(index) num_threads(num_threads)
	{
		int cur_thread = omp_get_thread_num();
		uint8_t working_key[0x10];

		// Separate buffer for decryption output
		uint8_t* out = malloc(16);

		// Load key bytes to guess 
		uint8_t key_guess[0x10];
		memcpy(key_guess,key,16);

		// List how workload is divided
		index = work_div_value * (cur_thread+1);
		printf("thread:%d index:0x%016llx\n",cur_thread,index);

		while(!bk)
		{

			int d =0;// needed for interating on only selected bytes

			// Pull bytes from iterating index
			if(performance_eval_mode)
			{
				// set key to be guessed, first bytes will be used in performance mode
				for(int i = 0; i < num_of_bytes_missing; i++)
					key_guess[i] = (uint8_t)(index >> i*8)&0xff;
			}
			else	
			{
				// attack mode selects target bytes in partial key
				for(int i = 0; i < 16; i++) //num_of_bytes_missing; i++)
				{				
					// its a byte that needs cracking in the key
					if(byte_check[i])
					{
						// assign the byte to the position in index and shift it for the remaining
						key_guess[i] = (uint8_t)(index >> d*8)&0xff;

						// move index by how many target bytes have been loaded
						d++; 
					}
				
				}
			}

			// Decrypt ciphertext
			aes128cbc (key_guess, iv, ct, 16, out);

			// Check if plaintext is 0s where we have bruteforced the correct symmetric key
			if(!memcmp(out, pt, 16))
			{

				// copy guess to working key if the key is correct so we can separate it out
				memcpy(working_key,key_guess,16);

				// alert of found key and bust out of loop
				printf("working key! solved on thread:%d\n",cur_thread);
				hexDump(working_key, 16);
				bk = 1; // break out of loop for thread
			}

			// keep working through key space
			index++;
		}
	}

  return 0;
}
