//Added by Mario Huang 2013-07-15
#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<unistd.h>

#include	<sys/ioctl.h>
#include	<arpa/inet.h>
#ifdef CONFIG_DEFAULTS_KERNEL_2_6_21
  #include	<linux/types.h>
  #include	<linux/socket.h>
  #include	<linux/if.h>
#endif
#include	<linux/wireless.h>
#include 	"nvram.h"
#include "nvram_rule.h"

// Tommy, Add syslog, 2009/10/21 04:47
#include <syslog.h>
#include "mid_common.h"
#include "mid_detail.h"
/*
Get VAP status
1: Up
0:Down
*/
int get_vap_status(int radio, int vapid, int mode, int *link_status)
{
	int ret;
	char cmd[128];
	char vap_name[8];
	FILE *fin;
	char str[255],c;
	char ptn[]="UP";

	ret = construct_vap(vap_name, radio, vapid, mode);
	if(T_FAILURE == ret)
		{
			printf("ERROR:Construct VAP Failure!\n");
			return T_FAILURE;
		}
	/*Set it as down first, get_vap_link_status wil modify it if it is up*/
	*link_status = VAP_DOWN;
	sprintf(cmd, "ifconfig %s > /tmp/vap_link_status",vap_name);
	printf("CMD in get_vap_status is %s\n", cmd);
	system(cmd);
	fin = fopen("/tmp/vap_link_status","r");
			
	while ((c=fgetc(fin)) != EOF){
		ungetc(c,fin);		
		readline(str,fin);
		if (get_vap_link_status(link_status, str, ptn) == T_SUCCESS)
			break;
	}

	fclose(fin);
	system("rm -f /tmp/vap_link_status");	

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
	char TempBuf_opmode[32];
	int mode_ap;
        int opmode;
	/*Get the operation mode*/
	if(RADIO_2G == radio)
		{
			ezplib_get_attr_val("system_mode", 0, "name", TempBuf_opmode, 32, EZPLIB_USE_CLI);
		}
#if 0
	else if(RADIO_5G == radio)
		{
			ezplib_get_attr_val("wl1_mode_rule", 0, "mode", TempBuf_opmode, 32, EZPLIB_USE_CLI);
		}
#endif
	if(!strcmp(TempBuf_opmode, "ap") || !strcmp(TempBuf_opmode, "normal"))
		{
			mode_ap = 1;
		}
	else
		{
			mode_ap = 0;
		}
	printf("opmode is %s\n", mode_ap ? "ap":"Not ap");

	/*We use the first VAP as the main vap, so get its status as radio status*/
        if(mode_ap)
            {
                opmode = WLAN_MODE_AP;
            }
        else
            {
                opmode = WLAN_MODE_STA;
            }
	ret = get_vap_status(radio,  0, opmode, radio_status);
	if(T_FAILURE == ret)
		{
			printf("ERROR:Get VAP Staus Failure!\n");
		}	
	return ret;
}

/*Get wlan rate*/
int get_wlan_rate(int radio,int *rate)
{
	char TempBuf_status[32];
	char TempBuf_rate[32];
	FILE *fp = NULL;
	char TempBuf_opmode[32];
	int mode_ap;
	/*Get the operation mode*/
	if(RADIO_2G == radio)
		{
			ezplib_get_attr_val("system_mode", 0, "name", TempBuf_opmode, 32, EZPLIB_USE_CLI);
		}
#if 0
	else if(RADIO_5G == radio)
		{
			ezplib_get_attr_val("wl1_mode_rule", 0, "mode", TempBuf_opmode, 32, EZPLIB_USE_CLI);
		}
#endif
	if(!strcmp(TempBuf_opmode, "ap") || !strcmp(TempBuf_opmode, "normal"))
		{
			mode_ap = 1;
		}
	else
		{
			mode_ap = 0;
		}
	printf("opmode is %s\n", mode_ap ? "ap":"Not ap");

	if(RADIO_2G == radio)
		{
			if(mode_ap)
				{
					
					ezplib_get_attr_val("wl_ap_basic_rule", 0, "enable", TempBuf_status, 32, EZPLIB_USE_CLI);
				}
			else
				{
				
					ezplib_get_attr_val("wl_basic_rule", 0, "enable", TempBuf_status, 32, EZPLIB_USE_CLI);
				}
		}
	else if(RADIO_5G == radio)
		{
			if(mode_ap)
				{
					ezplib_get_attr_val("wl5g_ap_basic_rule", 0, "enable", TempBuf_status, 32, EZPLIB_USE_CLI);
				}
			else
				{
					ezplib_get_attr_val("wl5g_basic_rule", 0, "enable", TempBuf_status, 32, EZPLIB_USE_CLI);
				}
		}
	
	if (!strcmp(TempBuf_status, "1")){
		if(RADIO_2G == radio)
			{
				system("/sbin/ezp-wps-set 1 0 0 0");

				if (NULL == (fp = fopen("/tmp/link_rate", "r")))
					{	
             					*rate = 300;
						return T_SUCCESS;
					}	

				fscanf(fp, "%s", TempBuf_rate);
				fclose(fp);
				system("rm -f /tmp/link_rate");
				if (strlen(TempBuf_rate)>0)
				{
					*rate = atoi(TempBuf_rate);
				}else{
					*rate = 300;
				}
				return T_SUCCESS;
			}
		else if(RADIO_5G == radio)
			{
				system("/sbin/ezp-wps-set 1 1 0 0");

				if (NULL == (fp = fopen("/tmp/link_rate", "r")))
					{	
             					*rate = 450;
						return T_SUCCESS;
					}	

				fscanf(fp, "%s", TempBuf_rate);
				fclose(fp);
				system("rm -f /tmp/link_rate");
				if (strlen(TempBuf_rate)>0)
				{
					*rate = atoi(TempBuf_rate);
				}else{
					*rate = 450;
				}
				return T_SUCCESS;
			}
	}else{
		*rate = 0;
	}
	return T_SUCCESS;
}	

