CC = cc 
CFLAGS = -std=c99 -Wall -ggdb -O0 -lraylib -lX11
LDFLAGS = 

main: 
	$(CC) -o armsim.exe main.c $(CFLAGS) $(LDFLAGS)
