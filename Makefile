default: tags

tags.o: tags.c
	gcc -c tags.c -o tags.o

tags: tags.o
	gcc tags.o -o tags

clean:
	-rm -f tags.o
	-rm -f tags