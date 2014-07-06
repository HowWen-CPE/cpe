/*
 *    Filename:  eth_config.c
 * Description:  operate ethernet PHY regester.
 *     Version:  v1.0
 *     Created:  2011.5.31
 *     Revision: 

 *    Compiler: mips(/arm)-linux-gcc
 *      Author:  Alvin Ke @ LITEON 
 */
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <assert.h>
#include <stddef.h>
#include <signal.h>
#include <setjmp.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/un.h>
#include <sys/poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <linux/if.h>
#include <linux/if_arp.h>
#include <linux/route.h>
#include <linux/ppp_defs.h>
//#include <linux/if_ppp.h>
#include <string.h>

#ifndef ATH_DEFAULT_RADIO
#define ATH_DEFAULT_RADIO "wifi0"
#endif
#ifndef ATH_DEFAULT_VAP
#define ATH_DEFAULT_VAP "ath0"
#endif
char global_radio[] = ATH_DEFAULT_RADIO;
char global_vap[] = ATH_DEFAULT_VAP;

#define SIOCSIWFREQ	0x8B04		/* set channel/frequency (Hz) */
#define SIOCGIWFREQ	0x8B05		/* get channel/frequency (Hz) */

/* Frequency flags */
#define IW_FREQ_AUTO		0x00	/* Let the driver decides */
#define IW_FREQ_FIXED		0x01	/* Force a specific value */

/*
 *	A frequency
 *	For numbers lower than 10^9, we encode the number in 'm' and
 *	set 'e' to 0
 *	For number greater than 10^9, we divide it by the lowest power
 *	of 10 to get 'm' lower than 10^9, with 'm'= f / (10^'e')...
 *	The power of 10 is in 'e', the result of the division is in 'm'.
 */
struct  iw_freq
{
    __s32       m;      /* Mantissa */
    __s16       e;      /* Exponent */
    __u8        i;      /* List index (when in range struct) */
    __u8        flags;      /* Flags (fixed/auto) */
};


/* ------------------------ IOCTL REQUEST ------------------------ */
/*
 * The structure to exchange data for ioctl.
 * This structure is the same as 'struct ifreq', but (re)defined for
 * convenience...
 *
 * Note that it should fit on the same memory footprint !
 * You should check this when increasing the above structures (16 octets)
 * 16 octets = 128 bits. Warning, pointers might be 64 bits wide...
 */
struct	iwreq 
{
	union
	{
		char	ifrn_name[IFNAMSIZ];	/* if name, e.g. "eth0" */
	} ifr_ifrn;

	/* Data part */
	union
	{
		/* Config - generic */
		char		name[IFNAMSIZ];
		/* Name : used to verify the presence of  wireless extensions.
		 * Name of the protocol/provider... */
		struct iw_freq	freq;	/* frequency or channel :
					 * 0-1000 = channel
					 * > 1000 = frequency in Hz */
	}u;
};


void usage()
{
    printf("Usage:\n");
    printf("sssetchannel interface channel [para1] [para2]\n");
    printf("		para1: channel number\n");
    printf("		para2: 1 means frequency hopping\n");
}

static inline int
ss_iw_set_ext(int			skfd,		/* Socket to the kernel */
	   const char *		ifname,		/* Device name */
	   int			request,	/* WE ID */
	   struct iwreq *	pwrq)		/* Fixed part of the request */
{
  /* Set device name */
  strncpy(pwrq->ifr_name, ifname, IFNAMSIZ);
  pwrq->ifr_name[IFNAMSIZ - 1] = '\0';
  /* Do the request */
  return(ioctl(skfd, request, pwrq));
}

static inline int
ss_iw_get_ext(int			skfd,		/* Socket to the kernel */
	   const char *		ifname,		/* Device name */
	   int			request,	/* WE ID */
	   struct iwreq *	pwrq)		/* Fixed part of the request */
{
  /* Set device name */
  strncpy(pwrq->ifr_name, ifname, IFNAMSIZ);
  pwrq->ifr_name[IFNAMSIZ - 1] = '\0';
  /* Do the request */
  return(ioctl(skfd, request, pwrq));
}


int ssa_iw_sockets_open(void)
{
  static const int families[] = {
    AF_INET, AF_IPX, AF_AX25, AF_APPLETALK
  };
  unsigned int	i;
  int		sock;
  for(i = 0; i < sizeof(families)/sizeof(int); ++i)
    {
      sock = socket(families[i], SOCK_DGRAM, 0);
      if(sock >= 0)
	return sock;
  }

  return -1;
}

static void ssa_iw_sockets_close(int skfd)
{
  close(skfd);
}

static int
ss_set_freq_info(int		skfd,
	      char *		ifname,
	      char *		args[],		/* Command line args */
	      int		count)		/* Args count */
{
  struct iwreq		wrq;
  int			i = 1;
  int        freq, fh;
  char *        unit;

  fh = 0;
  
    if(!strcasecmp(args[0], "channel"))
    {
        if (args[1] != NULL)
        freq = atoi(args[1]);
	
	if (args[2] != NULL)
		fh = atoi(args[2]);

        if((freq < 0 )||( freq > 1000))
        {
            usage();
            exit(0);
        }

        wrq.u.freq.m = freq;
        wrq.u.freq.e =0;
        wrq.u.freq.i =0xf0;/*0xf0 for spectral scan,liteon*/
    	if (fh == 1) 
    	{
    		wrq.u.freq.i |= 0x1;
    	}
        wrq.u.freq.flags = IW_FREQ_FIXED;


        /* Check for an additional argument */
        if((i < count) && (!strcasecmp(args[i], "auto")))
        {
        wrq.u.freq.flags = 0;
        ++i;
        }

        if((i < count) && (!strcasecmp(args[i], "fixed")))
        {
        wrq.u.freq.flags = IW_FREQ_FIXED;
        ++i;
        }
        
        if(ss_iw_set_ext(skfd, ifname, SIOCSIWFREQ, &wrq) < 0)
        {
          printf("ss_iw_set_ext error\n",freq);
          exit(0);
        }
        
    }
    else
    {
        usage();
    }
  /* Var args */
  return(i);
}


int main(int argc, char **argv)
{

    int skfd;
    int ss_channel;
    
    if((skfd = ssa_iw_sockets_open()) < 0)
    {
        perror("socket");
        printf("ssa_iw_sockets_open error\n");
        exit(0);
    }

    if(argc < 2 )
    {
        usage();
        exit(0);
    }	   

    ss_set_freq_info(skfd, argv[1], argv + 2, argc - 2);

    ssa_iw_sockets_close(skfd);
    return 0;
}

