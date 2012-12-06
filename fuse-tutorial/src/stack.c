/*
 * This code is a modificaiton of work cieted below.  It was modified by
 * ojblass June 16, 2009.
 * 
 * The original can be found at http://tldp.org/LDP/LG/issue87/vinayak.html
 * 
 * TERMS OF THE Open Publication License (OPL)
 * 
 * Copyright © 2003, Vinayak Hegde. Copying license
 * http://www.linuxgazette.com/copying.html Published in Issue 87 of Linux
 * Gazette, February 2003
 * 
 * 1. The modified version must be labeled as such. 2. The person making the
 * modifications must be identified and the modifications dated. 3.
 * Acknowledgement of the original author and publisher if applicable must be
 * retained according to normal academic citation practices. 4. The location
 * of the original unmodified document must be identified. 5. The original
 * author's (or authors') name(s) may not be used to assert or imply
 * endorsement of the resulting document without the original author's (or
 * authors') permission.
 */

#include <openssl/blowfish.h>
#include <openssl/evp.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#define IP_SIZE 1024
#define OP_SIZE 1024 + EVP_MAX_BLOCK_LENGTH
unsigned char   key[16];
unsigned char   iv[8];

int
generate_key()
{
	int             i, fd;
	if ((fd = open("/dev/random", O_RDONLY)) == -1)
		perror("open error");

	if ((read(fd, key, 16)) == -1)
		perror("read key error");

	if ((read(fd, iv, 8)) == -1)
		perror("read iv error");

	printf("128 bit key:\n");
	for (i = 0; i < 16; i++)
		printf("%4d ", key[i]);

	printf("\nInitialization vector\n");
	for (i = 0; i < 8; i++)
		printf("%4d ", iv[i]);
	printf("\n");


	close(fd);
	return 0;
}

int
do_decrypt(int infd, int outfd)
{
	unsigned char           *inbuff, *outbuf;
	int             olen=0, tlen=0, n=0;
	EVP_CIPHER_CTX  ctx;
	EVP_CIPHER_CTX_init(&ctx);
	EVP_DecryptInit(&ctx, EVP_bf_cbc(), key, iv);

	outbuf = (unsigned char *) malloc(sizeof(unsigned char) * OP_SIZE);
	inbuff = (unsigned char *) malloc(sizeof(unsigned char) * IP_SIZE);

	/* keep reading until a break */
	for (;;) {
		memset(inbuff, 0, IP_SIZE);
		if ((n = read(infd, inbuff, IP_SIZE)) == -1) {
			perror("read error");
			break;
		} else if (n == 0)
			break;

		memset(outbuf, 0, OP_SIZE);

		if (EVP_DecryptUpdate(&ctx, outbuf, &olen, inbuff, n) != 1) {
			printf("error in decrypt update\n");
			return 0;
		}
		if ((n = write(outfd, outbuf, olen)) == -1)
			perror("write error");
	}

	tlen=0;
	if (EVP_DecryptFinal(&ctx, outbuf + olen, &tlen) != 1) {
		perror("error in decrypt final");
		return 0;
	}

	if ((n = write(outfd, outbuf+olen, tlen)) == -1)
		perror("write error");

	EVP_CIPHER_CTX_cleanup(&ctx);
	return 1;
}

int
do_encrypt(int infd, int outfd)
{
	unsigned char           *inbuff, *outbuf;

	int             olen=0, tlen=0, n=0;
	EVP_CIPHER_CTX  ctx;
	EVP_CIPHER_CTX_init(&ctx);
	EVP_EncryptInit(&ctx, EVP_bf_cbc(), key, iv);

	outbuf = (unsigned char *) malloc(sizeof(unsigned char) * OP_SIZE);
	inbuff = (unsigned char *) malloc(sizeof(unsigned char) * IP_SIZE);

	for (;;) {
		memset(inbuff, 0, IP_SIZE);

		if ((n = read(infd, inbuff, IP_SIZE)) == -1) {
			perror("read error");
			break;
		} else if (n == 0)
			break;

		if (EVP_EncryptUpdate(&ctx, outbuf, &olen, inbuff, n) != 1) {
			printf("error in encrypt update\n");
			return 0;
		}

		if ((n = write(outfd, outbuf, olen)) == -1)
			perror("write error");
	}
	tlen=0;
	if (EVP_EncryptFinal(&ctx, outbuf + olen, &tlen) != 1) {
		printf("error in encrypt final\n");
		return 0;
	}

	if ((n = write(outfd, outbuf+olen, tlen)) == -1)
		perror("write error");

	EVP_CIPHER_CTX_cleanup(&ctx);
	return 1;
}

int
main(int argc, char *argv[])
{
	int             flags1 = 0, flags2 = 0, outfd, infd;
	mode_t          mode;

	memset(key, 0, 16);
	memset(iv, 0, 8);
	memset(&mode, 0, sizeof(mode));

	flags1 = flags1 | O_RDONLY;
	flags2 = flags2 | O_RDONLY;
	flags2 = flags2 | O_WRONLY;
	flags2 = flags2 | O_CREAT;

	mode = mode | S_IRUSR;
	mode = mode | S_IWUSR;


	generate_key();


	if ((infd = open(argv[1], flags1, mode)) == -1)
		perror("open input file error");

	if ((outfd = open(argv[2], flags2, mode)) == -1)
		perror("open output file error");

	do_encrypt(infd, outfd);

	close(infd);
	fsync(outfd);
	close(outfd);

	if ((infd = open(argv[2], flags1, mode)) == -1)
		perror("open output file error");

	if ((outfd = open(argv[3], flags2, mode)) == -1)
		perror("open output file error");

	do_decrypt(infd, outfd);

	close(infd);
	fsync(infd);
	close(outfd);

	return 0;
}
