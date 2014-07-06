 /******************************************************************************
* Copyright (C), 2014, LiteOn Technology Corp.
* Filename: clicbk.c
* Description: CLI library functions.
* Created by: Mario Huang
* Revision history:
* Revision  Date               Author                    Description
* -----------------------------------------------------------------------------------------------------------------
* v0.1     2014/05/05     Mario Huang              Create this file
*/
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <net/if.h>

#include "clicbklib.h"
#include "include/cli.h"
#include "nvram.h"
#include "nvram_rule.h"
int g_advSettingI;
char g_advSettingC;
char g_advArr[5];

extern int g_curVap;
//Deleted by Mario Huang, Close it temporary
#if 0
#if defined(WallAP)
extern int curPort;
#endif
#endif

isoTocuntrycode allCountries[] = {
	{"AD", "ANDORRA"},          // 1
	{"AR", "ARGENTINA"},
	{"AU", "AUSTRALIA"},
	{"AT", "AUSTRIA"},
	{"AZ", "AZERBAIJAN"},       // 5
	{"BS", "BAHAMAS"},
	{"BH", "BAHRAIN"},
	{"BY", "BELARUS"},
	{"BE", "BELGIUM"},
	{"BZ", "BELIZE"},           // 10
	{"BO", "BOLIVIA"},
	{"BA", "BOSNIA AND HERZEGOVINA"},
	{"BR", "BRAZIL"},
	{"BG", "BULGARIA"},
	{"CA", "CANADA"},
	{"CL", "CHILE"},            // 16
	{"CN", "CHINA"},            // 17
	{"CO", "COLOMBIA"},
	{"HR", "CROATIA (Hrvatska)"},
	{"CY", "CYPRUS"},
	{"CZ", "CZECH REPUBLIC"},
	{"DK", "DENMARK"},
	{"DO", "DOMINICAN REPUBLIC"},
	{"EG", "EGYPT"},
	{"EE", "ESTONIA"},
	{"FI", "FINLAND"},
	{"FR", "FRANCE"},
	{"DE", "GERMANY"},
	{"GI", "GIBRALTAR"},
	{"GR", "GREECE"},
	{"GL", "GREENLAND"},
	{"GP", "GUADELOUPE"},
	{"GT", "GUATEMALA"},
	{"GY", "GUYANA"},
	{"VA", "HOLY SEE (VATICAN CITY STATE)"},
	{"HK", "HONG KONG"},
	{"HU", "HUNGARY"},
	{"IS", "ICELAND"},
	{"IN", "INDIA"},
	{"ID", "INDONESIA"},
	{"IE", "IRELAND"},
	{"IL", "ISRAEL"},
	{"IT", "ITALY"},
	{"JP", "JAPAN"},
	{"JO", "JORDAN"},
	{"KZ", "KAZAKHSTAN"},
	{"KR", "KOREA,REPUBLIC OF"},
	{"KW", "KUWAIT"},
	{"LV", "LATVIA"},
	{"LI", "LIECHTENSTEIN"},
	{"LT", "LITHUANIA"},
	{"LU", "LUXEMBOURG"},
	{"MK", "MACEDONIA,THE FORMER YUGOSLAV REPUBLIC OF"},
	{"MY", "MALAYSIA"},
	{"MT", "MALTA"},
	{"MQ", "MARTINIQUE"},
	{"MX", "MEXICO"},           // 57
	{"MD", "MOLDOVA,REPUBLIC OF"},
	{"MC", "MONACO"},
	{"MA", "MOROCCO"},
	{"NL", "NETHERLANDS"},
	{"NZ", "NEW ZEALAND"},
	{"NO", "NORWAY"},
	{"OM", "OMAN"},
	{"PK", "PAKISTAN"},
	{"PA", "PANAMA"},
	{"PY", "PARAGUAY"},
	{"PE", "PERU"},
	{"PH", "PHILIPPINES"},
	{"PL", "POLAND"},
	{"PT", "PORTUGAL"},
	{"PR", "PUERTO RICO"},
	{"QA", "QATAR"},
	{"RO", "ROMANIA"},
	{"RU", "RUSSIAN FEDERATION"},
	{"SM", "SAN MARINO"},
	{"SA", "SAUDI ARABIA"},
	{"CS", "SERBIA AND MONTENEGRO"},
	{"SG", "SINGAPORE"},
	{"SK", "SLOVAKA"},
	{"SI", "SLOVENIA"},
	{"ZA", "SOUTH AFRICA"},
	{"ES", "SPAIN"},
	{"LK", "SRI LANKA"},
	{"SE", "SWEDEN"},
	{"CH", "SWITZERLAND"},
	{"TW", "TAIWAN,PROVINCE OF CHINA"},
	{"TH", "THAILAND"},
	{"TR", "TURKEY"},
	{"UA", "UKRAINE"},
	{"AE", "UNITED ARAB EMIRATES"},
	{"GB", "UNITED KINGDOM"},
	{"US", "UNITED STATES"},
	{"UY", "URUGUAY"},
	{"UZ", "UZBEKISTAN"},
	{"VE", "VENEZUELA"},
	{"VN", "VIET NAM"},
	{"VI", "VIRGIN  ISLANDS (U.S.)"},
	{"RW", "REST OF WORLD"}
};

  /***********************************************************************
 * Function Name : sub_replace
 * Description    : funciton@special character replace with 
 *parameter: source src str,sub str,replace str
 * Input         : source, the source string
 *                    sub, sub string to be replace
 *                    rep, string used to replace the sub string
 * Output        : 
 * Return value  : T_SUCCESS, command success
 ***********************************************************************/

char *sub_replace(char *source, char *sub, char *rep)
{
	char *result;
	char *pc1, *pc2, *pc3;
	int isource, isub, irep;
	isub = strlen(sub);
	irep = strlen(rep);
	isource = strlen(source);
	if (strlen(sub) == 0)	//sub[0]=='\0'; is empty str or not!
		return strdup(source);
	result = (char *)malloc(((irep > isub) ? (float)strlen(source) / isub * irep + 1 : isource) * sizeof(char));
	pc1 = result;
	while (*source != '\0') {
		pc2 = source;
		pc3 = sub;
		while (*pc2 == *pc3 && *pc3 != '\0' && *pc2 != '\0')
			pc2++, pc3++;
		if (strlen(pc3) == 0) {
			pc3 = rep;
			while (*pc3 != '\0')
				*pc1++ = *pc3++;
			pc2--;
			source = pc2;
		} else
			*pc1++ = *source;
		source++;
	}
	*pc1 = '\0';
	return result;
}
   /***********************************************************************
 * Function Name : is_integer
 * Description    : check if a string are digital
 * Input         : str, system name to be checked
 * Output        : 
 * Return value  : T_TRUE, ckeck un-passed
 *                       T_FALSE, check passed
 ***********************************************************************/
int is_integer(char *str)
{
	int i;
	int str_len=0;
	str_len = strlen(str);
	for (i=0; i<str_len; i++) {
		if ( str[i] >= '0' && str[i] <= '9')
			continue;
		else
			return T_FALSE;
	}
	return T_TRUE;
}
  /***********************************************************************
 * Function Name : check_systemname
 * Description    : check if the charactors of system name are leagul
 * Input         : str, system name to be checked
 * Output        : 
 * Return value  : T_TRUE, ckeck un-passed
 *                       T_FALSE, check passed
 ***********************************************************************/
int check_systemname(char *str)
{
	int i;
	int str_len=0;
	str_len = strlen(str);
	for (i=0; i<str_len; i++) {
		if ( (str[i] >= '0' && str[i] <= '9') ||
				(str[i] >= 'a' && str[i] <= 'z') ||
				(str[i] >= 'A' && str[i] <= 'Z') ||
				(str[i] == '_') || (str[i] == '-'))
			continue;
		else
			return T_FALSE;
	}
	return T_TRUE;
}
  
   /***********************************************************************
 * Function Name : get_sysname_from_nvram
 * Description    : get system name from nvram
 * Input         :
 * Output        : sysname, device name
 * Return value  : T_SUCCESS, get success
 *                       T_FAILURE, get failure
 ***********************************************************************/
int get_sysname_from_nvram(char *sysname)
{
	char *name_get=NULL;
	char *str=NULL;
	name_get = nvram_safe_get("hostname");
	
	if(strlen(name_get) <= 0)
	{
		uiPrintf("Get system name from nvram error!\n");
		return T_FAILURE;
	}
	
	strcpy(sysname, name_get);
	if(strstr(sysname,"\"")){
		str = sub_replace(sysname,"\"","&#34;");
		strcpy(sysname,str);
		free(str);
	}
	
	return T_SUCCESS;
}

  /***********************************************************************
 * Function Name : del_space
 * Description    : delete space of one string and copy it to another string
 * Input         : Buf1, string to copy
 * Output        : Buf2, string to be copy to
 * Return value  : 
 ***********************************************************************/
