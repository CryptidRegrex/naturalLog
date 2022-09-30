# Justin Tobiason comment symbol = #
# Program 1 threads
prog1 : prog1.c
	gcc -Wall prog1.c -o prog1 -lpthread -lm

clean :
	rm*~ ./prog1	
