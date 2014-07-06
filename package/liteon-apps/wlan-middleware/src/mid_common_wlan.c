#include    <stdlib.h>
#include    <stdio.h>
#include <string.h>
#include        "nvram.h"
#include "nvram_rule.h"

#include "mid_common.h"
#include	<sys/ioctl.h>
#include	<unistd.h>

/*
*
*String Process
*
*/
const int days[12]={31,28,31,30,31,30,31,31,30,31,30,31};
 

//Added by Mario Huang
int length(char str[])
{
    int i=0;
    while (str[i] !='\0' && i<255) i++;
    if (i==255 && str[255]!='\0')
       str[i--]='\0';
    return(i);
}

int CheckHexString(char *p)
{
    int i;
    char c;

    for (i = 0; i < strlen(p); i++)
    {
        c = p[i];
        if ((c >= '0' && c <= '9') ||
            (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
            continue;

        /* if reach here, then non hex string */
        return T_FAILURE;
    }
    return 1;
}

//Ignore space charactors at the end of string
int null_string(char *str)
{
        while(*str!= '\0')
        {
                if((*str) != 0x20)
                {
                        return 0;
                }
                str++;
        }

        return 1;
}


/**
 * @brief the center chann of HT80 bandwidth
 * @param[in] channel	1~11, 36~64, 100~140, 149~165			
 * @return 	0 if 80MHz is not supported in this channel;
 * 			else the center channel of the HT80 bandwidth
 *
 * @author frank_zhou@lccbj.com
 */
unsigned char centerHT80(int channel)
{
	switch(channel) {
		case 36:
		case 40:
		case 44:
		case 48:
			return 42;
		case 52:
		case 56:
		case 60:
		case 64:
			return 58;
		case 100:
		case 104:
		case 108:
		case 112:
			return 106;
		case 116:
		case 120:
		case 124:
		case 128:
			return 122;
		case 149:
		case 153:
		case 157:
		case 161:
			return 155;
		default:
			return 0;
	}
}

//Determine if the charactor is the last meaningfull charactor of one string
int string_end(char *str)
{
    char *buf_tmp;
    buf_tmp = str + 1;
    if(*buf_tmp == '\0')
        {
            return 1;
        }
    else
        {
            return null_string(buf_tmp);
        }
}

/* Add by Andy in 2014/01/23: Get br-lan0 MAC */
int get_lan_mac(char *lanhwaddr)
{
	char ifname[16] = {0};
	char device[16] = {0};
	char hwaddr[64] = {0};
	int ret = -1;

	strcpy(ifname, nvram_safe_get("lan0_ifname"));

	/* PPPoE interface doesn't have MAC address. */
	if (!strncmp(ifname, "ppp", 3)) {
		if (snprintf(device, 16, "lan0_device") >= 16)
		{
			return T_FAILURE;
		}
		strcpy(ifname, nvram_safe_get(device));
	}

	ret = get_hwaddr(ifname, hwaddr, 64);
	if (T_FAILURE == ret) {
		printf("ERROR: Get LAN MAC Error\n");
		return T_FAILURE;
	}
	else {
		strcpy(lanhwaddr, hwaddr);
	}
	
	return T_SUCCESS;
}

int get_hwaddr(char *ifname, char *ptr, int length)
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
	return T_SUCCESS;

err:
	if (fd > -1)
		close(fd);
	return T_FAILURE;
}

//Find a pattern form string,if pattern is include in the string, return T_SUCCESS, or return T_FAILURE
int find_pattern_from_str(char str[], char pattern[])
{
    int lnth, lnthdif,i,j,k;
    lnth=length(pattern);
    lnthdif=length(str)-lnth+1;
    if (lnth>0 && lnthdif>0){
            i=0;
            while (i<lnthdif){
                    j=i; k=0;
                    while (k<lnth && str[j++]==pattern[k]) k++;
                    if (k==lnth){
                            return T_SUCCESS;
                    }else{
                            i++;
                    }
            }
    }
    return T_FAILURE;
}

void delspace(char *Buf1, char* Buf2)
{
        while(*Buf1 != '\0')
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
//Copy string2 to string1, ingnore space charactors
void strcpy_delspace(char *str1, char *str2)
{
        while(*str1 != '\0')
        {
                if((*str1) == 0x20)
                {
                        ++str1; // if space skip 
                }
                else
                *str2++ = *str1++; // if not space copy to buf 2
        }
        *str2 = '\0'; //NULL Terminate

}

void delspace_bssid(char *Buf1, char* Buf2)
{
        while(*Buf1 != '\0')
        {
                if((*Buf1) == 0x20)
                {
                        ++Buf1; // if space skip 
                }
                else if((*Buf1) <= 'f' && (*Buf1) >= 'a')
                {
                    *Buf2++ = (*Buf1) - 32;
                    Buf1++; 
                }
                else
                *Buf2++ = *Buf1++; // if not space copy to buf 2
        }
        *Buf2 = '\0'; //NULL Terminate
}

void convert_bssid(char *Buf1, char* Buf2)
{
        while(*Buf1 != '\0')
        {
                if((*Buf1) == 0x20)
                {
                        ++Buf1; // if space skip 
                }
                else if((*Buf1) <= 'F' && (*Buf1) >= 'A')
                {
                    *Buf2++ = (*Buf1) + 32;
                    Buf1++; 
                }
                else
                *Buf2++ = *Buf1++; // if not space copy to buf 2
        }
        *Buf2 = '\0'; //NULL Terminate
}
//Get bssid from a string and convert lower-case letter to upper-case letter
void format_bssid_upper(char *str1, char *str2)
{
        while(*str1 != '\0')
        {
                if((*str1) == 0x20)
                {
                        ++str1; // if space skip 
                }
                else if((*str1) <= 'f' && (*str1) >= 'a')
                {
                    *str2++ = (*str1) - 32;
                    str1++; 
                }
                else
                *str2++ = *str1++; // if not space copy to buf 2
        }
        *str2 = '\0'; //NULL Terminate
}

void delspace2(char *Buf1, char* Buf2)
{
	int i=0;
        while(*Buf1 != '\0')
        {
                if((*Buf1) == 0x20 && null_string(Buf1))
                //if((*Buf1) == 0x20)
                {
                        break;
                }
                else if(((*Buf1) == '\'' || (*Buf1) == '\"') && (i==0 || string_end(Buf1)))
                {
                        ++Buf1; // if " or ' skip 
			i++;
                }
                else
		{
                	*Buf2++ = *Buf1++; // if not space copy to buf 2
			i++;
		}
        }
        *Buf2 = '\0'; //NULL Terminate
}
//Get bssid from a string and convert upper-case letter to lower-case letter
void format_bssid_lower(char *str1, char* str2)
{
        while(*str1 != '\0')
        {
                if((*str1) == 0x20)
                {
                        ++str1; // if space skip 
                }
                else if((*str1) <= 'F' && (*str1) >= 'A')
                {
                    *str2++ = (*str1) + 32;
                    str1++; 
                }
                else
                *str2++ = *str1++; // if not space copy to buf 2
        }
        *str2 = '\0'; //NULL Terminate
}

#if defined(GP_WP688)
void delspace1(char *Buf1, char* Buf2)
{
        while(*Buf1 != '\0')
        {
                if((*Buf1) == 0x20 && null_string(Buf1))
                //if((*Buf1) == 0x20)
                {
                        break;
                }
                else
                *Buf2++ = *Buf1++; // if not space copy to buf 2
        }
        *Buf2 = '\0'; //NULL Terminate

}
#elif defined(GP_WP838)
//Get bssid from a string and delete the ' or " at the begining or end of the string
void format_ssid(char *str1, char* str2)
{
    int i=0;
        while(*str1 != '\0')
        {
                if((*str1) == 0x20 && null_string(str1))
                //if((*str1) == 0x20)
                {
                        break;
                }
                else if(((*str1) == '\'' || (*str1) == '\"') && (i==0 || string_end(str1)))
                {
                        ++str1; // if " or ' skip 
                        i++;
                }
                else
                {
                    *str2++ = *str1++; // if not space copy to buf 2
                    i++;
                }
        }
        *str2 = '\0'; //NULL Terminate
}
#endif


#if defined(GP_WP688)
#elif defined(GP_WP838)
int convert_special_characters(int radio, char *target_str, int target_len, char *origin_str)
{
    int i, j;
    for(i=0,j=0;j<strlen(origin_str);i++,j++) {
        if( '\"' == origin_str[j] 
            || '`' == origin_str[j]
            || '\\' == origin_str[j]) 
        {
            if(i < target_len)
            {
                target_str[i] = '\\';
                i++;
            }
            else
            {
                MID_ERROR("Target string length is not enough!!!");
                return T_FAILURE;
            }
        }

        if(i < target_len)
        {
            target_str[i] = origin_str[j];
        }
        else
        {
            MID_ERROR("Target string length is not enough!!!");
            return T_FAILURE;
        }
       
    }
    
    if(i < target_len)
    {
        target_str[i] = '\0';
    }
    else
    {
        MID_ERROR("Target string length is not enough!!!");
        return T_FAILURE;
    }
    
    return T_SUCCESS;
}
#endif


/* Adjust Leap Year or not? */
int isLeapYear(int year)
{
    if(((year%4==0)&&(year%100!=0)) || (year%400==0)){
        return 1;
    }
    return 0;
}

date sec2date(long sec)
{
    date d={YEAR,1,1,0,0,0};
    int ds,sum;

    ds=sec/(24*60*60);
    sec-=ds*(24*60*60);
    d.hour=sec/(60*60);
    sec-=d.hour*(60*60);
    d.min=sec/60;
    sec-=d.min*60;
    d.sec=sec;
    while(1){
        sum=365;
        if(isLeapYear(d.year)){
            sum+=1;
        }
        
        if(ds<sum){
            break;
        }
    
        d.year++;
        ds-=sum;
    }
    ds++;    
    
    while(1){
        sum=days[d.month-1];
        if(d.month==2){
            if(isLeapYear(d.year)){
                sum+=1;
            }
        }
        
        if(ds <= sum){            
            d.day=ds;
            break;
        }
        
        d.month++;
        ds-=sum;
    }
    return d;
}

#if defined(GP_WP688)
#elif defined(GP_WP838)
//Get channel from string
int extract_channel(char channel[], char str[], char pattern[])
{
    int lnth, lnthdif,i,j,k;
    lnth=length(pattern);
    lnthdif=length(str)-lnth+1;
    if (lnth>0 && lnthdif>0){
        i=0;
        while (i<lnthdif){
            j=i; k=0;
            while (k<lnth && str[j++]==pattern[k]) k++;
            if (k==lnth){
                if (str[i+lnth+1] == ' '){
                channel[0] = str[i+lnth];
                channel[1] = '\0';
                return T_SUCCESS;
                }else if(str[i+lnth+2] == ' '){
                channel[0] = str[i+lnth];
                channel[1] = str[i+lnth+1];
                channel[2] = '\0';
                return T_SUCCESS;
                }else{
                channel[0] = str[i+lnth];
                channel[1] = str[i+lnth+1];
                channel[2] = str[i+lnth+2];
                channel[3] = '\0';
                return T_SUCCESS;
               }
            }else{
                i++;
            }
        }
    }
    return T_FAILURE;
}
#endif

//Get associated bssid of sta from a string
#if 0
#if defined(GP_WP688)
int extract_assoc_bssid(char bssid[], char str[], char pattern[])
{
    int lnth, lnthdif,i,j,k;
        char tmpBssid[32];
    lnth=length(pattern);
    lnthdif=length(str)-lnth+1;
    if (lnth>0 && lnthdif>0){
        i=0;
        while (i<lnthdif){
            j=i; k=0;
                        //printf("str is %s\n", &str[j]);
                        //printf("pattern is %s\n", pattern);
            while (k<lnth && str[j++]==pattern[k]) k++;
            if (k==lnth){
                    //snprintf(tmpBssid, 18, "%s", &str[i+lnth]);
                                strcpy(tmpBssid, (str+i+lnth));
                                printf("tmpBssid is %s\n", tmpBssid);
                                format_bssid_upper(tmpBssid, bssid);
                                //strcpy(bssid, &str[i+lnth]);
                return T_SUCCESS;
            }else{
                i++;
            }
        }
    }
    return T_FAILURE;
}
#elif defined(GP_WP838)
#endif
#endif

#if defined(GP_WP688)
#elif defined(GP_WP838)
int extract_assoc_ssid(char ssid[], char str[], char pattern[])
{
    int lnth, lnthdif,i,j,k;
        char tmpSsid[64];
    lnth=length(pattern);
    lnthdif=length(str)-lnth+1;
    if (lnth>0 && lnthdif>0){
        i=0;
        while (i<lnthdif)
        {
            j=i; k=0;
            //printf("str is %s\n", &str[j]);
            //printf("pattern is %s\n", pattern);
            while (k<lnth && str[j++]==pattern[k]) k++;
            if (k==lnth)
            {
                //snprintf(tmpBssid, 18, "%s", &str[i+lnth]);
                strcpy(tmpSsid, (str+i+lnth));
                printf("tmpSsid is %s\n", tmpSsid);
                format_ssid(tmpSsid, ssid);
                //strcpy(bssid, &str[i+lnth]);
                return T_SUCCESS;
            }else{
                i++;
            }
        }
    }
    return T_FAILURE;
}
#endif

//Read one line from a file
void readline(char str[], FILE *fin)
{
    int n=0;
    char c;
    while ((c=fgetc(fin)) != '\n' && n<255)
        str[n++] = c;
    str[n] = '\0';
}

#if defined(GP_WP688)
int convert_vap_id(int radio, unsigned char vap_id)
{
    int actual_id;
    actual_id = vap_id;
    return actual_id;
}
#elif defined(GP_WP838)
int convert_vap_id(int radio, unsigned char vap_id)
{
    int actual_id = 0;
    
#ifdef SINGLE_RADIO
    actual_id = vap_id*2;
#else

    if(RADIO_2G == radio)
        {
            actual_id = vap_id*2;
        }
    else if(RADIO_5G == radio)
        {
            actual_id = vap_id*2 + 1;
        }
#endif
    return actual_id;

}

#endif
/**
 * \brief  Construct VAP Interface Name according to radio(2.4G/5G), 
 *		   vapi_id(0~7) and mode(WLAN_MODE_AP|WLAN_MODE_STA)
 * \return T_SUCCESS on Success, T_FAILURE on Failure
 * \param[out] name the generated interface name
 * \param[in] radio RADIO_2G or RADIO_5G
 * \param[in] vap_id 0~7
 * \param[in] mode WLAN_MODE_AP or WLAN_MODE_STA
 */
int construct_vap(char* name, unsigned int radio, unsigned char vap_id, unsigned char mode)
{
    int actual_id;

    actual_id = convert_vap_id(radio, vap_id); 
    if(RADIO_2G == radio)
        {
            if(WLAN_MODE_AP == mode)
                {
                  sprintf(name, "%s%d", AP_NAME_2G, actual_id);
                }
            else if(WLAN_MODE_STA == mode)
                {
                  sprintf(name, "%s%d", STA_NAME_2G, actual_id);
                }
            else
                {
                  printf("ERROR:Construct vap error!\n");
                   return T_FAILURE;
                }
        }
    else if(RADIO_5G == radio)
      {
            if(WLAN_MODE_AP == mode)
                {
                  sprintf(name, "%s%d", AP_NAME_5G, actual_id);
                }
            else if(WLAN_MODE_STA == mode)
                {
                  sprintf(name, "%s%d", STA_NAME_5G, actual_id);
                }
            else
                {
                  printf("ERROR:Construct vap error!\n");
                   return T_FAILURE;
                }
      }
    else
        {
            printf("ERROR:Construct vap error!\n");
            return T_FAILURE;
        }

    return T_SUCCESS;
}


/*Construct main ap name, only used in Ralink now*/
int construct_main_ap(char* name, unsigned int radio)
{
    return construct_vap(name, radio, 0, WLAN_MODE_AP);
}

static int vap_already_in_bridge(int radio, int vapid, int mode)
{
    char vap_name[VAP_NAME_LEN];
    char cmd[128] = {0};
    FILE *fin;
    char str[256] = {0},c;
    char bridge_name[32] = {0};
    int opmode = -1;
    //Get bridge name 
    //if wlan mode is ap, uset lan bridge
    if(WLAN_MODE_AP == mode)
    {
        strcpy(bridge_name, nvram_get("lan0_ifname"));
    }
    //if wlan mode is sta, we have to judge if it is station mode or wisp mode
    else
    {
        nvram_get_system_mode(&opmode);
        if((OP_MODE_WISP0 == opmode) || (OP_MODE_WISP1 == opmode))
        {
            strcpy(bridge_name, nvram_get("wan0_device"));
        }
        else
        {
            strcpy(bridge_name, nvram_get("lan0_ifname"));
        }
    }

    /*Construct vap name by radio, vap_id and vap mode*/
    construct_vap(vap_name, radio, vapid, mode);
    
    sprintf(cmd, "brctl show %s | grep -w %s > /tmp/bridge_info", bridge_name, vap_name);
    EXE_COMMAND(cmd);
    fin = fopen("/tmp/bridge_info","r");
    /*Judge if there is vap name available in the bridge*/
    while ((c=fgetc(fin)) != EOF){
        ungetc(c,fin);        
        readline(str,fin);
        if (find_pattern_from_str(str, vap_name) == T_SUCCESS)
            {
                printf("vap %s is in bridge %s\n", vap_name, bridge_name);
                fclose(fin);
                EXE_COMMAND("rm -f /tmp/bridge_info");    
                return 1;
            }
    }

    printf("vap %s is not in bridge %s now\n", vap_name, bridge_name);
    fclose(fin);
    EXE_COMMAND("rm -f /tmp/bridge_info");    

    return 0;
}

/*If the bridge has not been set up or the VAPs of one radio has not added into the bridge, wait for a moment*/
int wait_for_bridge_setup(int radio, int mode)
{
    int max_wait = 20;
    int vap_num = 0;
    int i=0;

    if(WLAN_MODE_AP == mode)
    {
        //get vap count first
        vap_num = atoi(nvram_safe_get("wlv_rule_num")); 
    }

    while(max_wait > 0)
    {
        if(WLAN_MODE_AP == mode)
        {
            for(; i<vap_num; i++)
            {
                if (vap_already_in_bridge(radio, i, mode))
                {
                    printf("VAP has been added into bridge successfully\n");
                    continue;
                }
                else
                {
                    printf("VAP has not been added into bridge\n");
                    break;
                }  
            }

            if(i == vap_num)
            {
                printf("All the VAPs have been added into bridge successfully\n");
                break;
            }
	}
        else
        {
            if (vap_already_in_bridge(radio, 0, mode))
            {
                printf("VAP has been added into bridge successfully\n");
                break;
            }
            else
            {
                printf("VAP has not been added into bridge\n");
                break;
            }  

        }
        sleep(2);
        max_wait=max_wait - 2;
    }

    if(max_wait <= 0)
    {
        return T_FAILURE;
    }
    else
    {
        return T_SUCCESS;
    }
}


#if defined(GP_WP688)
//VAP up and down
int vap_up_down(int radio, int vapid, int mode, int up)
{
    char vap_name[VAP_NAME_LEN];
    int ret;
    char cmd[128];
    /*Construct vap name by radio, vap_id and vap mode*/
    ret = construct_vap(vap_name, radio, vapid, mode);
    if(T_FAILURE == ret)
        {
                printf("ERROR:Construct VAP failure!\n");
        }

    if(up)
        {
            sprintf(cmd, "ifconfig %s up", vap_name);
            EXE_COMMAND(cmd);
        }
    else
        {
            sprintf(cmd, "ifconfig %s down", vap_name);
            EXE_COMMAND(cmd);
        }
    return T_SUCCESS;
}
#elif defined(GP_WP838)
//VAP up and down
int vap_up_down(int radio, int vapid, int mode, int up)
{
    char vap_name[VAP_NAME_LEN];
    int ret;
    char cmd[128];
    /*Construct vap name by radio, vap_id and vap mode*/
    ret = construct_vap(vap_name, radio, vapid, mode);
    if(T_FAILURE == ret)
        {
                printf("ERROR:Construct VAP failure!\n");
        }

    if(up)
        {
            sprintf(cmd, "ifconfig %s up", vap_name);
            EXE_COMMAND(cmd);
        }
    else
        {
            sprintf(cmd, "ifconfig %s down", vap_name);
            EXE_COMMAND(cmd);
        }

    if(up)
    {
        /* wait 500 million seconds */
        usleep(500000);
    }
    else
    {
        usleep(500000);
    }

    return T_SUCCESS;
}

static int vap_already_exist(int radio, int vapid, int mode)
{
    char vap_name[VAP_NAME_LEN];
    char cmd[128] = {0};
    FILE *fin;
    char str[256] = {0},c;

    /*Construct vap name by radio, vap_id and vap mode*/
    construct_vap(vap_name, radio, vapid, mode);
    
    sprintf(cmd, "iwconfig | grep -w %s > /tmp/vap_validity", vap_name);
    EXE_COMMAND(cmd);
    fin = fopen("/tmp/vap_validity","r");
    /*If there is vap name available in the file*/
    while ((c=fgetc(fin)) != EOF){
        ungetc(c,fin);        
        readline(str,fin);
        if (find_pattern_from_str(str, vap_name) == T_SUCCESS)
            {
                printf("vap %s is exist\n", vap_name);
                fclose(fin);
                EXE_COMMAND("rm -f /tmp/vap_validity");    
                return 1;
            }
    }

    printf("vap %s is not exist now\n", vap_name);
    fclose(fin);
    EXE_COMMAND("rm -f /tmp/vap_validity");    

    return 0;
}

static int authentication_daemon_exist(char *daemon_pid)
{
    char cmd[128] = {0};
    FILE *fin;
    char str[256] = {0},c;
    sprintf(cmd, "ps > /tmp/auth_daemon_validity");
    EXE_COMMAND(cmd);
    fin = fopen("/tmp/auth_daemon_validity","r");
    if(NULL == fin)
    {
        printf("Open file /tmp/auth_daemon_validity error!!!\n");
        return 0;
    }
    /*If there is wpa_supplicant available in the file*/
    while((c=fgetc(fin)) != EOF){
        ungetc(c,fin);        
        readline(str,fin);
        if (find_pattern_from_str(str, daemon_pid) == T_SUCCESS)
            {
                printf("authentication daemon is exist\n");
                fclose(fin);
                EXE_COMMAND("rm -f /tmp/auth_daemon_validity");    
                return 1;
            }
    }

    printf("authentication daemon is not exist now\n");
    fclose(fin);
    EXE_COMMAND("rm -f /tmp/auth_daemon_validity");    

    return 0;
}


int create_vap(int radio, int vapid, int mode)
{
    char vap_name[VAP_NAME_LEN];
    int vap_exist = 0;
    int ret = T_FAILURE;
    char cmd[128] = {0};
    int max_sleep=20;
    /*Construct vap name by radio, vap_id and vap mode*/
    ret = construct_vap(vap_name, radio, vapid, mode);
    MID_ASSERT((T_SUCCESS == ret), "ERROR:Construct VAP failure!");
    MID_ASSERT(((WLAN_MODE_AP == mode) || (WLAN_MODE_STA == mode)), "ERROR:Invalid wlan mode!");
    //Create vap first
    if(WLAN_MODE_AP == mode) 
    {
        sprintf(cmd, "wlanconfig %s create wlandev wifi%d wlanmode ap", vap_name, radio);
        EXE_COMMAND(cmd);
        //sleep(1);
    }
    else if(WLAN_MODE_STA == mode) 
    {
        sprintf(cmd, "wlanconfig %s create wlandev wifi%d wlanmode sta nosbeacon", vap_name, radio);
        EXE_COMMAND(cmd);
        //sleep(1);
    }
    
    while(max_sleep > 0)
    {
        //Check if the VAP has been created currectly
        vap_exist = vap_already_exist(radio, vapid, mode);
        //Created successful, break the cycle
        if(vap_exist)
        {
            printf("Create vap Success!!!\n");
            break;
        }
        //Have not set up successfully, wait for another 1 second
        else
        {
            sleep(2);
	}
        max_sleep = max_sleep - 2;
    }

    vap_exist = vap_already_exist(radio, vapid, mode);
    if(!vap_exist)
    {
        MID_ERROR("VAP create failure!!!");
        return T_FAILURE;
    }
    return T_SUCCESS;
}

int destroy_vap(int radio, int vapid, int mode)
{
    char vap_name[VAP_NAME_LEN];
    int vap_exist = 0;
    int ret = T_FAILURE;
    char cmd[128] = {0};
    int max_sleep=20;
  
    printf("Enter destroy_vap\n");
    /*Construct vap name by radio, vap_id and vap mode*/
    ret = construct_vap(vap_name, radio, vapid, mode);
    MID_ASSERT((T_SUCCESS == ret), "ERROR:Construct VAP failure!");
    MID_ASSERT(((WLAN_MODE_AP == mode) || (WLAN_MODE_STA == mode)), "ERROR:Invalid wlan mode!");
    //Down the vap before destory
    //vap_up_down(radio, vapid, mode, VAP_DOWN);    
    //Destroy the vap by command
    sprintf(cmd, "wlanconfig %s destroy", vap_name);
    EXE_COMMAND(cmd);

    while(max_sleep > 0)
    {
        //Check if the VAP has been destroy
        vap_exist = vap_already_exist(radio, vapid, mode);
        //Already estory, break the cycle
        if(!vap_exist)
        {
            printf("Destroy vap Success!!!\n");
            break;
        }
        //Have not been destroy, wait for another 1 second
        else
        {
            sleep(2);
	}
        max_sleep = max_sleep - 2;
    }

    //Check if the VAP has been destroy
    vap_exist = vap_already_exist(radio, vapid, mode);
    if(vap_exist)
    {
        MID_ERROR("VAP has not been destroy correctly!!!");
        return T_FAILURE;
    }

    return T_SUCCESS;
}

int create_all_vap(int radio, int mode)
{
    int i;
    //RalinkVAP_t vap_list[RADIOA_VAP_MAX_NUM];
    int vap_num=0;
    /*Get VAP num from wlv_rule_num*/
    vap_num = atoi(nvram_safe_get("wlv_rule_num"));
    printf("vap_num is %d\n",vap_num);
    MID_ASSERT(((vap_num > 0) && (vap_num < 9)), "ERROR:VAP count error!!!");
    MID_ASSERT(((WLAN_MODE_AP == mode) || (WLAN_MODE_STA == mode)), "ERROR:Invalid wlan mode!");

    if(WLAN_MODE_AP == mode)
    {
        for(i=0; i < vap_num; i++)
        {
            create_vap(radio, i, mode);
        }
    }
    else if(WLAN_MODE_STA == mode)
    {
        create_vap(radio, 0, mode);
#if defined(GP_WP838)
		set_extra_sta(radio);
#endif
    }

    return T_SUCCESS;
}

int destroy_all_vap(int radio, int mode)
{
    int i;
    //RalinkVAP_t vap_list[RADIOA_VAP_MAX_NUM];
    int vap_num=0;
    printf("Enter destroy_all_vap!!!\n");
    /*Get VAP num from wlv_rule_num*/
    vap_num = atoi(nvram_safe_get("wlv_rule_num"));
    printf("vap_num is %d\n",vap_num);
    printf("mode is %d\n",mode);

    MID_ASSERT(((vap_num > 0) && (vap_num < 9)), "ERROR:VAP count error!!!");
    MID_ASSERT(((WLAN_MODE_AP == mode) || (WLAN_MODE_STA == mode)), "ERROR:Invalid wlan mode!");
//    if(WLAN_MODE_AP == mode)
//    {
        //Down the radio 
        radio_up_down(radio, RADIO_DOWN, mode);
        //Kill all the security daemon first
        kill_all_authentication_daemon(radio, mode);
//    }
//    else if(WLAN_MODE_STA == mode) 
//    {
        //Kill all the security daemon first
//        kill_all_authentication_daemon(radio, mode);
        //Down the radio 
//        radio_up_down(radio, RADIO_DOWN, mode);
//    }
    if(WLAN_MODE_AP == mode)
    {
        for(i=0; i < vap_num; i++)
        {
            destroy_vap(radio, i, mode);
        }
    }
    else if(WLAN_MODE_STA == mode)
    {
        destroy_vap(radio, 0, mode);
    }

    return T_SUCCESS;
}
#endif //End of    #if defined(GP_WP688)




//Radio up or down
#if defined(GP_WP688)
/*Set Radio(2.4G or 5G) on/off, we use rai0(2.4G) or ra0(5G) as main ap*/
int radio_up_down(int radio, int up)
{
    int ret;
    int i;
    char radio_name[VAP_NAME_LEN];
    //RalinkVAP_t vap_list[RADIOA_VAP_MAX_NUM];
    int vap_num;
    int radio_status;
    char cmd[128];
    /*Construct main VAP name*/
    construct_main_ap(radio_name, radio);

    /*Judge if up or down*/
    if(up)
        {
            /*Get the radio status, if is already up, return*/
	     ret = get_radio_status(radio, &radio_status);
	     if(T_FAILURE == ret) 
	     	{
			printf("get radio status failure!\n");
			return T_FAILURE; 
		}
	     if(RADIO_UP == radio_status)
	     	{
	     		return T_SUCCESS;
	     	}
#if 0
            /*main VAP up*/
	     sprintf(cmd, "ifconfig %s up", radio_name);
            system(cmd);
	     /*Wait for a moment*/
	     sleep(5);

            /*Set RadioOn as on*/
	     memset(cmd, 0, sizeof(cmd));
            /*Set RadioOn as on*/
	     sprintf(cmd, "iwpriv %s set RadioOn=1", radio_name);
            system(cmd);
#endif 
            /*main VAP up*/
	     sprintf(cmd, "/etc/wl/wlan-init create");
            system(cmd);
	     /*Wait for a moment*/
	     sleep(5);
#if 0
	     memset(cmd, 0, sizeof(cmd));
	     sprintf(cmd, "/etc/wl/wlan-init init");
            system(cmd);
#endif
            /*Set RadioOn as on*/
	     memset(cmd, 0, sizeof(cmd));
            /*Set RadioOn as on*/
	     sprintf(cmd, "iwpriv %s set RadioOn=1", radio_name);
            system(cmd);
       }
    /*Down the radio*/
    else
    	{
	     /*Get VAP num from wlv_rule_num*/
	     vap_num = atoi(nvram_safe_get("wlv_rule_num"));
            printf("vap_num is %d\n",vap_num);
            /*Down all the VAPs exclude main VAP*/
            for(i=1; i < vap_num; i++)
            	{	
			ret = vap_up_down(radio,  i, WLAN_MODE_AP, VAP_DOWN);	
			if(T_FAILURE == ret)
				{
					printf("ERROR:up/dwon VAP failure\n");
					return T_FAILURE;
				}
            	}
	     /*Down all the VAPs in WDS mod*/
#if 0
            for(i=0; i < WDS_NUM; i++)
            	{
			ret = vap_up_down(radio,  i, MODE_WDS, VAP_DOWN);	
			if(T_FAILURE == ret)
				{
					printf("ERROR:up/dwon WDS VAP failure\n");
					return T_FAILURE;
				}
            	}
#endif
	     /*Down APCLI*/
	     ret = vap_up_down(radio,  0, WLAN_MODE_STA, VAP_DOWN);	
	     if(T_FAILURE == ret)
		{
			printf("ERROR:up/dwon APCLI VAP failure\n");
			return T_FAILURE;
		}
	     
            /*main VAP down*/
	     sprintf(cmd, "ifconfig %s down", radio_name);
            system(cmd);
	     /*Wait for a moment*/
	     sleep(3);
		 
	     memset(cmd, 0, sizeof(cmd));
            /*Set RadioOn as off*/
	     sprintf(cmd, "iwpriv %s set RadioOn=0", radio_name);
            system(cmd);
    	}
    return T_SUCCESS;
}
#elif defined(GP_WP838)
int kill_all_authentication_daemon(int radio, int mode)
{
    int i;
    int vap_num;
    char vap_name[VAP_NAME_LEN];
    char cmd[128]={0};
    char daemon_pid[64]={0};
    int max_sleep=20;
    int daemon_exist = 0;
    /*Get VAP num from wlv_rule_num*/
    vap_num = nvram_get_vap_num(radio); 
    printf("vap_num is %d\n",vap_num);

    /*Down all the VAPs, down main VAP at last*/
    if(WLAN_MODE_AP == mode)
    {
        for(i = vap_num - 1; i >= 0; i--)
            {    
                //Kill the security task 
                construct_vap(vap_name, radio, i, mode);
                memset(cmd, 0, sizeof(cmd));
                sprintf(cmd, "kill `cat /var/run/hostapd_%s.pid`",vap_name);
                EXE_COMMAND(cmd);
                //sleep(1);
                daemon_exist = 0;
                memset(daemon_pid, 0, sizeof(daemon_pid));
                sprintf(daemon_pid, "hostapd_%s.pid",vap_name);
                while(max_sleep > 0)
                {
                    //Check if the VAP has been destroy
                    daemon_exist = authentication_daemon_exist(daemon_pid);
            
                    //Already estory, break the cycle
                    if(!daemon_exist)
                    {
                        printf("Destroy daemon Success!!!\n");
                        break;
                    }
                    //Have not been destroy, wait for another 2 second
                    else
                    {
                        sleep(2);
            	    }
                    max_sleep = max_sleep - 2;
                }
            }
    }
    else if(WLAN_MODE_STA == mode)
    {
        //Kill the security task 
        construct_vap(vap_name, radio, 0, mode);
        memset(cmd, 0, sizeof(cmd));
        //sprintf(cmd, "kill `cat /var/run/wpa_supplicant_%s.pid`",vap_name);
        sprintf(cmd, "wpa_cli terminate");
        EXE_COMMAND(cmd);
        sleep(1);
        sprintf(daemon_pid, "wpa_supplicant_%s.pid",vap_name);
        while(max_sleep > 0)
        {
            //Check if the VAP has been destroy
            daemon_exist = authentication_daemon_exist(daemon_pid);
    
            //Already estory, break the cycle
            if(!daemon_exist)
            {
                printf("Destroy daemon Success!!!\n");
                break;
            }
            //Have not been destroy, wait for another 2 second
            else
            {
                sleep(2);
    	    }
            max_sleep = max_sleep - 2;
        }

    }

    return T_SUCCESS;
}


int radio_up_down(int radio, int up, int mode)
{
    int i;
    int vap_num;
    int vap_status;
 
    /*Get VAP num from wlv_rule_num*/
    vap_num = nvram_get_vap_num(radio); 
    printf("vap_num is %d\n",vap_num);

    if(up)
    {
        /*Up the VAP which is enable*/
        if(WLAN_MODE_AP == mode)
        {
            for(i = 0; i < vap_num; i++)
                {    
                    nvram_get_vap_status(radio, i, &vap_status);
                    if(VAP_UP == vap_status)
                    {
                        vap_up_down(radio, i, mode, VAP_UP);    
                    }
                }
        }
        else if(WLAN_MODE_STA == mode)
        {
            vap_up_down(radio, 0, mode, VAP_UP);    
        }
        //sleep(1);
    }
    else
    {
        /*Down all the VAPs, down main VAP at last*/
        if(WLAN_MODE_AP == mode)
        {
            for(i = vap_num - 1; i >= 0; i--)
                {    
                    vap_up_down(radio, i, mode, VAP_DOWN);    
                }
        }
        else if(WLAN_MODE_STA == mode)
        {
            vap_up_down(radio, 0, mode, VAP_DOWN);    
        }
        //sleep(1);
    }

    return T_SUCCESS;
}
#endif //End of    #if defined(GP_WP688)


int get_channel(char channel[], char str[], char pattern[])
{
	int lnth, lnthdif,i,j,k;
	lnth=length(pattern);
	lnthdif=length(str)-lnth+1;
	if (lnth>0 && lnthdif>0){
		i=0;
		while (i<lnthdif){
			j=i; k=0;
			while (k<lnth && str[j++]==pattern[k]) k++;
			if (k==lnth){
			    if (str[i+lnth+1] == ' '){
				channel[0] = str[i+lnth];
				channel[1] = '\0';
				return T_SUCCESS;
			    }else if(str[i+lnth+2] == ' '){
				channel[0] = str[i+lnth];
				channel[1] = str[i+lnth+1];
				channel[2] = '\0';
				return T_SUCCESS;
			    }else{
				channel[0] = str[i+lnth];
				channel[1] = str[i+lnth+1];
				channel[2] = str[i+lnth+2];
				channel[3] = '\0';
				return T_SUCCESS;
			   }
			}else{
			    i++;
			}
		}
	}
	return T_FAILURE;
}

int get_sta_bssid(char bssid[], char str[], char pattern[])
{
	int lnth, lnthdif,i,j,k;
        char tmpBssid[32];
	lnth=length(pattern);
	lnthdif=length(str)-lnth+1;
	if (lnth>0 && lnthdif>0){
		i=0;
		while (i<lnthdif){
			j=i; k=0;
                        //printf("str is %s\n", &str[j]);
                        //printf("pattern is %s\n", pattern);
			while (k<lnth && str[j++]==pattern[k]) k++;
			if (k==lnth){
			        //snprintf(tmpBssid, 18, "%s", &str[i+lnth]);
                                strcpy(tmpBssid, (str+i+lnth));
                                printf("tmpBssid is %s\n", tmpBssid);
			        delspace_bssid(tmpBssid, bssid);
                                //strcpy(bssid, &str[i+lnth]);
				return T_SUCCESS;
			}else{
			    i++;
			}
		}
	}
	return T_FAILURE;
}

int get_sta_ssid(char ssid[], char str[], char pattern[])
{
	int lnth, lnthdif,i,j,k;
        char tmpSsid[64];
	lnth=length(pattern);
	lnthdif=length(str)-lnth+1;
	if (lnth>0 && lnthdif>0){
		i=0;
		while (i<lnthdif){
			j=i; k=0;
                        //printf("str is %s\n", &str[j]);
                        //printf("pattern is %s\n", pattern);
			while (k<lnth && str[j++]==pattern[k]) k++;
			if (k==lnth){
			        //snprintf(tmpBssid, 18, "%s", &str[i+lnth]);
                                strcpy(tmpSsid, (str+i+lnth));
                                printf("tmpSsid is %s\n", tmpSsid);
			        delspace2(tmpSsid, ssid);
                                //strcpy(bssid, &str[i+lnth]);
				return T_SUCCESS;
			}else{
			    i++;
			}
		}
	}
	return T_FAILURE;
}

#if defined(GP_WP688)
/*Get the link status of one vap by reading ifconfig result*/
int get_vap_link_status(int *status, char str[], char pattern[])
{
	int lnth, lnthdif,i,j,k;
	lnth=length(pattern);
	lnthdif=length(str)-lnth+1;
	if (lnth>0 && lnthdif>0){
		i=0;
		while (i<lnthdif){
			j=i; k=0;
			while (k<lnth && str[j++]==pattern[k]) k++;
			if (k==lnth){
				*status = VAP_UP;
				printf("VAP Link UP\n");
				return T_SUCCESS;
			}else{
				i++;
			}
		}
	}
	//printf("ERROR: get link status error!\n");
	return T_FAILURE;
}

/*Set all the SSIDs off, except apclii0*/
int down_all_ssid(int radio)
{
    int ret;
    int i;
    char radio_name[VAP_NAME_LEN];
    //RalinkVAP_t vap_list[RADIOA_VAP_MAX_NUM];
    int vap_num;
    char cmd[128];
    /*Construct main VAP name*/
    construct_main_ap(radio_name, radio);
    /*Get VAP num from wlv_rule_num*/
    vap_num = atoi(nvram_safe_get("wlv_rule_num"));
    printf("vap_num is %d\n",vap_num);
    /*Down all the VAPs exclude main VAP*/
    for(i=1; i < vap_num; i++)
        {	
	    ret = vap_up_down(radio,  i, WLAN_MODE_AP, VAP_DOWN);	
	    if(T_FAILURE == ret)
	        {
		    printf("ERROR:up/dwon VAP failure\n");
		    return T_FAILURE;
		}
        }
#if 0
    /*Down all the VAPs in WDS mod*/
    for(i=0; i < WDS_NUM; i++)
        {
	    ret = vap_up_down(radio,  i, MODE_WDS, VAP_DOWN);	
            if(T_FAILURE == ret)
	        {
                    printf("ERROR:up/dwon WDS VAP failure\n");
                    return T_FAILURE;
                }
        }
#endif
    /*main VAP down*/
    sprintf(cmd, "ifconfig %s down", radio_name);
    system(cmd);
    /*Wait for a moment*/
    sleep(3);
    return T_SUCCESS;
}
#elif defined(GP_WP838)
#endif


