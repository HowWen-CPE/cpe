/******************************************************************************
* Copyright (C), 2014, LiteOn Technology Corp.
* Filename: cli.h
* Description: AP CLI interface definitions.
* Created by: Mario Huang
* Revision history:
* Revision  Date                  Author                 Description
* -----------------------------------------------------------------------------------------------------------------
* v0.1     2014/05/05     Mario Huang              Create this file
*/

#ifndef _CLI_H_
#define _CLI_H_

typedef char A_CHAR;
typedef A_CHAR A_INT8;
typedef short A_INT16;
typedef long A_INT32;

typedef unsigned char A_UCHAR;
typedef A_UCHAR A_UINT8;
typedef unsigned short A_UINT16;
typedef unsigned long A_UINT32;
typedef unsigned long A_UINT;
typedef unsigned long UINT;

typedef int A_BOOL;
typedef long long A_INT64;
typedef unsigned long long A_UINT64;
typedef unsigned long ULONGLONG;

#define A_LL                    "ll"

typedef A_UINT64 A_LONGSTATS;
typedef A_INT8 A_RSSI;
typedef A_INT32 A_RSSI32;

//Added by Mario Huang start
typedef unsigned long ULONG;
typedef unsigned short USHORT;
typedef unsigned int T_UINT32;
typedef unsigned short T_UINT16;

typedef unsigned char T_UINT8;
typedef short T_INT16;
typedef int T_INT32;
typedef char T_INT8;
typedef unsigned char T_UCHAR8;
typedef char T_CHAR8;
typedef char T_BOOL8;

/*typedef double          T_FLOAT256;*/
typedef void VOID;
typedef void T_VOID;
typedef T_UINT32 T_SEMID_t;
typedef T_UINT32 Semaphore_t;
//Added by Mario Huang end

#ifndef LOCAL
#define LOCAL static
#endif

/* Standard Definitions */
#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

#ifndef OK
#define OK      0
#endif

#ifndef NULL
#define NULL    ((void*)0)
#endif

/* for the CompareType type */

#ifndef EQUAL
#define EQUAL           0
#endif

#ifndef GREATER_THAN
#define GREATER_THAN    1
#endif

#ifndef LESS_THAN
#define LESS_THAN       -1
#endif

#define STD_IN          0
#define STD_OUT         1
#define STD_ERR         2

#define MAX_TOKEN       50

/*dennis modify for cli. 2008-03-25. ----->*/

/*Note: In case the max length of system name etc. is 255, 256 is not enough for both command and data*/
#define CLI_INBUF       /*256*/ 4096

/*<-----*/
//Modified by Mario Huang
#if defined (GP_WP838) || defined (GP_WP688) || defined(GP_WR869)
#define MAX_LOGIN_ATTEMPT       3
#else
#define MAX_LOGIN_ATTEMPT       5
#endif

//#include "dev_cfg.h"

#ifdef WP741AP
#define SYS_NAME "WP741 AP"
#define CLI_SUPERUSER   "WP741"
#define CLI_SUPWD   "WP741"
#endif
#ifdef WP731AP
#define SYS_NAME "WP731 AP"
#define CLI_SUPERUSER   "WP731"
#define CLI_SUPWD   "WP731"
#endif
#ifdef WP755AP
#define SYS_NAME "WP755 AP"
#define CLI_SUPERUSER   "WP755"
#define CLI_SUPWD   "WP755"
#endif
#ifdef WP732AP
#define SYS_NAME "WP732 AP"
#define CLI_SUPERUSER   "WP732"
#define CLI_SUPWD   "WP732"
#endif

#ifdef WP735AP
#define SYS_NAME "WP735 AP"
#define CLI_SUPERUSER   "WP735"
#define CLI_SUPWD   "WP735"
#endif


#ifdef WP733AP
#define SYS_NAME "WP733 AP"
#define CLI_SUPERUSER   "WP733"
#define CLI_SUPWD   "WP733"
#endif

//Deleted by Mario Huang, Close it temporary
#if 0
#if defined(WP854AP) && defined(WP713AP)
#define SYS_NAME "WP854 AP"
#define CLI_SUPERUSER   "WP854"
#define CLI_SUPWD   "WP854"
#elif defined(WP841AP) && defined(WP713AP)
#define SYS_NAME "WP841 AP"
#define CLI_SUPERUSER   "WP841"
#define CLI_SUPWD   "WP841"
#elif defined(WallAP) && defined(WP713AP)
#define SYS_NAME "WP733 AP"
#define CLI_SUPERUSER   "WP733"
#define CLI_SUPWD   "WP733"
#elif defined(WP713AP)
#define SYS_NAME "WP713 AP"
#define CLI_SUPERUSER   "WP713"
#define CLI_SUPWD   "WP713"
#endif
#endif
//Added by Mario Huang for GP
#if defined(GP_WP838)
#define SYS_NAME "GP_WP838"
#define CLI_SUPERUSER   "WP838"
#define CLI_SUPWD   "WP838"
#elif defined(GP_WP688)
#define SYS_NAME "GP_WP688"
#define CLI_SUPERUSER   "WP688"
#define CLI_SUPWD   "WP688"
#elif  defined(GP_WR869)
#define SYS_NAME "GP_WR869"
#define CLI_SUPERUSER   "WR869"
#define CLI_SUPWD   "WR869"
#elif  defined(GP_WP868)
#define SYS_NAME "GP_WP868"
#define CLI_SUPERUSER   "WP868"
#define CLI_SUPWD   "WP868"
#endif


