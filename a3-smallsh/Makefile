main:
	gcc -std=c99 -Wall -g -o smallsh smallsh.c 

run:
	valgrind ./smallsh --leak-check=full --track-origins=yes

clean: 
	rm -f smallsh 