void del_space(char *Buf1, char* Buf2)
{
	while((*Buf1 != '\0') && (*Buf1 != '\n') && (*Buf1 != '\r'))
	{
		if((*Buf1) == 0x20)
		{
			++Buf1; // if space skip 
		}
		else
			*Buf2++ = *Buf1++; // if not space copy to buf 2
	}
	*Buf2 = '\0'; //NULL Terminate

}

  /***********************************************************************
 * Function Name : IsValidIpAddress
 * Description    : check if the ip address is legal
 * Input         : IpAddr, ip address
 * Output        :
 * Return value  : T_TRUE, valid ip address
 *                       T_FALSE, illegal ip address
 ***********************************************************************/
T_BOOL8 IsValidIpAddress(T_CHAR8 *IpAddr)
{
	T_INT32 ii,kk;

	for (ii = 24; ii >= 0; ii -= 8)
	{
		/* Skip any leading stuff (e.g., spaces, '[') */
		while (IpAddr != '\0' && !isdigit(*IpAddr))
			IpAddr++;
		kk =(T_INT32)atoi(IpAddr);
		if (kk < 0 || 255 < kk)
			return T_FALSE;
		if ( ((IpAddr = strchr(IpAddr, '.')) == NULL) && (ii > 0) )
			return T_FALSE;
		IpAddr++;
	}
	return T_TRUE;
}

//Added by Mario Huang
#if 0
int Authorware(int year)
{
	if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
		return 1;
	else
		return 0;

}
#endif
  /***********************************************************************
 * Function Name : readpass
 * Description    : read password form console or telnet
 * Input         : ifp
 *                    ofp
 *                    with_echo
 *                    max_asterisks
 * Output        : inpurt, the password input
 * Return value  : inpurt, the password input
 ***********************************************************************/
static char *readpass(FILE * ifp, FILE * ofp, int with_echo, int max_asterisks)
{
	static char input[33], asterix[33];
	static char once;
	char *cp, *ap, c;
	int i;

	if (max_asterisks < 0)
	{

		/* traditional code using fgets() */
		if (fgets(input, sizeof input, ifp) != input)
			return NULL;
		cp = strrchr(input, '\n');
		if (cp)
			*cp = '\0';
		else
			input[sizeof input - 1] = '\0';
		return input;
	}

	if (!once)
	{
		srandom(time(0) * getpid());
		once = 1;
	}

	cp = input;
	ap = asterix;
	while (read(fileno(ifp), &c, 1))
	{

		if ((cp >= input + 32) && c != '\n' && c != '\r' && c != '\b'
				&& c != 127 && c != '\025')
		{
			//putc ('\a', ofp); /* BEL */
			continue;
		}

		switch (c)
		{
			case '\n':
			case '\r':
				goto endwhile;
			case '\b':
			case 127:
				if (cp > input)
				{
					cp--;
					ap--;
					for (i = *ap; i > 0; i--)
						fputs("\b \b", ofp);
					*cp = '\0';
					*ap = 0;
				}
				else
				{
					putc('\a', ofp);    /* BEL */
				}
				break;
			case '\025':           /* Ctrl-U = erase everything typed so far */
				if (cp == input)
				{
					putc('\a', ofp);    /* BEL */
				}
				else
					while (cp > input)
					{
						cp--;
						ap--;
						for (i = *ap; i > 0; i--)
							fputs("\b \b", ofp);
						*cp = '\0';
						*ap = 0;
					}
				break;
			default:               //in our case that with_echo is 0 and max_asterisks = 1, *ap is always 1.
				*cp++ = c;
				if (with_echo)
				{
					*ap = 1;
					putc(c, ofp);
				}
				else if (max_asterisks > 0)
				{
					*ap = (random() % max_asterisks) + 1;
					for (i = *ap; i > 0; i--)
						putc('*', ofp);
				}
				else
				{
					*ap = 0;
				}
				ap++;
				break;
		}
		fflush(ofp);
#if 0
		if ((cp >= input + 32) && c != '\n' && c != '\r')
		{
			putc('\a', ofp);    /* BEL */
			continue;
		}
#endif
	}
endwhile:
	*cp = '\0';
	putc('\n', ofp);
	return input;
}
  /***********************************************************************
 * Function Name : getPasswordFromTerminal
 * Description    : read password form console or telnet
 * Input         : 
 * Return value  : password, the password input
 ***********************************************************************/
char *getPasswordFromTerminal()
{
	struct termios attr;

	/*
	   static char password[MAX_STRING_LEN];
	   memset(password,0,MAX_STRING_LEN);
	   */
	static char *password;




	fflush(stderr);

	if (tcgetattr(STDIN_FILENO, &attr) != 0)
		return NULL;
	attr.c_lflag = 8;
	attr.c_lflag &= ~(ICANON | ECHO);

	//attr.c_lflag |= (ICANON|ECHOE);
	//attr.c_cc[VERASE] = 8;

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &attr) != 0)
		return NULL;

	/*
	   while ((password[n] = getchar()) != '\n')
	   {
	   if(( password[n]>'~')||(password[n]<' '))
	   {
	   continue;
	   }
	   tcsetattr(STDIN_FILENO, TCSAFLUSH, &attr);
	   attr.c_lflag &= ECHO;
	   putchar('*');
	   if (n < sizeof(password) - 1 && password[n] >= ' ' && password[n] <= '~')
	   {
	   n++;
	   }
	   else
	   {
	   fflush(stderr);
	   }
	   }
	   password[n] = '\0';
	   if (n > (MAX_STRING_LEN - 1)) {
	   password[MAX_STRING_LEN - 1] = '\0';
	   }

*/
	password = readpass(stdin, stdout, 0, 1);
	attr.c_lflag |= ECHO;
	attr.c_lflag |= ISIG;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &attr);
	uiPrintf("\n");
	//return (char*) &password;
	return password;
}
  /***********************************************************************
 * Function Name : verifyIP
 * Description    : verify the ip
 * Input         : 
 * Return value  : legal IP
 ***********************************************************************/
int verifyIP(char *IpAddr)
{

	if (NULL == IpAddr)
	{
		return 0;
	}
	return INADDR_NONE != inet_addr(IpAddr);
}

//Deleted by Mario Huang
#if 0
unsigned long pow_10_long(int exp)
{
	unsigned long result = 1;

	while (exp)
	{
		result *= 10;
		exp--;
	}

	return result;
}

double pow_10(int exp)
{
	double result = 1;

	while (exp)
	{
		result *= 10;
		exp--;
	}

	return result;
}

unsigned long strToUnsigned(char *pStr)
{
	if (NULL == pStr)
		return -1;
	int iLength = strlen(pStr);
	unsigned long ret = 0;
	int ii;

	for (ii = 0; ii < strlen(pStr); ii++)
	{
		ret += ((pStr[ii] - '0') * pow_10_long(iLength - ii - 1));
	}

	return (unsigned long) ret;
}


long StrToLong(char *pStr)
{
	if (NULL == pStr)
		return -1;
	int iLength = strlen(pStr);
	double ret = 0.0;
	int ii;

	for (ii = 0; ii < iLength; ii++)
	{
		ret += ((pStr[ii] - '0') * pow_10(iLength - ii - 1));
	}
	return (long) ret;
}
int StartHttpServer(http_type_t httpConfig)
{
	char cmd[64];

	memset(cmd, 0, sizeof(cmd));

	/* Use Https protocol */
	if (httpConfig.bIsHttps)
		sprintf(cmd, "/sbin/boa -s -p %d -c /etc/httpd", httpConfig.iHttpsPort);
	else                        /* Use Http protocol */
		sprintf(cmd, "/sbin/boa -p %d -c /etc/httpd", httpConfig.iHttpPort);

	system(cmd);

	return 0;
}
#endif
//Deleted by Mario Huang
#if 0
int iw_get_ext(int skfd,        /* Socket to the kernel */
		const char *ifname,  /* Device name */
		int request,     /* WE ID */
		struct iwreq *pwrq)  /* Fixed part of the request */
{
	/* Set device name */
	strncpy(pwrq->ifr_name, ifname, IFNAMSIZ);
	/* Do the request */
	return (ioctl(skfd, request, pwrq));
}
#endif
//TODO:Use nvram instead
#if 0
int getCurrentWirelessIfName(T_CHAR8 *ifname, T_UINT32 opmode, int radio)
{

	if (NULL == ifname)
	{
		fprintf(stderr, "ifname was not allocated memory space.\n");
		return T_FAILURE;
	}
	BasicWlanConfig_t wlanConfig;

	memset(&wlanConfig, 0, sizeof(BasicWlanConfig_t));
	T_UINT32 uiProfileNum;

	wds_wds_t wdswds_t;

	memset(&wdswds_t, 0, sizeof(wdswds_t));
	cfg_get_wds(&wdswds_t, radio);

	switch (opmode)
	{
		case 0:                    /*Access Point */
			if (cfg_get_wlan_basic(&wlanConfig, radio) != T_SUCCESS)
			{
				uiPrintf("cfg_get_wlan_basic failed \n");
				return T_SUCCESS;
			}
			for (uiProfileNum = 0; uiProfileNum < 4; uiProfileNum++)
			{
				if (wlanConfig.ProfilebEnabled[uiProfileNum] == T_TRUE)
				{
					/* dennis modify. 2.4G(radio is 0): ath0/2/4/6. And 5G(radio is 1): ath1/3/5/7. */
					sprintf(ifname, "ath%d", (radio ? 1 : 0) + uiProfileNum * 2);
					break;
				}
			}


			break;
		case 1:                    /*Wireless Workgroup Bridge */
			break;
		case 2:                    /*Ad-hoc (Peer-to-Peer) */
			break;
		case 3:                    /*Repeater */
			break;
		case 4:                    /*Wireless Bridge */
			for (uiProfileNum = 0; uiProfileNum < 8; uiProfileNum++)
			{
				if (wdswds_t.record[uiProfileNum].IsActive == T_TRUE)
				{
					break;
				}
			}
			sprintf(ifname, "wds%d", uiProfileNum + 4);
			break;
		default:
			break;
	}
	return T_SUCCESS;
}

