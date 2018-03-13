/* 
Names: Yukun Li, Yuhan Hu 
NSID : yul040, yuh347
Std# : 11173592, 11212253
Group: group07
*/

#ifndef READER_WRITER_MONITOR_H
#define READER_WRITER_MONITOR_H

PROCESS reader(void *arg);

PROCESS writer(void *arg);

void StartWrite(int);

void StopWrite(int);

void StartRead(int);

void StopRead(int);

#endif	/*READER_WRITER_MONITOR_H*/
