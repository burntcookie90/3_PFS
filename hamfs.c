#define FUSE_USE_VERSION  26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <libexif/exif-data.h>
#include "params.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <unistd.h>


static int ham_error(char *str){
	int ret = -errno;

	return ret;
}

static void ham_fullpath(char fpath[PATH_MAX], const char *path){
	strcpy(fpath, BB_DATA->rootdir);
	strncat(fpath, path, PATH_MAX);
}

static int ham_getattr(const char *path, struct stat *stbuf){
	int retstat = 0;
	char fpath[PATH_MAX];

	ham_fullpath(fpath, path);

	retstat = lstat(fpath, statbuf);

	if(retstat !=0)
		retstat = ham_error("bb_getattr lstat");

	return retstat;
}

static int ham_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
	int retstat = 0;
	DIR *dp;
	struct direct *de;

	dp = (DIR *) (uintptr_t) fi->fh;

	de = readdir(dp);

	if(de == 0){
		retstat = ham_error("bb_readdir readdir");
		return retstat;
	}

	do{
		if(filler(buf, de->d_name,NULL,0) !=0){
			return -ENOMEM;
		}
	} while((de = readdir(dp))!=NULL);

	return retstat;

}

static int ham_open(const char *path, struct fuse_file_info *fi) {
	int retstat = 0;
	int fd;
	char fpath[PATH_MAX];

	ham_fullpath(fpath,path);
	fd = open(fpath,fi->flags);

	if(fd<0)
		retstat = ham_error("ham_open open");

	fi->fh = fd;
	
	return retstat;
}

static int ham_read(const char *path, char *buf, size_t size, off_t offset,struct fuse_file_info *fi){

	int retstat = 0;

	retstat = pread(fi->fh,buf,size,offset);
	if(retstat<0)
		restat = ham_error("ham_read read");

	return restat;
}

int ham_mkdir(const char *path, mode_t mode){
	int retstat = 0;
	char fpath[PATH_MAX];

	ham_fullpath(fpath,path);

	retstat = mkdir(fpath,mode);

	if(retstat < 0)
		retstat = ham_error("ham_mkdir mkdir");

	return retstat;

}

static struct fuse_operations ham_oper= {
	.getattr   = ham_getattr,
	.readdir = ham_readdir,
	.open   = ham_open,
	.read   = ham_read,
	.mkdir = ham_mkdir;
};


int main(int argc, char *argv[])
{
	return fuse_main(argc, argv, &ham_oper, NULL);
}