int iw_sockets_open(void)
{
	static const int families[] = {
		AF_INET, AF_IPX, AF_AX25, AF_APPLETALK
	};
	unsigned int i;
	int sock;

	/*
	 * Now pick any (exisiting) useful socket family for generic queries
	 * Note : don't open all the socket, only returns when one matches,
	 * all protocols might not be valid.
	 * Workaround by Jim Kaba <jkaba@sarnoff.com>
	 * Note : in 99% of the case, we will just open the inet_sock.
	 * The remaining 1% case are not fully correct...
	 */

	/* Try all families we support */
	for (i = 0; i < sizeof(families) / sizeof(int); ++i)
	{
		/* Try to open the socket, if success returns it */
		sock = socket(families[i], SOCK_DGRAM, 0);
		if (sock >= 0)
			return sock;
	}

	return -1;
}
#endif

/*------------------------------------------------------------------*/

/*
 * Get the range information out of the driver
 */
//Deleted by Mario Huang
#if 0
int iw_get_range_info(int skfd, const char *ifname, iwrange * range)
{
	struct iwreq wrq;
	char buffer[sizeof(iwrange) * 2];   /* Large enough */
	union iw_range_raw *range_raw;

	/* Cleanup */
	bzero(buffer, sizeof(buffer));

	wrq.u.data.pointer = (caddr_t) buffer;
	wrq.u.data.length = sizeof(buffer);
	wrq.u.data.flags = 0;
	if (iw_get_ext(skfd, ifname, SIOCGIWRANGE, &wrq) < 0)
		return (-1);


	/* Point to the buffer */
	range_raw = (union iw_range_raw *) buffer;

	/* For new versions, we can check the version directly, for old versions
	 * we use magic. 300 bytes is a also magic number, don't touch... */
	if (wrq.u.data.length < 300)
	{
		/* That's v10 or earlier. Ouch ! Let's make a guess... */
		range_raw->range.we_version_compiled = 9;
	}

	/* Check how it needs to be processed */
	if (range_raw->range.we_version_compiled > 15)
	{
		/* This is our native format, that's easy... */
		/* Copy stuff at the right place, ignore extra */
		memcpy((char *) range, buffer, sizeof(iwrange));
	}

	else
	{
		/* Zero unknown fields */
		bzero((char *) range, sizeof(struct iw_range));

		/* Initial part unmoved */
		memcpy((char *) range, buffer, iwr15_off(num_channels));
		/* Frequencies pushed futher down towards the end */
		memcpy((char *) range + iwr_off(num_channels),
				buffer + iwr15_off(num_channels),
				iwr15_off(sensitivity) - iwr15_off(num_channels));
		/* This one moved up */
		memcpy((char *) range + iwr_off(sensitivity),
				buffer + iwr15_off(sensitivity),
				iwr15_off(num_bitrates) - iwr15_off(sensitivity));
		/* This one goes after avg_qual */
		memcpy((char *) range + iwr_off(num_bitrates),
				buffer + iwr15_off(num_bitrates),
				iwr15_off(min_rts) - iwr15_off(num_bitrates));
		/* Number of bitrates has changed, put it after */
		memcpy((char *) range + iwr_off(min_rts),
				buffer + iwr15_off(min_rts),
				iwr15_off(txpower_capa) - iwr15_off(min_rts));
		/* Added encoding_login_index, put it after */
		memcpy((char *) range + iwr_off(txpower_capa),
				buffer + iwr15_off(txpower_capa),
				iwr15_off(txpower) - iwr15_off(txpower_capa));
		/* Hum... That's an unexpected glitch. Bummer. */
		memcpy((char *) range + iwr_off(txpower),
				buffer + iwr15_off(txpower),
				iwr15_off(avg_qual) - iwr15_off(txpower));
		/* Avg qual moved up next to max_qual */
		memcpy((char *) range + iwr_off(avg_qual),
				buffer + iwr15_off(avg_qual), sizeof(struct iw_quality));
	}


	/* We are now checking much less than we used to do, because we can
	 * accomodate more WE version. But, there are still cases where things
	 * will break... */
	if (!iw_ignore_version)
	{
		/* We don't like very old version (unfortunately kernel 2.2.X) */
		if (range->we_version_compiled <= 10)
		{
			fprintf(stderr,
					"Warning: Driver for device %s has been compiled with an ancient version\n",
					ifname);
			fprintf(stderr,
					"of Wireless Extension, while this program support version 11 and later.\n");
			fprintf(stderr, "Some things may be broken...\n\n");
		}

		/* We don't like future versions of WE, because we can't cope with
		 * the unknown */
		if (range->we_version_compiled > WE_VERSION)
		{
			fprintf(stderr,
					"Warning: Driver for device %s has been compiled with version %d\n",
					ifname, range->we_version_compiled);
			fprintf(stderr,
					"of Wireless Extension, while this program supports up to version %d.\n",
					WE_VERSION);
			fprintf(stderr, "Some things may be broken...\n\n");
		}

		/* Driver version verification */
		if ((range->we_version_compiled > 10) &&
				(range->we_version_compiled < range->we_version_source))
		{
			fprintf(stderr,
					"Warning: Driver for device %s recommend version %d of Wireless Extension,\n",
					ifname, range->we_version_source);
			fprintf(stderr,
					"but has been compiled with version %d, therefore some driver features\n",
					range->we_version_compiled);
			fprintf(stderr, "may not be available...\n\n");
		}
		/* Note : we are only trying to catch compile difference, not source.
		 * If the driver source has not been updated to the latest, it doesn't
		 * matter because the new fields are set to zero */
	}

	/* Don't complain twice.
	 * In theory, the test apply to each individual driver, but usually
	 * all drivers are compiled from the same kernel. */
	iw_ignore_version = 1;

	return (0);
}
#endif


/*------------------------------------------------------------------*/

/*
 * Convert our internal representation of frequencies to a floating point.
 */
//Deleted by Mario Huang
#if 0
double iw_freq2float(const iwfreq * in)
{
	int i;
	double res = (double) in->m;

	for (i = 0; i < in->e; i++)
		res *= 10;
	return (res);

}
#endif
//Deleted by Mario Huang
#if 0
char isPositiveNumber(char *tmp)
{
	int ii = 0;

	if (tmp == NULL)
		return 0;
	for (; ii < strlen(tmp); ii++)
	{
		if (!((tmp[ii] >= '0') && (tmp[ii] <= '9')))
			return 0;
	}
	return 1;
}


int strToNum(char *tmp, int size, int *num)
{
	char *pTmp = tmp;
	int i;
	int total = 0;

	if (size <= 0)
		return 0;
	for (i = 0; i < size; i++)
	{
		if (!isdigit(*(pTmp + i)))
		{
			return 0;
		}
		else
		{
			total = total * 10 + *(pTmp + i) - '0';
		}
	}

	*num = total;
	return 1;
}



int MhztoChanNum(int number)
{
	int channel = -1;

	if (number >= 5140)
		channel = (number - 5000) / 5;
	else if ((number >= 2512) && (number <= 2732))
		channel = (number - 2512) / 20 + 15;
	else if (number <= 2472)
		channel = (number - 2407) / 5;
	else;
	return channel;

}

