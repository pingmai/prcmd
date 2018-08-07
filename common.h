#ifndef common_h
#define common_h

#ifdef DEBUG

extern uint32_t debugflag;

/* debug modules */
#define	D_BPFCAP	0x00010000
#define	D_RINGBUFFER	0x00020000

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
#define DL10		0x00000200
#define DL11		0x00000400
#define DL12		0x00000800
#define DL13		0x00001000
#define DL14		0x00002000
#define DL15		0x00004000
#define DL16		0x00008000
#define DLMAX		0x0000f000

/* debug macros */
#define D_BPFCAP1	0x00010001
#define D_BPFCAP2	0x00010002
#define D_BPFCAP3	0x00010004
#define D_BPFCAP4	0x00010008
#define D_BPFCAP5	0x00010010
#define D_BPFCAP6	0x00010020
#define D_BPFCAP7	0x00010040
#define D_BPFCAP8	0x00010080
#define D_BPFCAP9	0x00010100
#define D_BPFCAP10	0x00010200
#define D_BPFCAP11	0x00010400
#define D_BPFCAP12	0x00010800
#define D_BPFCAP13	0x00011000
#define D_BPFCAP14	0x00012000
#define D_BPFCAP15	0x00014000
#define D_BPFCAP16	0x00018000
#define D_BPFCAPMAX	0x0001f000

#define D_RINGBUFFER1	0x00020001
#define D_RINGBUFFER2	0x00020002
#define D_RINGBUFFER3	0x00020004
#define D_RINGBUFFER4	0x00020008
#define D_RINGBUFFER5	0x00020010
#define D_RINGBUFFER6	0x00020020
#define D_RINGBUFFER7	0x00020040
#define D_RINGBUFFER8	0x00020080
#define D_RINGBUFFER9	0x00020100
#define D_RINGBUFFER10	0x00020200
#define D_RINGBUFFER11	0x00020400
#define D_RINGBUFFER12	0x00020800
#define D_RINGBUFFER13	0x00021000
#define D_RINGBUFFER14	0x00022000
#define D_RINGBUFFER15	0x00024000
#define D_RINGBUFFER16	0x00028000
#define D_RINGBUFFERMAX	0x0002f000

#define DPF(d, fmt, args...)	\
do { \
	if ( ((d) & (debugflag) & 0xffff0000) && ((d) & (debugflag) & 0xffff)) \
		fprintf(stderr, "%s: " fmt "\n", __func__, ##args); \
} while (0)
#else
#define DPF(d, fmt, args...)
#endif /* DEBUG */

#define MTX_INIT(mtx) \
do { \
	(mtx) = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t)); \
	if ((mtx) == NULL) err(EX_OSERR, "malloc %s", #mtx); \
	errno = pthread_mutex_init((mtx), NULL); \
	if (errno != 0) err(EX_SOFTWARE, "pthread_mutex_init %s", #mtx); \
} while (0)

#define COND_INIT(c) \
do { \
	(c) = (pthread_cond_t *)malloc(sizeof(pthread_cond_t)); \
	if ((c) == NULL) err(EX_OSERR, "malloc %s", #c); \
	errno = pthread_cond_init((c), NULL); \
	if (errno != 0) err(EX_SOFTWARE, "pthread_cond_init %s", #c); \
} while (0)

#define MTX_DELETE(mtx) \
do { \
	pthread_mutex_destroy((mtx)); \
	free(mtx); \
} while (0)
 
#define COND_DELETE(mtx) \
do { \
	errno = pthread_cond_destroy((mtx)); \
	if (errno != 0) err(EX_SOFTWARE, "pthread_cond_destroy %s", #mtx); \
	free(mtx); \
} while (0)

#define MTX_LOCK(mtx) \
do { \
	errno = pthread_mutex_lock((mtx)); \
	if (errno != 0) err(EX_SOFTWARE, "%s: pthread_mutex_lock %s", \
		__func__, #mtx); \
} while (0)

#define MTX_UNLOCK(mtx) \
do { \
	errno = pthread_mutex_unlock((mtx)); \
	if (errno != 0) err(EX_SOFTWARE, "%s: pthread_mutex_unlock %s", \
		__func__, #mtx); \
} while (0)

#define COND_SIGNAL(c) \
do { \
	errno = pthread_cond_signal((c)); \
	if (errno != 0) err(EX_SOFTWARE, "pthread_cond_signal %s", #c); \
} while (0)

#define COND_WAIT(c,m) \
do { \
	errno = pthread_cond_wait((c),(m)); \
	if (errno != 0) err(EX_SOFTWARE, "pthread_cond_wait %s, %s", #c, #m); \
} while (0)

/* import from libpcap/pcap-int.h */
typedef	int bpf_int32;
typedef	u_int bpf_u_int32;

struct pcap_timeval {
    bpf_int32 tv_sec;		/* seconds */
    bpf_int32 tv_usec;		/* microseconds */
};

struct pcap_sf_pkthdr {
    struct pcap_timeval ts;	/* time stamp */
    bpf_u_int32 caplen;		/* length of portion present */
    bpf_u_int32 len;		/* length this packet (off wire) */
};

#define min(A,B)	((A) < (B) ? (A) : (B))
#define max(A,B)	((A) > (B) ? (A) : (B))

#endif /* common_h */
