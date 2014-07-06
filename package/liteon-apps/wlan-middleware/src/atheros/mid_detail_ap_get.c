//Added by Mario Huang 2013-07-15
#include    <stdlib.h>
#include    <stdio.h>
#include	<string.h>
#include	<unistd.h> /*close*/

#include    <sys/ioctl.h>
#include    <arpa/inet.h>
#ifdef CONFIG_DEFAULTS_KERNEL_2_6_21
#include    <linux/types.h>
#include    <linux/socket.h>
#include    <linux/if.h>
#endif//End of #ifdef CONFIG_DEFAULTS_KERNEL_2_6_21
#include    <linux/wireless.h>
// Tommy, Add syslog, 2009/10/21 04:47
#include <syslog.h>
#include <assert.h>

//kernel
#include <linux/wireless.h>

#include "nvram.h"
#include "nvram_rule.h"

#include "mid_detail.h"
#include "mid_common.h"

/*Get the link status of one vap by reading ifconfig result*/
#if 0
static int parse_vap_link_status(int *status, char str[], char pattern[])
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
#endif

/*************************************/
/*>>>>>>>>>>general process>>>>>>>>>>*/
/*************************************/
int get_wifi_status(int radio, int *status)
{
    char cmd[128] = {0};
    char wifi_name[8] = {0};
    /*Diff Use same name in all the file*/
    FILE *fin;
    char str[256] = {0},c;

    MID_ASSERT((radio == RADIO_2G || radio == RADIO_5G),"No such radio");

    if(RADIO_2G == radio)
    {
        strcpy(wifi_name, "wifi0");
    }
    else if(RADIO_5G == radio) 
    {
        strcpy(wifi_name, "wifi1");
    }    
    /*Set it as down first, modify it later if it is up*/
    *status = RADIO_DOWN;
    sprintf(cmd, "ifconfig | grep -rn %s > /tmp/wifi_link_status",wifi_name);
    EXE_COMMAND(cmd);
    fin = fopen("/tmp/wifi_link_status","r");
            
    /*If there is wifi0/wifi1 available in the file, return UP,or return Down*/
    while ((c=fgetc(fin)) != EOF){
        ungetc(c,fin);        
        readline(str,fin);
        if (find_pattern_from_str(str, wifi_name) == T_SUCCESS)
            {
                *status = RADIO_UP;
                break;
            }
    }

    fclose(fin);
    EXE_COMMAND("rm -f /tmp/wifi_link_status");    

    return T_SUCCESS;
}


/*
get vap status
1: up
0:down
*/
int get_vap_status(int radio, int vapid, int mode, int *vap_status)
{
    int ret;
    char cmd[128] = {0};
    char vap_name[8] = {0};
    /*Diff Use same name in all the file*/
    FILE *fin;
    char str[256] = {0},c;
    /*SAME construct vap name first*/
    ret = construct_vap(vap_name, radio, vapid, mode);
    if(T_FAILURE == ret)
        {
            printf("ERROR:Construct VAP Failure!\n");
            return T_FAILURE;
        }
    /*Set it as down first, get_vap_link_status wil modify it if it is up*/
    *vap_status = VAP_DOWN;
    sprintf(cmd, "ifconfig | grep -rn %s > /tmp/vap_link_status",vap_name);
    EXE_COMMAND(cmd);
    fin = fopen("/tmp/vap_link_status","r");
            
    /*If vap_name is available in the file, return UP,or return Down*/
    while ((c=fgetc(fin)) != EOF){
        ungetc(c,fin);        
        readline(str,fin);
        if (find_pattern_from_str(str, vap_name) == T_SUCCESS)
            {
                *vap_status = VAP_UP;
                break;
            }
    }

    fclose(fin);
    EXE_COMMAND("rm -f /tmp/vap_link_status");    

    return T_SUCCESS;
}

/*
Get Radio status
1: Up
0:Down
*/
int get_radio_status(int radio, int *radio_status)
{
    int ret;
    int wlan_mode;
    int wifi_status;
    int vap_status;
    int vap_num = 0;
    int i;
    //If wifi0 or wifi1 is down, the radio is down
    get_wifi_status(radio, &wifi_status);   
    if(RADIO_DOWN == wifi_status)
    {
        *radio_status = RADIO_DOWN;
        return T_SUCCESS;
    }
    //Get wlan_mode from nvram first
    nvram_get_wlan_mode(radio, &wlan_mode);
    if(WLAN_MODE_AP == wlan_mode)
    {
        //Get VAP num of the Radio
        vap_num = nvram_get_vap_num(radio);
        //search for all the VAPs, radio is DOWN while all the VAPs are down, or radio is up.
        for(i=0; i < vap_num; i++)
        {
            ret = get_vap_status(radio, i, wlan_mode, &vap_status);
            if(T_FAILURE == ret)
                {
                    printf("ERROR:Get VAP Staus Failure!\n");
                    return T_FAILURE;
                }    
    
            if(VAP_UP == vap_status)
                {
                    *radio_status = RADIO_UP;
                    return T_SUCCESS;
                }
        }
           
        *radio_status = RADIO_DOWN;
    }
    else if(WLAN_MODE_STA == wlan_mode)
    {
        ret = get_vap_status(radio, 0, wlan_mode, &vap_status);
        if(T_FAILURE == ret)
            {
                printf("ERROR:Get VAP Staus Failure!\n");
                return T_FAILURE;
            }    

        if(VAP_UP == vap_status)
            {
                *radio_status = RADIO_UP;
                return T_SUCCESS;
            }
        else
            {
                *radio_status = RADIO_DOWN;
            }
    }

    return T_SUCCESS;
}

/*Get wlan rate*/
int get_wlan_rate(int radio,int *rate)
{
    char str[256] = {0},c;
    char rate_s[8] = {0};
    char vap_name[8];
    int wlan_mode;
    int ret;
    FILE *fin = NULL;
    char cmd[256];

    //Set rate as 0 first
    *rate = 0;
 
    nvram_get_wlan_mode(radio, &wlan_mode);
    ret = construct_vap(vap_name, radio, 0, wlan_mode);
	if (T_FAILURE == ret) {
        printf("ERROR:Get VAP Name Failure!\n");
        return T_FAILURE;
    }

    sprintf(cmd, "iwconfig %s | grep Rate | awk '{print substr($2,6,3)}' > /tmp/current_wlan_rate", vap_name);
    EXE_COMMAND(cmd);
    fin = fopen("/tmp/current_wlan_rate","r");
            
    /*Parse rate from /tmp/current_wlan_rate file*/
    while ((c=fgetc(fin)) != EOF){
        ungetc(c,fin);        
        readline(str,fin);
        strcpy_delspace(str, rate_s);
        *rate = atoi(rate_s);
    }

    fclose(fin);
    EXE_COMMAND("rm -f /tmp/current_wlan_rate");    

   
    return T_SUCCESS;
}    
    
