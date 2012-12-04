//Compile: # gcc -o blowfish sym_funcs.c -lcrypto
//sudo yum openssl 
//sudo yum openssl-dev

#include <string.h>
#include <openssl/blowfish.h>
#include <openssl/evp.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "encrypt.h"

/*#define IP_SIZE 1024*/
/*#define OP_SIZE 1032*/

unsigned char key[16];
unsigned char iv[8];

int generate_key ()
{
	int i, j, fd;
	if ((fd = open ("/dev/random", O_RDONLY)) == -1)
		perror ("open error");

	if ((read (fd, key, 16)) == -1)
		perror ("read key error");

	if ((read (fd, iv, 8)) == -1)
		perror ("read iv error");
	
	printf("128 bit key:\n");
	for (i = 0; i < 16; i++)
		printf ("%d \t", key[i]);
	printf ("\n ------ \n");

	printf("Initialization vector\n");
	for (i = 0; i < 8; i++)
		printf ("%d \t", iv[i]);

	printf ("\n ------ \n");
	close (fd);
	return 0;
}

int blowfish_decrypt (int infd, int outfd)
{
	unsigned char outbuf[IP_SIZE];
	int olen, tlen, n;
	char inbuff[OP_SIZE];
	EVP_CIPHER_CTX ctx;
	EVP_CIPHER_CTX_init (&ctx);
	EVP_DecryptInit (&ctx, EVP_bf_cbc (), key, iv);

	for (;;)
	  {
		  bzero (&inbuff, OP_SIZE);
		  if ((n = read (infd, inbuff, OP_SIZE)) == -1)
		    {
			    perror ("read error");
			    break;
		    }
		  else if (n == 0)
			  break;

		  bzero (&outbuf, IP_SIZE);

		  if (EVP_DecryptUpdate (&ctx, outbuf, &olen, inbuff, n) != 1)
		    {
			    printf ("error in decrypt update\n");
			    return 0;
		    }

		  if (EVP_DecryptFinal (&ctx, outbuf + olen, &tlen) != 1)
		    {
			    printf ("error in decrypt final\n");
			    return 0;
		    }
		  olen += tlen;
		  if ((n = write (outfd, outbuf, olen)) == -1)
			  perror ("write error");
	  }

	EVP_CIPHER_CTX_cleanup (&ctx);
	return 1;
}

int blowfish_encrypt (int infd, int outfd)
{
	unsigned char outbuf[OP_SIZE];
	int olen, tlen, n;
	char inbuff[IP_SIZE];
	EVP_CIPHER_CTX ctx;
	EVP_CIPHER_CTX_init (&ctx);
	EVP_EncryptInit (&ctx, EVP_bf_cbc (), key, iv);

	for (;;)
	  {
		  bzero (&inbuff, IP_SIZE);

		  if ((n = read (infd, inbuff, IP_SIZE)) == -1)
		    {
			    perror ("read error");
			    break;
		    }
		  else if (n == 0)
			  break;

		  if (EVP_EncryptUpdate (&ctx, outbuf, &olen, inbuff, n) != 1)
		    {
			    printf ("error in encrypt update\n");
			    return 0;
		    }

		  if (EVP_EncryptFinal (&ctx, outbuf + olen, &tlen) != 1)
		    {
			    printf ("error in encrypt final\n");
			    return 0;
		    }
		  olen += tlen;
		  if ((n = write (outfd, outbuf, olen)) == -1)
			  perror ("write error");
	  }
	EVP_CIPHER_CTX_cleanup (&ctx);
	return 1;
}

/* ***HOW TO USE THESE FUNCTIONS *** */

/*

 if ((infd = open ("test.txt", flags1, mode)) == -1)
				    perror ("open input file error");

			    if ((outfd = open ("testy.txt", flags2, mode)) == -1)
				    perror ("open output file error");

			    encrypt (infd, outfd);

			    close (infd);
			    close (outfd);
			    
if ((outfd = open ("testy.txt", flags1, mode)) == -1)
				    perror ("open output file error");

			    if ((decfd = open ("done.txt", flags2, mode)) == -1)
				    perror ("open output file error");

			    decrypt (outfd, decfd);

			    close (outfd);
			    fsync (decfd);
			    close (decfd);


THIS IS FOR CHECKING FOR THE +PRIVATE HEADER IN THE FILENAME (INPUT AS STRING)

int check_private(char * filename)
{
	if(strstr(filename, "+private"))
		return 1;
	return 0;
}

*/
