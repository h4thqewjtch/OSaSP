all: parent child
parent: parent.c makefile
	gcc parent.c func_parent.c -o parent
child: child.c makefile
	gcc child.c func_child.c -o child
clean:
	rm parent child
