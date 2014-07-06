/*
 * security.c -- Security handler
 *
 * Copyright (c) GoAhead Software Inc., 1995-2000. All Rights Reserved.
 *
 * See the file "license.txt" for usage and redistribution license requirements
 *
 * $Id: security.c,v 1.9 2003/09/19 17:04:44 bporter Exp $
 */

/******************************** Description *********************************/

/*
 *	This module provides a basic security policy.
 */

/********************************* Includes ***********************************/

//Steve add
#include  <sys/sysinfo.h>
#include "nvram.h"
#include "nvram_rule.h"

#include	"wsIntrn.h"
#include	"um.h"
#include    <arpa/inet.h>

#ifdef DIGEST_ACCESS_SUPPORT
#include	"websda.h"
#endif

/********************************** Defines ***********************************/
/*
 *	The following #defines change the behaviour of security in the absence 
 *	of User Management.
 *	Note that use of User management functions require prior calling of
 *	umInit() to behave correctly
 */

#ifndef USER_MANAGEMENT_SUPPORT
#define umGetAccessMethodForURL(url) AM_FULL
#define umUserExists(userid) 0
#define umUserCanAccessURL(userid, url) 1
#define umGetUserPassword(userid) websGetPassword()
#define umGetAccessLimitSecure(accessLimit) 0
#define umGetAccessLimit(url) NULL
#endif

/******************************** Local Data **********************************/

static char_t	websPassword[WEBS_MAX_PASS];	/* Access password (decoded) */
#ifdef _DEBUG
static int		debugSecurity = 1;
#else
static int		debugSecurity = 0;
#endif

//Added by Andy Yu in 2013/10/15: Force Login Timer Stamp  
/******************************** global Data **********************************/
int g_Force_login_inactivity_timer_stamp=0;
int g_Force_login_current_username=0;

//2010/04/22 Steve  patched base on NBG-419N
#if 1//Arthur Chow 2008-12-30
/******************************** global Data **********************************/
int g_Admin_inactivity_timer_stamp=0;
#endif

/*********************************** Code *************************************/
/*
 *	Determine if this request should be honored
 */
//2010/04/22 Steve patched base on NBG-419N
#if 1//Arthur Chow 2008-12-17 : New sub-routine	
int CheckRootFile(char_t *url)
{
	int len=0;
	int i=0;
	int path_sign=0;

	if (!strcmp(url, "/goform/web_login"))
		return 1;	

	if (!strncmp(url, "/lang", 5))
		return 1;	

	len=strlen(url);
	for (i=0; i<len; i++)
	{
		if (url[i]=='/')
			path_sign++;
	}
	if (path_sign>=2)
		return 0;
	return 1;	
}

int GetAccessableIpaddr(char_t *accessable_ip)
{
		FILE *fp = NULL;
		int len=0;

		//Steve modified 2010/04/22
		//if (NULL == (fp = fopen("/etc_ro/web/accessip", "r")))
		if (NULL == (fp = fopen("/web/accessip", "r")))
		{
			error(E_L, E_LOG, T("GetAccessableIpaddr: open /web/accessip error"));
	  	exit(0);//Arthur Chow 2009-01-06: exit goahead
			return 0;
		}
		len = fscanf(fp, "%s", accessable_ip);
		fclose(fp);
		if (len)
		{
			return 1;
		}
		return 0;
}

