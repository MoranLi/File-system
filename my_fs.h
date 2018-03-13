/*
Names: Yukun Li, Yuhan Hu 
NSID : yul040, yuh347
Std# : 11173592, 11212253
Group: group07
*/


#ifndef MY_FS_H
#define MY_FS_H

int my_mkdir(char *name, struct threadInfo *info);
int my_cd(char *name, struct threadInfo *info);
int my_rmdir(char *name, struct threadInfo *info);
int my_open(char *name, struct threadInfo *info);
int my_close(int fd, struct threadInfo *info);
int my_rm(char* name, struct threadInfo *info);
int my_seek(int fd, int pos, struct threadInfo *info);
int my_read(int fd,char* data, struct threadInfo *info);
int my_write(int fd,char* data, struct threadInfo *info);

#endif
