#CFLAGS=		-Werror -Wall
CFLAGS=		-Werror -Wall -DDEBUG -g

all:	tcpconnect

tcpconnect: tcpconnect.c common.h tcpconnect.h
	$(CC) tcpconnect.c -o $@
