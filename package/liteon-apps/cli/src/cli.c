/******************************************************************************
* Copyright (C), 2014, LiteOn Technology Corp.
* Filename: cli.c
* Description: Command line interface.
* Created by: Mario Huang
* Revision history:
* Revision  Date                Author           Description
* -----------------------------------------------------------------------------------------------------------------
* v0.1     2014/05/05     Mario Huang              Create this file
*/
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <signal.h>
#include <termios.h>
#include <sys/socket.h>
#include "cli.h"
#include "clicbk.h"
#include "clicbklib.h"
#include "readline.h"
//Added by Mario Huang start
#include "nvram.h"
#include "nvram_rule.h"
#include "mid_func.h"
#include "mid_detail.h"
#include <errno.h>
#include <linux/if.h>
//Added by Mario Huang end

#ifdef  DEBUG
#define CLIDEBUG(arg)  uiPrintf arg
#else
#define CLIDEBUG(arg)
#endif

#ifndef MAX_STRING_LEN
#define MAX_STRING_LEN 256
#endif

#define PWDLENGTH 32

int g_curRadio;                   /* current Radio */
int g_curVap;                     /* current VAP */
/*
 * For CLI super user login.
 */
T_BOOL8 g_su;                     /* super user login */
char g_loginuser[MAX_USER_LEN];   /* name of login user */

/*
 * Local References
 */
LOCAL int parser(CLI *, int, struct parse_token_s *);
LOCAL A_BOOL cliLoginRead(CLI * pCli, int telnet);


#define PARSE_TOKEN_DELIMITER { "", NULL, NULL, NULL, NULL, NULL, 0 }

LOCAL struct apply_action_s applyActionTbl[] = {
    {applyIfAddress},
    {applyWlanBasicSettings},
};

typedef enum
{
    APPLY_IFADDR = 0,
    APPLY_WLAN,
} APPLY_ACTION_NO;

/*
 * NOTICE: This table is arranged in alphabetical order
 */

LOCAL struct parse_token_s getCmdTbl[] = {
    {"account", "Display Account", accountGet, NULL, NULL, NULL, 0},
	{"connrssithr", "Display connect rssi threshold", connectRssiThresholdGet, NULL, NULL, NULL, 0},
    {"disrssithr", "Display disconnect rssi threshold", disconnectRssiThresholdGet, NULL, NULL, NULL, 0},
    {"dhcp", "Display dhcp status", dhcpGet, NULL, NULL, NULL, 0},
    {"ipaddr", "Display IP address", ipaddrGet, NULL, NULL, NULL, 0},
	{"mac", "Display mac", macGet, NULL, NULL, NULL, 0},
    {"rssi", "Display rssi", rssiGet, NULL, NULL, NULL, 0},
    {"netmask", "Display IP subnet mask", ipmaskGet, NULL, NULL, NULL, 0},
    {"security", "Display security", securityGet, NULL, NULL, NULL, 0},
    {"ssid", "Display ssid", ssidGet, NULL, NULL, NULL, 0},
    {"deviceid", "Display device id", systemNameGet, NULL, NULL, NULL, 0},

    PARSE_TOKEN_DELIMITER
};

/*
 * NOTICE: This table is arranged in alphabetical order
 */
LOCAL struct parse_token_s setCmdTbl[] = {
    {"account", "Set Account, Usage: set account -u admin -o admin -n 1234",
		accountSet, NULL, NULL, NULL, 0},
    {"rssithr", "Set disconnect or connect rssi thresshold, Usage: set rssithr conn_value1 disconn_value2",
     rssithrSet, NULL, NULL, NULL, 0},
	{"connrssithr", "Set connect rssi threshold, Usage: set connrssithr -10", connectRssiThresholdSet, NULL, NULL, NULL, 0},
	{"disrssithr", "Set disnnect rssi threshold, Usage: set disrssithr -20", disconnectRssiThresholdSet, NULL, NULL, NULL, 0},
	{"dhcp", "Set dhcp status, Usage: set security enable|disable", dhcpSet, NULL, NULL, NULL, 0},
	{"ipaddr", "Set IP address, Usage: Usage: set ipaddr 192.168.1.2", ipAddrSet, &applyActionTbl[APPLY_IFADDR], NULL,CLI_SUPERUSER, 0},
    {"netmask", "Set IP subnet mask, Usage: set netmask 24", ipmaskSet, &applyActionTbl[APPLY_IFADDR],NULL, NULL, 0},
    {"security", "Set security, Usage: set security 3", securitySet, NULL, NULL, NULL, 0},
    {"ssid", "Set ssid, Usage: set ssid wlan", ssidSet, NULL, NULL, NULL, 0},
	{"deviceid", "Set device id, Usage: set device id \"HoWen CPE\"", systemNameSet, NULL, NULL, CLI_SUPERUSER, 0},

    PARSE_TOKEN_DELIMITER
};

struct parse_token_s cmdTbl[] = {
    {"?", "Display CLI command list", helpCmdHandler, NULL, NULL, CLI_HIDDENCMD,0},
    {"applycfg", "Apply configuration changes", applycfgCmdHandler, NULL, NULL, NULL, 0},
	{"cancelcfg", "cancel configuration", cancelcfgCmdHandler, NULL, NULL, CLI_HIDDENCMD,0},
	{"forcequit", "access linux system", accessCmdHandler, NULL, NULL, CLI_HIDDENCMD,0},
    {"get", NULL, genericCmdHandler, NULL, getCmdTbl, NULL, 0},
    {"help", "Display CLI command list", helpCmdHandler, NULL, NULL, NULL, 0},
    {"quit", "Logoff", quitCmdHandler, NULL, NULL, NULL, 0},

    {"reboot", "Reboot access point", rebootCmdHandler, NULL, NULL, NULL, 0},
    {"factory", "Factory default", factorydefaultSet, NULL, NULL, NULL, 0},
    {"set", NULL, genericCmdHandler, NULL, setCmdTbl, NULL, 0},
    {"debug", "enable debug", debugCmdHandler, NULL, NULL, CLI_HIDDENCMD, 0},
    //{"version", "Software version", versionHandler, NULL, NULL, NULL, 0},

