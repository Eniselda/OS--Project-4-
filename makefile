all: ds

ds: ds.c
	gcc -o ds ds.c -lm