int chanNumtoMhz(int number)
{
	int freq = 14;

	if (number < 14)
		freq = number * 5 + 2407;
	else if ((number > 14) && (number <= 26))
		freq = (number - 15) * 20 + 2512;
	else if (number > 27)
		freq = (number * 5) + 5000;
	return freq;
}

/*Channel 36/5180Mhz ---->>> 36 5180*/
int getChanAndFreqInwlanConfig_channel(int *chan, int *freq, char *pStr)
{
	int ii = 0;
	int jj = 0;
	int kk = 0;

	char pChan[4] = { 0 };
	char pFreq[5] = { 0 };
	int flag = 0;

	while (pStr[ii] != '\0')
	{
		if (pStr[ii] == '(')
			flag = 1;
		if ((pStr[ii] >= '0') && (pStr[ii] <= '9'))
		{
			/*dennis modify for cli. pStr is such as "2437MHz (Channel 6)". 2008-03-26. -----> */
			if (flag == /*0 */ 1)
				/*<----- */
				pChan[jj++] = pStr[ii];
			else
				pFreq[kk++] = pStr[ii];

		}
		ii++;
	}
	pChan[jj] = '\0';
	pFreq[kk] = '\0';
	strToNum(pChan, strlen(pChan), chan);
	strToNum(pFreq, strlen(pFreq), freq);
	return 0;

}




static inline void iw_sockets_close(int skfd)
{
	close(skfd);
}
//TODO: Deleted by Mario Huang, complete it later
#if 0
int set_wlan_power(int pt, int radio)
{
	if(ATHR == wlan_vendor)
	{
		return athr_set_wlan_power(pt, radio);
	}else if(BRCM == wlan_vendor)
	{
		return brcm_set_wlan_power(pt, radio);
	}
}

int get_wlan_power(int *ppt_value, int *ptxpl_value, int radio)
{
	if(ATHR == wlan_vendor)
	{
		return athr_get_wlan_power(ppt_value, ptxpl_value, radio);
	}
	else if(BRCM == wlan_vendor)
	{
		return brcm_get_wlan_power(ppt_value, ptxpl_value, radio);
	}	

}
#endif
int getChannelAndFreq_AP(int *pchan, int *pfreq, int *pextendedChan,
		int *pextendedFreq, int radio)
{
	//TODO:Use nvram instead
#if 0
	int channel = -1;
	int frequency = -1;
	int extendedChannel;
	int extendedFrequncy;



	int ii;

	int isTweety = 1;

	extendedChannel = -1;
	extendedFrequncy = -1;

	int skfd;
	unsigned char ifname[10] = { 0 };

	double freq;

	FILE *fp = NULL;

	struct iw_range range;
	memset(&range, 0, sizeof(struct iw_range));


	struct iwreq wrq;
	memset(&wrq, 0, sizeof(struct iwreq));

	unsigned char arrLine[100] = { 0 };


	/****************************************/
	/*get the channel and freq */

	getCurrentWirelessIfName((unsigned char *) ifname, 0, radio);

	/* Create a channel to the NET kernel. */
	if ((skfd = iw_sockets_open()) < 0)
	{
		perror("socket");
		return -1;
	}
	if (iw_get_range_info(skfd, ifname, &range) < 0)
		fprintf(stderr, "%-8.16s  no frequency information.\n\n", "ath0");
	if (iw_get_ext(skfd, ifname, SIOCGIWFREQ, &wrq) >= 0)
	{
		freq = iw_freq2float(&(wrq.u.freq));
		frequency = (int) (freq / 1000000);
		channel = MhztoChanNum(frequency);
	}
	iw_sockets_close(skfd);

	/***************************************/


	/*START:Get the Subsidiary channel No */
	unsigned char strCmd[60];

	memset(strCmd, 0, sizeof(strCmd));
	sprintf(strCmd, "iwpriv %s get_chextoffset > /tmp/offsettmp.cinfo", ifname);
	system(strCmd);
	fp = fopen("/tmp/offsettmp.cinfo", "r");
	memset(arrLine, 0, sizeof(arrLine));
	if (fgets(arrLine, 100, fp) != NULL)
	{
		for (ii = 4; ii < 100; ii++)
		{
			if ((arrLine[ii] == '0') || (arrLine[ii] == '1') || (arrLine[ii] == '-'))   /*0,1,-1 */
			{
				if (arrLine[ii] == '1')
				{
					extendedChannel = channel + 4;
					isTweety = 0;
				}
				else if (arrLine[ii] == '-')
				{
					extendedChannel = channel - 4;
					isTweety = 0;
				}
				else
				{
					extendedChannel = -1;   /*have no Subsidiary channel */
					isTweety = 1;
				}
				break;
			}
		}
	}
	fclose(fp);
	system("rm /tmp/offsettmp.cinfo");



	BasicWlanConfig_t wlanConfig;

	memset(&wlanConfig, 0, sizeof(BasicWlanConfig_t));
	if (cfg_get_wlan_basic(&wlanConfig, radio) != T_SUCCESS)
	{
		uiPrintf("cfg_get_wlan_basic failed.\n");
		return T_FAILURE;
	}
	switch (wlanConfig.mode)
	{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
			*pchan = channel;
			*pfreq = frequency;
			*pextendedChan = 0;
			*pextendedFreq = 0;
			break;
		case 5:
			if (extendedChannel != -1)
			{
				extendedFrequncy = chanNumtoMhz(extendedChannel);
				*pchan = channel;
				*pfreq = frequency;
				*pextendedChan = extendedChannel;
				*pextendedFreq = extendedFrequncy;

			}
			else
			{
				*pchan = channel;
				*pfreq = frequency;
				*pextendedChan = 0;
				*pextendedFreq = 0;
			}
			break;
		case 6:
			if (extendedChannel != -1)
			{
				extendedFrequncy = chanNumtoMhz(extendedChannel);
				*pchan = channel;
				*pfreq = frequency;
				*pextendedChan = extendedChannel;
				*pextendedFreq = extendedFrequncy;

			}
			else
			{
				*pchan = channel;
				*pfreq = frequency;
				*pextendedChan = 0;
				*pextendedFreq = 0;
			}
			break;
		default:

			break;
	}
#endif
	return T_SUCCESS;

}

int getChannelAndFreq_WDS(int *pchan, int *pfreq, int *pextendedChan,
		int *pextendedFreq, int radio)
{
	//TODO:Use nvram instead
#if 0
	BasicWlanConfig_t wlanConfig;

	memset(&wlanConfig, 0, sizeof(BasicWlanConfig_t));

	/* Get wireless mode and channel info from Basic Wlan Config */
	if (cfg_get_wlan_basic(&wlanConfig, radio) != T_SUCCESS)
	{
		printf("cfg_get_wlan_basic failed \n");
		return T_FAILURE;
	}
	getChanAndFreqInwlanConfig_channel(pchan, pfreq, wlanConfig.channel);
	//printf("channel: %s, chan: %d, freq: %d \n", wlanConfig.channel, *pchan, *pfreq);

	if (wlanConfig.extoffset == -1)
	{
		*pextendedChan = *pchan - 4;
		*pextendedFreq = chanNumtoMhz(*pextendedChan);
	}
	else if (wlanConfig.extoffset == 1)
	{
		*pextendedChan = *pchan + 4;
		*pextendedFreq = chanNumtoMhz(*pextendedChan);
	}
	else
	{
		*pextendedChan = 0;
		*pextendedFreq = 0;
	}




	switch (wlanConfig.mode)
	{
		case 0:
			if (strcmp(wlanConfig.channel, "SmartSelect") == 0)
			{
				*pchan = 36;
				*pfreq = 5180;
				*pextendedChan = 0;
				*pextendedFreq = 0;
			}

			break;
		case 1:
			break;
		case 2:

			if (strcmp(wlanConfig.channel, "SmartSelect") == 0)
			{
				*pchan = 1;
				*pfreq = 2412;
				*pextendedChan = 0;
				*pextendedFreq = 0;
			}
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			if (strcmp(wlanConfig.channel, "SmartSelect") == 0)
			{
				*pchan = 1;
				*pfreq = 2412;
				*pextendedChan = 0;
				*pextendedFreq = 0;
			}


			break;
		case 6:
			if (strcmp(wlanConfig.channel, "SmartSelect") == 0)
			{
				*pchan = 36;
				*pfreq = 5180;
				*pextendedChan = 0;
				*pextendedFreq = 0;
			}


		default:

			break;
	}
#endif
	return T_SUCCESS;

}

int downDevice(char *ifname)
{
	char down[25] = { 0 };
	sprintf(down, "ifconfig %s down", ifname);
	system(down);
	return 1;
}
int upDevice(char *ifname)
{
	char up[25] = { 0 };
	sprintf(up, "ifconfig %s up", ifname);
	system(up);
	return 1;
}



