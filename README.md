# prcmd
Parallel remote commands
- needs 'make' and 'cc' to build tcpconnect
- tcpconnect tests connection to port [22] and times out after 0.5s default
- it's faster to use ssh keys from ssh-agent running on local workstation or somewhere close to it.
- if you want to run large number of jobs in parallel, check your 'ulimit'.  make sure you have enough memory and file descriptors.

```
[2241]ping@porte:~/git/prcmd$ make
cc tcpconnect.c -o tcpconnect

[2242]ping@porte:~/git/prcmd$ ./prcmd
Usage: ./prcmd [-f] [-h file] [-l] <cmds>
      -f : remote execute in background.
      -h file : list of hostnames, 1 per line.  may be use multiple times
      -i seconds : sleep seconds between n parrallel runs [10]
      -l : local only, no ssh.  '\$h' is replaced by hostname
      -n parrallel : n parrallel jobs [100]
      -o ssh_options : extra ssh options
         e.g.: ./prcmd "-oIdentitiesOnly=yes -i .ssh/id_rsa" uname -n
      -q quiet
      -t microseconds : connection test timeout
      -u user : remote username [ping]
      -y : don\'t print NNNN at beginning of line
[2243]ping@porte:~/git/prcmd$ grep pinelake /etc/hosts|awk '{print $3}'| ./prcmd -n 200 uname -a
0000 porte FreeBSD porte.pinelake.stepnet.com 11.2-STABLE FreeBSD 11.2-STABLE #2 r336079: Sun Jul  8 11:49:31 PDT 2018     root@porte.pinelake.stepnet.com:/usr/obj/usr/src/sys/PORTE  amd64
0001 urumqi FAILED: connect failed
0002 ganden FAILED: connect failed
0003 jodphur FAILED: connect failed
0004 udaipur FAILED: connect failed
0005 goa FAILED: connect failed
0006 mx FreeBSD mx.pinelake.stepnet.com 11.2-STABLE FreeBSD 11.2-STABLE #2 r336079: Sun Jul  8 11:49:31 PDT 2018     root@porte.pinelake.stepnet.com:/usr/obj/usr/src/sys/PORTE  amd64
0007 apc FAILED: connect failed
0008 kyoto FAILED: connect failed
0009 ladakh FAILED: connect failed
0010 ds700 FAILED: connect failed
0011 turpan FAILED: connect failed
0012 agra FAILED: connect failed
0013 ftp FreeBSD ftp.pinelake.stepnet.com 11.2-STABLE FreeBSD 11.2-STABLE #2 r336079: Sun Jul  8 11:49:31 PDT 2018     root@porte.pinelake.stepnet.com:/usr/obj/usr/src/sys/PORTE  amd64
0014 www FreeBSD www.pinelake.stepnet.com 11.2-STABLE FreeBSD 11.2-STABLE #2 r336079: Sun Jul  8 11:49:31 PDT 2018     root@porte.pinelake.stepnet.com:/usr/obj/usr/src/sys/PORTE  amd64
0015 xps8900 FAILED: connect failed
0016 sc1523 FAILED: connect failed
0017 envisa FAILED: connect failed
0018 hik FAILED: connect failed
0019 sonytv FAILED: connect failed

[2288]ping@porte:~/git/prcmd$ ./tcpconnect localhost 22
[2289]ping@porte:~/git/prcmd$ echo $?
0
[2290]ping@porte:~/git/prcmd$ ./tcpconnect localhost 23
[2291]ping@porte:~/git/prcmd$ echo $?
61
[2292]ping@porte:~/git/prcmd$ time ./tcpconnect -d 0x0001ffff -t 2000 www.google.com 80
tcpconnect: debugflag is 0x0001ffff
tcpconnect: connected within timeout

real    0m0.014s
user    0m0.005s
sys     0m0.000s
[2293]ping@porte:~/git/prcmd$ time ./tcpconnect -d 0x0001ffff -t 2000 www.google.com 81
tcpconnect: debugflag is 0x0001ffff
tcpconnect: timed out 2 s 0 us

real    0m2.074s
user    0m0.000s
sys     0m0.005s
[2294]ping@porte:~/git/prcmd$ ./tcpconnect
tcpconnect version 0.1 by ping@stepnet.com
tcpconnect: test tcp server presence
  -V                 print version
  -t usec            set timeout to micro seconds
  -d debugflag       debug flag.  see source code

Example:
     tcpconnect -t 500 host port
```