/****************************************************************************************************************************/
/*                                                                                                                                                                                                                     */
/*                                                                                                                                                                                                                     */
/*Set and Get WLAN Basic Settings for radio                                                                                                                                                         */
/*                                                                                                                                                                                                                     */
/*                                                                                                                                                                                                                     */
/****************************************************************************************************************************/
/*Get the channel selection mode, auto or fixed*/
 int get_channel_select_mode(int radio, int *mode)
{
    char *channel_s = NULL;
    char buf_channel[32];
    int channel;
    if(RADIO_2G == radio)
    	{
            channel_s = nvram_safe_get("AP_Channel");
    	}
    else if(RADIO_5G == radio)
    	{
            channel_s = nvram_safe_get("AP1_Channel");
    	}
    strcpy(buf_channel,channel_s);
    channel = atoi(buf_channel);
    if(CHANNEL_AUTO == channel)
    	{
    	    *mode = CHANNEL_AUTO;
    	}
    else
    	{
     	    *mode = CHANNEL_FIXED;   	
    	}
    printf("Channel select mode is %s\n", (channel == 0? "AUTO":"Fixed"));
    return T_SUCCESS;
}


/*get the channel current in use*/
int get_current_channel(int radio, Channel_t *channel_get)
{

	FILE *fin;
	char str[255],c;
	char channel[3] = {0};
	char ptn[]="Channel=";
	int channel_no;
	
	if(radio != RADIO_2G && radio != RADIO_5G)
		{
			printf("ERROR:Such radio is not exist!\n");
			return T_FAILURE;
		}
	
/*Get current channel on 2.4G radio*/	
	if(RADIO_2G == radio)
		{
			system("/sbin/ezp-wps-set 1 0 1 0");
			fin = fopen("/tmp/opchannel","r");
			
			while ((c=fgetc(fin)) != EOF){
				ungetc(c,fin);		
				readline(str,fin);
				if (get_channel(channel, str, ptn) == 0)
			   	break;
			}

			fclose(fin);

			system("rm -f /tmp/opchannel");	
			printf("current channel is %s\n", channel);
			channel_no = atoi(channel);


			//if (channel_no == 0){
			    //channel_get->chan_number = 1;
			    //channel_get->frequency = 2412;
			//}else if (channel_no == 1){
			if (channel_no == 1){
			    channel_get->chan_number = 1;
			    channel_get->frequency = 2412;
			}else if (channel_no == 2){
			    channel_get->chan_number = 2;
			    channel_get->frequency = 2417;
			}else if (channel_no == 3){	
			    channel_get->chan_number = 3;
			    channel_get->frequency = 2422;
			}else if (channel_no == 4){	
			    channel_get->chan_number = 4;
			    channel_get->frequency = 2427;
			}else if (channel_no == 5){	
			    channel_get->chan_number = 5;
			    channel_get->frequency = 2432;
			}else if (channel_no == 6){	
			    channel_get->chan_number = 6;
			    channel_get->frequency = 2437;
			}else if (channel_no == 7){	
			    channel_get->chan_number = 7;
			    channel_get->frequency = 2442;
			}else if (channel_no == 8){	
			    channel_get->chan_number = 8;
			    channel_get->frequency = 2447;
			}else if (channel_no == 9){	
			    channel_get->chan_number = 9;
			    channel_get->frequency = 2452;
			}else if (channel_no == 10){
			    channel_get->chan_number = 10;
			    channel_get->frequency = 2457;
			}else if (channel_no == 11){
			    channel_get->chan_number = 11;
			    channel_get->frequency = 2462;
			}else if (channel_no == 12){
			    channel_get->chan_number = 12;
			    channel_get->frequency = 2467;
			}else if (channel_no == 13){
			    channel_get->chan_number = 13;
			    channel_get->frequency = 2472;
			}else if (channel_no == 14){
			    channel_get->chan_number = 14;
			    channel_get->frequency = 2484;
			}else{
			    printf("ERROR:Get channel no error!\n");
			    return T_FAILURE;
			}
		}
/*Get current channel on 5G radio*/	
	else if(RADIO_5G == radio)
		{
			system("/sbin/ezp-wps-set 1 1 1 0");

			fin = fopen("/tmp/opchannel","r");
			
			while ((c=fgetc(fin)) != EOF){
				ungetc(c,fin);		
				readline(str,fin);
				if (get_channel(channel, str, ptn) == 0)
			   	break;
			}

			fclose(fin);

			system("rm -f /tmp/opchannel");	
			printf("current channel is %s\n", channel);
			channel_no = atoi(channel);
			//if(channel_no == 0){
			    //channel_get->chan_number = 36;
			    //channel_get->frequency = 5180;
			//}else if (channel_no == 36){
			if (channel_no == 36){
			    channel_get->chan_number = 36;
			    channel_get->frequency = 5180;
			}else if (channel_no == 40){
			    channel_get->chan_number = 40;
			    channel_get->frequency = 5200;
			}else if (channel_no == 44){
			    channel_get->chan_number = 44;
			    channel_get->frequency = 5220;
			}else if (channel_no == 48){
			    channel_get->chan_number = 48;
			    channel_get->frequency = 5240;
			}else if (channel_no == 52){
			    channel_get->chan_number = 52;
			    channel_get->frequency = 5260;
			}else if (channel_no == 56){
			    channel_get->chan_number = 56;
			    channel_get->frequency = 5280;
			}else if (channel_no == 60){
			    channel_get->chan_number = 60;
			    channel_get->frequency = 5300;
			}else if (channel_no == 64){
			    channel_get->chan_number = 64;
			    channel_get->frequency = 5320;
			}else if (channel_no == 100){
			    channel_get->chan_number = 100;
			    channel_get->frequency = 5500;
			}else if (channel_no == 104){
			    channel_get->chan_number = 104;
			    channel_get->frequency = 5520;
			}else if (channel_no == 108){
			    channel_get->chan_number = 108;
			    channel_get->frequency = 5540;
			}else if (channel_no == 112){
			    channel_get->chan_number = 112;
			    channel_get->frequency = 5560;
			}else if (channel_no == 116){
			    channel_get->chan_number = 116;
			    channel_get->frequency = 5580;
			}else if (channel_no == 132){
			    channel_get->chan_number = 132;
			    channel_get->frequency = 5660;
			}else if (channel_no == 136){
			    channel_get->chan_number = 136;
			    channel_get->frequency = 5680;
			}else if (channel_no == 140){
			    channel_get->chan_number = 140;
			    channel_get->frequency = 5700;
			}else if (channel_no == 149){
			    channel_get->chan_number = 1;
			    channel_get->frequency = 2412;
			}else if (channel_no == 153){
			    channel_get->chan_number = 153;
			    channel_get->frequency = 5765;
			}else if (channel_no == 157){
			    channel_get->chan_number = 157;
			    channel_get->frequency = 5785;
			}else if (channel_no == 161){
			    channel_get->chan_number = 161;
			    channel_get->frequency = 5805;
			}else if (channel_no == 165){
			    channel_get->chan_number = 165;
			    channel_get->frequency = 5825;
			}else{
			    printf("ERROR:Get channel no error!\n");
			    return T_FAILURE;
			}
		
		}
	return T_SUCCESS;
}	

