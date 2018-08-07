/* 
 * tcpconnect.c - test connection to host port
 * usage: tcpconnect <host> <port>
 * c1995 ping@stepnet.com
 */

#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h> 
#include <string.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <err.h>
#include <sysexits.h>
#include <errno.h>
#include <ctype.h>
#include <arpa/inet.h>
#include "tcpconnect.h"


extern char *optarg;
extern int optind;
extern int optopt;
extern int opterr;
extern int optreset;

char *program_name;
uint32_t debugflag = 0;

static void usage(void) __attribute__((noreturn));

int main(int argc, char **argv) {
	int sockfd;
	struct in_addr		inaddr;
	struct sockaddr_in	saddr;
	struct hostent 		*server;
	struct servent		*sp;
	char *host, *service;
	register char *cp;
	register int timeout;
	register uint op;
	int retval = 0;

	timeout = 0;
	if ((cp = strrchr(argv[0], '/')) != NULL)
		program_name = cp + 1;
	else
		program_name = argv[0];

	while ((op = getopt(argc, argv, "Vd:t:")) != -1)
		switch (op) {
		case 'V':
			usage();
			break;
#ifdef DEBUG
                case 'd': {
                        debugflag = strtoul(optarg, NULL, 0);
                        DPF(D_TC1, "debugflag is 0x%08x", debugflag);
                        break;
                }
#endif
		case 't':
			timeout = atoi(optarg);
			break;
		default:
			usage();
		}

	argc -= optind;
	argv += optind;

	if (argc != 2)
		usage();

	host = argv[0];
	service = argv[1];

	bzero((char *) &saddr, sizeof(saddr));
	saddr.sin_family = AF_INET;

	if (isalpha(service[0])) {
		if ( (sp = getservbyname(service, "tcp")) == NULL)
			err(EX_UNAVAILABLE, "no such service");
		saddr.sin_port = sp->s_port;
	} else {
		saddr.sin_port = htons(atoi(service));
	}

	if ((inaddr.s_addr = inet_addr(host)) != INADDR_NONE) {
		server = gethostbyaddr( (char *) &inaddr,
				sizeof(struct in_addr), AF_INET );
		if (server == NULL)
			warn("cannot find host by ip");
		bcopy((char *) &inaddr.s_addr, (char *)&saddr.sin_addr,
			sizeof(inaddr.s_addr));
	} else {
		server = gethostbyname(host);
		if (server == NULL)
			err(EX_NOHOST, "no such host");
		bcopy(server->h_addr, (char *)&saddr.sin_addr,
			server->h_length);
	}

	/* socket: create the socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		err(EX_CANTCREAT, "ERROR opening socket");

	if (timeout == 0) {
		if (connect(sockfd, (struct sockaddr *)&saddr, sizeof(saddr))
			== 0) {
			DPF(D_TC1, "connect ok");
			close(sockfd);
			exit(0);
		} else {
			retval = errno;
			DPF(D_TC1, "connect failed: %s", strerror(retval));
			close(sockfd);
			exit(retval);
		}
	}

	fcntl(sockfd, F_SETFL, O_NONBLOCK); 

	if (connect(sockfd, (struct sockaddr *)&saddr,
			sizeof(saddr)) == 0) {
		close(sockfd);
		exit(0);

	}

	if (errno != EINPROGRESS) {
		DPF(D_TC1, "non-blocking connect failed: %s", strerror(errno));
		close(sockfd);
		exit(1);
	}

	fd_set wfds;
	struct timeval tv;

	tv.tv_sec = timeout / 1000 ;
	tv.tv_usec = timeout % 1000 ;
	FD_ZERO(&wfds);
	FD_SET(sockfd, &wfds);

	retval = select(sockfd + 1, NULL, &wfds, NULL, &tv);

	if (retval == -1) {
		DPF(D_TC1, "select: %s", strerror(errno));
		close(sockfd);
		exit(1);
	}

	if (retval == 0) {
		DPF(D_TC1, "timed out %ld s %ld us", tv.tv_sec, tv.tv_usec);
		close(sockfd);
		exit(1);
	}

	socklen_t l = sizeof(retval);
	if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &retval, &l) != 0) {
		DPF(D_TC1, "getsockopt: %s", strerror(errno));
		close(sockfd);
		exit(1);
	}

	if (retval != 0) {
		DPF(D_TC1, "getsockopt: %s", strerror(retval));
		close(sockfd);
		exit(retval);
	}

	DPF(D_TC1, "connected within timeout");
	close(sockfd);
	exit (0);
}

static void     
usage(void)             
{
/* Vb:d:i:ls:w: */
        (void)fprintf(stderr, "%s version %s by %s\n", program_name,
		VERSION, MY_CONTACT);
        (void)fprintf(stderr,
"%s: test tcp server presence\n", program_name);
        (void)fprintf(stderr,
"  -V                 print version\n"
"  -t usec            set timeout to micro seconds\n"
#ifdef DEBUG
"  -d debugflag       debug flag.  see source code\n"
#endif
"\n"
"Example:\n"
"     %s -t 500 host port\n"
"\n",
		program_name);
        exit(1);
}

