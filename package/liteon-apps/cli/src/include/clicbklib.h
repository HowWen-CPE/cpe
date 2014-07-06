#ifndef _CLICBKLIB_H_
#define  _CLICBKLIB_H_
//Deleted by Mario Huang
//#include "sysconf.h"

#define T_SUCCESS  0
#define T_FAILURE  -1

#define T_TRUE            1
#define T_FALSE           0

#define MAX_STRING_LEN 256
#define ERR_OVERFLOW 5


//#ifndef VAP_COMMONNAME
//#define VAP_COMMONNAME               "wifi0"
//#endif

#ifndef VAP_NUMBERS
/*Modified by Mario Huang because VAP numbers is 8 in Wall AP program*/
#ifdef SUPPORT_WALLAP
#define VAP_NUMBERS                       8
#else
#define VAP_NUMBERS                       4
#endif
#endif

//#ifndef VAP_NAME
//#define VAP_NAME                               "ath"
//#endif

#define SET_STR   "set"
#define TIMEOFDAY_STR "timeofday"
#define TIMEOFZONE_STR "timezone"
#define FACT_DEF                "factorydefault"
#define ADD_STR               "add"
//Added by Mario Huang
#include "cli.h"

typedef struct timeofday_s
{
	char pYear[5];
	int iYear;

	char pMonth[3];
	int iMon;

	char pDay[3];
	int iDay;

	char pHour[3];
	int iHour;

	char pMin[3];
	int iMin;
} timeofday_t;

typedef struct daylighttime_s
{
	char sDay[3];
	int isDay;
	char sMon[3];
	int isMon;
	char eDay[3];
	int ieDay;
	char eMon[3];
	int ieMon;
} daylighttime_t;

/*add by simon for country list*/
typedef struct
{

	const char *isoName;
	const char *name;
} isoTocuntrycode;


unsigned long strToUnsigned(char *pStr);
int verifyIP(char *pIP);
double pow_10(int exp);
long StrToLong(char *pStr);
char isPositiveNumber(char *tmp);
char *getPasswordFromTerminal();
//Deleted by Mario Huang
//int StartHttpServer(http_type_t httpConfig);

int strToNum(char *tmp, int size, int *num);

int getChannelAndFreq_AP(int *pchan, int *pfreq, int *pextendedChan,
		int *pextendedFreq, int radio);
int getChannelAndFreq_WDS(int *pchan, int *pfreq, int *pextendedChan,
		int *pextendedFreq, int radio);


int getCurrentMode();

int DownUpDevice(char bUp, int radio);

//int DownUpDeviceWDS(char bUp);

int downDevice(char *ifname);
int upDevice(char *ifname);

//void getVapName(char *vapName, int *vapIndex);
char *getOneAdvanceSetting(int ii, T_UINT8 ifindex);
void printTimeZoneByIndex(int index);
char *isoTocuntrycodeFunc(char *iso);


#define 	MATCH_FIRST 1
#define   MATCH_SECOND 2
#define   MATCH_NEEDMORE  3
#define   MATCH_NONE    0

int obscureMatch(char *first, char *second, char *input);
int PerfomeWlanAdvSetting_onlyOne_byName(int iname, unsigned char ifindex);
char CheckTimeOfDays(char *pStr, timeofday_t * ptimeofday);
char CheckDaylighttime(char *pStr, daylighttime_t * pDaylighttime, int year);
char isBssidValid(char *bssid);

int cliConfirm(const char *msg);


/*dennis add for cli. 2008-03-26. ----->*/
char IsValidServerIpAddress(char * IpAddr);
int get_wlan_power(int *ppt_value, int *ptxpl_value, int radio);
int set_wlan_power(int pt, int radio);

/*<-----*/
//Added by Mario Huang
T_BOOL8 IsValidIpAddress(T_CHAR8 *IpAddr);
int Authorware(int year);
char *sub_replace(char *source, char *sub, char *rep);
void del_space(char *Buf1, char* Buf2);
int check_systemname(char *str);
int get_sysname_from_nvram(char *sysname);
T_INT32 getCliConfig(cliConfig_t * cliConfig);

int show_hwaddr(char *ifname, char *ptr, int length);
int is_integer(char *str);


#endif