int CheckAccessRight(webs_t wp, char_t *url)
{
	char_t *cur_ip=websGetRequestIpaddr(wp);
	char_t accessable_ip[33];
	time_t tm, admin_inactivity_timer=0;
	struct sysinfo info;
	char_t faccessable_ip[33] = {0};
	
	//Steve
	char_t TempBuf[32];	

	/*----------Added By Andy Yu in 2013/10/17: Force Login Timeout---------*/
	if (GetForceAccessableIpaddr((char_t *)&faccessable_ip)) {
		if (strcmp(faccessable_ip, "0.0.0.0")) {
			time_t ftm, fadmin_inactivity_timer=0;
			struct sysinfo finfo;
			sysinfo(&finfo);
			ftm = (int) finfo.uptime - g_Force_login_inactivity_timer_stamp;
			if (ftm >= 60) {
				system("echo '0.0.0.0' > /web/faccessip");
			}
		}
		if (!strcmp(faccessable_ip, cur_ip)) {
			if (!strcmp(url, "/force_login.html")) {
				return 1;
			} else if ((!strcmp(url, "/index.asp"))) {
				system("echo '0.0.0.0' > /web/faccessip");
			}
		}
	}
	
	if (GetAccessableIpaddr((char_t *)&accessable_ip))
	{
		//Steve modified
		//admin_inactivity_timer = atoi(nvram_bufget(RT2860_NVRAM, "AdminInactivityTimer"));
		//admin_inactivity_timer = 0;
		ezplib_get_attr_val("http_rule", 0, "adm_timeout", TempBuf, 32, EZPLIB_USE_CLI);
		admin_inactivity_timer = atoi(TempBuf);
		//printf("\n\n Steve debug admin_inactivity_timer=%d\n\n", admin_inactivity_timer);
		
		if (strcmp(accessable_ip, "0.0.0.0"))
		{
			if (admin_inactivity_timer>0)
			{
			  sysinfo(&info);
			  //printf("@@@@@@@@@@@@@@@@@@g_Admin_inactivity_timer_stamp=%d\n",g_Admin_inactivity_timer_stamp);
			  //printf("@@@@@@@@@@@@@@@@@@(int) info.uptime=%d\n",(int) info.uptime);
				tm = (int) info.uptime - g_Admin_inactivity_timer_stamp;
				if (tm >= (admin_inactivity_timer*60)) {
					//Steve
					//system("echo '0.0.0.0' > /etc_ro/web/accessip");
					system("echo '0.0.0.0' > /web/accessip");
					//doSystem("echo '0.0.0.0' > /etc_ro/web/accessip");
					//syslog(LOG_INFO, "Web management is auto-logouted because user 'admin' from %s has idled over %d minutes.\n", cur_ip, admin_inactivity_timer);
					return CheckRootFile(url);;
				}
			}
		}
		if (!strcmp(accessable_ip, cur_ip))
		{
			if (admin_inactivity_timer>0)
			{
			  sysinfo(&info);
				g_Admin_inactivity_timer_stamp=(int) info.uptime;
			}
			return 1;
		}
	}
	return CheckRootFile(url);	
}

/*---------Added By Andy Yu in 2013/10/17: Force Login----------*/
int GetForceAccessableIpaddr(char_t *faccessable_ip)
{
		FILE *fp = NULL;
		int len=0;

		if (NULL == (fp = fopen("/web/faccessip", "r")))
		{
			printf("/web/faccessip Not Exist\n");
			return 0;
		}
		len = fscanf(fp, "%s", faccessable_ip);
		fclose(fp);
		if (len)
		{
			return 1;
		}
		return 0;
}
#endif

/* check lan,  2013-05-06 Ricardo -------->*/
int checkSubNet(char* accessip){
	char* lanip = nvram_safe_get("lan0_ipaddr");
	char* lanmask = nvram_safe_get("lan0_mask");
	int idx = atoi(lanmask);
	lanmask = prefix2mask(idx);
	if((inet_addr(lanip) & inet_addr(lanmask)) == (inet_addr(accessip) & inet_addr(lanmask)))
		return 1;
	else
		return 0;	
}
/* <---------------------------Ricardo */
int checkSelectedIp( char* accessip){
	char selectip[20];
	ezplib_get_attr_val("http_rule", 0, "secipaddr", selectip, 
		20, EZPLIB_USE_CLI);

	if(checkSubNet(accessip))
		return 0;

	if(!strcmp(selectip, "")){
		return 0;
	}

	if(!strcmp(selectip, accessip)){
		//system("echo same >>/dev/console");
		return 0;
	}else{
		//system("echo different >>/dev/console");
		return 1;
	}
}

