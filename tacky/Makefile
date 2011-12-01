all: main

main: headers.h main.c
	gcc -o mytcpdump main.c -lpcap

clean:
	rm -v ./mytcpdump