int DownUpDevice(char bUp, int radio)
{
	//TODO:Use nvram instead
#if 0
	char cmd[25] = { 0 };
	int ii = 0;
	char ifname[32];

	BasicWlanConfig_t wlanConfig;

	cfg_get_wlan_basic(&wlanConfig, radio);
	for (; ii < VAP_NUMBERS; ii++)
		if (wlanConfig.ProfilebEnabled[ii])
		{
			/* Peter modify 2008.7.24. only radio enabled can be up. */
			if (bUp && radio_enabled(radio))
			{
				construct_ifname(ifname, AP_MODE, radio, ii);
				sprintf(cmd, "ifconfig %s up", ifname);
			}
			else
			{
				construct_ifname(ifname, AP_MODE, radio, ii);
				sprintf(cmd, "ifconfig %s%d down", ifname);
			}
			system(cmd);
		}
#endif
	return 1;
}
#endif
#if 0
int DownUpDeviceWDS(char bUp)
{
	char cmd[25] = { 0 };
	int ii = 0;
	wds_wds_t wdswds_t;

	memset(&wdswds_t, 0, sizeof(wdswds_t));
	cfg_get_wds(&wdswds_t);
	for (ii = WDS_BEGIN_ID; ii < WDS_END_ID; ii++)
	{
		if (wdswds_t.record[ii - WDS_BEGIN_ID].IsActive)
		{
			if (bUp)
				sprintf(cmd, "ifconfig %s%d up", WDS_IFNAME, ii);
			else
				sprintf(cmd, "ifconfig %s%d down", WDS_IFNAME, ii);
			system(cmd);
		}
	}
	return 1;
}
#endif

#if 0
void getVapName(char *vapName, int *vapIndex)
{

	cliConfig_t cliConfig;

	getCliConfig(&cliConfig);
	sprintf(vapName, "ath%d", cliConfig.vap);
	*vapIndex = cliConfig.vap;
	return;
}
#endif

//Deleted by Mario Huang
#if 0
char *getOneAdvanceSetting(int ii, T_UINT8 ifindex)
{
	//TODO:Replace it by nvram
#if 0
	wlanAdvConfig_t wlanadvconfig;

	cfg_get_wlan_adv(&wlanadvconfig, ifindex);
	memset(g_advArr, 0, sizeof(g_advArr));
	switch (ii)
	{
		case ADV_DATE_RATE:
			strcpy(g_advArr, wlanadvconfig.cDataRate);
			return g_advArr;
		case ADV_TR_POWER:
			g_advSettingI = wlanadvconfig.iTRPower;
			return (char *) &g_advSettingI;
		case ADV_BEACON_INTERVAL:
			g_advSettingI = wlanadvconfig.iBeaconInterval;
			return (char *) &g_advSettingI;
		case ADV_BEACON_RATE:
			g_advSettingI = wlanadvconfig.iBeaconRate;
			return (char *) &g_advSettingI;
		case ADV_ASSO_TIMEOUT:
			g_advSettingI = wlanadvconfig.iAssoTimeout;
			return (char *) &g_advSettingI;
		case ADV_FRAG_LEN:
			g_advSettingI = wlanadvconfig.iFragLen;
			return (char *) &g_advSettingI;
		case ADV_THRESHOLD:
			g_advSettingI = wlanadvconfig.iThreshold;
			return (char *) &g_advSettingI;
		case ADV_11D_SUPPORT:
			g_advSettingC = wlanadvconfig.b11dSupport;
			return (char *) &g_advSettingC;
		case ADV_DISTANCE:
			g_advSettingI = wlanadvconfig.iDistance;
			return (char *) &g_advSettingI;
		case ADV_ANTEN_TYPE:
			g_advSettingI = wlanadvconfig.iAntenType;
			return (char *) &g_advSettingI;
		default:
			break;
	}
#endif
	return NULL;
}
#endif
  /***********************************************************************
 * Function Name : obscureMatch
 * Description    : match the string not only by whole words but by part of words
 * Input         : 
 * Return value  :MATCH_NONE, no string mactchs the inpurt
 *                      MATCH_FIRST, input match the first string
 *                      MATCH_SECOND, input match the second string
 *                      MATCH_NEEDMORE, need more information to decide the matchment
 ***********************************************************************/
int obscureMatch(char *first, char *second, char *input)
{
	if ((strlen(input) > strlen(first)) && (strlen(input) > strlen(second)))
		return MATCH_NONE;
	else if ((strlen(input) > strlen(first))
			&& (strlen(input) <= strlen(second)))
	{
		if (strncasecmp(input, second, strlen(input)) == 0)
			return MATCH_SECOND;
		else
			return MATCH_NONE;
	}
	else if ((strlen(input) <= strlen(first))
			&& (strlen(input) > strlen(second)))
	{
		if (strncasecmp(input, first, strlen(input)) == 0)
			return MATCH_FIRST;
		else
			return MATCH_NONE;
	}
	else if ((strlen(input) <= strlen(first))
			&& (strlen(input) <= strlen(second)))
	{
		if ((strncasecmp(input, first, strlen(input)) == 0)
				&& (strncasecmp(input, second, strlen(input)) == 0))
			return MATCH_NEEDMORE;
		if (strncasecmp(input, first, strlen(input)) == 0)
			return MATCH_FIRST;
		else if (strncasecmp(input, second, strlen(input)) == 0)
			return MATCH_SECOND;
		else
			return MATCH_NONE;
	}
	else
		return MATCH_NONE;

}

//Deleted by Mario Huang
#if 0
int PerfomeWlanAdvSetting_onlyOne_byName(int iname, T_UINT8 ifindex)
{
	//Deleted by Mario Huang
#if 0
	char down = 0;
	char up = 1;
	int opMode;

	getCurrentOpMode(&opMode, ifindex);
	switch (opMode)
	{
		case AP_MODE:
			DownUpDevice(down, ifindex);
			//DownUpDeviceWDS(down);
			UpDownWDSDev(down, ifindex);
			SetWlanAdvSetting_onlyOne(ifindex, iname);
			DownUpDevice(up, ifindex);
			//DownUpDeviceWDS(up);
			UpDownWDSDev(up, ifindex);
			break;
		case WBR_MODE:
			break;
		case AD_HOC_MODE:
			break;
		case REPEATER_MODE:
			break;
		case BRIDGE_MODE:
			//DownUpDeviceWDS(down);
			UpDownWDSDev(down, ifindex);
			SetWlanAdvSetting_onlyOne(ifindex, iname);
			//DownUpDeviceWDS(up);
			UpDownWDSDev(up, ifindex);
			break;
	}
#endif
	return 1;
}



