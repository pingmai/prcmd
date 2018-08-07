#ifndef tcpconnect_h
#define tcpconnect_h

#define VERSION "0.1"
#define MY_CONTACT "ping@stepnet.com"
#define DEBUG TRUE

#ifdef DEBUG

extern uint32_t debugflag;

/* debug modules */
#define	D_TC	0x00010000

/* debug levels */
#define DL1		0x00000001
#define DL2		0x00000002
#define DL3		0x00000004
#define DL4		0x00000008
#define DL5		0x00000010
#define DL6		0x00000020
#define DL7		0x00000040
#define DL8		0x00000080
#define DL9		0x00000100
#define DL10	0x00000200
#define DL11	0x00000400
#define DL12	0x00000800
#define DL13	0x00001000
#define DL14	0x00002000
#define DL15	0x00004000
#define DL16	0x00008000
#define DLMAX	0x0000f000

/* debug macros */
#define D_TC1	0x00010001
#define D_TC2	0x00010002
#define D_TC3	0x00010004
#define D_TC4	0x00010008
#define D_TC5	0x00010010
#define D_TC6	0x00010020
#define D_TC7	0x00010040
#define D_TC8	0x00010080
#define D_TC9	0x00010100
#define D_TC10	0x00010200
#define D_TC11	0x00010400
#define D_TC12	0x00010800
#define D_TC13	0x00011000
#define D_TC14	0x00012000
#define D_TC15	0x00014000
#define D_TC16	0x00018000
#define D_TCMAX	0x0001f000

#define DPF(d, fmt, args...)	\
do { \
	if ( ((d) & (debugflag) & 0xffff0000) && ((d) & (debugflag) & 0xffff)) \
		fprintf(stderr, "%s: " fmt "\n", program_name, ##args); \
} while (0)
#else
#define DPF(d, fmt, args...)
#endif /* DEBUG */

#define min(A,B)	((A) < (B) ? (A) : (B))
#define max(A,B)	((A) > (B) ? (A) : (B))

#endif /* tcpconnect_h */
