#define IP_SIZE 1024
#define OP_SIZE 1032
int generate_key();
int blowfish_decrypt(int infd, int outfd);
int blowfish_encrypt(int infd, int outfd);