void printTimeZoneByIndex(int index)
{
	switch (index)
	{
		case 1:
			uiPrintf("(GMT-12:00) Eniwetok,Kwajalein\n");
			break;
		case 2:
			uiPrintf("(GMT-11:00) Midway Island,Samoa\n");
			break;
		case 3:
			uiPrintf("(GMT-10:00) Hawaii\n");
			break;
		case 4:
			uiPrintf("(GMT-09:00) Alaska\n");
			break;
		case 5:
			uiPrintf("(GMT-08:00) Pacific Time (US & Canada);Tijuana\n");
			break;
		case 6:
			uiPrintf("(GMT-07:00) Arizona\n");
			break;
		case 7:
			uiPrintf("(GMT-07:00) Mountain Time (US & Canada)\n");
			break;
		case 8:
			uiPrintf("(GMT-06:00) Central America\n");
			break;
		case 9:
			uiPrintf("(GMT-06:00) Central Time (US & Canada)\n");
			break;
		case 10:
			uiPrintf("(GMT-06:00) Mexico City\n");
			break;
		case 11:
			uiPrintf("(GMT-06:00) Saskatchewan\n");
			break;
		case 12:
			uiPrintf("(GMT-05:00) Bogota,Lima,Quito\n");
			break;
		case 13:
			uiPrintf("(GMT-05:00) Eastern Time (US & Canada)\n");
			break;
		case 14:
			uiPrintf("(GMT-05:00) Indiana (East)\n");
			break;
		case 15:
			uiPrintf("(GMT-04:00) Atlantic Time (Canada)\n");
			break;
		case 16:
			uiPrintf("(GMT-04:00) Caracas,La Paz\n");
			break;
		case 17:
			uiPrintf("(GMT-04:00) Santiago\n");
			break;
		case 18:
			uiPrintf("(GMT-03:30) Newfoundland\n");
			break;
		case 19:
			uiPrintf("(GMT-03:00) Brasilia\n");
			break;
		case 20:
			uiPrintf("(GMT-03:00) Buenos Aires,Georgetown\n");
			break;
		case 21:
			uiPrintf("(GMT-03:00) Greenland\n");
			break;
		case 22:
			uiPrintf("(GMT-02:00) Mid-Atlantic\n");
			break;
		case 23:
			uiPrintf("(GMT-01:00) Azores\n");
			break;
		case 24:
			uiPrintf("(GMT-01:00) Cape Verde Is.\n");
			break;
		case 25:
			uiPrintf("(GMT) Casablanca,Monrovia\n");
			break;
		case 26:
			uiPrintf
				("(GMT) Greenwich Mean Time ; Dublin,Edinburgh,Lisbon,London\n");
			break;
		case 27:
			uiPrintf("(GMT+01:00) Amsterdam,Berlin,Bern,Rome,Stockholm,Vienna\n");
			break;
		case 28:
			uiPrintf("(GMT+01:00) Belgrade,Bratislava,Budapest,Ljubljana,Prague\n");
			break;
		case 29:
			uiPrintf("(GMT+01:00) Brussels,Copenhagen,Madrid,Paris\n");
			break;
		case 30:
			uiPrintf("(GMT+01:00) Sarajevo,Skopje,Sofija,Vilnius,Warsaw,Zagreb\n");
			break;
		case 31:
			uiPrintf("(GMT+01:00) West Central Africa\n");
			break;
		case 32:
			uiPrintf("(GMT+02:00) Athens,Istanbul,Minsk\n");
			break;
		case 33:
			uiPrintf("(GMT+02:00) Bucharest\n");
			break;
		case 34:
			uiPrintf("(GMT+02:00) Cairo\n");
			break;
		case 35:
			uiPrintf("(GMT+02:00) Harare,Pretoria\n");
			break;
		case 36:
			uiPrintf("(GMT+02:00) Helsinki,Riga,Tallinn\n");
			break;
		case 37:
			uiPrintf("(GMT+02:00) Jerusalem\n");
			break;
		case 38:
			uiPrintf("(GMT+03:00) Baghdad\n");
			break;
		case 39:
			uiPrintf("(GMT+03:00) Kuwait,Riyadh\n");
			break;
		case 40:
			uiPrintf("(GMT+03:00) Moscow,St.Petersburg,Volgograd\n");
			break;
		case 41:
			uiPrintf("(GMT+03:00) Nairobi\n");
			break;
		case 42:
			uiPrintf("(GMT+03:30) Tehran\n");
			break;
		case 43:
			uiPrintf("(GMT+04:00) Abu Dhabi,Muscat\n");
			break;
		case 44:
			uiPrintf("(GMT+04:00) Baku,Tbilisi,Yerevan\n");
			break;
		case 45:
			uiPrintf("(GMT+04:30) Kabul\n");
			break;
		case 46:
			uiPrintf("(GMT+05:00) Ekaterinburg\n");
			break;
		case 47:
			uiPrintf("(GMT+05:00) Islamabad,Karachi,Tashkent\n");
			break;
		case 48:
			uiPrintf("(GMT+05:30) Calcutta,Chennai,Mumbai,New Delhi\n");
			break;
		case 49:
			uiPrintf("(GMT+05:45) Kathmandu\n");
			break;
		case 50:
			uiPrintf("(GMT+06:00) Almaty,Novosibirsk\n");
			break;
		case 51:
			uiPrintf("(GMT+06:00) Astana,Dhaka\n");
			break;
		case 52:
			uiPrintf("(GMT+06:00) Sri Jayawardenepura\n");
			break;
		case 53:
			uiPrintf("(GMT+06:30) Rangoon\n");
			break;
		case 54:
			uiPrintf("(GMT+07:00) Bangkok,Hanoi,Jakarta\n");
			break;
		case 55:
			uiPrintf("(GMT+07:00) Krasnoyarsk\n");
			break;
		case 56:
			uiPrintf("(GMT+08:00) Beijing,Chongqing,Hong Kong,Urumqi\n");
			break;
		case 57:
			uiPrintf("(GMT+08:00) Irkutsk,Ulaan Bataar\n");
			break;
		case 58:
			uiPrintf("(GMT+08:00) Kuala Lumpur,Singapore\n");
			break;
		case 59:
			uiPrintf("(GMT+08:00) Perth\n");
			break;
		case 60:
			uiPrintf("(GMT+08:00) Taipei\n");
			break;
		case 61:
			uiPrintf("(GMT+09:00) Osaka,Sapporo,Tokyo\n");
			break;
		case 62:
			uiPrintf("(GMT+09:00) Seoul\n");
			break;
		case 63:
			uiPrintf("(GMT+09:00) Yakutsk\n");
			break;
		case 64:
			uiPrintf("(GMT+09:30) Adelaide\n");
			break;
		case 65:
			uiPrintf("(GMT+09:30) Darwin\n");
			break;
		case 66:
			uiPrintf("(GMT+10:00) Brisbane\n");
			break;
		case 67:
			uiPrintf("(GMT+10:00) canberra,Melbourne,Sydney\n");
			break;
		case 68:
			uiPrintf("(GMT+10:00) Guam,Port Moresby\n");
			break;
		case 69:
			uiPrintf("(GMT+10:00) Hobart\n");
			break;
		case 70:
			uiPrintf("(GMT+10:00) Vladivostok\n");
			break;
		case 71:
			uiPrintf("(GMT+11:00) Magadan,Solomon Is.,New Caledonia\n");
			break;
		case 72:
			uiPrintf("(GMT+12:00) Auckland,Wellington\n");
			break;
		case 73:
			uiPrintf("(GMT+12:00) Fiji,Kamchatka,Marshall Is.\n");
			break;
		case 74:
			uiPrintf("(GMT+13:00) Nuku'alofa\n");
			break;
		default:
			break;
	}
}