    PARSE_TOKEN_DELIMITER
};

int tokenCount(CLI * pCli);
//static int getCountryCode(void);

 /***********************************************************************
 * Function Name : strncasecmp
 * Description    : test for NULL pointers before and NULL characters after
 * comparing possibly non-NULL strings.
 * Input         : @s1, string to compare
 *                    @s2, string to be compared
 *                    @nch, string length
 * Output        : 
 * Return value  : 0, s1 is the same as s2
 *                        negtive, s1 is shorter than s2
 *                        positive, s1 is longer than s2
 ***********************************************************************/
int strncasecmp(const char *s1, const char *s2, size_t nch)
{
    size_t ii;
    int res = -1;

    if (!s1)
    {
        if (!s2)
            return 0;
        return (-1);
    }
    if (!s2)
        return (1);

    for (ii = 0; (ii < nch) && *s1 && *s2; ii++, s1++, s2++)
    {
        res = (int) (tolower(*s1) - tolower(*s2));
        if (res != 0)
            break;
    }

    if (ii == nch)
    {
        s1--;
        s2--;
    }

    if (!*s1)
    {
        if (!*s2)
            return 0;
        return (-1);
    }
    if (!*s2)
        return (1);

    return (res);
}

 /***********************************************************************
 * Function Name : uiVprintf
 * Description    : output by console or throuth telnet
 * Input         : @format, string to print
 *                    @argList, other aguments, the same as in printf
 * Output        : 
 * Return value  : Positive, count of charactors output successfully
 *                        -1, failure
 ***********************************************************************/
A_UINT16 uiVprintf(const char *format, va_list argList)
{
    int retval;
#if defined(INCLUDE_TELNET)
    if (nTelnetConnections)
    {
        retval = telnetPrintf(format, argList);
    }
    else
#endif
    {
        retval = vprintf(format, argList);  /* straight to the console */
    }

    return retval;
}

/***********************************************************************
 * Function Name : uiPrintf
 * Description    : printf API for the AP.  In the CLI code as the cli
 * owns the console and telnet sessions.
 * Input         : @format, string to print
 *                     ...,other aguments, the same as in printf
 * Output        : 
 * Return value  : Positive, count of charactors output successfully
 *                        -1, failure
 ***********************************************************************/
A_INT16 uiPrintf(const char *format, ...)
{
    va_list argList;            /* holds the arguement list passed in */
    int retval;

    va_start(argList, format);

    retval = uiVprintf(format, argList);

    va_end(argList);

    fflush(stdout);

    return retval;
}

#if 0
LOCAL void cliInputEcho(int fd, int echoEnable)
{

/*
    int options;

    options = ioctl(fd, FIOGETOPTIONS, 0);
    if (echoEnable) {
        options |= OPT_ECHO | OPT_LINE;
        options &= ~(OPT_ABORT | OPT_MON_TRAP);
    } else {
        options &= ~(OPT_ECHO | OPT_LINE);
    }
    ioctl(fd, FIOSETOPTIONS, options);
*/
    return;
}
#endif

 /***********************************************************************
 * Function Name : cliLoginVerify
 * Description    : Verify login username
 * Input         : @pUsername, user name input from cosole or telnet
 * Output        : 
 * Return value  : A_OK, success
 ***********************************************************************/
LOCAL int cliLoginVerify(char *pUsername)
{
    if (strlen(pUsername) < sizeof(g_loginuser))
        strcpy(g_loginuser, pUsername);
    return A_OK;
}


 /***********************************************************************
 * Function Name : cliLoginPwVerify
 * Description    : Verify login password
 * Input         : @pPassword, password input from console or telnet
 * Output        : 
 * Return value  :  A_OK, success
 ***********************************************************************/
LOCAL int cliLoginPwVerify(char *pPassword)
{
	char admpass[256] = {0};
	char guestuser[256] = {0};
	char admuser[256] = {0};
	char passwd[256] = {0};
    /*
     * Verify CLI super user name and password.
     * add by Tony 2008.3.12
     */
    ezplib_get_attr_val("http_rule", 0, "admusername", admuser, 256, EZPLIB_USE_CLI);
    ezplib_get_attr_val("http_rule", 0, "admpasswd", admpass, 256, EZPLIB_USE_CLI);
    ezplib_get_attr_val("http_rule", 0, "username", guestuser, 256, EZPLIB_USE_CLI);    
    ezplib_get_attr_val("http_rule", 0, "passwd", passwd, 256, EZPLIB_USE_CLI);

    if (!strcmp(g_loginuser, admuser) && !strcmp(pPassword, admpass))
        g_su = T_TRUE;
    else if(!strcmp(g_loginuser, guestuser) && !strcmp(pPassword, passwd))
        return A_OK;
    else
	 return A_ERROR;
    return A_OK;
}

 /***********************************************************************
 * Function Name : cliConsoleReadPassword
 * Description    : test for NULL pointers before and NULL characters after
 * comparing possibly non-NULL strings.
 * Input         : @s1, string to compare
 *                    @s2, string to be compared
 *                    @nch, string length
 * Output        : 
 * Return value  : 0, s1 is the same as s2
                          negtive, s1 is shorter than s2
                          positive, s1 is longer than s2
 ***********************************************************************/
LOCAL int cliConsoleReadPassword(CLI * pCli)
{
    strcpy(pCli->ibuf, getPasswordFromTerminal());
    return A_OK;
}

 /***********************************************************************
 * Function Name : cliConsoleRead
 * Description    : read from concole
 * Input         : @prompt, output of readline function
 * Output        : pCli,  cli control structure
 * Return value  : A_ERROR, read cli from console or telnet error
 *                       A_OK, success
 ***********************************************************************/
