#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

char *dirPath = "/home/pei/Documents";
char *logPath = "/home/pei/Documents/sisop/modul4/soal-shift-sisop-modul-4-I01-2021/fs.log";

void encrypt(char *key){
   int a ,len = strlen(key);
   char ch;
   for(int i=0;i<len;i++){

      if(key[i] == '/')
         break;
      if(key[i] == '.')
      {
         len = i - 1;
         break;
      }
      a = key[i];
      a=91-(a-64);
      ch= a;
      key[i]=ch;
   }
}
void decrypt(char *key){
   int a;
   char ch;
   for(int i=0;i<strlen(key);i++){
      a = key[i];
      a = 91-(a-64);
      ch = a;
      key[i]=ch;
   }
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
  int res;
  char temp[1000];

  strcpy(temp, path);

  if(strncmp(path, "/AtoZ_", 6) == 0)
    decrypt(temp);

  char fpath[1000];
  sprintf(fpath, "%s%s", dirPath, temp);

  res = lstat(fpath, stbuf);

  if(res == -1)
    return -errno;

  return 0;
}

void Log (char *level, char *cmd_desc)
{
  FILE * fp;
  fp = fopen (logPath, "a+");

  time_t rawtime = time(NULL);
  
  struct tm *info = localtime(&rawtime);
  
  char time[100];
  strftime(time, 100, "%y%m%d-%H:%M:%S", info);

  char log[100];
  sprintf(log, "%s::%s::%s\n", level, time, cmd_desc);
  fputs(log, fp);

  fclose(fp);
}

static struct fuse_operations xmp_oper = {
  .getattr  = xmp_getattr,
//   .readdir  = xmp_readdir,
//   .mknod    = xmp_mknod,
};
int main(int argc, char *argv[]){
   umask(0);
  return fuse_main(argc, argv, &xmp_oper, NULL);
}