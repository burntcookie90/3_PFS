hamfs : bbfs.o log.o encimp.o
	gcc -g -o hamfs bbfs.o log.o encimp.o `pkg-config fuse --libs` -lexif -L/usr/local/lib -lsqlite3 -lcrypto

bbfs.o : bbfs.c log.h params.h encrypt.h
	gcc -g -Wall `pkg-config fuse --cflags` -c  bbfs.c 

log.o : log.c log.h params.h
	gcc -g -Wall `pkg-config fuse --cflags` -c log.c

encimp.o : encimp.c encrypt.h
	gcc -c encimp.c -lcrypto

clean:
	rm -f hamfs *.o

dist:
	rm -rf fuse-tutorial/
	mkdir fuse-tutorial/
	cp ../*.html fuse-tutorial/
	mkdir fuse-tutorial/example/
	mkdir fuse-tutorial/example/mountdir/
	mkdir fuse-tutorial/example/rootdir/
	echo "a bogus file" > fuse-tutorial/example/rootdir/bogus.txt
	mkdir fuse-tutorial/src
	cp Makefile COPYING COPYING.LIB *.c *.h fuse-tutorial/src/
	tar cvzf ../../fuse-tutorial.tgz fuse-tutorial/
	rm -rf fuse-tutorial/