LOCAL int cliConsoleRead(CLI * pCli, char *prompt)
{
    char *p;
    //int bufLen, fd;
	
    if (!pCli)
    {
        return A_ERROR;
    }
//Deleted by Mario Huang
#if 0
    if (opaque)
    {
        cliInputEcho(pCli->fin, 0); /* Turn off input echoing */
    }
#endif
    //fd = pCli->fin;
    p = pCli->ibuf;
    //bufLen = sizeof(pCli->ibuf) - 1;
    bzero(p, sizeof(pCli->ibuf));   // cliConsoleTimeout(pCli, fd);
    p = readline(prompt);
    if (p == NULL)
    {
        uiPrintf("\n");
        return A_ERROR;
    }
    strncpy(pCli->ibuf, p, sizeof(pCli->ibuf));
//Deleted by Mario Huang
#if 0
    if (opaque)
    {
        cliInputEcho(pCli->fin, 1);

/* Turn on input echoing again */
    }
#endif

    return A_OK;
}

/***********************************************************************
 * Function Name : tokenInit
 * Description    : This routine initializes the token data structure before
 *  calling the parser to parse the CLI command line.
 * Input         : 
 * Output        : pCli, cli control structure
 * Return value  : 
 ***********************************************************************/
LOCAL void tokenInit(CLI * pCli)
{
    bzero((char *) pCli->token, sizeof(pCli->token));
    pCli->tokenIdx = 0;
    pCli->tokenLvl = 0;
    pCli->parseTblIdx = 0;
    bzero((char *) &pCli->ParseTbl, sizeof(pCli->ParseTbl));
}

/*
 *  This function returns the number of remaining tokens in
 *  the token stack
 */
int tokenCount(CLI * pCli)
{
    int i, idx, cnt = 0;

   /*avoid null pointer*/
   if (pCli == NULL)
   {
   	return CLI_PARSE_NOMESSAGE;
   }   	
    idx = pCli->tokenIdx;
    for (i = 0; i < MAX_TOKEN; i++)
    {
        if (pCli->token[idx])
        {
            cnt++;
        }
        idx = (idx + 1) % MAX_TOKEN;
    }
    return cnt;
}

  /***********************************************************************
 * Function Name : strncasecmp
 * Description    : Pop a token off the top of the token stack
 * Input         : 
 * Output        : pCli, cli control structure
 * Return value  : NULL, no token poped
 *                        p, pointer to the poped token
 ***********************************************************************/
char *tokenPop(CLI * pCli)
{
    int i, idx;

   /*avoid null pointer*/
   if (pCli == NULL)
   {
   	return NULL;
   }   
   
    idx = pCli->tokenIdx;
    for (i = 0; i < MAX_TOKEN; i++)
    {
        if (pCli->token[idx])
        {
            int cnt;
            char *p;

            p = pCli->token[idx];
            cnt = tokenCount(pCli);
            pCli->token[idx] = 0;
            CLIDEBUG(("tokenPop %p -- %s, len: %d, cnt: %d\n", (void *) pCli, p,
                      strlen(p), cnt));
            return p;
        }
        idx = (idx + 1) % MAX_TOKEN;
    }
    return NULL;
}

 /***********************************************************************
 * Function Name : tokenPopWoCase
 * Description    : Pop an item from the token stack and convert the string to lower case
 * Input         : 
 * Output        : pCli, cli control structure
 * Return value  : pBuf, item poped from token stack
 *                        NULL, no item poped
 ***********************************************************************/
char *tokenPopWoCase(CLI * pCli)
{
    int i, idx;

    idx = pCli->tokenIdx;
    for (i = 0; i < MAX_TOKEN; i++)
    {
        if (pCli->token[idx])
        {
            char *pBuf = (char *) pCli->ibuf;
            char *p = pCli->token[idx];
            char ch;

            pCli->token[idx] = 0;
            while (*p)
            {
                ch = *pBuf = *p;
                if (isupper(ch))
                {               /* Convert upper to lower case */
                    *pBuf = ch - 'A' + 'a';
                }
                p++;
                pBuf++;
            }
            *pBuf = 0;
            pBuf = (char *) pCli->ibuf;
            CLIDEBUG(("tokenPopWoCase -- %s, len: %d, cnt: %d\n",
                      pBuf, strlen(pBuf), tokenCount(pCli)));
            return pBuf;
        }
        idx = (idx + 1) % MAX_TOKEN;
    }
    return NULL;
}

/***********************************************************************
 * Function Name : tokenPush
 * Description    : Push a token onto the token stack
 * Input         : @pCli, cli control structure
 *                    @p, item to be pused into stack
 * Output        : 
 * Return value  : 
 ***********************************************************************/
LOCAL void tokenPush(CLI * pCli, char *p)
{
    pCli->token[pCli->tokenIdx] = p;
//Deleted by Mario Huang
#if 0
    if (pCli->tokenIdx == 0)
    {
        pCli->pToken0 = p;
    }
#endif
    pCli->tokenIdx = (pCli->tokenIdx + 1) % MAX_TOKEN;
    CLIDEBUG(("tokenPush %p -- %s, len: %d, cnt: %d\n", (void *) pCli, p,
              strlen(p), tokenCount(pCli)));
}

/*
 *  
 */
  /***********************************************************************
 * Function Name : tokencmp
 * Description    : A case-insensitive version of bcmp
 * comparing possibly non-NULL strings.
 * Input         : @p1, string to compare
 *                    @p2, string to be compared
 *                    @len, string length
 * Output        : 
 * Return value  : 0, s1 is the same as s2
 *                        1, s1 is different from s2
 ***********************************************************************/
LOCAL int tokencmp(char *p1, char *p2, int len)
{
    int i;

    for (i = 0; i < len; i++)
    {
        if (tolower(p1[i]) != tolower(p2[i]))
        {
            return 1;
        }
    }
    return 0;
}
 /***********************************************************************
 * Function Name : tokenIsProtected
 * Description    : judge if token is protected, which can be used only by admin user
 * Input         : @pTbl, token
 * Output        : 
 * Return value  : TRUE, protected token
 *                        FALSE, un-protected token
 ***********************************************************************/
