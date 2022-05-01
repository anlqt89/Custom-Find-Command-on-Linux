CC = gcc
EXE0 = opt.exe

%.exe : %.o
	$(CC) -o $@ $^
%.o : %c
	$(CC) -c -o $@ $^

all:	$(EXE0)

clean:
	rm -f *.exe *.o

test1:
	./tq1.sh

test2:
	./tq2.sh

test3:
	./tq3.sh

test4:
	./tq4.sh
idirs:
	./initDirs.sh

ddirs:
	rm -r testdir