/*get the channel current in use*/
int get_current_channel(int radio, Channel_t *channel_get)
{
    char str[256] = {0},c;
    char channel_s[8] = {0};
    char frequency_s[8] = {0};
    char vap_name[8] = {0};
    int wlan_mode = 0;
    int ret;
    FILE *fin;
    char cmd[256];
    float tmp_frequency = 0;

    nvram_get_wlan_mode(radio, &wlan_mode);
    ret = construct_vap(vap_name, radio, 0, wlan_mode);
	if (T_FAILURE == ret) {
        printf("ERROR:Get VAP Name Failure!\n");
        return T_FAILURE;
    }

    //The command I wanna is iwlist ath0 channel | grep Current  | awk '{gsub(/)/,"");print}' | awk '{print substr($5,0)}'
    sprintf(cmd, "iwlist %s channel | grep Current  | awk '{gsub(/)/,\"\");print}' | awk '{print substr($5,0)}' > /tmp/current_channel", vap_name);
    EXE_COMMAND(cmd);
    fin = fopen("/tmp/current_channel","r");
            
    while ((c=fgetc(fin)) != EOF){
        ungetc(c,fin);        
        readline(str,fin);
        strcpy_delspace(str, channel_s);
        channel_get->chan_number = atoi(channel_s);
    }

    fclose(fin);
    EXE_COMMAND("rm -f /tmp/current_channel");    

    memset(cmd, 0, sizeof(cmd));
    //The command I wanna is iwlist ath0 channel | grep Current  | awk '{gsub(/\./,"");print}' | awk '{print substr($2,11)}'
    //sprintf(cmd, "iwlist %s channel | grep Current  | awk '{gsub(/\\./,\"\");print}' | awk '{print substr($2,11)}' > /tmp/current_frequency", vap_name);
    sprintf(cmd, "iwlist %s channel | grep Current | awk '{print substr($2,11)}' > /tmp/current_frequency", vap_name);
    EXE_COMMAND(cmd);
    fin = fopen("/tmp/current_frequency","r");
            
    while ((c=fgetc(fin)) != EOF){
        ungetc(c,fin);        
        readline(str,fin);
        strcpy_delspace(str, frequency_s);
        //channel_get->frequency = atoi(frequency_s);
        tmp_frequency = atof(frequency_s);
        tmp_frequency = tmp_frequency * 1000; 
        channel_get->frequency = (int)tmp_frequency;
    }

    fclose(fin);
    EXE_COMMAND("rm -f /tmp/current_frequency");    

    return T_SUCCESS;
}

/*get the channel current in use*/
int get_extchannel(int radio, Channel_t *extchannel_get)
{
	int ret = 0;
	int bandWidth = 0;
	int wlan_mode = 0;
	FILE *fin;
	char c;
    char str[256] = {0};
	char cmd[256] = {0};
	char exchannel[8] = {0};
	char vap_name[8] = {0};
	Channel_t channel_get;
	memset(&channel_get, 0, sizeof(Channel_t));

	ret = get_bandwidth(radio, &bandWidth);

	if (ret == T_FAILURE) {
		printf("Error: Get BandWidth Failure\n");
		return T_FAILURE;
	}

	if (BANDWIDTH_40 == bandWidth) {
		
		ret = get_current_channel(radio, &channel_get);
		
		if (T_FAILURE == ret) {
			
			printf("Error:Get Operation Channel Failure\n");
			return T_FAILURE;
			
		} else {

			nvram_get_wlan_mode(radio, &wlan_mode);
    		ret = construct_vap(vap_name, radio, 0, wlan_mode);

			if (T_FAILURE == ret) {
				
				printf("Error:Get Vap Name Failure\n");
				return T_FAILURE;
			}

			//iwpriv athN get_chextoffset | awk '{gsub(/get_chextoffset:/,"");print}' | awk '{print $2}' > /tmp/extchannel
			sprintf(cmd, "iwpriv %s get_chextoffset | awk '{gsub(/get_chextoffset:/,\"\");print}' | awk '{print $2}' > /tmp/extchannel", vap_name);
    		EXE_COMMAND(cmd);
    		fin = fopen("/tmp/extchannel","r");
            
    		while ((c=fgetc(fin)) != EOF){
        		ungetc(c,fin);        
        		readline(str,fin);
        		strcpy_delspace(str, exchannel);
   			}
			
			fclose(fin);
			EXE_COMMAND("rm -fr /tmp/extchannel");

			if (!strcmp(exchannel, "1")) {
				extchannel_get->chan_number = channel_get.chan_number + 4;
				extchannel_get->frequency = channel_get.frequency + 20;
			} else if (!strcmp(exchannel, "-1")) {
				extchannel_get->chan_number = channel_get.chan_number - 4;
				extchannel_get->frequency = channel_get.frequency - 20;
			} else {
				printf("Error: CMD get_chextoffset Failure\n");
				return T_FAILURE;
			}
		}
		
	} else {
	
		printf("Get BandWidth Not 40MHz\n");
		return T_FAILURE;
	}

	return T_SUCCESS;
}

/*get the bandwidth current in use*/
int get_bandwidth(int radio, int *htbw)
{
    char str[256] ={0},c;
    //int bandwidth;
    int wlan_mode;
    char wireless_mode[32] = {0};
    char vap_name[8];
    int ret;
    FILE *fin;
    char cmd[256];

    nvram_get_wlan_mode(radio, &wlan_mode);
    ret = construct_vap(vap_name, radio, 0, wlan_mode);
	if (T_FAILURE == ret) {
        printf("ERROR:Get VAP Name Failure!\n");
        return T_FAILURE;
    }

	if(WLAN_MODE_STA == wlan_mode){
        sprintf(cmd, "iwpriv %s get_chwidth | awk -F\: '{print $2}' >> /tmp/current_chwidth", vap_name);
        EXE_COMMAND(cmd);
        fin = fopen("/tmp/current_chwidth","r");
                
        /*Parse rate from /tmp/current_chwidth file*/
        while ((c=fgetc(fin)) != EOF){
            ungetc(c,fin);        
            readline(str,fin);
            strcpy_delspace(str, wireless_mode);
        }

    	fclose(fin);
        EXE_COMMAND("rm -f /tmp/current_chwidth");

    	if (strstr(wireless_mode, "0") != NULL) {
    		*htbw = BANDWIDTH_20;
    	} else if (strstr(wireless_mode, "1") != NULL) {
    		*htbw = BANDWIDTH_40;
    	} else if (strstr(wireless_mode, "2") != NULL) {
    		*htbw = BANDWIDTH_80;
    	} else {
    		MID_ERROR("Can not get wireless mode or the wireless mode is not supported by now!");
    		return T_FAILURE;
    	}
	}else{
        sprintf(cmd, "iwpriv %s get_mode | awk '{print substr($2,10,20)}' >> /tmp/current_wireless_mode", vap_name);
        EXE_COMMAND(cmd);
        fin = fopen("/tmp/current_wireless_mode","r");
                
        /*Parse rate from /tmp/current_wlan_rate file*/
        while ((c=fgetc(fin)) != EOF){
            ungetc(c,fin);        
            readline(str,fin);
            strcpy_delspace(str, wireless_mode);
        }

    	fclose(fin);
        EXE_COMMAND("rm -f /tmp/current_wireless_mode");

    	if (strstr(wireless_mode, "20") != NULL) {
    		*htbw = BANDWIDTH_20;
    	} else if (strstr(wireless_mode, "40") != NULL) {
    		*htbw = BANDWIDTH_40;
    	} else if (strstr(wireless_mode, "80") != NULL) {
    		*htbw = BANDWIDTH_80;
    	} else {
    		MID_ERROR("Can not get wireless mode or the wireless mode is not supported by now!");
    		return T_FAILURE;
    	}
    }
	
    return T_SUCCESS;
}

