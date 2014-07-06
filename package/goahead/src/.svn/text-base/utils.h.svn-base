/*
 *	utils.h -- System Utilitie Header
 *
 *	Copyright (c) Ralink Technology Corporation All Rights Reserved.
 *
 *	$Id: utils.h,v 1.25 2008-03-17 09:28:40 yy Exp $
 */
#ifndef _UTILS_H
#define _UTILS_H

#include 	"webs.h"
#include	"uemf.h"

#define LED_ON				1
#define LED_OFF				0

#define WPS_LED_RESET			1
#define WPS_LED_PROGRESS		2
#define WPS_LED_ERROR			3
#define WPS_LED_SESSION_OVERLAP		4
#define WPS_LED_SUCCESS			5

#define LONG_BUF_LEN			8192
#define SHORT_BUF_LEN			32	
/*
#define websDebugWrite(wp, fmt, args...)                            \
	({ error_value = 1;                                     \
	 if(!config_validate_msg)                             \
	 wfputs("message=", wp);                            \
	 int TMPVAR = wfprintf(wp, fmt, ## args);             \
	 if(!config_validate_msg) wfputs("\n", wp);          \
	 wfflush(wp); TMPVAR; })
*/
#define websDebugWrite(wp, fmt, args...)                             
struct service
{
    int64_t flag;
    char *script;
    char *script2;
};

struct variable
{
    char *name;
    char *longname;
    int (*valid) (webs_t wp, char *value, struct variable *v);
    int (*save) (webs_t wp, char *value, struct variable *v, struct service *s);
    char **argv;
    int nullok;
    int64_t action_flag;
};

int valid_choice(webs_t wp, char *value, struct variable *v);
int valid_word(webs_t wp, char *value, struct variable *v);
int valid_ipaddr(webs_t wp, char * value, struct variable * v);
int valid_domain(webs_t wp, char * value, struct variable * v);
int valid_number(webs_t wp, char * value, struct variable * v);
int valid_range(webs_t wp, char * value, struct variable * v);
int wfputs(char *buf, FILE * fp);
int wfprintf(FILE * fp, char *fmt, ...);
int wfflush(FILE * fp);
int is_digit_str(char * str);
char *str_replace(char *source,char *sub,char *rep);
int is_legal_ipaddr(char *value);

int valid_strict_name(webs_t wp, char *value, struct variable *v);
int valid_if(webs_t wp, char *value, struct variable *v);
int valid_netmask(webs_t wp, char *value, struct variable *v);
int valid_length_range(webs_t wp, char *value, struct variable *v);


int doKillPid(char_t *fmt, ...);
int doSystem(char_t *fmt, ...);
char *getNthValue(int index, char *values);
char *setNthValue(int index, char *old_values, char *new_value);
int deleteNthValueMulti(int index[], int count, char *value, char delimit);
int getValueCount(char *values);
void formDefineUtilities(void);
int checkSemicolon(char *str);
int getModIns(char *modname);
int isInLan(char *radius_ip_addr);
int getNthValueSafe(int index, char *value, char delimit, char *result, int len);
int setTimer(int mili, void ((*sigroutine)(int)));
void stopTimer(void);
int GetFirmTag(int eid, webs_t wp, int argc, char_t **argv);
extern int MatchIpAndMac(int eid, webs_t wp, int argc, char_t **argv);
int GetWireTag(int eid, webs_t wp, int argc, char_t **argv);

// Tommy, let it complier
//int ledAlways(int gpio, int on);
//int ledWps(int gpio, int mode);
unsigned int Sleep(unsigned int secs);
char *racat(char *s, int i);
void arplookup(char *ip, char *arp);
void websLongWrite(webs_t wp, char *longstr);
int netmask_aton(const char *ip);
inline int getOnePortOnly(void);
#if 0
#define STEVEPRINT(fmt, args...)		printf(fmt, ## args)
#else
#define STEVEPRINT(fmt, args...)
#endif
#define FW_VERSION "V0.3.2"
#define FW_DATE ",2011/07/29"


#endif //_UTILS_H


