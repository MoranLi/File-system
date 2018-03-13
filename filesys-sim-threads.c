/*
Names: Yukun Li, Yuhan Hu 
NSID : yul040, yuh347
Std# : 11173592, 11212253
Group: group07
*/



#include <stdio.h>
#include <standards.h>
#include <os.h>
#include <time.h>

#include "monitor.h"
#include "filesys-sim-monitor.h"
#include "block.h"
#include "my_fs.h"
#include "fileDisk.h"

int BLOCK_SIZE;

int i, rprint; 
int j, wprint;

int readWait = 0;
int writeWait = 0;

char *files[4] = {"afile", "bfile", "cfile", "dfile"};
char *dirs[5] = {"..", "adir", "bdir", "cdir", "ddir"};
char *data;


void init(struct threadInfo *info) {
    int i;
    for (i = 0; i < INODE_NUM; i++) {
      info->openbit[i] = 0;
      info->seek_ptr[i] = 0;
    }
    info->curBlockNum = 1;
}

PROCESS reader(void *arg)
{
  clock_t start, diff;
  struct threadInfo *info;
  int myId, option, file;
  char buff[1024];
  
  myId = (long) arg;
  info = malloc(sizeof(struct threadInfo));
  init(info);
  
  
  for(;;)
    {
      if (i == 100) break;
      i++;
      option = rand()%4;
      /* open file */
      printf("%d: open %s\n", myId, files[option]);
      file = my_open(files[option], info);
BLOCK_SIZE = 256;
      
      /* read file */
      printf("%d: read %s\n", myId, files[option]);
      start = clock();                      /* start timing */
      if(my_read(file, buff, info) > -1)
      	printf("%d: %s\n",myId, buff);
      diff = clock() - start;               /* stop timing */
      readWait += diff*1000000/CLOCKS_PER_SEC;
      
      /* close file */
      printf("%d: close %s\n", myId, files[option]);
      my_close(file, info);
      
      Sleep((int) (rand() % 3));
    }
  if(rprint == 0) {
    rprint = 1;
    Sleep(300);
    printf("Read Wait Time: %d\n", readWait);
  }
}

PROCESS writer(void *arg)
{
  clock_t start, diff;
  struct threadInfo *info;
  int myId, option, file;
  
  myId = (long) arg;
  info = malloc(sizeof(struct threadInfo));
  init(info);
   
  for(;;)
    {
      if (i == 100) break;
      i++;
      option = rand()%4;
      /* open file */
      printf("%d: open %s\n", myId, files[option]);
      file = my_open(files[option], info);
      
      /* write file */
      start = clock();                      /* start timing */
      printf("%d: write %s\n", myId, files[option]);
      my_write(file, data, info);
      diff = clock() - start;               /* stop timing */
      writeWait += diff*1000000/CLOCKS_PER_SEC;
      
      /* close file */
      printf("%d: close %s\n", myId, files[option]);
      my_close(file, info);
      
      Sleep((int) (rand() % 30));
    }
  if(wprint == 0) {
     wprint = 1;
     Sleep(300);
     printf("Write Wait Time: %d\n", writeWait); 
  }
}


PROCESS director(void *arg)
{
  struct threadInfo *info;
  int myId, cmd, name;
  
  myId = (long) arg;
  info = malloc(sizeof(struct threadInfo));
  init(info);
   
  for(;;)
    {
      if (j == 20) break;
      j++;
      cmd = rand()%3;
      name = rand()%5;
      
      switch(cmd){
      	case 0:
	  printf("%d: mkdir %s\n", myId, dirs[name]);
	  my_mkdir(dirs[name], info);
	  break;
	case 1:
	  printf("%d: rmdir %s\n", myId, dirs[name]);
	  my_rmdir(dirs[name], info);
	  break;
	case 2:
	  printf("%d: cd %s\n", myId, dirs[name]);
	  my_cd(dirs[name], info);
	  break;
      }
      
      Sleep((int) (rand() % 30));
    }

}

int mainp(int argc, char** argv)
{
    int i;
    /* set BLOCK_SIZE */
    if(strcmp(argv[4], "s") == 0)
	   BLOCK_SIZE = 256;
    else if(strcmp(argv[4], "m") == 0)
	   BLOCK_SIZE = 512;
    else if(strcmp(argv[4], "l") == 0)
	   BLOCK_SIZE = 1024;
    else
	   printf("wrong BLOCK_SIZE argument");
    /* set data length */
    if(strcmp(argv[1], "s") == 0)
        data = "ssssssssssssssss";
    else if(strcmp(argv[1], "m") == 0)
        data = "mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm";
    else if(strcmp(argv[1], "l") == 0)
        data = "lllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllll";
    else
	printf("wrong data length argument");

    setbuf(stdout, 0);
    
    MonInit();
    my_mkfs();
   
    for (i = 0; i < atoi(argv[2]); i++) {
        Create( (void(*)()) reader, 16000, "R", (void *)(long)i, NORM, USR );
    }
    for (i = 0; i < atoi(argv[3]); i++) {
        Create( (void(*)()) writer, 16000, "W", (void *)(long)10+i, NORM, USR );
    }
    Create( (void(*)()) director, 16000, "D", (void *)101, NORM, USR );
    Create( (void(*)()) director, 16000, "D", (void *)102, NORM, USR );

    printf("processes created\n");
    
    return 0;
}
