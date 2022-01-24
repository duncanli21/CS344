main:
	gcc -std=c99 -Wall -g -o files files.c 

run:
	valgrind ./files --leak-check=full --track-origins=yes

clean: 
	rm -f files 