/*
   1982/11/01 10:30

   return value:
0:format is invalid

1:year is invalid
2:month is invalid
29:day is invalid
30:day is invalid
31:day is invalid
28:day is invalid

24:hour is invalid

59:Minute is invalid
100:OK
*/
char CheckTimeOfDays(char *pStr, timeofday_t * ptimeofday)
{
	if (NULL == pStr)
		return 0;
	if (strlen(pStr) < strlen("1980/1/1 1:1"))  /*The minimum length */
		return 0;

	char strYear[5] = { 0 };
	int iYear;
	char strMon[3] = { 0 };
	int iMon;
	char strDay[3] = { 0 };
	int iDay;
	char strHour[3] = { 0 };
	int iHour;
	char strMin[3] = { 0 };
	int iMin;
	int ii = 0;
	int jj = 0;
	char isLeapYear;

	/*Check Year */
	while (ii != 4)
	{
		strYear[jj] = pStr[ii];
		jj++;
		ii++;
	}
	strYear[4] = '\0';
	if (pStr[4] != '/')
		return 0;
	if (!isPositiveNumber(strYear))
		return 1;
	strToNum(strYear, strlen(strYear), &iYear);
	if ((iYear < 1970) || (iYear > 2100))
		return 1;
	isLeapYear = Authorware(iYear);
	/*Now 1980/ */
	ii = 5;



	/*Now pStr[4] is equal to / */
	/*Check month */
	if (pStr[ii + 1] != '/')
		if (pStr[ii + 2] != '/')
			return 0;
	if ((pStr[ii + 1] == '/') && (pStr[ii + 2] != '/')) /*1980/1/ */
	{
		strMon[0] = pStr[ii];
		strMon[1] = '\0';
		if (!isPositiveNumber(strMon))
			return 0;
		strToNum(strMon, strlen(strMon), &iMon);
		strMon[0] = '0';
		strMon[1] = pStr[ii];
		strMon[2] = '\0';
		ii = 7;
	}
	if ((pStr[ii + 1] != '/') && (pStr[ii + 2] == '/'))
	{
		strMon[0] = pStr[ii];
		strMon[1] = pStr[ii + 1];
		strMon[2] = '\0';
		if (!isPositiveNumber(strMon))
			return 0;
		strToNum(strMon, strlen(strMon), &iMon);
		if (iMon > 12)
			return 2;
		ii = 8;
	}




	/*Check day */
	if ((pStr[ii] >= '0') && (pStr[ii] <= '9'))
	{
		if ((pStr[ii + 1] >= '0') && (pStr[ii + 1] <= '9'))
		{
			strDay[0] = pStr[ii];
			strDay[1] = pStr[ii + 1];
			strDay[2] = '\0';
			strToNum(strDay, strlen(strDay), &iDay);
			if (isLeapYear)
			{
				if (iMon == 2)
				{
					if (iDay > 29)
						return 29;
				}
				else if ((iMon == 1) ||
						(iMon == 3) ||
						(iMon == 5) ||
						(iMon == 7) ||
						(iMon == 8) || (iMon == 10) || (iMon == 12))
				{
					if (iDay > 31)
						return 31;
				}
				else if (iDay > 30)
					return 30;
			}
			else
			{
				if (iMon == 2)
				{
					if (iDay > 28)
						return 28;
				}
				else if ((iMon == 1) ||
						(iMon == 3) ||
						(iMon == 5) ||
						(iMon == 7) ||
						(iMon == 8) || (iMon == 10) || (iMon == 12))
				{
					if (iDay > 31)
						return 31;
				}
				else if (iDay > 30)
					return 30;
			}
			ii += 2;

		}
		else if (pStr[ii + 1] == ' ')
		{
			strDay[0] = pStr[ii];
			strDay[1] = '\0';
			if (!isPositiveNumber(strDay))
				return 0;
			strToNum(strDay, strlen(strDay), &iDay);
			strDay[0] = '0';
			strDay[1] = pStr[ii];
			strDay[2] = '\0';
			ii += 1;
		}
		else
			return 0;
	}
	else
		return 0;

	if (pStr[ii] != ' ')
		return 0;


	while (pStr[ii] != '\0')
	{
		if ((pStr[ii] >= '0') && (pStr[ii] <= '9'))
			break;
		ii++;
	}


	if (pStr[ii] == '\0')
		return 0;

	if (pStr[ii + 1] == '\0')
		return 0;

	if (pStr[ii + 2] == '\0')
		return 0;

	if ((pStr[ii + 1] != ':') && (pStr[ii + 2] != ':'))
		return 0;
	if ((pStr[ii + 1] == ':') && (pStr[ii + 2] == ':'))
		return 0;
	if (pStr[ii + 1] == ':')
	{
		strHour[0] = pStr[ii];
		strHour[1] = '\0';
		if (!isPositiveNumber(strHour))
			return 0;
		strToNum(strHour, strlen(strHour), &iHour);
		strHour[0] = '0';
		strHour[1] = pStr[ii];
		strHour[2] = '\0';
		ii += 2;
	}
	if (pStr[ii + 2] == ':')
	{
		strHour[0] = pStr[ii];
		strHour[1] = pStr[ii + 1];
		strHour[2] = '\0';
		if (!isPositiveNumber(strHour))
			return 0;
		strToNum(strHour, strlen(strHour), &iHour);
		if (iHour > 24)
		{
			return 24;
		}
		ii += 3;
	}
	if (pStr[ii] == '\0')
		return 0;
	if (pStr[ii + 1] != '\0')
	{
		if (pStr[ii + 2] != '\0')
			return 0;
		else
		{
			strMin[0] = pStr[ii];
			strMin[1] = pStr[ii + 1];
			strMin[2] = '\0';
			if (!isPositiveNumber(strMin))
				return 0;
			strToNum(strMin, strlen(strMin), &iMin);
			if (iMin > 59)
				return 59;

		}
	}
	else
	{
		strMin[0] = pStr[ii];
		strMin[1] = '\0';
		if (!isPositiveNumber(strMin))
			return 0;
		strToNum(strHour, strlen(strHour), &iHour);
		strMin[0] = '0';
		strMin[1] = pStr[ii];
		strMin[2] = '\0';
	}
	strcpy(ptimeofday->pYear, strYear);
	strcpy(ptimeofday->pMonth, strMon);
	strcpy(ptimeofday->pDay, strDay);
	strcpy(ptimeofday->pHour, strHour);
	strcpy(ptimeofday->pMin, strMin);
	ptimeofday->iMon = iMon;
	ptimeofday->iDay = iDay;
	ptimeofday->iMin = iMin;
	ptimeofday->iYear = iYear;
	ptimeofday->iHour = iHour;

	return 100;
}

char CheckDaylighttime(char *pStr, daylighttime_t * pDaylighttime, int year)
{
	if (NULL == pStr)
		return 0;
	if (strlen(pStr) < strlen("1/1-1/1"))   /*The minimum length */
		return 0;
	if (strlen(pStr) > strlen("11/11-11/11"))
		return 0;
	int ii = 0;
	char sMon[3] = { 0 };
	int isMon;
	char sDay[3] = { 0 };
	int isDay;
	char eMon[3] = { 0 };
	int ieMon;
	char eDay[3] = { 0 };
	int ieDay;
	int isLeapyear = Authorware(year);

	if ((pStr[1] != '/') && (pStr[2] != '/'))
		return 0;
	if ((pStr[1] == '/') && (pStr[2] == '/'))
		return 0;

	if (pStr[1] == '/')
	{
		sMon[0] = pStr[ii];
		sMon[1] = '\0';
		if (!isPositiveNumber(sMon))
			return 0;
		strToNum(sMon, strlen(sMon), &isMon);
		sMon[0] = '0';
		sMon[1] = pStr[ii];
		sMon[2] = '\0';
		ii = 2;
	}
	if (pStr[2] == '/')
	{
		sMon[0] = pStr[ii];
		sMon[1] = pStr[ii + 1];
		sMon[2] = '\0';
		if (!isPositiveNumber(sDay))
			return 0;
		strToNum(sMon, strlen(sMon), &isMon);
		if (isMon > 12)
			return 13;
		ii = 3;
	}


	if ((pStr[ii + 1] != '-') && (pStr[ii + 2] != '-'))
		return 0;
	if ((pStr[ii + 1] == '-') && (pStr[ii + 2] == '-'))
		return 0;

	if (pStr[ii + 1] == '-')
	{
		sDay[0] = pStr[ii];
		sDay[1] = '\0';
		if (!isPositiveNumber(sDay))
			return 0;
		strToNum(sDay, strlen(sDay), &isDay);
		sDay[0] = '0';
		sDay[1] = pStr[ii];
		sDay[2] = '\0';
		ii += 2;
	}
	if (pStr[ii + 2] == '-')
	{
		sDay[0] = pStr[ii];
		sDay[1] = pStr[ii + 1];
		sDay[2] = '\0';
		if (!isPositiveNumber(sDay))
			return 0;
		strToNum(sDay, strlen(sDay), &isDay);
		if (isLeapyear)
		{
			if (isMon == 2)
				if (isDay > 29)
					return 30;
		}
		else
		{
			if ((isMon == 1) ||
					(isMon == 3) ||
					(isMon == 5) ||
					(isMon == 7) || (isMon == 8) || (isMon == 10) || (isMon == 12))
			{
				if (isDay > 31)
					return 32;
			}
			else if (isMon == 2)
			{
				if (isDay > 28)
					return 29;

			}
			else
			{
				if (isDay > 30)
					return 31;
			}
		}
		ii += 3;
	}


	if ((pStr[ii + 1] != '/') && (pStr[ii + 2] != '/'))
		return 0;
	if ((pStr[ii + 1] == '/') && (pStr[ii + 2] == '/'))
		return 0;
	if (pStr[ii + 1] == '/')
	{
		eMon[0] = pStr[ii];
		eMon[1] = '\0';
		if (!isPositiveNumber(eMon))
			return 0;
		strToNum(eMon, strlen(eMon), &ieMon);
		eMon[0] = '0';
		eMon[1] = pStr[ii];
		eMon[2] = '\0';
		ii += 2;
	}
	if (pStr[ii + 2] == '/')
	{
		eMon[0] = pStr[ii];
		eMon[1] = pStr[ii + 1];
		eMon[2] = '\0';
		if (!isPositiveNumber(eMon))
			return 0;
		strToNum(eMon, strlen(eMon), &ieMon);
		if (ieMon > 12)
			return 13;
		ii += 3;
	}

	if (pStr[ii] == '\0')
		return 0;

	if (pStr[ii + 1] == '\0')
	{
		eDay[0] = pStr[ii];
		eDay[1] = '\0';
		if (!isPositiveNumber(eDay))
			return 0;
		strToNum(eDay, strlen(eDay), &ieDay);
		eDay[0] = '0';
		eDay[1] = pStr[ii];
		eDay[2] = pStr[ii + 1];
		if (isLeapyear)
		{
			if (isMon == 2)
				if (ieDay > 29)
					return 30;
		}
		else
		{
			if ((ieMon == 1) ||
					(ieMon == 3) ||
					(ieMon == 5) ||
					(ieMon == 7) || (ieMon == 8) || (ieMon == 10) || (ieMon == 12))
			{
				if (ieDay > 31)
					return 32;
			}
			else if (ieMon == 2)
			{
				if (ieDay > 28)
					return 29;

			}
			else
			{
				if (ieDay > 30)
					return 31;
			}
		}
	}
	else if (pStr[ii + 2] == '\0')
	{
		eDay[0] = pStr[ii];
		eDay[1] = pStr[ii + 1];
		eDay[2] = '\0';
		if (!isPositiveNumber(eDay))
			return 0;
		strToNum(eDay, strlen(eDay), &ieDay);
	}
	strcpy(pDaylighttime->eDay, eDay);
	strcpy(pDaylighttime->eMon, eMon);
	strcpy(pDaylighttime->sDay, sDay);
	strcpy(pDaylighttime->sMon, sMon);

	pDaylighttime->ieDay = ieDay;
	pDaylighttime->ieMon = ieMon;
	pDaylighttime->isDay = isDay;
	pDaylighttime->isMon = isMon;

	return 1;
}

