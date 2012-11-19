 #define FUSE_USE_VERSION  26
   
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>



static int ham_getattr(const char *path, struct stat *stbuf){

}

static int ham_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {

}

static int ham_open(const char *path, struct fuse_file_info *fi) {

}

static int ham_read(const char *path, char *buf, size_t size, off_t offset,struct fuse_file_info *fi){

}
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