/*get the wireless mode current in use*/
int get_wirelessmode(int radio, int *wmode)
{
    char str[256] = {0},c;
    char wireless_mode[32] = {0};
    char vap_name[8];
    int wlan_mode;
    int ret;
    FILE *fin;
    char cmd[256];

    nvram_get_wlan_mode(radio, &wlan_mode);
    ret = construct_vap(vap_name, radio, 0, wlan_mode);
	if (T_FAILURE == ret) {
        printf("ERROR:Get VAP Name Failure!\n");
        return T_FAILURE;
    }

    sprintf(cmd, "iwpriv %s get_mode | awk '{print substr($2,10,20)}' >> /tmp/current_wireless_mode", vap_name);
    EXE_COMMAND(cmd);
    fin = fopen("/tmp/current_wireless_mode","r");
            
    /*Parse rate from /tmp/current_wlan_rate file*/
    while ((c=fgetc(fin)) != EOF){
        ungetc(c,fin);        
        readline(str,fin);
        strcpy_delspace(str, wireless_mode);
    }

	fclose(fin);
    EXE_COMMAND("rm -f /tmp/current_wireless_mode");

	if (strstr(wireless_mode, "11NG") != NULL) {
		*wmode = WMODE_11BGN;
	} else if (strstr(wireless_mode, "11NA") != NULL) {
		*wmode = WMODE_11AN;
	} else if (strstr(wireless_mode, "11ACV") != NULL) {
		*wmode = WMODE_11AC_MIXED;
	} else {
        MID_ERROR("Can not get wireless mode or the wireless mode is not supported by now!");
        return T_FAILURE;
    }

    return T_SUCCESS;
}


/*Get SSID*/
int get_ssid(int radio, int mode, int vap_id, char *ssid)
{
    char c;
    char ssid_got[33] = {0};
    char vap_name[8] = {0};
    int wlan_mode;
    int ret;
    FILE *fin;
    char cmd[256] = {0} ;

    nvram_get_wlan_mode(radio, &wlan_mode);
    ret = construct_vap(vap_name, radio, 0, wlan_mode);

	sprintf(cmd, "iwconfig %s | awk '/ESSID/{print}' | sed 's/[^\"]*\"\\(.*\\)\"[^\"]*/\\1/g' > /tmp/current_ssid", vap_name);
    EXE_COMMAND(cmd);
    fin = fopen("/tmp/current_ssid","r");
            
    while ((c=fgetc(fin)) != EOF){
        ungetc(c,fin);        
        readline(ssid_got,fin);
    }

    if(!strcmp(ssid_got,""))
    {
        MID_ERROR("Can not get SSID or SSID is empty");
        fclose(fin);
        EXE_COMMAND("rm -f /tmp/current_ssid");  
        return T_FAILURE;
    }   

    strcpy(ssid, ssid_got); 

    fclose(fin);
    EXE_COMMAND("rm -f /tmp/current_ssid");    

    return T_SUCCESS;
}

int get_channel_select_mode(int radio, int *mode)
{
    return nvram_get_ap_channel_select_mode(radio, mode);
}



/**
 * Convert MHz frequency to IEEE channel number. 
 * copied from atheros wifi by frank, invoked locally.
 */
u_int ieee80211_mhz2ieee(u_int16_t freq)
{
#define IS_CHAN_IN_PUBLIC_SAFETY_BAND(_c) ((_c) > 4940 && (_c) < 4990)

	if (freq == 2484)
		return 14;
	if (freq < 2484)
		return (freq - 2407) / 5;
	if (freq < 5000) {
		if (IS_CHAN_IN_PUBLIC_SAFETY_BAND(freq)) {
			return ((freq * 10) +
					(((freq % 5) == 2) ? 5 : 0) - 49400)/5;
		} else if (freq > 4900) {
			return (freq - 4000) / 5;
		} else {
			return 15 + ((freq - 2512) / 20);
		}
	}
	return (freq - 5000) / 5;
#undef IS_CHAN_IN_PUBLIC_SAFETY_BAND
}

/**
 * @brief get the Channel list depending on radio 
 * @param[in] radio			[0:2.4g|1:5g]
 * @param[out] channel_list
 * @param[in] isHT80		[1:HT80|0:HT20/40]
 * @return	T_SUCCESS / T_FAILURE	
 *
 * @date 2014-03-12
 * @author frank
 */
