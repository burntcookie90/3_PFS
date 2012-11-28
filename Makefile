hamfs: hamfs.o 
	gcc -g -o hamfs hamfs.o  `pkg-config fuse --libs`

hamfs.o : hamfs.c  params.h
	gcc -g -Wall `pkg-config fuse --cflags` -c hamfs.c

clean:
	rm -f hamfs.o

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
