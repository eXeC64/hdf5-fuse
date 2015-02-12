/*
 * hdf5-fuse: fuse wrapper around the hdf5 file format
 */

#define FUSE_USE_VERSION 26

#include <errno.h>
#include <fuse.h>
#include <hdf5.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int hdf5_fuse_getattr(const char* path, struct stat *stbuf)
{
  memset(stbuf, 0, sizeof(struct stat));
  if (strcmp(path, "/") == 0) {
    stbuf->st_mode = S_IFDIR | 0755;
    stbuf->st_nlink = 2;
  } else {
    return -ENOENT;
  }
  return 0;
}

static int hdf5_fuse_readdir(const char* path, void* buf, fuse_fill_dir_t filler,
    off_t offset, struct fuse_file_info *fi)
{
  (void) offset;
  (void) fi;

  if (strcmp(path, "/") != 0)
    return -ENOENT;

  filler(buf, ".", NULL, 0);
  filler(buf, "..", NULL, 0);

  return 0;
}

static struct fuse_operations hdf5_oper = {
  .getattr = hdf5_fuse_getattr,
  .readdir = hdf5_fuse_readdir,
};

int main(int argc, char** argv)
{
  if (argc != 3) {
    printf("usage: %s <mount point> <hdf5 file>\n", argv[0]);
    exit(0);
  }

  H5open();
  //Check for hdf5 file
  if (!H5Fis_hdf5(argv[2])) {
    printf("invalid hdf5 file: %s\n", argv[2]);
    exit(1);
  }

  //Attempt to open hdf5 file
  hid_t file = H5Fopen(argv[2], H5F_ACC_RDONLY, H5P_DEFAULT);
  if (file < 0) {
    printf("failed to open hdf5 file: %s\n", argv[2]);
    exit(1);
  }

  root_group = H5Gopen(file, "/", H5P_DEFAULT);

  int ret = 0;
  ret = fuse_main(argc - 1, argv, &hdf5_oper, NULL);
  H5Fclose(file);
  H5close();
  return ret;
}