int _get_channel_list(int radio, ChannelListInfo_t *channel_list, int isHT80)
{
	assert(NULL != channel_list);
	assert( RADIO_2G == radio || RADIO_5G == radio );

/*+++struct from wifi driver+++*/
struct ieee80211_channel {
	u_int16_t       ic_freq;        /* setting in Mhz */
	u_int32_t       ic_flags;       /* see below */
	u_int8_t        ic_flagext;     /* see below */
	u_int8_t        ic_ieee;        /* IEEE channel number */
	int8_t          ic_maxregpower; /* maximum regulatory tx power in dBm */
	int8_t          ic_maxpower;    /* maximum tx power in dBm */
	int8_t          ic_minpower;    /* minimum tx power in dBm */
	u_int8_t        ic_regClassId;  /* regClassId of this channel */
	u_int8_t        ic_antennamax;  /* antenna gain max from regulatory */
	u_int8_t        ic_vhtop_ch_freq_seg1;         /* Channel Center frequency */
	u_int8_t        ic_vhtop_ch_freq_seg2;         /* Channel Center frequency applicable
													* for 80+80MHz mode of operation */
}; 

#define IEEE80211_CHAN_MAX      255
struct ieee80211req_chaninfo {
	u_int   ic_nchans;
	struct ieee80211_channel ic_chans[IEEE80211_CHAN_MAX];
};

/*SIOCIWFIRSTPRIV is in wireless.h*/
/*chan or freq*/
#define IEEE80211_IOCTL_GETCHANINFO (SIOCIWFIRSTPRIV+7) 
/*active*/
#define IEEE80211_IOCTL_GETCHANLIST (SIOCIWFIRSTPRIV+13)
#define IEEE80211_CHAN_5GHZ             0x00000100 /* 5 GHz spectrum channel */
#define IEEE80211_CHAN_VHT80            0x00800000 /* VHT 80 channel */
#define IEEE80211_CHAN_HT40PLUS         0x00020000 /* HT 40 with extension channel above */
#define IEEE80211_CHAN_HT40MINUS        0x00040000 /* HT 40 with extension channel below */
#define IEEE80211_CHAN_VHT40PLUS        0x00200000 /* VHT 40 with extension channel above */
#define IEEE80211_CHAN_VHT40MINUS       0x00400000 /* VHT 40 with extension channel below */
#define IEEE80211_CHAN_DFS              0x0002  /* DFS required on channel */

#define IEEE80211_CHAN_11AC_VHT80 \
    (IEEE80211_CHAN_5GHZ | IEEE80211_CHAN_VHT80)
#define IEEE80211_CHAN_11AC_VHT40PLUS \
	    (IEEE80211_CHAN_5GHZ | IEEE80211_CHAN_VHT40PLUS)
#define IEEE80211_CHAN_11AC_VHT40MINUS \
	    (IEEE80211_CHAN_5GHZ | IEEE80211_CHAN_VHT40MINUS) 

#define IEEE80211_IS_CHAN_11AC_VHT40PLUS(_c) \
	    (((_c)->ic_flags & IEEE80211_CHAN_11AC_VHT40PLUS) == IEEE80211_CHAN_11AC_VHT40PLUS)
#define IEEE80211_IS_CHAN_11AC_VHT40MINUS(_c) \
	    (((_c)->ic_flags & IEEE80211_CHAN_11AC_VHT40MINUS) == IEEE80211_CHAN_11AC_VHT40MINUS)   

#define IEEE80211_IS_CHAN_11N_CTL_U_CAPABLE(_c) \
	    (((_c)->ic_flags & IEEE80211_CHAN_HT40PLUS) == IEEE80211_CHAN_HT40PLUS)
#define IEEE80211_IS_CHAN_11N_CTL_L_CAPABLE(_c) \
	    (((_c)->ic_flags & IEEE80211_CHAN_HT40MINUS) == IEEE80211_CHAN_HT40MINUS)
#define IEEE80211_IS_CHAN_DFSFLAG(_c) \
	    (((_c)->ic_flagext & IEEE80211_CHAN_DFS) == IEEE80211_CHAN_DFS)

#define IEEE80211_IS_CHAN_11AC_VHT80(_c) \
	    (((_c)->ic_flags & IEEE80211_CHAN_11AC_VHT80) == IEEE80211_CHAN_11AC_VHT80)

#ifndef     IFNAMSIZ
#define     IFNAMSIZ    16
#endif
/*---struct from wifi driver---*/

    char vap_name[8] = {0};
	struct iwreq iwr;
	struct ieee80211req_chaninfo achans;

	//construct vap_name
    int ret = construct_vap(vap_name, radio, 0, WLAN_MODE_AP);
	if(T_FAILURE == ret)
	{
		fprintf(stderr, "%s:Construct VAP failure!\n", __FUNCTION__);
		return T_FAILURE;
	}

	//set iwr
	memset(&iwr, 0, sizeof(iwr));
	int len = sizeof(achans);
	strncpy(iwr.ifr_name, vap_name, IFNAMSIZ);
	if (len < IFNAMSIZ) {
		memcpy(iwr.u.name, &achans, len);
	}
	else {
		iwr.u.data.pointer = &achans;
		iwr.u.data.length = len;
	}

	//open socket
	int s = -1;
	if (s < 0) {
		s = socket(AF_INET, SOCK_DGRAM, 0);
		if (s < 0){
			perror("socket(SOCK_DRAGM)");
			return T_FAILURE;
		}
	}

	//ioctl
	if (ioctl(s, IEEE80211_IOCTL_GETCHANINFO, &iwr) < 0) {
		perror("ioctl");
		return T_FAILURE;
	}

    if (len < IFNAMSIZ){
		memcpy(&achans, iwr.u.name, len);
    }

	//close socket
	close(s);


	int i = 0;
	int chn_count = 0;
	for(i = 0; i < achans.ic_nchans; i++) {
		const struct ieee80211_channel *c = &achans.ic_chans[i];

		//HT80
		if(isHT80 && !IEEE80211_IS_CHAN_11AC_VHT80(c)) {
			continue;
		}
		else {
			channel_list->channel_list[chn_count].centerHT80 
				= c->ic_vhtop_ch_freq_seg1;
		}
		//DFS
		channel_list->channel_list[chn_count].isDFS 
			= IEEE80211_IS_CHAN_DFSFLAG(c);
		//chan
		channel_list->channel_list[chn_count].chan_number 
			= ieee80211_mhz2ieee(c->ic_freq);
		//freq
		channel_list->channel_list[chn_count].frequency 
			= c->ic_freq;

		if(RADIO_2G == radio) {
			channel_list->channel_list[chn_count].hasUpper 
				= IEEE80211_IS_CHAN_11N_CTL_U_CAPABLE(c);
			channel_list->channel_list[chn_count].hasLower 
				= IEEE80211_IS_CHAN_11N_CTL_L_CAPABLE(c);
		}
		else if(RADIO_5G == radio) {
			channel_list->channel_list[chn_count].hasUpper 
				= IEEE80211_IS_CHAN_11AC_VHT40PLUS(c);
			channel_list->channel_list[chn_count].hasLower 
				= IEEE80211_IS_CHAN_11AC_VHT40MINUS(c);
		}

		chn_count++;
	}
	channel_list->num = chn_count;

	/* use original method to get current channel*/
    char str[255],c;
	char cmd[64] = {0};
	sprintf(cmd, "iwlist %s channel | awk -f /etc/wl/chn.awk > /tmp/atheros_chn", vap_name);
	EXE_COMMAND(cmd);

    FILE *fin = fopen("/tmp/atheros_chn","r");
    if(NULL == fin) {
		return T_FAILURE;
    }
    while ((c=fgetc(fin)) != EOF) {
        ungetc(c,fin);        
        readline(str,fin);
		sscanf(str, "%d %d", &chn_count, &channel_list->current_chan);
		break;
    }
    fclose(fin);
    EXE_COMMAND("rm -f /tmp/atheros_chn");    

    return T_SUCCESS;
}

/**
 * @brief get the Channel list depending on radio , FOR HT80
 * @param[in] radio			[0:2.4g|1:5g]
 * @param[out] channel_list
 * @return	T_SUCCESS / T_FAILURE	
 *
 * @date 2014-03-13
 * @author frank
 */
int get_channel_listHT80(int radio, ChannelListInfo_t *channel_list)
{
	return _get_channel_list(radio, channel_list, 1);
}

/**
 * @brief get the Channel list depending on radio , FOR HT20 HT40
 * @param[in] radio			[0:2.4g|1:5g]
 * @param[out] channel_list
 * @return	T_SUCCESS / T_FAILURE	
 *
 * @date 2014-03-13
 * @author frank
 */
int get_channel_list(int radio, ChannelListInfo_t *channel_list)
{
	return _get_channel_list(radio, channel_list, 0);
}


