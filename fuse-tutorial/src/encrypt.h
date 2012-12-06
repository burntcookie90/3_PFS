#define IP_SIZE 1024
#define OP_SIZE 1032
int generate_key();
int blowfish_decrypt(int infd, int outfd, char* rootDir);
int blowfish_encrypt(int infd, int outfd, char* rootDir);