/**
 * @brief create channellist in /tmp if it doesn't exist
 * @return T_SUCCESS on success and T_FAILURE on fail.
 * @author frank
 * @date 2014-03-13
 */
#define CL_FILE "/tmp/channellist"
int createChannelList(void)
{
	char cmd[64] = {0};
	FILE *f = NULL;
	f = fopen(CL_FILE, "r");
	if( NULL == f ) {
		sprintf(cmd, "wlanconfig ath0 list chan | cut -b1-48 > %s", CL_FILE);
		EXE_COMMAND(cmd);
		sprintf(cmd, "wlanconfig ath0 list chan | cut -b53- >> %s", CL_FILE);
		EXE_COMMAND(cmd);
		sprintf(cmd, "wlanconfig ath1 list chan | cut -b1-48 >> %s", CL_FILE);
		EXE_COMMAND(cmd);
		sprintf(cmd, "wlanconfig ath1 list chan | cut -b53- >> %s", CL_FILE);
		EXE_COMMAND(cmd);
		return T_SUCCESS;
	}
	else {
		fclose(f);
		return T_FAILURE;
	}
}

/**
 * @brief check whether the channel has upper extension channel
 * @param[in] channel	1~11, 36~64, 100~140, 149~165			
 * @return has(1) or not(0) 
 *
 * @author frank_zhou@lccbj.com
 */
unsigned char hasUpper(int channel)
{
	char cmd[128] = {0};
	sprintf(cmd, "grep Channel[\\ ]*%d[\\ ]*:.*CU %s > /dev/null", channel, CL_FILE);
	createChannelList();
	printf("\033[1;33;40m%s\033[0m\n", cmd);
	return 0 == system(cmd);
}