#define VER_STR "1.6.26"
#define CLI_BANNER      "\nCPE Rev " VER_STR "\n"
#define CLI_WLAN_PROMPT "wlan"
#define CLI_PROMPT      "->"
#define CLI_LAN_PROMPT "lan"

/* default CLI super user name and password, add by Tony 2008.3.12 --> */
/* <-- add by Tony 2008.3.12 */
#define CLI_HIDDENCMD   "."

#define ERROR (-1)
#define A_ERROR (-1)
#define A_OK 0

#define A_SUCCESS(x)        (x == A_OK)
#define A_FAILED(x)         (!A_SUCCESS(x))

#define CLI_PARSE_OK                    A_OK    /*  0 */
#define CLI_PARSE_ERROR                 A_ERROR /* -1 */
#define CLI_PARSE_NO_TBL                1
#define CLI_PARSE_NO_VALUE              2
#define CLI_PARSE_TOO_FEW               3
#define CLI_PARSE_TOO_MANY              4
#define CLI_PARSE_UNKNOWN               5
#define CLI_PARSE_INPUT_ERROR           6
#define CLI_PARSE_INVALID_PARAMETER     7
#define CLI_PARSE_MULTI_MATCHES              8
#define CLI_PARSE_QUIT                 	 9999
#define CLI_PARSE_NOMESSAGE             9
#define CLI_PARSE_ACCESS_LINUX          99999

#if 0
#define SETFALIED      "Set failed.\n"
#endif
#define SETFALIED      ""

#define CLI_TOKEN_WILDCARD              "*"
#define CLI_TOKEN_NULL_STRING           "\"\""

//Added by Mario Huang
#define MAX_USER_LEN            ( 64 + 1 )
#define MAX_PASSWORD_LEN        33
#define PRIVL_MGN_ENABLE        1
#define PRIVL_MGN_DISABLE       2

//for debug cli
static T_BOOL8 cli_debug = 0;

typedef struct UserInfo_s
{
	unsigned char UserName[MAX_USER_LEN];
	unsigned char PassWord[MAX_PASSWORD_LEN];
	int privl;
} UserInfo_t;

typedef struct country_config_s
{
	int iCountryCode;       /* country code */
	char cCountryName[64];   /* country name */
} country_config_t;


typedef struct lan_ip_config_s
{
	char bIsDhcp;
	unsigned long uiIpAddr;
	unsigned long uiNetMask;
	unsigned long uiGateway;
} lan_ip_config_t;


//Added by Mario Huang for GPA Start====================================>
typedef struct cliConfig_s
{
	int wlanX;
	int vap;
	//Deleted by Mario Huang, Close it temporary
#if defined (WallAP)
	int port;
#endif
} cliConfig_t;
#if 0
typedef struct sys_property_s
{
	char cSglRadioDevName[256];
	char cDualRadioDevName[256];
	char cDevLocation[256];
	/* 2008.7.24. Peter modify. each radio mode separation and radio enable/disable. */
	int R24G_enable;
	int R5G_enable;
	int R24G_opmode;
	int R5G_opmode;
	int iBootMode;
	/*Added by frank for WallAP start*/
	char bEnableStat;
	/*Added by frank for WallAP end*/
} sys_property_t;
#endif
//<=====================================Added by Mario Huang for GPA End
struct cli_s
{
	struct parse_token_s *ParseTbl[MAX_TOKEN];
	A_BOOL securityCheck;
	//Deleted by Mario Huang
	//A_BOOL autoLogOff;
	int fin;
	int fout;
	int pid;
	int unit;                   /* radio unit */
	char *token[MAX_TOKEN];
	//Deleted by Mario Huang
	char *pToken;
	//char *pToken0;
	int tokenIdx;
	int tokenLvl;
	//Deleted by Mario Huang
	//int token_count;
	int parseTblIdx;
	int keyLast;
	char ibuf[CLI_INBUF];
	int vap;
	//Deleted by Mario Huang, Close it temporary
#if 0
#if defined (WallAP)
	int port;
#endif
#endif
};

typedef struct cli_s CLI;

struct apply_action_s
{
	int (*handler) (void);      /* Handler function for applying config */
	int settingCnt;             /* count of related 'set' cmd excution */
};

struct parse_token_s
{
	char *pCmd;
	char *pHelpText;
	int (*fHandler) (struct cli_s *, char *, struct parse_token_s *);
	struct apply_action_s *pApplyActionTbl;
	struct parse_token_s *pNxtParseTbl;
	char *pProtected;
	int opMode;                 /* Wireless mode */
	int abbrevMatchCnt;
	//Deleted by Mario Huang because it is useless
	//A_BOOL cfgChanged;          /* whether this part of config is changed */
};
//Deleted by Mario Huang 2014-04-18
#if 0
typedef enum
{
	CLI_STATIC_TURBO = 1,
	CLI_DYNAMIC_TURBO = 2
} CLI_TURBO_TYPE;
#endif

A_INT16 uiPrintf(const char *format, ...);
int tokenCount(CLI *);
char *tokenPopWoCase(CLI *);
char *tokenPop(CLI *);
//int tokenPopNum(CLI *, A_UINT32 * pV, int base);
int helpCmdScan(CLI * pCli, struct parse_token_s *pTbl, char *pTxt,
		char *pFilter, int verbose);

void tokenCurTblSet(struct parse_token_s *pTbl);
struct parse_token_s *tokenCurTblGet();

int genericCmdHandler(CLI * pCli, char *p, struct parse_token_s *pTbl);
int helpCmdHandler(CLI * pCli, char *p, struct parse_token_s *pTbl);

#endif /* _CLI_H_ */