LOCAL A_BOOL tokenIsProtected(struct parse_token_s * pTbl)
{
    if (pTbl->pProtected)
    {
        //Modified by Mario Huang
        if (bcmp(pTbl->pProtected, CLI_SUPERUSER, sizeof(CLI_SUPERUSER)) == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}
 /***********************************************************************
 * Function Name : IsSuperUser
 * Description    : judge if user is admin user
 * Input         : 
 * Output        : 
 * Return value  : TRUE, admin user
 *                        FALSE, guest user
 ***********************************************************************/
A_BOOL IsSuperUser(void)
{
    /* Tony@2008.04.02 */
    return g_su ? TRUE : FALSE;
}
 /***********************************************************************
 * Function Name : tokenIsHidden
 * Description    : judge if the token is invisiable or not
 * Input         : pTbl, token table
 * Output        : 
 * Return value  : TRUE, invisiable token
*                         FALSE, visiable token
 ***********************************************************************/
LOCAL A_BOOL tokenIsHidden(struct parse_token_s * pTbl)
{
    if (pTbl->pProtected)
    {
        if (bcmp(pTbl->pProtected, CLI_HIDDENCMD, sizeof(CLI_HIDDENCMD)) == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}
 /***********************************************************************
 * Function Name : tokenIsValidOpMode
 * Description    : judge if token can be used in the specific operation mode or not
 * Input         : @pCli, cli control structure
 *                    @pTbl, token table
 * Output        : 
 * Return value  : TRUE, token can be used in this operation mode
 *                        FALSE, token can not be used in this operation mode
 ***********************************************************************/
LOCAL A_BOOL tokenIsValidOpMode(CLI * pCli, struct parse_token_s * pTbl)
{

    return TRUE;
}
 /***********************************************************************
 * Function Name : tokenParser
 * Description    : parse the token and process it
 * comparing possibly non-NULL strings.
 * Input         : @pCli, cli control structure
 *                    @pTbl, token table
 * Output        : 
 * Return value  : CLI_PARSE_OK, parse token success 
 *                 CLI_PARSE_ERROR, parse token failure 
 *                 CLI_PARSE_NO_TBL, no token table valid                
 *                 CLI_PARSE_NO_VALUE,no parameter in token              
 *                 CLI_PARSE_TOO_FEW, not enough items in token               
 *                 CLI_PARSE_TOO_MANY, items of token is more than it have to              
 *                 CLI_PARSE_UNKNOWN, do not match any token in the token table               
 *                 CLI_PARSE_INPUT_ERROR, withdraw charactor error form input           
 *                 CLI_PARSE_INVALID_PARAMETER, parameter from item is invalide     
 *                 CLI_PARSE_MULTI_MATCHES, inputed token matchs more than one token in the token table              
 *                 CLI_PARSE_QUIT, parse terminate unexpected                 	 
 *                 CLI_PARSE_NOMESSAGE, no message parse from token 
 ***********************************************************************/
LOCAL int tokenParser(CLI * pCli, struct parse_token_s *pTbl)
{
    char *p;
    char *pFilter;
    int rc = 0;

    CLIDEBUG(("tokenParser\n"));
    pCli->tokenLvl++;
    pCli->ParseTbl[pCli->parseTblIdx++] = pTbl;
    p = tokenPopWoCase(pCli);
    if (p)
    {
        struct parse_token_s *pTblx;
        int len = strlen(p);
        int k, match;

        for (pTblx = pTbl; pTblx->fHandler; pTblx++)
        {
            pTblx->abbrevMatchCnt = 0;
        }

        CLIDEBUG(("p: %s, len: %d\n", p, strlen(p)));
        pFilter = p;
        match = 0;
        for (pTblx = pTbl; pTblx->fHandler; pTblx++)
        {
            if (len == strlen(pTblx->pCmd) &&
                tokenIsValidOpMode(pCli, pTblx) == TRUE &&
                tokencmp(p, pTblx->pCmd, len) == 0)
            {
                CLIDEBUG(("Exact match found\n"));
                match = 1;      /* exact match */
                pTblx->abbrevMatchCnt = len;
                break;
            }
        }

        if (match == 0)
        {
            for (k = 0; k < len; k++)
            {
                for (pTblx = pTbl; pTblx->fHandler; pTblx++)
                {
                    /* Don't let priv and hidden tokens out of the bag */
                    if (pCli->securityCheck && tokenIsProtected(pTblx) == TRUE
                        && IsSuperUser() == FALSE)
                    {
                        continue;
                    }
                    if (tokenIsHidden(pTblx)
                        || tokenIsValidOpMode(pCli, pTblx) == FALSE)
                    {
                        continue;
                    }

                    if (*(p + k) && *(pTblx->pCmd + k))
                    {
                        if (*(p + k) == *(pTblx->pCmd + k))
                        {
                            if (k == 0)
                            {
                                match++;
                            }
                            if (k == pTblx->abbrevMatchCnt)
                            {
                                pTblx->abbrevMatchCnt++;
                                CLIDEBUG(("%s -- abbrevMatch: %d\n",
                                          pTblx->pCmd, pTblx->abbrevMatchCnt));
                            }
                        }
                    }
                }
            }
        }

        CLIDEBUG(("%d match found, filter: %s\n", match, pFilter));
        if (match == 1)
        {
            match = 0;
            pFilter = p;
            len = strlen(p);
            for (pTblx = pTbl; pTblx->fHandler; pTblx++)
            {
                if (pTblx->abbrevMatchCnt)
                {
                    if (len <= strlen(pTblx->pCmd))
                    {
                        if (tokencmp(p, pTblx->pCmd, len) == 0)
                        {
                            if (!pCli->securityCheck || IsSuperUser() == TRUE ||
                                tokenIsProtected(pTblx) == FALSE)
                            {
                                //Deleted by Mario Huang, it seems useless
                                //tokenCurTblSet(pTblx);
                                rc = (*pTblx->fHandler) (pCli, p,
                                                         pTblx->pNxtParseTbl);
                                if (rc == CLI_PARSE_NO_VALUE
                                    || rc == CLI_PARSE_UNKNOWN
                                    || rc == CLI_PARSE_INVALID_PARAMETER)
                                {
                                    if (rc == CLI_PARSE_INVALID_PARAMETER)
                                    {
                                        helpCmdScan(pCli,
                                                    pCli->ParseTbl[pCli->
                                                                   parseTblIdx -
                                                                   1], "", NULL,
                                                    1);
                                    }
                                    else
                                    {
                                        if (rc == CLI_PARSE_UNKNOWN)
                                        {
                                            helpCmdScan(pCli,
                                                        pCli->ParseTbl[pCli->
                                                                       parseTblIdx
                                                                       - 1],
                                                        pTblx->pCmd, pFilter,
                                                        1);
                                        }
                                        else
                                        {
                                            helpCmdScan(pCli,
                                                        pTblx->pNxtParseTbl,
                                                        pTblx->pCmd, NULL, 1);
                                        }
                                    }
                                    rc = (rc == CLI_PARSE_NO_VALUE) ?
                                        CLI_PARSE_TOO_FEW : CLI_PARSE_ERROR;
                                }
                                /* Count the related setting functions have been done successfully,
                                 * so as to do associated applying action while excute apply cmd.
                                 * add by Tony 2007.9.30
                                 */
                                else if (rc == CLI_PARSE_OK)
                                {
                                    if (pTblx->pApplyActionTbl)
                                        pTblx->pApplyActionTbl->settingCnt++;
                                }
                                return rc;
                            }
                            match = 1;
                        }
                        CLIDEBUG(("tokenParser -- %s can't match with table item %s, pTbl: %p\n", p, pTblx->pCmd, (void *) pTblx));
                    }
                }
            }
        }
        else if (match != 0)
        {
            /*
             *  Multiple matches found. Find the one with the highest match
             */
            int cnt;

            cnt = pTbl->abbrevMatchCnt;
            for (pTblx = pTbl; pTblx->fHandler; pTblx++)
            {
                if (pTblx->abbrevMatchCnt)
                {
                    if (pTblx->abbrevMatchCnt > cnt)
                    {
                        cnt = pTblx->abbrevMatchCnt;
                    }
                }
            }
            CLIDEBUG(("highest abmatch cnt: %d\n", cnt));
            match = 0;
            for (pTblx = pTbl; pTblx->fHandler; pTblx++)
            {
                if (pTblx->abbrevMatchCnt == cnt)
                {
                    if (cnt == strlen(p) && cnt == strlen(pTblx->pCmd))
                    {           /* any possibility??? these equals exact matching */
                        match = 1;
                        break;
                    }
                    match++;
                }
            }
            CLIDEBUG(("match -- %d\n", match));
            if (match)
            {
                len = strlen(p);
                for (pTblx = pTbl; pTblx->fHandler; pTblx++)
                {
                    if (pTblx->abbrevMatchCnt == cnt)
                    {
                        if (len <= strlen(pTblx->pCmd))
                        {
                            if (tokencmp(p, pTblx->pCmd, len) == 0)
                            {
                                /* Handler the highest matched command */
                                //if(match == 1) {
                                if (!pCli->securityCheck
                                    || IsSuperUser() == TRUE
                                    || tokenIsProtected(pTblx) == FALSE)
                                {
                                    //Deleted by Mario Huang, it seems useless
					 //tokenCurTblSet(pTblx);
                                    rc = (*pTblx->fHandler) (pCli, p,
                                                             pTblx->
                                                             pNxtParseTbl);
                                    if (rc == CLI_PARSE_NO_VALUE)
                                    {
                                        helpCmdScan(pCli, pTblx->pNxtParseTbl,
                                                    pTblx->pCmd, NULL, 1);
                                        rc = CLI_PARSE_TOO_FEW;
                                    }
                                    /* Count the related setting functions have been done successfully,
                                     * so as to do associated applying action while excute apply cmd.
                                     * add by Tony 2007.9.30
                                     */
                                    else if (rc == CLI_PARSE_OK)
                                    {
                                        if (pTblx->pApplyActionTbl)
                                            pTblx->pApplyActionTbl->
                                                settingCnt++;
                                    }
                                    return rc;
                                }
                                //}
                                //else {
                                //helpCmdScan(pCli, pTblx, pTblx->pCmd, pTblx->pCmd, 1);
                                //rc = CLI_PARSE_MULTI_MATCHES;
                                //}
                            }
                        }
                        break;
                    }
                }
                if (rc == CLI_PARSE_MULTI_MATCHES)
                    return rc;
                CLIDEBUG(("%s won't match with table item %s, pTbl: %p\n",
                          p, pTblx->pCmd, (void *) pTblx));
            }
        }

        if (pCli->tokenLvl > 1)
        {
            uiPrintf("Invalid parameter: %s", p);
        }
        else
        {
            uiPrintf("Unknown command: %s", p);
        }

        while ((p = tokenPop(pCli)) != NULL)
        {
            uiPrintf(" %s", p);
        }
        uiPrintf("\n");

        CLIDEBUG(("parseTblIdx: %d\n", pCli->parseTblIdx));
        if (!(pCli->parseTblIdx - 1 == 0 &&
              *(pCli->ParseTbl[pCli->parseTblIdx - 1]->pCmd) == '#'))
        {
            if (pCli->tokenLvl <= 2)
            {
                uiPrintf("Type \"help\" for a list of valid commands.\n");
            }
            else
            {
                uiPrintf("List of valid parameter(s):\n");
            }
        }
        return pCli->tokenLvl <=
            2 ? CLI_PARSE_UNKNOWN : CLI_PARSE_INVALID_PARAMETER;
    }
    return CLI_PARSE_OK;
}
//Deleted by Mario Huang
#if 0
int tokenPopNum(CLI * pCli, A_UINT32 * pV, int base)
{
    char *p;
    char *pEnd;

    pCli->tokenLvl++;
    p = tokenPop(pCli);
    if (p)
    {
        CLIDEBUG(("p: %s, len: %d\n", p, strlen(p)));
        *pV = strtoul(p, &pEnd, base);
        if (p == pEnd || *pEnd)
        {
            uiPrintf("Invalid input\n");
            return CLI_PARSE_ERROR;
        }
        return CLI_PARSE_OK;
    }

    uiPrintf("Error -- No value specified\n");
    return CLI_PARSE_ERROR;
}
#endif

 /***********************************************************************
 * Function Name : cliLoginRead
 * Description    : Display login prompt, get and verify the login info
 * comparing possibly non-NULL strings.
 * Input         : @pCli, cli control structure
 *                    @telnet, if telnet IO or not
 * Output        : 
 * Return value  : TRUE, read cli successful
 *                        FALSE, read cli error
 ***********************************************************************/
LOCAL A_BOOL cliLoginRead(CLI * pCli, int telnet)
{
    int attempt;
    A_BOOL ok;
    //char cmd[128] = {0};
    //char dev_get[33] = {0};
    char dev_name[33] = {0};
    //int name_len = 0;
    //FILE *fp;
    char prompt[100];

    if(T_FAILURE == get_sysname_from_nvram(dev_name))
    {
    	strcpy(dev_name, SYS_NAME);
	}
	
    sprintf(prompt, "%s login: ", dev_name);

    for (;;)
    {
        for (;;)
        {
            uiPrintf("\n");
            for (attempt = 0; attempt < MAX_LOGIN_ATTEMPT; attempt++)
            {
                ok = TRUE;


                cliConsoleRead(pCli, prompt);

                //pCli->ibuf = readline("");

                if (cliLoginVerify(pCli->ibuf) != A_OK)
                {
                    ok = FALSE;
                }
                uiPrintf("Password: ");
                if (cliConsoleReadPassword(pCli) != A_OK)
                {
                    uiPrintf
                        ("Loginning failed for some unknown reason.Please try again.\n");
                    continue;
                }
                if (cliLoginPwVerify(pCli->ibuf) != A_OK)
                {
                    ok = FALSE;
                }

#ifdef CLI_LOGIN_RD_MODE
                strcpy(g_loginuser, CLI_SUPERUSER);
                g_su = T_TRUE;
                return TRUE;
#endif

				/* For CLI super user login, add by Tony 2008.3.12 */
                if (ok || g_su)   
                {
                    return TRUE;
                }
                uiPrintf("Incorrect login.Please try again.\n");
            }
            if (telnet)
            {
                return FALSE;
            }
            uiPrintf("Too many invalid login attempts. Pause for 10 seconds...\n");
            sleep(10);
        }

    }
    return TRUE;
}

 /***********************************************************************
 * Function Name : cliIoDevSet
 * Description    : file IO settings
 * Input         : @pMyCli, 
 *                    @fin, 
 *                    @fout,
 * Output        : 
 * Return value  : OK, set successfully
 ***********************************************************************/
int cliIoDevSet(struct cli_s *pMyCli, int fin, int fout)
{
    pMyCli->fin = fin;
    pMyCli->fout = fout;

    return OK;
}
/*
 *  
 */
  /***********************************************************************
 * Function Name : cliInit
 * Description    : Initialize CLI data structure for current CLI task
 *  A CLI data structure is allocated to every CLI task
 * Input         : 
 * Output        : pCli,cli control structure
 * Return value  : 
 ***********************************************************************/
struct cli_s *cliInit(void)
{
    CLI *pCli;

    cliConfig_t cliConfig;

    getCliConfig(&cliConfig);
    pCli = calloc(1, sizeof(struct cli_s));
    if (!pCli)
    {
        return NULL;
    }
    memset(pCli, 0, sizeof(struct cli_s));
    pCli->pid = getpid();
    pCli->unit = g_curRadio = 0;
    pCli->vap = cliConfig.vap;  /*Thomas modified */
    pCli->securityCheck = TRUE;
    //Deleted by Mario Huang,autoLogOff useless
    //pCli->autoLogOff = FALSE;
    return pCli;
}


  /***********************************************************************
 * Function Name : cliFree
 * Description    : cliFree - undo a cliInit()
 * comparing possibly non-NULL strings.
 * Input         : pCli, cli control structure
 * Output        : 
 * Return value  :
 ***********************************************************************/
void cliFree(CLI * pCli)
{
    free(pCli);
}

/*
 *  
 *
 *  Inputs:
 *      Input File Descriptor
 *      Pointer to the parse token table
 *
 *
 */
  /***********************************************************************
 * Function Name : parser
 * Description    : Parse the input line
 * Input         : @pCli, cli control structure
 *                    @fd, file destripter
 *                    @pTbl, token table
 * Output        : 
 * Return value  : CLI_PARSE_OK, parse token success 
 *                 CLI_PARSE_ERROR, parse token failure 
 *                 CLI_PARSE_NO_TBL, no token table valid                
 *                 CLI_PARSE_NO_VALUE,no parameter in token              
 *                 CLI_PARSE_TOO_FEW, not enough items in token               
 *                 CLI_PARSE_TOO_MANY, items of token is more than it have to              
 *                 CLI_PARSE_UNKNOWN, do not match any token in the token table               
 *                 CLI_PARSE_INPUT_ERROR, withdraw charactor error form input           
 *                 CLI_PARSE_INVALID_PARAMETER, parameter from item is invalide     
 *                 CLI_PARSE_MULTI_MATCHES, inputed token matchs more than one token in the token table              
 *                 CLI_PARSE_QUIT, parse terminate unexpected                 	 
 *                 CLI_PARSE_NOMESSAGE, no message parse from token 
 ***********************************************************************/
int
parser(CLI * pCli, int fd, struct parse_token_s *pTbl)
{
    char buf[CLI_INBUF];
    char *p;
    char *pBuf;
    char *pToken;
    char ch;
    int i, len, status;
    //A_UINT32 cksum = 0;
    int invalid = 0;
    char dev_name[33]={0};

    char prompt[100];
    //*eof = 0;
    p = pBuf = (char *) buf;
    len = 0;

    tokenInit(pCli);

    /*
     * Notice that country code has not been set.
     * modify by Tony 2008.4.16.
     */
     if(T_FAILURE == get_sysname_from_nvram(dev_name))
    	{
          return FALSE;
	}
	 
//#define SINGLECRAD_DUALRADIO

//#ifdef SINGLECRAD_DUALRADIO
           sprintf(prompt, "%s %s ", dev_name,CLI_PROMPT);
//#else
        //sprintf(prompt, "%s %s[%d, %d] %s ", dev_name, 
					//CLI_WLAN_PROMPT, pCli->unit, pCli->vap, CLI_PROMPT);
//#endif
    pBuf = readline(prompt);
#if 0
    if (*eof == 0)
    {
        if (pCksum)
        {
            *pCksum += cksum;
        }
    }
#endif
    if (invalid)
    {
        return CLI_PARSE_INPUT_ERROR;
    }

    if (pBuf != NULL)
    {

        /*dennis add to prevent buffer overflow. 2008-04-18. -----> */
        if (strlen(pBuf) >= CLI_INBUF)
            return CLI_PARSE_INPUT_ERROR;
        /*<----- */

        //ignore the heading spaces until the command begins
        while (*pBuf && isspace(*pBuf))
            pBuf++;

        p = pBuf;

        len = strlen(p) + 1;

        for (i = 0; i < len; i++)
        {
            if (isspace(*pBuf))
                *pBuf = 0;
            pBuf++;
        }

        pToken = NULL;
        for (i = 0; i < len; i++)
        {
            ch = *p;
            if (ch)
            {
                if (!pToken)
                {
                    pToken = p;
                }
            }
            else
            {
                if (pToken)
                {
                    tokenPush(pCli, pToken);
                }
                pToken = NULL;
            }
            p++;
        }
//Dleted by Mario Huang
        //specialTimeofDay(pCli);
//Dleted by Mario Huang
        //specialTimeofZone(pCli);
//Dleted by Mario Huang, useless
//        specialFactoryDefault(pCli);
        status = tokenParser(pCli, pTbl);
        return status;
    }
    else
        return -1;
}
//Deleted by Mario Huang because signal process is unneccessary
 /***********************************************************************
 * Function Name : init_signals
 * Description    : ignore the signals
 * comparing possibly non-NULL strings.
 * Input         : 
 * Output        : 
 * Return value  : 
 ***********************************************************************/
void init_signals(void)
{
#if 0
    struct sigaction sa;

    sa.sa_flags = 0;

    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGINT);
    sigaddset(&sa.sa_mask, SIGTRAP);
    sigaddset(&sa.sa_mask, SIGABRT);

    sa.sa_handler = SIG_IGN;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTRAP, &sa, NULL);
    sigaction(SIGABRT, &sa, NULL);
#endif
    struct sigaction sa;

    sa.sa_flags = 0;

    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGHUP);
    sigaddset(&sa.sa_mask, SIGTERM);
    sigaddset(&sa.sa_mask, SIGINT);
    sigaddset(&sa.sa_mask, SIGQUIT);
    sigaddset(&sa.sa_mask, SIGTRAP);
    sigaddset(&sa.sa_mask, SIGTSTP);
    sigaddset(&sa.sa_mask, SIGCLD);
    sigaddset(&sa.sa_mask, SIGUSR1);
    sigaddset(&sa.sa_mask, SIGUSR2);

    sa.sa_handler = SIG_IGN;
    /* Ignore the following signals */
    sigaction(SIGHUP, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGTRAP, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);
    sigaction(SIGCLD, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);

}

/*
  *  check nvram before using.
 */
int nvram_workable(void)
{
	FILE *fp;
	char cmd[64];
	sprintf(cmd, "ls /dev | grep -rn nvram > /tmp/nvram_mount_status");
	system(cmd);
	fp = fopen("/tmp/nvram_mount_status", "r");
	if(NULL == fp)
	{
		return 0;
	}

	if(EOF == fgetc(fp))
	{
		fclose(fp);
		return 0;
	}
	fclose(fp);
	return 1;
}
//Deleted by Mario Huang
#if 0
static int getCountryCode(void)
{
    country_config_t ctryCfg;

    if (cfg_get_country(&ctryCfg) != T_SUCCESS)
        return -1;
    else
        return ctryCfg.iCountryCode;
}
#endif


/*
 * cli main entry.
 */
  /***********************************************************************
 * Function Name : main
 * Description    : command line main function
 * comparing possibly non-NULL strings.
 * Input         : 
 * Output        : 
 * Return value  : 
 ***********************************************************************/
int main(int argc, char **argv)
{
    CLI *pCli;
    int telnet = 0;
    int status;
    //int eof;
    int logon = FALSE;
    char *accessMode;

    int fin = STD_IN;
    //Deleted by Mario Huang
    int fout = STD_OUT;
	int nvram_exist = 0;
    pCli = cliInit();
    cliIoDevSet(pCli, fin, fout);

    /* Disable int, trap and abort */
    init_signals();
    /* Tony@2008.04.02 */
    setenv("PATH", "/sbin:/usr/sbin:/bin:/usr/bin", 1);
    if ((accessMode = getenv("ACCESS_MODE")) != NULL)
    {
        if (!strcmp(accessMode, "telnet"))
            telnet = 1;
    }

	//check nvram work?
    nvram_exist = nvram_workable();
    if(!nvram_exist)
	{
		uiPrintf("System booting, please wait...");
		sleep(5);
	}

	while(!nvram_workable())
	{
		sleep(5);
	}

	//load config to ram
	init_global_config();

	//print config
	debug_global_config();
	
  login:

	do
    {
        logon = cliLoginRead(pCli, telnet);
		if(!logon)
			exit(0);
    }while(!logon);

    for (;;)
    {
        status = parser(pCli, fin, cmdTbl);
        if (status == CLI_PARSE_QUIT)
        {
            /*
             * CLI super user can exit to shell,
             * and others cannot but login again.
             * add by Tony 2008.3.12
             */
        	if (telnet || g_su)
                exit(0);
        	else
                goto login;
        }

		if (status == CLI_PARSE_ACCESS_LINUX)
		{
			break;
		}
		
        if ((tokenCount(pCli) > 2) || (status == CLI_PARSE_TOO_MANY))
        {
            uiPrintf("Too many parameters!\n");
        }
        else if (status == CLI_PARSE_TOO_FEW)
        {
            uiPrintf("Not enough parameters!\n");
        }
        else if (status == CLI_PARSE_INVALID_PARAMETER)
        {
            uiPrintf("Invalid parameter!\n");
        }
        else if (status == CLI_PARSE_INPUT_ERROR)
        {
            uiPrintf("Invalid input characters!\n");
        }
        else if (status == CLI_PARSE_NO_VALUE)
        {
            uiPrintf("No parameter specified!\n");
        }
//TODO:Modified by Mario Huang
        //else if ((tokenCount(pCli) >= 1) && (cTimeofZone == 0))
        else if (tokenCount(pCli) >= 1)
        {
            uiPrintf("Too many parameters!\n");
        }
    }

	//free config
	free_global_config();
	
    return 0;
}

 /***********************************************************************
 * Function Name : genericCmdHandler
 * Description    : generic command callback function
 * comparing possibly non-NULL strings.
 * Input         : @pCli, cli control structure
 *                    @pTbl, token table
 * Output        : 
 * Return value  : CLI_PARSE_NO_TBL, no token table error
 *                        CLI_PARSE_NO_VALUE, no token in the cli control
 ***********************************************************************/
int genericCmdHandler(CLI * pCli, char *p, struct parse_token_s *pTbl)
{
    if (tokenCount(pCli))
    {
        if (pTbl)
        {
            return tokenParser(pCli, pTbl);
        }
        return CLI_PARSE_NO_TBL;
    }
    return CLI_PARSE_NO_VALUE;
}

 /***********************************************************************
 * Function Name : helpCmdScan
 * Description    : search for the help command
 * Input         : @pCli, cli control structure
 *                    @pTbl, token table
 *                    @pTxt, next token
 *                    @pFilter, token filter
 *                    @verbose, version
 * Output        : 
 * Return value  : CLI_PARSE_OK, help scan success
 ***********************************************************************/
int helpCmdScan(CLI * pCli, struct parse_token_s *pTbl, char *pTxt,
                char *pFilter, int verbose)
{
    struct parse_token_s *pTblx;
    char buf[132];
    int bmatch;

    if (pTbl == NULL || pTxt == NULL)
    {
        return CLI_PARSE_OK;
    }

    for (pTblx = pTbl; pTblx->fHandler; pTblx++)
    {
        if (pCli->securityCheck)
        {
            if (tokenIsProtected(pTblx) == TRUE && IsSuperUser() == FALSE)
                continue;
            if (tokenIsHidden(pTblx) == TRUE
                || tokenIsValidOpMode(pCli, pTblx) == FALSE)
                continue;
        }
        /* murphy van 2004-06-11 add */
        if (tokenIsValidOpMode(pCli, pTblx) == FALSE)
            continue;

        if (verbose && pTblx->pNxtParseTbl)
        {
            sprintf(buf, "%s %s", pTxt, pTblx->pCmd);
            if (verbose == 1)
            {
                if (pFilter)
                {
                    bmatch = bcmp(pTblx->pCmd, pFilter, strlen(pFilter));
                    if (bmatch == 0)
                    {
                        helpCmdScan(pCli, pTblx->pNxtParseTbl, buf, NULL, 0);
                    }
                }
                else
                {
                    helpCmdScan(pCli, pTblx->pNxtParseTbl, buf, pFilter, 0);
                }
            }
            if (verbose >= 10)
            {
                helpCmdScan(pCli, pTblx->pNxtParseTbl, buf, NULL, verbose);
            }
        }
        else
        {
            sprintf(buf, "%s %s", pTxt, pTblx->pCmd);
            if (pTblx->pHelpText)
            {
                if (pFilter)
                {
                    bmatch = bcmp(pTblx->pCmd, pFilter, strlen(pFilter));
                    if (bmatch == 0)
                    {
                        uiPrintf("%-35s -- %s\n", buf,
                                 pTblx->pHelpText ? pTblx->pHelpText : "");
                    }
                }
                else
                {
                    uiPrintf("%-35s -- %s\n", buf,
                             pTblx->pHelpText ? pTblx->pHelpText : "");
                }
            }
        }
    }
    return CLI_PARSE_OK;
}

 /***********************************************************************
 * Function Name : helpCmdHandler
 * Description    : process help command
 * Input         : @pCli, cli control structure
 *                    @pTbl, token table
 * Output        : 
 * Return value  : CLI_PARSE_OK, help command success
 ***********************************************************************/
int helpCmdHandler(CLI * pCli, char *p, struct parse_token_s *pTbl)
{
    char *pToken;
    int verbose = 1;

    uiPrintf("List of CPE CLI commands:\n");

    if (tokenCount(pCli))
    {
        pToken = tokenPop(pCli);
        verbose = 10;
    }

//country code is not taken in count now, but it will be processed later
//    if (getCountryCode())
        helpCmdScan(pCli, cmdTbl, "", NULL, verbose);
//    else
//        helpCmdScan(pCli, naCmdTbl, "", NULL, verbose);
    return CLI_PARSE_OK;
}
#if 0
int applycfgCmdHandler(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
    struct parse_token_s *pCmdTbl = NULL;
    struct parse_token_s *pSubCmdTbl = NULL;

    for (pCmdTbl = cmdTbl; pCmdTbl->fHandler; pCmdTbl++)
    {
        pSubCmdTbl = pCmdTbl->pNxtParseTbl;

        if (pSubCmdTbl != NULL)
        {
            /* Invoke the applying handler function, if this part of config is changed */
            for (; pSubCmdTbl->fHandler; pSubCmdTbl++)
            {
                if (pSubCmdTbl->pApplyActionTbl
                    && pSubCmdTbl->pApplyActionTbl->settingCnt
                    && pSubCmdTbl->pApplyActionTbl->handler)
                {
                    (*pSubCmdTbl->pApplyActionTbl->handler) ();
                    pSubCmdTbl->pApplyActionTbl->settingCnt = 0;
                }
            }
        }
    }
//TODO: save it to nvram
 #endif
