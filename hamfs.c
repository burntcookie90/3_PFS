 #define FUSE_USE_VERSION  26
   
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

static struct fuse_operations ham_oper= {
	.getattr   = ham_getattr,
	.readdir = ham_readdir,
	.open   = ham_open,
	.read   = ham_read,
};
  


int main(int argc, char *argv[])
{
	return fuse_main(argc, argv, &ham_oper, NULL);
}