int get_max_txpower(int radio, int *max_txpower)
{
    char str[256] = {0},c;
    char txpower_s[8] = {0};
    char wifi_name[8] = {0};
    int wlan_mode = 0;
	int ret = 0;
    FILE *fin;
    char cmd[256] = {0};
	char TempBuf_opmode[32] = {0};

    if(RADIO_2G == radio)
    {
        //strcpy(wifi_name, "wifi0");
		//iwpriv wifi0 getTxMaxPower2G | awk '{gsub(/getTxMaxPower2G:/,"");print $2}'
		//sprintf(cmd, "iwpriv %s getTxMaxPower2G | awk '{gsub(/getTxMaxPower2G:/,\"\");print $2}' > /tmp/max_txpower", wifi_name);
		ezplib_get_attr_val("wl_mode_rule", 0, "mode", TempBuf_opmode, 32, EZPLIB_USE_CLI);
    }
    else if(RADIO_5G == radio)
    {
        //strcpy(wifi_name, "wifi1");
		//iwpriv wifi1 getTxMaxPower5G | awk '{gsub(/getTxMaxPower5G:/,"");print $2}'
		//sprintf(cmd, "iwpriv %s getTxMaxPower5G | awk '{gsub(/getTxMaxPower5G:/,\"\");print $2}' > /tmp/max_txpower", wifi_name);
		ezplib_get_attr_val("wl1_mode_rule", 0, "mode", TempBuf_opmode, 32, EZPLIB_USE_CLI);
    }
	else
	{
		printf("ERROR: Radio is Not 2.4G or 5G!\n");
		return T_FAILURE;
	}
	
    if(!strcmp(TempBuf_opmode,"ap"))
	{
		wlan_mode = WLAN_MODE_AP;          
	}
	else if(!strcmp(TempBuf_opmode,"client"))
	{
		wlan_mode = WLAN_MODE_STA;
	}
	else
	{
		printf("ERROR: WLAN OP MODE is Not AP or Client!\n");
		return T_FAILURE;
	}
	
	ret = construct_vap(wifi_name, radio, 0, wlan_mode);
	if(T_FAILURE == ret)
	{
		printf("ERROR:Construct VAP failure!\n");
		return T_FAILURE;
	}

	if((WLAN_MODE_STA == wlan_mode) && (RADIO_5G == radio))
	{
		//sprintf(cmd, "iwconfig %s txpower %d;sleep 5;iwconfig %s | awk '/Tx-Power/{print substr($4,10);}' > /tmp/max_txpower", wifi_name, 100, wifi_name);
		printf("Wireless Client Mode: Txpower Shell\n");
		return T_SUCCESS;
	}
	else
	{
		if (RADIO_5G == radio) 
		{
			sprintf(cmd, "iwconfig %s txpower %d;sleep 2;iwconfig %s | awk '/Tx-Power/{print substr($4,10);}' > /tmp/max_txpower", 
			wifi_name, 100, wifi_name);
		}
		else
		{
			//iwpriv wifi0 getTxMaxPower2G | awk '{gsub(/getTxMaxPower2G:/,"");print $2}'
			sprintf(cmd, "iwpriv wifi0 getTxMaxPower2G | awk '{gsub(/getTxMaxPower2G:/,\"\");print $2}' > /tmp/max_txpower");
		}
	}
	
    EXE_COMMAND(cmd);
    fin = fopen("/tmp/max_txpower","r");
            
    //while ((c=fgetc(fin)) != EOF){
    //    ungetc(c,fin);        
    //    readline(str,fin);
    //    strcpy_delspace(str, txpower_s);
    //    *max_txpower = atoi(txpower_s);
    //}
    memset(cmd, 0, sizeof(cmd));
    if (!fgets(cmd, 16, fin)) {
		fclose(fin);
		printf("ERROR: get max power fail\n");
		return T_FAILURE;
	}
	
	*max_txpower = atoi(cmd);

    fclose(fin);
    EXE_COMMAND("rm -f /tmp/max_txpower");    

    return T_SUCCESS;
}

/************Get Txchainmask Num**********
**1,2,4:	Signal Chain
**3,5:	Double Chain
**7:	Three Chain
**************************************/
int get_chainmask_num(int radio, int *chain_num)
{
	FILE *fin;
	int chain_tmp = 0;
	int chain_count = 0;
	char c;
    char str[256] = {0};
    char chain_str[8] = {0};
    char wifi_name[8] = {0};
    char cmd[256] = {0};
	char buf[16] = {0};

    if(RADIO_2G == radio)
    {
        strcpy(wifi_name, "wifi0");
    }
    else if(RADIO_5G == radio)
    {
        strcpy(wifi_name, "wifi1");
    }
    //iwpriv wifiN get_txchainmask | awk '{gsub(/get_txchainmask:/,"");print}' | awk '{print $2}'
    sprintf(cmd, "iwpriv %s get_txchainmask | awk '{gsub(/get_txchainmask:/,\"\");print}' | awk '{print $2}' > /tmp/chainmask_num", wifi_name);
    EXE_COMMAND(cmd);
    fin = fopen("/tmp/chainmask_num","r");
            
    //while ((c=fgetc(fin)) != EOF){
    //    ungetc(c,fin);        
    //    readline(str,fin);
    //    strcpy_delspace(str, chain_str);
    //    chain_tmp = atoi(chain_str);
	//	printf("-----------------\n");
    //}
    if (!fgets(buf, 8, fin)) {
		fclose(fin);
		return T_FAILURE;
	}

    fclose(fin);
    EXE_COMMAND("rm -f /tmp/chainmask_num");    

	//while (chain_tmp){
	//	chain_count++;
	//	chain_tmp &= (chain_tmp - 1);
	//}

	if ((atoi(buf) < 1) || (atoi(buf) > 7)){
		printf("TxChainMask illegal value\n");
		return T_FAILURE;
	}
	*chain_num = chain_count;
    return T_SUCCESS;
}

/**
 * @brief Get station list
 * @param[in] 	radio			[0:2.4g|1:5g]
 * @param[out] 	sta_list_info
 * @return	T_SUCCESS / T_FAILURE	
 *
 * @author frank_zhou@lccbj.com
 */
int get_sta_list(int radio, StationListInfo_t *sta_list_info)
{
	assert(NULL != sta_list_info);
	FILE *fin = NULL;
	FILE *fin2 = NULL;
	int sta_count = 0;
    char str[255],c,c2;
	char cmd[64] = {0};
	char cmd2[64] = {0};
	unsigned int radio_count = 0;
	while (radio_count < 8) {
		sprintf(cmd, "wlanconfig ath%d list sta | awk '{if(NR!=1){print %d, $1, $6}}' >> /tmp/atheros_sta_list", 
				radio + 2 * radio_count, radio + 2 * radio_count);
		radio_count++;
		EXE_COMMAND(cmd);
	}

    fin = fopen("/tmp/atheros_sta_list","r");
    if(NULL == fin) {
		return T_FAILURE;
    }

    while ((c=fgetc(fin)) != EOF) {
        ungetc(c,fin);        
        readline(str,fin);
        unsigned int vap_index,mac0,mac1,mac2,mac3,mac4,mac5;
        int rssi;
		sscanf(str, "%d %02X:%02X:%02X:%02X:%02X:%02X %d",
			&vap_index, &mac0, &mac1, &mac2, &mac3, &mac4, &mac5, &rssi);
		sta_list_info->sta_list[sta_count].Addr[0] = mac0;
		sta_list_info->sta_list[sta_count].Addr[1] = mac1;
		sta_list_info->sta_list[sta_count].Addr[2] = mac2;
		sta_list_info->sta_list[sta_count].Addr[3] = mac3;
		sta_list_info->sta_list[sta_count].Addr[4] = mac4;
		sta_list_info->sta_list[sta_count].Addr[5] = mac5;
		sta_list_info->sta_list[sta_count].rssi = rssi;
		
		//get ssid
		//sprintf(cmd2, "iwconfig ath%d | awk '{if(NR==1){print $4}}'|cut -b8- > /tmp/atheros_vapid_2_ssid", vap_index);
		sprintf(cmd2, "iwconfig ath%d | awk '/ESSID/{print substr($0, 33, length($0)-35);}' > /tmp/atheros_vapid_2_ssid", vap_index);
		EXE_COMMAND(cmd2);
		fin2 = fopen("/tmp/atheros_vapid_2_ssid","r");
		if(NULL == fin2) {
			return T_FAILURE;
		}
		int i = 0;
		while ((c2=fgetc(fin2)) != EOF){
			if( i > 31 || '\n' == c2 ) break;	
			sta_list_info->sta_list[sta_count].ssid[i] = c2;
			i++;	
		}
		sta_list_info->sta_list[sta_count].ssid[i] = '\0';
		fclose(fin2);
		EXE_COMMAND("rm -f /tmp/atheros_vapid_2_ssid");    

		//  & < > in ssid need to be transfer to &#38; &#60; &62;
		char tmpstr[33];
		strcpy(tmpstr,sta_list_info->sta_list[sta_count].ssid);
		int j;
		for ( j=0,i=0;i<strlen(tmpstr);i++) {
			switch(tmpstr[i]) {
				case '&':
					strncpy(sta_list_info->sta_list[sta_count].ssid + j, "&#38;", 5);
					j+=5;
					break;
				case '<':
					strncpy(sta_list_info->sta_list[sta_count].ssid + j, "&#60;", 5);
					j+=5;
					break;
				case '>':
					strncpy(sta_list_info->sta_list[sta_count].ssid + j, "&#62;", 5);
					j+=5;
					break;
				default:
					sta_list_info->sta_list[sta_count].ssid[j] = tmpstr[i];
					j++;
					break;
			}
		}
		sta_list_info->sta_list[sta_count].ssid[j+1] = '\0';

		//iterator
		sta_count++;
    }
    
	sta_list_info->sta_num=sta_count;
    fclose(fin);
    EXE_COMMAND("rm -f /tmp/atheros_sta_list");    
    return T_SUCCESS;
}