#if 1//Arthur Chow 2009-01-17
int my_websSecurityHandler(webs_t wp, char_t *url)
{ 
	int				nRet=0;
	//Steve
	//char_t *first_config=nvram_get(RT2860_NVRAM, "FirstConfig");
	char_t *first_config=nvram_safe_get("FirstConfig");
	int idx=0;
	char *value_ip,*value_mask;
	value_ip = nvram_safe_get("lan0_ipaddr");
	value_mask = nvram_safe_get("lan0_mask");
	idx = atoi(value_mask);
	value_mask = prefix2mask(idx);
	if (!CheckAccessRight(wp, url))
	{
	#if 0 //Ricardo 2013-01-08
		if (!strcmp(first_config, "1"))
			websRedirect(wp, "/easy.asp");
		else
	#endif
			websRedirect(wp, "/index.asp");
		return 1;
	}
	else
	{
	if (!strcmp(url, "/index.asp"))
	{
		char_t *cur_ip=websGetRequestIpaddr(wp);
		char_t accessable_ip[33];
		if (GetAccessableIpaddr((char_t *)&accessable_ip))
		{
			if (!strcmp(cur_ip, accessable_ip))
			{
			/*	if (!strcmp(first_config, "1"))
					websRedirect(wp, "local/genie.asp");
				else
			*/	
					websRedirect(wp, "local/index_login.asp");
				return 1;
			}
			//Delete By Andy Yu in 2013/10/17: Bug 4980
			/*else
			{
				if (strcmp(accessable_ip, "0.0.0.0"))
				{
                   // system("echo warning1>/dev/console");
                    if((inet_addr(value_ip) & inet_addr(value_mask)) != (inet_addr(accessable_ip) & inet_addr(value_mask)))
                    {
						system("echo '0.0.0.0' > /web/accessip");
						websRedirect(wp, "/index.asp");
					}
				    else
						websRedirect(wp, "index_warning.asp");
					return 1;
				}else if(checkSelectedIp(cur_ip)){
					//system("echo warning2>/dev/console");
                    websRedirect(wp, "index_warning2.asp");
					return 1;
				}
			} */
		}
	}
	if (!strcmp(url, "/logout.html"))
	{
		char_t *cur_ip=websGetRequestIpaddr(wp);
		char cmd[1024];
		//Steve
		system("echo '0.0.0.0' > /web/accessip");
		snprintf(cmd, 1024, "logger EZP_USR  %s\n ","logout successful" );
		system(cmd);			
		//system("echo '0.0.0.0' > /etc_ro/web/accessip");
		//doSystem("echo '0.0.0.0' > /etc_ro/web/accessip");
		//syslog(LOG_INFO, "Web management logout by user 'admin' from %s.\n", cur_ip);
		websRedirect(wp, "/index.asp");
		return 1;
	}
	/*----------------Added By Andy Yu in 2013/10/17: Force Login------------------*/
	if (!strcmp(url, "/force_login.html"))
	{
		char_t *cur_ip=websGetRequestIpaddr(wp);
		char cmd[1024] = {0};
		char_t faccessable_ip[33] = {0};
		struct sysinfo info;

		if (GetForceAccessableIpaddr((char_t *)&faccessable_ip)) {
			if (!strcmp(cur_ip, faccessable_ip)) {
				doSystem("echo '%s' > /web/accessip", cur_ip);
				
				sysinfo(&info);
				g_Admin_inactivity_timer_stamp=(int) info.uptime;
				
				system("echo '0.0.0.0' > web/faccessip");
				
				if (g_Force_login_current_username == 0) {
					ezplib_replace_attr("http_rule", 0, "curusername", "admin");
				} else {
					ezplib_replace_attr("http_rule", 0, "curusername", "guest");
				}
				
				snprintf(cmd, 1024, "logger EZP_USR  %s\n ","force login successful" );
				system(cmd);
				
				websRedirect(wp, "local/advance/dashboard.asp");
				return 1;
			}
		}
		websRedirect(wp, "/index.asp");
		return 1;
	}
	}
	return nRet;
}
#endif
int websSecurityHandler(webs_t wp, char_t *urlPrefix, char_t *webDir, int arg, 
						char_t *url, char_t *path, char_t *query)
{
	char_t			*type, *userid, *password, *accessLimit;
	int				flags, nRet;
	accessMeth_t	am;

	a_assert(websValid(wp));
	a_assert(url && *url);
	a_assert(path && *path);
/*
 *	Get the critical request details
 */
	type = websGetRequestType(wp);
	password = websGetRequestPassword(wp);
	userid = websGetRequestUserName(wp);
	flags = websGetRequestFlags(wp);
/*
 *	Get the access limit for the URL.  Exit if none found.
 */
	accessLimit = umGetAccessLimit(path);
	if (accessLimit == NULL) {
		//Steve
		#if 1//Arthur Chow 2009-01-17
			nRet = my_websSecurityHandler(wp, url);
			return nRet;
		#else		
			return 0;
		#endif	
	}
		 
/*
 *	Check to see if URL must be encrypted
 */
#ifdef WEBS_SSL_SUPPORT
	nRet = umGetAccessLimitSecure(accessLimit);
	if (nRet && ((flags & WEBS_SECURE) == 0)) {
		websStats.access++;
		websError(wp, 405, T("Access Denied\nSecure access is required."));
		trace(3, T("SEC: Non-secure access attempted on <%s>\n"), path);
      /* bugfix 5/24/02 -- we were leaking the memory pointed to by
       * 'accessLimit'. Thanks to Simon Byholm.
       */
      bfree(B_L, accessLimit);
		return 1;
	}
#endif

/*
 *	Get the access limit for the URL
 */
	am = umGetAccessMethodForURL(accessLimit);

	nRet = 0;
	if ((flags & WEBS_LOCAL_REQUEST) && (debugSecurity == 0)) {
/*
 *		Local access is always allowed (defeat when debugging)
 */
	} else if (am == AM_NONE) {
/*
 *		URL is supposed to be hidden!  Make like it wasn't found.
 */
		websStats.access++;
		websError(wp, 404, T("Page Not Found"));
		nRet = 1;
	} else 	if (userid && *userid) {
		if (!umUserExists(userid)) {
			websStats.access++;
			websError(wp, 401, T("Access Denied\nUnknown User"));
			trace(3, T("SEC: Unknown user <%s> attempted to access <%s>\n"), 
				userid, path);
			nRet = 1;
		} else if (!umUserCanAccessURL(userid, accessLimit)) {
			websStats.access++;
			websError(wp, 403, T("Access Denied\nProhibited User"));
			nRet = 1;
		} else if (password && * password) {
			char_t * userpass = umGetUserPassword(userid);
			if (userpass) {
				if (gstrcmp(password, userpass) != 0) {
					websStats.access++;
					websError(wp, 401, T("Access Denied\nWrong Password"));
					trace(3, T("SEC: Password fail for user <%s>")
								T("attempt to access <%s>\n"), userid, path);
					nRet = 1;
				} else {
/*
 *					User and password check out.
 */
				}

				bfree (B_L, userpass);
			}
#ifdef DIGEST_ACCESS_SUPPORT
		} else if (flags & WEBS_AUTH_DIGEST) {

			char_t *digestCalc;

/*
 *			Check digest for equivalence
 */
			wp->password = umGetUserPassword(userid);

			a_assert(wp->digest);
			a_assert(wp->nonce);
			a_assert(wp->password);
							 
			digestCalc = websCalcDigest(wp);
			a_assert(digestCalc);

			if (gstrcmp(wp->digest, digestCalc) != 0) {
				websStats.access++;
            /* 16 Jun 03 -- error code changed from 405 to 401 -- thanks to
             * Jay Chalfant.
             */
				websError(wp, 401, T("Access Denied\nWrong Password"));
				nRet = 1;
			}

			bfree (B_L, digestCalc);
#endif
		} else {
/*
 *			No password has been specified
 */
#ifdef DIGEST_ACCESS_SUPPORT
			if (am == AM_DIGEST) {
				wp->flags |= WEBS_AUTH_DIGEST;
			}
#endif
			websStats.errors++;
			websError(wp, 401, 
				T("Access to this document requires a password"));
			nRet = 1;
		}
	} else if (am != AM_FULL) {
//Steve		
#if 0//Arthur Chow 2008-11-14 : Don't check username password by standard http		
/*
 *		This will cause the browser to display a password / username
 *		dialog
 */
#ifdef DIGEST_ACCESS_SUPPORT
		if (am == AM_DIGEST) {
			wp->flags |= WEBS_AUTH_DIGEST;
		}
#endif
		websStats.errors++;
		websError(wp, 401, T("Access to this document requires a User ID"));
		nRet = 1;
#else//Arthur Chow 2008-12-17 : Check username password by login web page.	
		nRet = my_websSecurityHandler(wp, url);
#endif		
	}

	bfree(B_L, accessLimit);

	return nRet;
}

/******************************************************************************/
/*
 *	Delete the default security handler
 */

void websSecurityDelete()
{
	websUrlHandlerDelete(websSecurityHandler);
}

/******************************************************************************/
/*
 *	Store the new password, expect a decoded password. Store in websPassword in 
 *	the decoded form.
 */

void websSetPassword(char_t *password)
{
	a_assert(password);

	gstrncpy(websPassword, password, TSZ(websPassword));
}

/******************************************************************************/
/*
 *	Get password, return the decoded form
 */

char_t *websGetPassword()
{
	return bstrdup(B_L, websPassword);
}

/******************************************************************************/