/**
 * @brief check whether the channel has lower extension channel
 * @param[in] channel	1~11, 36~64, 100~140, 149~165			
 * @return has(1) or not(0) 
 *
 * @author frank_zhou@lccbj.com
 */
unsigned char hasLower(int channel)
{
	char cmd[128] = {0};
	sprintf(cmd, "grep Channel[\\ ]*%d[\\ ]*:.*CL %s > /dev/null", channel, CL_FILE);
	createChannelList();
	printf("\033[1;33;40m%s\033[0m\n", cmd);
	return 0 == system(cmd);
}

/*get the channel list*/
int get_channel_list(int radio, ChannelListInfo_t *channel_list)
{
	char *channel_s;
    	char buf[TMP_LEN]; 
	char buf_channel[32];
	char wirelessmode[4] = {0};
	char *value, *tmpvalue;
	
	int idx = 0, channel,i = 0,j = 0,k  = 0;
	FILE *fin;
	char str[255],c;
	char ptn[]="Channel=";
	char channels[3];

	char TempBuf_opmode[32];
	int mode_ap;
	/*Get the operation mode*/
	if(RADIO_2G == radio)
		{
			ezplib_get_attr_val("system_mode", 0, "name", TempBuf_opmode, 32, EZPLIB_USE_CLI);
		}
#if 0
	else if(RADIO_5G == radio)
		{
			ezplib_get_attr_val("wl1_mode_rule", 0, "mode", TempBuf_opmode, 32, EZPLIB_USE_CLI);
		}
#endif
	if(!strcmp(TempBuf_opmode, "ap") || !strcmp(TempBuf_opmode, "normal"))
		{
			mode_ap = 1;
		}
	else
		{
			mode_ap = 0;
		}
	printf("opmode is %s\n", mode_ap ? "ap":"Not ap");
 	
	tmpvalue = nvram_safe_get("wl_region");

	if (!strcmp (tmpvalue,"0")){
		printf("	==>US\n");
        value = "US";
	}
	else if (!strcmp (tmpvalue,"1")){
		printf("	==>FR\n");
		value = "FR";
	}
	else if (!strcmp (tmpvalue,"5")){
    	printf("	==>JP\n");
		value = "JP";
	}else{
		printf("	Other==>FR\n");
		value = "FR";
	}

	if(RADIO_2G ==radio)
		{
			channel_s = nvram_safe_get("AP_Channel");
			strcpy(buf_channel,channel_s);
			if(mode_ap)
				{
					ezplib_get_attr_val("wl_ap_basic_rule", 0, "net_mode", buf, TMP_LEN, EZPLIB_USE_CLI); 
				}
			else
				{
					ezplib_get_attr_val("wl_basic_rule", 0, "net_mode", buf, TMP_LEN, EZPLIB_USE_CLI); 
				}
			strcpy(wirelessmode, buf);

			if( !strcmp(channel_s,"0")){
			      //system("iwconfig ra0 > /tmp/opchannel");
			      system("/sbin/ezp-wps-set 1 0 1 0");
				fin = fopen("/tmp/opchannel","r");		
				while ((c=fgetc(fin)) != EOF){
					ungetc(c,fin);		
					readline(str,fin);
					if (get_channel(channels, str, ptn) == 0)
				   	break;
				}

				fclose(fin);
				system("rm -f /tmp/opchannel");	
				strcpy(buf_channel,channels);
			}
			
			channel = (buf_channel == NULL)? 0 : atoi(buf_channel);
			printf("selected channel in channel list is %d\n", channel);//Added by Mario Huang for print

			if ((value == NULL) || (strcmp(value, "") == 0) ||
					(strcmp(value, "US") == 0) || (strcmp(value, "JP") == 0) ||
					(strcmp(value, "FR") == 0) || (strcmp(value, "IE") == 0) ||
					(strcmp(value, "TW") == 0) || (strcmp(value, "CN") == 0) ||
					(strcmp(value, "HK") == 0)) {
				for (idx = 0; idx < 11; idx++)
					{
					  	channel_list->channel_list[idx].chan_number = idx+1;
						channel_list->channel_list[idx].frequency = 2412+5*idx;
						channel_list->channel_list[idx].hasUpper = hasUpper(idx+1);
						channel_list->channel_list[idx].hasLower = hasLower(idx+1);
						channel_list->channel_list[idx].centerHT80 = centerHT80(idx+1);
					}
					channel_list->current_chan = channel;
				channel_list->num = 11;//Added by Mario Huang
			}                                                                           
			if ((value == NULL) || (strcmp(value, "") == 0) ||
					(strcmp(value, "TW") == 0) || (strcmp(value, "JP") == 0) || 
					(strcmp(value, "FR") == 0) || (strcmp(value, "IE") == 0) ||
					(strcmp(value, "CN") == 0) || (strcmp(value, "HK") == 0)) {
				for (idx = 11; idx < 13; idx++)
					{
					  	channel_list->channel_list[idx].chan_number = idx+1;
						channel_list->channel_list[idx].frequency = 2412+5*idx;
						channel_list->channel_list[idx].hasUpper = hasUpper(idx+1);
						channel_list->channel_list[idx].hasLower = hasLower(idx+1);
						channel_list->channel_list[idx].centerHT80 = centerHT80(idx+1);
					}
					channel_list->current_chan = channel;
				channel_list->num = 13;//Added by Mario Huang
			}

			if ((value == NULL) || (strcmp(value, "") == 0) ||
				((!strcmp(value, "JP")) && (!strcmp(wirelessmode, "1"))) ) {
					channel_list->channel_list[13].chan_number = 14;
					channel_list->channel_list[13].frequency = 2484;
					channel_list->channel_list[idx].hasUpper = hasUpper(14);
					channel_list->channel_list[idx].hasLower = hasLower(14);
					channel_list->channel_list[idx].centerHT80 = centerHT80(14);
					if(14 == channel)
						{
							channel_list->current_chan = 14;
						}					
				channel_list->num = 14;//Added by Mario Huang
			}
			return T_SUCCESS;
		}
	else if(RADIO_5G == radio)
		{
			channel_s = nvram_safe_get("AP1_Channel");
			strcpy(buf_channel,channel_s);
			if(mode_ap)
				{
					ezplib_get_attr_val("wl5g_ap_basic_rule", 0, "net_mode", buf, TMP_LEN, EZPLIB_USE_CLI); 
				}
			else
				{
					ezplib_get_attr_val("wl5g_basic_rule", 0, "net_mode", buf, TMP_LEN, EZPLIB_USE_CLI); 
				}
			strcpy(wirelessmode, buf);

			
			if( !strcmp(channel_s,"0")){
			      //system("iwconfig ra0 > /tmp/opchannel");
			      system("/sbin/ezp-wps-set 1 1 1 0");
				fin = fopen("/tmp/opchannel","r");		
				while ((c=fgetc(fin)) != EOF){
					ungetc(c,fin);		
					readline(str,fin);
					if (get_channel(channels, str, ptn) == 0)
				   	break;
				}

				fclose(fin);
				system("rm -f /tmp/opchannel");	
				strcpy(buf_channel,channels);
			}
			channel = (buf_channel == NULL)? 36 : atoi(buf_channel);
			printf("selected channel in channel list is %d\n", channel);//Added by Mario Huang for print

			if ((value == NULL) || (strcmp(value, "") == 0) ||
					(strcmp(value, "US") == 0) || (strcmp(value, "JP") == 0) ||
					(strcmp(value, "FR") == 0) || (strcmp(value, "IE") == 0) ||
					(strcmp(value, "TW") == 0) || (strcmp(value, "CN") == 0) ||
					(strcmp(value, "HK") == 0)) {
				for (idx = 0; idx < 21; idx++){		
		  		      if(idx < 8){
					  	channel_list->channel_list[idx].chan_number = idx*4+36;
						channel_list->channel_list[idx].frequency = 5180+10*i;
						i = i+2;
				     }else if(idx > 15){
					  	channel_list->channel_list[idx].chan_number = 149+j*2;
						channel_list->channel_list[idx].frequency = 5745+10*j;
						j = j+2;
				   }else{
					  	channel_list->channel_list[idx].chan_number = 100+k*2;
						channel_list->channel_list[idx].frequency = 5500+10*k;
						if(idx == 12 )
							k = k+8;
						else
							k = k+2;
				   }
				}			
				channel_list->current_chan = channel;
				channel_list->num = 21;//Added by Mario Huang
			}
			return T_SUCCESS;
		}

	printf("ERROR:Wrong radio number!");
	return T_FAILURE;
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
	/* just to easy compile WP688, by frank*/
	return get_channel_list(radio, channel_list);
}