/*************************************************************/
/*<<<<<<<<<<Get ACL Sync Info: Association List and ACL list>>>>>>>>>>>>>*/
/************************************************************/
int get_acl_sync_list(int radio, int vap_id, ACL_LIST *acl_mac_list, ACL_LIST *asso_mac_list)
{
	int ret = 0;
	int acl_num = 0;
	int asso_num = 0;
	int wlan_mode = 0;
	FILE *fin;
	char asso_c, acl_c;
    char asso_str[256] = {0};
	char acl_str[256] = {0};
    char vap_name[8] = {0};
    char cmd[256] = {0};
	
	nvram_get_wlan_mode(radio, &wlan_mode);
    ret = construct_vap(vap_name, radio, vap_id, wlan_mode);
	if (T_FAILURE == ret) {
        printf("ERROR:Get VAP Name Failure!\n");
        return T_FAILURE;
    }

	//wlanconfig athN list sta | awk '{if(NR!=1){print  $1}}' >> /tmp/asso_sta_list
	sprintf(cmd, "wlanconfig %s list sta | awk '{if(NR!=1){print $1}}' >> /tmp/asso_sta_list", vap_name);
	EXE_COMMAND(cmd);

	fin = fopen("/tmp/asso_sta_list","r");
	while ((asso_c=fgetc(fin)) != EOF){
        ungetc(asso_c,fin);        
        readline(asso_str,fin);
        strcpy_delspace(asso_str, asso_mac_list->mac[asso_num++].addr);
    }
	asso_mac_list->acl_num = asso_num;
	
	fclose(fin);
    EXE_COMMAND("rm -f /tmp/asso_sta_list"); 

	if (0 == asso_num) {
		printf("No Station Association\n");
		return T_SUCCESS;
	}
	
	//iwpriv athN getmac | sed -e 's/athN//g;s/getmac://g;y/ABCDEF/abcdef/' | awk '{print $1}' | sed '/^$/d'>> /tmp/acl_mac_list
	sprintf(cmd, "iwpriv %s getmac | sed -e 's/%s//g;s/getmac://g;y/ABCDEF/abcdef/' | awk '{print $1}' | sed '/^$/d' >> /tmp/acl_mac_list", vap_name, vap_name);
	EXE_COMMAND(cmd);

	fin = fopen("/tmp/acl_mac_list","r");
	while ((acl_c=fgetc(fin)) != EOF){
        ungetc(acl_c,fin);        
        readline(acl_str,fin);
        strcpy_delspace(acl_str, acl_mac_list->mac[acl_num++].addr);
    }
	acl_mac_list->acl_num = acl_num;
	
	fclose(fin);
    EXE_COMMAND("rm -f /tmp/acl_mac_list");
	
	return T_SUCCESS;
}

