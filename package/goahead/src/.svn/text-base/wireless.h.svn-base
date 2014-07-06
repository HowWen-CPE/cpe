/*
 *	wireless.h -- Wireless Configuration Header 
 *
 *	Copyright (c) Ralink Technology Corporation All Rights Reserved.
 *
 *	$Id: wireless.h,v 1.9 2008-01-25 06:27:28 yy Exp $
 */
#include	<stdlib.h>
// Tommy, 2008/12/25 11:27¤W¤È
#include	<stdio.h>
#include <string.h>

void formDefineWireless(void);
void restart8021XDaemon(int nvram);
void updateFlash8021x(int nvram);
void Security(int nvram, webs_t wp, char_t *path, char_t *query);
void confWPAGeneral(int nvram, webs_t wp, int mbssid);
void confWEP(int nvram, webs_t wp, int mbssid);
//void conf8021x(int nvram, webs_t wp, int mbssid);
void conf8021x( webs_t wp, int mbssid);
//void getSecurity(int nvram, webs_t wp, char_t *path, char_t *query);
void getSecurity(webs_t wp, char_t *path, char_t *query);
void getMACFilter(webs_t wp, char_t *path, char_t *query);
void get5GMACFilter(webs_t wp, char_t *path, char_t *query);
void getSecurity_5g(webs_t wp, char_t *path, char_t *query);
//void DeleteAccessPolicyList(int nvram, webs_t wp, char_t *path, char_t *query);
void DeleteAccessPolicyList( webs_t wp, char_t *path, char_t *query);
char *sub_replace(char *source,char *sub,char *rep);
char *get_translate_char(int flag,char * sour_ssid);
char *get_html_translate_char(int flag,char * sour_ssid);
char *get_javascript_translate_char(int flag,char * sour_ssid);