//#if 0
/*Get extension channel*/
int get_extchannel(int radio, Channel_t *extchannel_get)
{
    char cmd[128] = {0};
    FILE *fin;
    char str[255] = {0};
    char c;
    char ptn[]="Ext channel=";
    char channel[3] = {0};
    int channel_no;
    int channel_freq;
 
    memset(channel, 0, sizeof(channel));
    sprintf(cmd, "iwpriv rai0 channel_info | grep \"Ext channel\" > /tmp/channel_info");
    system(cmd);

    fin = fopen("/tmp/channel_info","r");
    while ((c=fgetc(fin)) != EOF){
        ungetc(c,fin);		
        readline(str,fin);
        if (get_channel(channel, str, ptn) == 0)
        break;
    }

    fclose(fin);

    system("rm -f /tmp/channel_info");	
    printf("current channel is %s\n", channel);
    if(!strcmp(channel, ""))
        {
            printf("ERROR:Can not get ext channel\n");
            return T_FAILURE; 
        }
    else
        {
            channel_no = atoi(channel);
            channel_freq = 2407 + 5*channel_no;
        }
    extchannel_get->chan_number = channel_no;
    extchannel_get->frequency = channel_freq;
    printf("ext channel:%d, frequency:%d\n", extchannel_get->chan_number, extchannel_get->frequency);
    return T_SUCCESS;
}
//#endif

