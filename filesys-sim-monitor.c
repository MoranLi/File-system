/*
Names: Yukun Li, Yuhan Hu 
NSID : yul040, yuh347
Std# : 11173592, 11212253
Group: group07
*/


#include <stdio.h>


#include </student/cmpt332/pthreads/standards.h>
#include </student/cmpt332/pthreads/os.h>

#include "monitor.h"


/* blockNum = OKtoWrite */
/* blockNum*2 = OKtoRead */

int numReaders = 0;
int busy =0;


void StartRead(int blockNum)
{
  MonEnter();

  if (busy)
    {
      MonWait(blockNum*2);
    }
  numReaders++;
  /*printf("starting value of numReaders = %d\n", numReaders);*/
  MonSignal(blockNum*2);
  
  MonLeave();
}

void StopRead(int blockNum)
{
  MonEnter();
  numReaders--;
  /*printf("stopping value of numReaders = %d\n", numReaders);*/
  if (numReaders == 0) 
    MonSignal(blockNum);
  MonLeave();
}

void StartWrite(int blockNum)
{
  MonEnter();  
  if ((numReaders !=0) || busy)
    MonWait(blockNum);
  busy = 1;
  MonLeave();
  
  
}

void StopWrite(int blockNum)
{
  MonEnter();

  busy = 0;

  /*printf("about to signal reader %d\n", MyPid());*/
  
  MonSignal(blockNum*2);

  MonLeave();

}
