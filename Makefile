.PHONY: build clean

build:
	gcc main.c -o quadtree

clean:
	rm quadtree