/*****************************************************************/
/*                                    ROGUE  AP SCAN AND FILTER                                      */ 
/*****************************************************************/
int get_rogue_ap_filter_list(int radio, SCAN_AP_LIST *ap_list)
{
	FILE *fp;
	int i=0;
	int k=0;
	int ret = 0;
	int rMode = 0;
	int apNum = 0;
	int tmpNum = 0;
	int oui_num = 0;
	int bssid_num = 0;
	int tmp_nosec = 0;
	int tmp_leagl = 0;
	int apNTWType = 0;
	int apChannel = 0;
	int apSignal = 0;
	int apEncryption = 0;
	int apAuthmode = 0;
	char buf[32] = {0};
	char cmd[128] = {0};
	char apSsid[64] = {0};
	char tmpBssid[16] = {0};
	char security_enable[32] = {0};
	char legalap_enable[32] = {0};
	char vap_name[8] = {0};
	char apWMode[8] = {0};
	char apBssid[32] = {0};
	char ModeTmpBuf[32] = {0};
	char oui_str[512] = {0};
	char bssid_str[512] = {0};
	
	if (RADIO_2G == radio) {
		ezplib_get_attr_val("wl_mode_rule", 0, "mode", ModeTmpBuf, 32, EZPLIB_USE_CLI);
		
		if (!strcmp(ModeTmpBuf, "ap")) {
			rMode = WLAN_MODE_AP;
		}
		else {
			printf("2.4G Not AP Mode!\n");
			return T_FAILURE;
		}
	}
	else if (RADIO_5G == radio) {
		ezplib_get_attr_val("wl1_mode_rule", 0, "mode", ModeTmpBuf, 32, EZPLIB_USE_CLI);
		
		if (!strcmp(ModeTmpBuf, "ap")) {
			rMode = WLAN_MODE_AP;
		}
		else {
			printf("5G Not AP Mode!\n");
			return T_FAILURE;
		}
	}
	else {
		printf("ERROR:Radio error!\n");
        return T_FAILURE;
	}
	
	construct_vap(vap_name, radio, 0, rMode);
	
    /*iwlist athN scanning | awk -f /etc/wl/rogueap.awk*/    
    sprintf(cmd, "iwlist %s scanning | awk -f /etc/wl/rogueap.awk > /tmp/rogueaplt", vap_name);
    EXE_COMMAND(cmd);    
    
    /*Set ap_num as 0 first*/
    ap_list->ap_num = 0;
    if (NULL == (fp = fopen("/tmp/rogueaplt","r")))
    {
        printf("Ap list is NONE\n");
        return T_FAILURE;        
    }
    else
    {
    	ezplib_get_attr_val("wl_rap_rule", 0, "security_enable", security_enable, 32, EZPLIB_USE_CLI);

		ezplib_get_attr_val("wl_rap_rule", 0, "legalap_enable", legalap_enable, 32, EZPLIB_USE_CLI);
		if (!strcmp(legalap_enable, "1")) {
			oui_num = ezplib_get_rule_num("wl_rap_oui_rule");
			bssid_num = ezplib_get_rule_num("wl_rap_mac_rule");
			for(i=0; i<oui_num; i++){
				ezplib_get_attr_val("wl_rap_oui_rule", i, "oui", buf, 32, EZPLIB_USE_CLI);
				strncat(oui_str, buf, 512);
				if (i != (oui_num - 1)) {
					strncat(oui_str, "|", 512);
				}
			}
			for(k=0; k<bssid_num; k++){
				ezplib_get_attr_val("wl_rap_mac_rule", k, "mac", buf, 32, EZPLIB_USE_CLI);
				strncat(bssid_str, buf, 512);
				if (i != (bssid_num - 1)) {
					strncat(bssid_str, "|", 512);
				}
			}
		}
	
        while(!feof(fp)){
			tmp_nosec = 0;
			tmp_leagl = 0;
			memset(apBssid, 0x00, sizeof(apBssid));
			memset(apSsid, 0x00, sizeof(apSsid));
			memset(apWMode, 0x00, sizeof(apWMode));

			ret = fscanf(fp, "%d%s%d%d%d%s%d%d%[^\n]", &apNum, apBssid, 
				&apNTWType, &apChannel, &apSignal, apWMode, &apEncryption, &apAuthmode, apSsid);
			if ( 9 != ret) {
				printf("fscanf Number:%d\n", ret);
				printf("fsacnf End\n");
				break;
			}

			if (!strcmp(legalap_enable, "1")) {
				strncpy(tmpBssid, apBssid, 8);
				if ((strstr(bssid_str, apBssid) == NULL) && (strstr(oui_str, tmpBssid) == NULL)) {
					tmp_leagl = 2;
				}
			}

			if (!strcmp(security_enable, "1")) {
				if (0 == apAuthmode) {
					tmp_nosec = 1;
				}
			}

			if ((0==tmp_leagl) && (0==tmp_nosec)) {
				if ((!strcmp(security_enable, "1")) || (!strcmp(legalap_enable, "1"))) {
					continue;
				} else {
					ap_list->ap[tmpNum].flag = 4;
				}
			} else {
				/* 0->legal; 1->No Security; 2->Illegal; 3-> No Security&Illegal; 4->No Filter */
				ap_list->ap[tmpNum].flag = tmp_nosec + tmp_leagl;
			}
			
			memset(ap_list->ap[tmpNum].ssid, 0x00, sizeof(ap_list->ap[tmpNum].ssid));
            memset(ap_list->ap[tmpNum].bssid, 0x00, sizeof(ap_list->ap[tmpNum].bssid));
			memset(ap_list->ap[tmpNum].rssi_str, 0x00, sizeof(ap_list->ap[tmpNum].rssi_str));
			memset(ap_list->ap[tmpNum].wmode, 0x00, sizeof(ap_list->ap[tmpNum].wmode));

			/* AP BSSID */
			strcpy(ap_list->ap[tmpNum].bssid, apBssid);

			/* AP Network Type: 0->Auto; 1->Ad-hoc; 2->Managed; 3->Repeater; 4->Master; 5->Secondary; 6->Monitor; 7->Unknow*/
			ap_list->ap[tmpNum].network_type = apNTWType;

			/* AP Channel*/
			ap_list->ap[tmpNum].channel = apChannel;

			/* AP Signal*/
			ap_list->ap[tmpNum].rssi = - apSignal;
			sprintf(ap_list->ap[tmpNum].rssi_str, "%d dBm", ap_list->ap[tmpNum].rssi);

			/* AP Wireless Mode*/
			strcpy(ap_list->ap[tmpNum].wmode, apWMode);
			
			/* AP Encryption*/
			ap_list->ap[tmpNum].encry = apEncryption;

			/* AP Auth_mode*/
			ap_list->ap[tmpNum].auth_mode = apAuthmode;

			/* AP SSID*/
			strncpy(ap_list->ap[tmpNum].ssid, &apSsid[2], (strlen(apSsid) - 3));

			/* AP Number */
			tmpNum++;
			ap_list->ap_num = tmpNum;
			if(ap_list->ap_num == 128)
            {
                break;
            }
        }
    }

    fclose(fp);
#if 1
    sprintf(cmd, "rm -rf /tmp/rogueaplt");
	EXE_COMMAND(cmd);
#else
        int ap_num = ap_list->ap_num;
        printf("ap_num after is:%d\n",ap_num);
        int j;
        for(j=0; j< ap_num; j++)
            {
                printf("SSID%d:%s\n",j,ap_list->ap[j].ssid);
                printf("BSSID%d:%s\n",j,ap_list->ap[j].bssid);
                printf("RSSI%d:%d\n",j,ap_list->ap[j].rssi);
                printf("Channel%d:%d\n",j,ap_list->ap[j].channel);
                printf("AuthMode%d:%d\n",j,ap_list->ap[j].auth_mode);
                printf("ENCRY%d:%d\n",j,ap_list->ap[j].encry);
                printf("NET_TYPE%d:%d\n",j,ap_list->ap[j].network_type);
            }
#endif

    return T_SUCCESS;    
}

int ap_list_cmp(const void*a, const void*b)
{
	return ((SCAN_AP_STATE*)a)[0].rssi < ((SCAN_AP_STATE*)b)[0].rssi;
}

/*****************************************************************/
/*                                         GET  ROGUE  AP LIST                                           */ 
/*****************************************************************/

int get_rogue_ap_list(SCAN_AP_LIST *ap_list)
{
	int i = 0;
	int ret = 0;
	int tmpNum = 0;
	SCAN_AP_LIST *ap2G_list = (SCAN_AP_LIST *)malloc(sizeof(SCAN_AP_LIST));
	SCAN_AP_LIST *ap5G_list = (SCAN_AP_LIST *)malloc(sizeof(SCAN_AP_LIST));
	memset(ap2G_list, 0, sizeof(SCAN_AP_LIST));
	memset(ap5G_list, 0, sizeof(SCAN_AP_LIST));

	ret = get_rogue_ap_filter_list(RADIO_2G, ap2G_list);
	if (T_SUCCESS == ret) {
		qsort(&ap2G_list->ap, ap2G_list->ap_num, sizeof(ap2G_list->ap[0]), ap_list_cmp);
		for(i = 0; i < ap2G_list->ap_num; i++) {
			memset(ap_list->ap[i].ssid, 0x00, sizeof(ap_list->ap[i].ssid));
            memset(ap_list->ap[i].bssid, 0x00, sizeof(ap_list->ap[i].bssid));
			memset(ap_list->ap[i].rssi_str, 0x00, sizeof(ap_list->ap[i].rssi_str));
			
			strcpy(ap_list->ap[i].ssid, ap2G_list->ap[i].ssid);
			strcpy(ap_list->ap[i].bssid, ap2G_list->ap[i].bssid);
			ap_list->ap[i].network_type = ap2G_list->ap[i].network_type;
			ap_list->ap[i].channel = ap2G_list->ap[i].channel;
			ap_list->ap[i].rssi = ap2G_list->ap[i].rssi;
			strcpy(ap_list->ap[i].rssi_str, ap2G_list->ap[i].rssi_str);
			strcpy(ap_list->ap[i].wmode, ap2G_list->ap[i].wmode);
			ap_list->ap[i].encry = ap2G_list->ap[i].encry;
			ap_list->ap[i].auth_mode = ap2G_list->ap[i].auth_mode;
			ap_list->ap[i].flag = ap2G_list->ap[i].flag;
		}
		ap_list->ap_num = ap2G_list->ap_num;
	}

	ret = get_rogue_ap_filter_list(RADIO_5G, ap5G_list);
	if (T_SUCCESS == ret) {
		qsort(&ap5G_list->ap, ap5G_list->ap_num, sizeof(ap5G_list->ap[0]), ap_list_cmp);
		tmpNum = ap2G_list->ap_num;
		for(i = 0; i < ap5G_list->ap_num; i++) {
			memset(ap_list->ap[tmpNum + i].ssid, 0x00, sizeof(ap_list->ap[i].ssid));
            memset(ap_list->ap[tmpNum + i].bssid, 0x00, sizeof(ap_list->ap[i].bssid));
			memset(ap_list->ap[tmpNum + i].rssi_str, 0x00, sizeof(ap_list->ap[i].rssi_str));
			
			strcpy(ap_list->ap[tmpNum + i].ssid, ap5G_list->ap[i].ssid);
			strcpy(ap_list->ap[tmpNum + i].bssid, ap5G_list->ap[i].bssid);
			ap_list->ap[tmpNum + i].network_type = ap5G_list->ap[i].network_type;
			ap_list->ap[tmpNum + i].channel = ap5G_list->ap[i].channel;
			ap_list->ap[tmpNum + i].rssi = ap5G_list->ap[i].rssi;
			strcpy(ap_list->ap[tmpNum + i].rssi_str, ap5G_list->ap[i].rssi_str);
			strcpy(ap_list->ap[tmpNum + i].wmode, ap5G_list->ap[i].wmode);
			ap_list->ap[tmpNum + i].encry = ap5G_list->ap[i].encry;
			ap_list->ap[tmpNum + i].auth_mode = ap5G_list->ap[i].auth_mode;
			ap_list->ap[tmpNum + i].flag = ap5G_list->ap[i].flag;
		}
		ap_list->ap_num = tmpNum + ap5G_list->ap_num;
	}

	free(ap2G_list);
	free(ap5G_list);
	
	return T_SUCCESS;
}