char isBssidValid(char *bssid)
{
	if (strlen(bssid) != strlen("00:00:00:00:00:11"))
		return 0;
	if (strcmp(bssid, "00:00:00:00:00:00") == 0)
		return 0;
	if ((((bssid[0] >= '0') && (bssid[0] <= '9'))
				|| ((bssid[0] >= 'a') && (bssid[0] <= 'f')) || ((bssid[0] >= 'A')
					&& (bssid[0] <= 'F')))
			&& (((bssid[3] >= '0') && (bssid[3] <= '9'))
				|| ((bssid[3] >= 'a') && (bssid[3] <= 'f')) || ((bssid[3] >= 'A')
					&& (bssid[3] <=
						'F')))
			&& (((bssid[4] >= '0') && (bssid[4] <= '9'))
				|| ((bssid[4] >= 'a') && (bssid[4] <= 'f')) || ((bssid[4] >= 'A')
					&& (bssid[4] <=
						'F')))
			&& (((bssid[6] >= '0') && (bssid[6] <= '9'))
				|| ((bssid[6] >= 'a') && (bssid[6] <= 'f')) || ((bssid[6] >= 'A')
					&& (bssid[6] <=
						'F')))
			&& (((bssid[7] >= '0') && (bssid[7] <= '9'))
				|| ((bssid[7] >= 'a') && (bssid[7] <= 'f')) || ((bssid[7] >= 'A')
					&& (bssid[7] <=
						'F')))
			&& (((bssid[9] >= '0') && (bssid[9] <= '9'))
				|| ((bssid[9] >= 'a') && (bssid[9] <= 'f')) || ((bssid[9] >= 'A')
					&& (bssid[9] <=
						'F')))
			&& (((bssid[10] >= '0') && (bssid[10] <= '9'))
					|| ((bssid[10] >= 'a') && (bssid[10] <= 'f')) || ((bssid[10] >= 'A')
						&& (bssid[10] <=
							'F')))
			&& (((bssid[12] >= '0') && (bssid[12] <= '9'))
					|| ((bssid[12] >= 'a') && (bssid[12] <= 'f')) || ((bssid[12] >= 'A')
						&& (bssid[12] <=
							'F')))
			&& (((bssid[13] >= '0') && (bssid[13] <= '9'))
					|| ((bssid[13] >= 'a') && (bssid[13] <= 'f')) || ((bssid[13] >= 'A')
						&& (bssid[13] <=
							'F')))
			&& (((bssid[15] >= '0') && (bssid[15] <= '9'))
					|| ((bssid[15] >= 'a') && (bssid[15] <= 'f')) || ((bssid[15] >= 'A')
						&& (bssid[15] <=
							'F')))
			&& (((bssid[16] >= '0') && (bssid[16] <= '9'))
					|| ((bssid[16] >= 'a') && (bssid[16] <= 'f')) || ((bssid[16] >= 'A')
						&& (bssid[16] <=
							'F'))))
							{
							}
	else
		return 0;

	if ((bssid[1] == '1') || (bssid[1] == '3') || (bssid[1] == '5')
			|| (bssid[1] == '7') || (bssid[1] == '9') || (bssid[1] == 'b')
			|| (bssid[1] == 'd') || (bssid[1] == 'f') || (bssid[1] == 'B')
			|| (bssid[1] == 'D') || (bssid[1] == 'F'))
		return 0;

	if ((bssid[2] != ':') && (bssid[5] != ':') && (bssid[8] != ':')
			&& (bssid[11] != ':') && (bssid[14] != ':'))
		return 0;


	return 1;


}


char *isoTocuntrycodeFunc(char *iso)
{
	int ii = 0;

	while (ii < 99)
	{
		if (strncasecmp(iso, allCountries[ii].name, strlen(iso)) == 0)
			return (char *)(allCountries[ii].isoName);
		ii++;
	}
	return iso;


}
#endif
  /***********************************************************************
 * Function Name : setCliConfig
 * Description    : set command process
 * Input         : cliConfig, cli to be set
 * Return value  :T_SUCCESS, set successful
 ***********************************************************************/
T_INT32 setCliConfig(cliConfig_t * cliConfig)
{
	g_curVap = cliConfig->vap;
	//Deleted by Mario Huang, Close it temporary
#if 0
#if defined(WallAP)
	curPort = cliConfig->port;
#endif
#endif
	return T_SUCCESS;
}
  /***********************************************************************
 * Function Name : getCliConfig
 * Description    : get command process
 * Input         : cliConfig, cli to be get
 * Return value  :T_SUCCESS, get successful
 ***********************************************************************/
T_INT32 getCliConfig(cliConfig_t * cliConfig)
{
	cliConfig->vap = g_curVap;
	//Added by Mario Huang
	//printf("cliConfig->vap is %d\n",cliConfig->vap);
	//Deleted by Mario Huang, Close it temporary
#if 0
#if defined(WallAP)
	cliConfig->port = curPort;
#endif
#endif
	return T_SUCCESS;
}

 /***********************************************************************
 * Function Name : cliConfirm
 * Description    : cli confirming, 'y' is confirm and 'n' is not.
 * Input         : msg, confirm the message
 * Return value  :0, successful
 ***********************************************************************/
int cliConfirm(const char *msg)
{
	char ch, buf[2];

	uiPrintf(msg);
	uiPrintf(" (y or n)");

	for (;;)
	{
		if (read(1, buf, 1) != ERROR)
		{
			ch = *buf;
			if (ch == 'y' || ch == 'Y')
			{
				uiPrintf("\n");
				return 1;
			}
			else if (ch == 'n' || ch == 'N')
			{
				uiPrintf("\n");
				return 0;
			}
			else if (ch == '\b')
				uiPrintf(" \b");
			else
				uiPrintf("\b \b");
		}
		else
		{
			uiPrintf("read error!\n");
			break;
		}
	}return 0;
}
 /***********************************************************************
 * Function Name : show_hwaddr
 * Description    : according to the ifname getting the mac address of interface
 * Input         : ifname, ptr, length
 * Return value  :T_TRUE, legal ip address
 *                      T_FALSE,illegal ip address
 ***********************************************************************/

int show_hwaddr(char *ifname, char *ptr, int length)
{
    int fd = -1, n;
    unsigned char *tmp;
    struct ifreq ifr;

    if ((fd = socket (AF_INET, SOCK_DGRAM, IPPROTO_IP)) <= -1)
        goto err;

    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy (ifr.ifr_name, ifname, IFNAMSIZ);

    if (ioctl(fd, SIOCGIFHWADDR, &ifr) < 0)
        goto err;

    tmp = (unsigned char *) &ifr.ifr_hwaddr.sa_data[0];

    n = snprintf(ptr, length, "%02X:%02X:%02X:%02X:%02X:%02X", 
            tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], tmp[5]);

    if (n <= 0)
        goto err;

    close(fd);
    return 0;

err:
    if (fd > -1)
        close(fd);
	
    return 1;
}

 /***********************************************************************
 * Function Name : IsValidServerIpAddress
 * Description    : No Broadcast Byte allowed in address, check each Byte
 * Input         : IpAddr, ip address
 * Return value  :T_TRUE, legal ip address
 *                      T_FALSE,illegal ip address
 ***********************************************************************/
T_BOOL8 IsValidServerIpAddress(T_CHAR8 * IpAddr)
{
	T_INT32 ii, kk[4];

	for (ii = 24; ii >= 0; ii -= 8)
	{
		/* Skip any leading stuff (e.g., spaces, '[') */
		while (IpAddr != '\0' && !isdigit(*IpAddr))
			IpAddr++;
		kk[ii / 8] = (T_INT32) atoi(IpAddr);
		if (kk[ii / 8] < 0 || 255 < kk[ii / 8])
			return T_FALSE;
		if (((IpAddr = strchr(IpAddr, '.')) == NULL) && (ii > 0))
			return T_FALSE;
		IpAddr++;
	}

	/*0.x.x.x is not allowed */
	if (kk[3] == 0)
		return T_FALSE;
	/*127.0.0.0, 224~255.0.0.0 is not allowed */
	if ((kk[3] == 127 || (kk[3] >= 224 && kk[3] <= 255)) && !kk[2] && !kk[1]
			&& !kk[0])
		return T_FALSE;
	return T_TRUE;
}

/*<-----*/
