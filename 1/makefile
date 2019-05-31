CC = vc +kick13
CFLAGS = -I$(NDK_INC) -O2
LDFLAGS = -lamiga -lauto 
EXES = session1

.PHONY : clean check
.SUFFIXES : .o .c

all: $(EXES)

clean:
	del *.o $(EXES)

.c.o:
	$(CC) $(CFLAGS) $^ -c -o $@

session1: main.o
	$(CC) $^ $(LDFLAGS) -o $@

main.o: main.c
	$(CC) $(CFLAGS) $^ -c -o $@