/*****************************************************************/
/*                                         GET  DFS CHANNEL LIST                                        */
/*                                     Added By Andy Yu in 20140310                                   */
/*****************************************************************/
int get_dfschannel(DFS_CHAN_LIST *dfs_list)
{
	FILE *fp;
	int ret = 0;
	int num = 0;
	int tmp_num = 0;
	int tmp_freq = 0;
	char cmd[128] = {0};
	char vap_name[8] = {0};
	
	if(T_FAILURE == construct_vap(vap_name, RADIO_5G, 0, WLAN_MODE_AP))
	{
		printf("ERROR: Get VAP Name Error\n");
		return T_FAILURE;
	}

	sprintf(cmd, "wlanconfig %s list chan | awk '{split($0,a,\"Channel\"); print a[2] \"\\n\" a[3]}' | awk '/\~/{print $1 \"\\t\" $3}' | sort -n > /tmp/DFSChan", vap_name);
	EXE_COMMAND(cmd);

	if (NULL == (fp = fopen("/tmp/DFSChan","r")))
	{
		printf("DFS Channel list is NONE\n");
        return T_FAILURE;
	}
	else
	{
		while(!feof(fp))
		{
			ret = fscanf(fp, "%d%d", &tmp_num, &tmp_freq);
			if (2 != ret)
			{
				printf("DFS Chan Read End, fscanf Num: %d\n", ret);
				break;
			}
			dfs_list->chan_list[num].chan_num = tmp_num;
			dfs_list->chan_list[num].chan_freq = tmp_freq;
			num++;
			dfs_list->dfs_num = num;
		}
	}
	
	fclose(fp);

#if 1
		sprintf(cmd, "rm -rf /tmp/DFSChan");
		EXE_COMMAND(cmd);
#else
		int k = 0;
		printf("DFS Num: %d\n", dfs_list->dfs_num);
		for (k = 0; k < dfs_list->dfs_num; k++)
		{
			printf("%d %d\n", dfs_list->chan_list[k].chan_num, dfs_list->chan_list[k].chan_freq);
		}
#endif

	return T_SUCCESS;
}

/*****************************************************************/
/*                                         GET  RADAR CHANNEL LIST                                        */
/*                                     Added By Andy Yu in 20140311                                   */
/*****************************************************************/
int get_radarchannel(DFS_REQ_NOLINFO *dfs_list)
{
	struct ifreq ifr;
	DFS_RADAR_HANDLER radarnol;
	memset(&radarnol, 0, sizeof(DFS_RADAR_HANDLER));
	DFS_RADAR_HANDLER *radar = &radarnol;

	radar->s = socket(AF_INET, SOCK_DGRAM, 0);
	if (radar->s < 0)
	{
		printf("SOCKET ERROR: Radar List\n");
		return T_FAILURE;
	}
	strncpy(radar->atd.ad_name, RADIO5_DEFAULT, sizeof (radar->atd.ad_name));

    radar->atd.ad_id = DFS_GET_NOL | ATH_DIAG_DYN;
    radar->atd.ad_in_data = NULL;
    radar->atd.ad_in_size = 0;
    radar->atd.ad_out_data = (void *) dfs_list;
    radar->atd.ad_out_size = sizeof(DFS_REQ_NOLINFO);
    strcpy(ifr.ifr_name, radar->atd.ad_name);
    ifr.ifr_data = (void *) &radar->atd;

    if (ioctl(radar->s, SIOCGATHPHYERR, &ifr) < 0)
    {
    	printf("ERROR: wifi1 get radar list\n");
		close(radar->s);
		return T_FAILURE;
    }
	close(radar->s);
#if 0
	int i = 0;
	for(i=0; i<dfs_list->ic_nchans; i++)
	{
		printf("nol:%d channel=%d MHz width=%d MHz time left=%u seconds nol starttick=%llu \n", 
            i, dfs_list->dfs_nol[i].nol_freq, dfs_list->dfs_nol[i].nol_chwidth, ((dfs_list->dfs_nol[i].nol_timeout_ms)/1000),
            (unsigned long long)dfs_list->dfs_nol[i].nol_start_ticks);
	}
#endif
	return T_SUCCESS;
}

/*****************************************************************/
/*                                         GET  DFS CAC STATE                                        	  */
/*                                     Added By Andy Yu in 20140311                                   */
/*****************************************************************/
int get_cacstate(unsigned int *cac_state)
{
	struct ifreq ifr;
	DFS_RADAR_HANDLER radarnol;
	memset(&radarnol, 0, sizeof(DFS_RADAR_HANDLER));
	DFS_RADAR_HANDLER *radar = &radarnol;

	radar->s = socket(AF_INET, SOCK_DGRAM, 0);
	if (radar->s < 0)
	{
		printf("SOCKET ERROR: CAC State\n");
		return T_FAILURE;
	}
	strncpy(radar->atd.ad_name, RADIO5_DEFAULT, sizeof (radar->atd.ad_name));

    radar->atd.ad_id = DFS_IS_CAC_DONE | ATH_DIAG_DYN;
    radar->atd.ad_in_data = NULL;
    radar->atd.ad_in_size = 0;
    radar->atd.ad_out_data = (void *) cac_state;
    radar->atd.ad_out_size = sizeof(unsigned int);
    strcpy(ifr.ifr_name, radar->atd.ad_name);
    ifr.ifr_data = (void *) &radar->atd;

    if (ioctl(radar->s, SIOCGATHPHYERR, &ifr) < 0)
    {
    	printf("ERROR: Get CAC State\n");
		close(radar->s);
		return T_FAILURE;
    }
	close(radar->s);
#if 0
	printf("CAC STATE: %d\n", *cac_state);
#endif
	return T_SUCCESS;
}


/*************************************/
/*<<<<<<<<<<general process<<<<<<<<<<*/
/*************************************/
