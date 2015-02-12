/*
 * hdf5-fuse: fuse wrapper around the hdf5 file format
 */

#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <string.h>
#include <errno.h>

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
  return fuse_main(argc, argv, &hdf5_oper, NULL);
}
