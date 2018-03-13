# Names: Yukun Li, Yuhan Hu
# NSID : yul040, yuh347
# Std# : 11173592, 11212253
# Group: group 07

CC = gcc
CFLAGS = -g
CPPFLAGS = -Wall -pedantic -std=c90 -Wno-unused-parameter -Wno-pointer-arith

# Directory for Monitor
MONI = -L./
MONL = -L./ -lMonitor
# Directory for List
LISTI = -I./
LISTL64 = -L./ -llist64
# Directory for partB
UBCDIR = /student/cmpt332/pthreads/
UBCLIB = $(UBCDIR)lib/Linuxx86_64/
UBCI = -I$(UBCDIR)
UBCL = -L$(UBCLIB) -lpthreads

.PHONY: all clean

all: filesys-sim-threads fstest

fstest: fileDisk.o test.o my_fs_pure.o
	gcc fileDisk.o test.o my_fs_pure.o -o fstest

fileDisk.o: fileDisk.c fileDisk.h block.h
my_fs.o: fileDisk.h block.h my_fs.c
test.o: fileDisk.h block.h my_fs.h test.c

filesys-sim-threads: filesys-sim-threads.o filesys-sim-monitor.o libMonitor liblist64 my_fs.o fileDisk.o
	$(CC) -o final filesys-sim-threads.o filesys-sim-monitor.o my_fs.o fileDisk.o $(UBCL) $(MONL) $(LISTL64)

filesys-sim-threads.o: filesys-sim-threads.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c filesys-sim-threads.c -I$(UBCDIR)

filesys-sim-monitor.o: filesys-sim-monitor.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c filesys-sim-monitor.c $(UBCI) $(MONI) $(LISTI)

###################
# Monitor Library #
###################
libMonitor: monitor.o
	ar cr libMonitor.a monitor.o

monitor.o: monitor.c monitor.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c monitor.c

########################
# List(m64) Library ####
########################
liblist64: liblist64.a

list_adder.o: list_adders.c list.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c list_adders.c -o list_adders.o

list_movers.o: list_movers.c list.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c list_movers.c -o list_movers.o

list_removers.o: list_removers.c list.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c list_removers.c -o list_removers.o

list_errno.o: list_errno.c list.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c list_errno.c -o list_errno.o

liblist64.a:  list_adders.o list_movers.o list_removers.o list_errno.o
	ar rcs liblist64.a $@ $^

clean:
	rm -f *.o core.* fstest final *.gch test *.a
