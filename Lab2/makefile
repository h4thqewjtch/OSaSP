CC = gcc
.PHONY: clean

all: parent child
parent: parent.c makefile
	$(CC) parent.c -o parent
child: child.c makefile
	$(CC) child.c -o child
clean:
	rm parent child