/*
Get bandwidth
0: 20MHz
1: 40MHz
*/
int get_bandwidth(int radio, int *htbw)
{
    //int htbw;
    char buf[TMP_LEN]; 
    char htbw_s[TMP_LEN];
    char TempBuf_opmode[32];
    int mode_ap;
    /*Get the operation mode*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("system_mode", 0, "name", TempBuf_opmode, 32, EZPLIB_USE_CLI);
        }
#if 0
    else if(RADIO_5G == radio)
        {
            ezplib_get_attr_val("wl1_mode_rule", 0, "mode", TempBuf_opmode, 32, EZPLIB_USE_CLI);
        }
#endif
    if(!strcmp(TempBuf_opmode, "ap") || !strcmp(TempBuf_opmode, "normal"))
        {
            mode_ap = 1;
        }
    else
        {
            mode_ap = 0;
        }
    printf("opmode is %s\n", mode_ap ? "ap":"Not ap");

    if(RADIO_2G == radio)
    	{
            if(mode_ap)
                {
                    ezplib_get_attr_val("wl_ap_advanced_rule", 0, "htbw", buf, TMP_LEN, EZPLIB_USE_CLI); 
                }
            else
                {
                    ezplib_get_attr_val("wl_advanced_rule", 0, "htbw", buf, TMP_LEN, EZPLIB_USE_CLI); 
                }
    	}
    else if(RADIO_5G == radio)
    	{
            if(mode_ap)
                {
                    ezplib_get_attr_val("wl1_ap_advanced_rule", 0, "htbw", buf, TMP_LEN, EZPLIB_USE_CLI); 
                }
            else
                {
                    ezplib_get_attr_val("wl1_advanced_rule", 0, "htbw", buf, TMP_LEN, EZPLIB_USE_CLI); 
                }
    	}

    strcpy(htbw_s, buf);
    *htbw = atoi(htbw_s); 
    return T_SUCCESS;
}


/*Get wireless mode*/
int get_wirelessmode(int radio, int *mode)
{
    char buf[TMP_LEN];
    char net_mode_s[TMP_LEN]; 
    int net_mode;
    char TempBuf_opmode[32];
    int mode_ap;
    /*Get the operation mode*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("system_mode", 0, "name", TempBuf_opmode, 32, EZPLIB_USE_CLI);
        }
#if 0
    else if(RADIO_5G == radio)
        {
            ezplib_get_attr_val("wl1_mode_rule", 0, "mode", TempBuf_opmode, 32, EZPLIB_USE_CLI);
        }
#endif
    if(!strcmp(TempBuf_opmode, "ap") || !strcmp(TempBuf_opmode, "normal"))
        {
            mode_ap = 1;
        }
    else
        {
            mode_ap = 0;
        }
    printf("opmode is %s\n", mode_ap ? "ap":"Not ap");
    /*Get Wireless Mode From NVRAM*/
    if(RADIO_2G == radio)
    	{
           if(mode_ap)
               {
                   ezplib_get_attr_val("wl_ap_basic_rule", 0, "net_mode", buf, TMP_LEN, EZPLIB_USE_CLI); 
               }
           else
               {
                   ezplib_get_attr_val("wl_basic_rule", 0, "net_mode", buf, TMP_LEN, EZPLIB_USE_CLI); 
               }
	}
    else if(RADIO_5G == radio)
    	{
           if(mode_ap)
               {
                   ezplib_get_attr_val("wl5g_ap_basic_rule", 0, "net_mode", buf, TMP_LEN, EZPLIB_USE_CLI); 
               }
           else
               {
                   ezplib_get_attr_val("wl5g_basic_rule", 0, "net_mode", buf, TMP_LEN, EZPLIB_USE_CLI); 
               }
    	}
    strcpy(net_mode_s, buf);
    net_mode = atoi(net_mode_s);
    *mode = net_mode;
    return T_SUCCESS;
}


/*Get SSID*/
int get_ssid(int radio, int mode, int vap_id, char *ssid)
{
    char buf[33]; 
    //char ssid[33];	//max length of ssid is 32
    int ssid_num;
    /*Get max ssid number*/
    ssid_num = atoi(nvram_safe_get("wlv_rule_max"));
    printf("ssid_num in get_ssid is %d\n", ssid_num);
	
    if(vap_id < 0 || vap_id >= ssid_num)
    	{
    		printf("ERROR:This VAP is not available!\n");
		return T_FAILURE;
    	}
    /*Get the SSID from NVRAM*/
    if(RADIO_2G == radio)
    	{
           ezplib_get_attr_val("wl0_ssid_rule", vap_id, "ssid", buf, 33, EZPLIB_USE_CLI);
    	}
    else if(RADIO_5G == radio)
    	{
           ezplib_get_attr_val("wl1_ssid_rule", vap_id, "ssid", buf, 33, EZPLIB_USE_CLI);
    	}
    strcpy(ssid, buf);    	   	
    printf("Get SSID:%s\n",ssid);
    return T_SUCCESS;
}


int get_sta_list(int radio, StationListInfo_t *sta_list_info)
{
	int i, s;
	char radio_name[8];
	struct iwreq iwr;
	int ret;
	RT_802_11_MAC_TABLE table_2g = {0};		
	RT1_802_11_MAC_TABLE table_5g = {0};		

	s = socket(AF_INET, SOCK_DGRAM, 0);
	
	/*Construct AP*/
	ret = construct_main_ap(radio_name, radio);
	if(T_FAILURE == ret)
		{
			printf("ERRO:Construct main AP failure!\n");
			return T_FAILURE;
		}
	strcpy(iwr.ifr_name, radio_name);

	if(RADIO_2G == radio)
		{
			iwr.u.data.pointer = (caddr_t) &table_2g;
		}
	else if(RADIO_5G == radio)
		{
			iwr.u.data.pointer = (caddr_t) &table_5g;
		}
	
	if (s < 0) {
		printf("ERROR:Open Socket error!\n");
		return T_FAILURE;
	}

	if (ioctl(s, RTPRIV_IOCTL_GET_MAC_TABLE, &iwr) < 0) {
		printf("ERROR:Get MAC Table error!\n");
		close(s);
		return T_FAILURE;
	}
	/*set sta_num as 32 if table.Num exceed 32*/ 
	if(RADIO_2G == radio)
		{
			sta_list_info->sta_num = table_2g.Num>MAX_STA_NUM? MAX_STA_NUM : table_2g.Num;
		}
	else if(RADIO_5G == radio)
		{
			sta_list_info->sta_num = table_5g.Num>MAX_STA_NUM? MAX_STA_NUM : table_5g.Num;
		}

	for (i = 0; i < sta_list_info->sta_num; i++) {
		if(RADIO_2G == radio)
			{
				sta_list_info->sta_list[i].Addr[0] = table_2g.Entry[i].Addr[0];
				sta_list_info->sta_list[i].Addr[1] = table_2g.Entry[i].Addr[1];
				sta_list_info->sta_list[i].Addr[2] = table_2g.Entry[i].Addr[2];
				sta_list_info->sta_list[i].Addr[3] = table_2g.Entry[i].Addr[3];
				sta_list_info->sta_list[i].Addr[4] = table_2g.Entry[i].Addr[4];
				sta_list_info->sta_list[i].Addr[5] = table_2g.Entry[i].Addr[5];
			}
		else if(RADIO_5G == radio)
			{
				sta_list_info->sta_list[i].Addr[0] = table_5g.Entry[i].Addr[0];
				sta_list_info->sta_list[i].Addr[1] = table_5g.Entry[i].Addr[1];
				sta_list_info->sta_list[i].Addr[2] = table_5g.Entry[i].Addr[2];
				sta_list_info->sta_list[i].Addr[3] = table_5g.Entry[i].Addr[3];
				sta_list_info->sta_list[i].Addr[4] = table_5g.Entry[i].Addr[4];
				sta_list_info->sta_list[i].Addr[5] = table_5g.Entry[i].Addr[5];
			}
	
	long sec = 0L;
	//date d;
	int timezone = 0;
	
	char TempBuf[32];
		
	ezplib_get_attr_val("ntp_rule", 0, "zone", TempBuf, 32, EZPLIB_USE_CLI);
		
	if (!strcmp(TempBuf, "UTC12")){
		timezone = 60 * 60 * (-12);
	}else if (!strcmp(TempBuf, "UTC11")){
		timezone = 60 * 60 * (-11);
	}else if (!strcmp(TempBuf, "UTC10")){
		timezone = 60 * 60 * (-10);
	}else if (!strcmp(TempBuf, "NAST9NADT,M3.2.0/2,M11.1.0/2")){
		timezone = 60 * 60 * (-9);
	}else if (!strcmp(TempBuf, "PST8PDT,M3.2.0/2,M11.1.0/2")){
		timezone = 60 * 60 * (-8);
	}else if (!strcmp(TempBuf, "UTC7")){
		timezone = 60 * 60 * (-7);
	}else if (!strcmp(TempBuf, "UTC6")){
		timezone = 60 * 60 * (-6);
	}else if (!strcmp(TempBuf, "CST6CDT,M3.2.0/2,M11.1.0/2")){
		timezone = 60 * 60 * (-6);
	}else if (!strcmp(TempBuf, "UTC5")){
		timezone = 60 * 60 * (-5);
	}else if (!strcmp(TempBuf, "EST5EDT,M3.2.0/2,M11.1.0/2")){
		timezone = 60 * 60 * (-5);
	}else if (!strcmp(TempBuf, "UTC4")){
		timezone = 60 * 60 * (-4);
	}else if (!strcmp(TempBuf, "AST4ADT,M3.2.0/2,M11.1.0/2")){
		timezone = 60 * 60 * (-4);
	}else if (!strcmp(TempBuf, "BRWST4BRWDT,M10.3.0/0,M2.5.0/0")){
		timezone = 60 * 60 * (-4);
	}else if (!strcmp(TempBuf, "NST3:30NDT,M3.2.0/0:01,M11.1.0/0:01")){
		timezone = 60 * 60 * (-3);
	}else if (!strcmp(TempBuf, "WGST3WGDT,M3.5.6/22,M10.5.6/23")){
		timezone = 60 * 60 * (-3);
	}else if (!strcmp(TempBuf, "BRST3BRDT,M10.3.0/0,M2.5.0/0")){
		timezone = 60 * 60 * (-3);
	}else if (!strcmp(TempBuf, "UTC3")){
		timezone = 60 * 60 * (-10);
	}else if (!strcmp(TempBuf, "UTC2")){
		timezone = 60 * 60 * (-2);
	}else if (!strcmp(TempBuf, "STD1DST,M3.5.0/2,M10.5.0/2")){
		timezone = 60 * 60 * (-1);
	}else if (!strcmp(TempBuf, "UTC0")){
		timezone = 60 * 60 * 0;
	}else if (!strcmp(TempBuf, "GMT0BST,M3.5.0/2,M10.5.0/2")){
		timezone = 60 * 60 * 0;
	}else if (!strcmp(TempBuf, "UTC-1")){
		timezone = 60 * 60 * 1;
	}else if (!strcmp(TempBuf, "STD-1DST,M3.5.0/2,M10.5.0/2")){
		timezone = 60 * 60 * 1;
	}else if (!strcmp(TempBuf, "UTC-2")){
		timezone = 60 * 60 * 2;
	}else if (!strcmp(TempBuf, "STD-2DST,M3.5.0/2,M10.5.0/2")){
		timezone = 60 * 60 * 2;
	}else if (!strcmp(TempBuf, "UTC-3")){
		timezone = 60 * 60 * 3;
	}else if (!strcmp(TempBuf, "UTC-4")){
		timezone = 60 * 60 * 4;
	}else if (!strcmp(TempBuf, "UTC-5")){
		timezone = 60 * 60 * 5;
	}else if (!strcmp(TempBuf, "UTC-5:30")){
		timezone = 19800 ;
	}else if (!strcmp(TempBuf, "UTC-6")){
		timezone = 60 * 60 * 6;
	}else if (!strcmp(TempBuf, "UTC-7")){
		timezone = 60 * 60 * 7;
	}else if (!strcmp(TempBuf, "UTC-8")){
		timezone = 60 * 60 * 8;
	}else if (!strcmp(TempBuf, "UTC-9")){
		timezone = 60 * 60 * 9;
	}else if (!strcmp(TempBuf, "CST-9:30CST,M10.5.0/2,M3.5.0/3")){
		timezone = 60 * 60 * 9;
	}else if (!strcmp(TempBuf, "UTC-10")){
		timezone = 60 * 60 * 10;
	}else if (!strcmp(TempBuf, "STD-10DST,M10.5.0/2,M3.5.0/2")){
		timezone = 60 * 60 * 10;
	}else if (!strcmp(TempBuf, "UTC-11")){
		timezone = 60 * 60 * 11;
	}else if (!strcmp(TempBuf, "UTC-12")){
		timezone = 60 * 60 * 12;
	}else if (!strcmp(TempBuf, "STD-12DST,M10.5.0/2,M3.5.0/2")){
		timezone = 60 * 60 * 12;
	}
	if(RADIO_2G == radio)
		{
			sec = table_2g.Entry[i].ConnectedTime - 1; // -1 : if (pEntry->StaConnectTime == 1){ } in linux-2.6.21.x\drivers\net\wireless\rt2860v2\ap\ap.c
		}
	else if(RADIO_5G == radio)
		{
			sec = table_5g.Entry[i].ConnectedTime - 1; // -1 : if (pEntry->StaConnectTime == 1){ } in linux-2.6.21.x\drivers\net\wireless\rt2860v2\ap\ap.c
		}	
       // Deal with time zone	
	sec = sec + timezone;
	sta_list_info->sta_list[i].ConnectedTime=sec2date(sec);
	}
	
	close(s);
	return T_SUCCESS;
}

int get_rogue_ap_list(SCAN_AP_LIST *ap_list)
{
	return T_FAILURE;
}

int get_dfschannel(DFS_CHAN_LIST *dfs_list)
{
	return T_FAILURE;
}
int get_radarchannel(DFS_REQ_NOLINFO *dfs_list)
{
	return T_FAILURE;
}
int get_cacstate(unsigned int *cac_state)
{
	return T_FAILURE;
}


