//Added by Mario Huang 2013-07-15
#include	<stdlib.h>
#include	<stdio.h>
#include <string.h>

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

#include	<assert.h>

// Tommy, Add syslog, 2009/10/21 04:47
#include <syslog.h>
#include "mid_common.h"
#include "mid_detail.h"
/*****************************************************************/
/*STA                                                            */
/*****************************************************************/
static int get_sta_secmode(int radio)
{
    char buf[32];
    char secmode_s[32];
    int secmode;
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl0_apcli_rule", 0, "secmode", buf, 32, EZPLIB_USE_CLI); 
        }
    else
        {
            ezplib_get_attr_val("wl1_apcli_rule", 0, "secmode", buf, 32, EZPLIB_USE_CLI); 
        }
    strcpy(secmode_s, buf);
    if(!strcmp(secmode_s, "wep"))
        {
            secmode = SECMODE_WEP;

        }
    else if(!strcmp(secmode_s, "wpa"))
        {
            secmode = SECMODE_WPA;
        }
    else if(!strcmp(secmode_s, "wpa2"))
        {
            secmode = SECMODE_WPA2;
        }
    else if(!strcmp(secmode_s, "psk"))
        {
            secmode = SECMODE_PSK;
        }
    else if(!strcmp(secmode_s, "psk2"))
        {
            secmode = SECMODE_PSK2;
        }
    else
        {
            secmode = SECMODE_OPEN;
        }

    return secmode;
}

/*Set STA SSID*/
static int set_sta_ssid(int radio)
{
    char buf[33] = {0};
    char ssid[128] = {0};
    char cmd[256] = {0};
    /*Get STA SSID From NVRAM*/
    ezplib_get_attr_val("wl0_apcli_rule", 0, "ssid", buf, 33, EZPLIB_USE_CLI);
    //strcpy(ssid, buf);
    printf("BUF in set_sta_ssid is %s\n", buf);

	int i, j;
	for(i=0,j=0;j<strlen(buf);i++,j++) {
		if( '\"' == buf[j] 
			|| '`' == buf[j]
			|| '\\' == buf[j]
			) {
			ssid[i] = '\\';
			i++;
		}
		ssid[i] = buf[j];
	}

    sprintf(cmd, "iwpriv apclii0 set ApCliSsid=\"%s\"", ssid);
    system(cmd);
    printf("CMD in set_sta_ssid is %s\n", cmd);
    return T_SUCCESS;
}

/*Set STA BSSID*/
static int set_sta_bssid(int radio)
{
    char buf[32];
    char bssid[32];
    char cmd[128];
    /*Get STA BSSID From NVRAM*/
    ezplib_get_attr_val("wl0_apcli_rule", 0, "bssid", buf, 32, EZPLIB_USE_CLI);
    strcpy(bssid, buf);
    sprintf(cmd, "iwpriv apclii0 set ApCliBssid=\'%s\'", bssid);
    system(cmd);
    printf("CMD in set_sta_bssid is %s\n", cmd);
    return T_SUCCESS;
}

/*Set STA Disallow TKIP*/
static int set_sta_disallow_tkip(int radio, int disallow)
{
    char cmd[128];
    sprintf(cmd, "iwpriv apclii0 set HtDisallowTKIP=%d", disallow);
    system(cmd);
    printf("CMD in set_sta_disallow_tkip is %s\n", cmd);
    return T_SUCCESS;
}

/*Set STA Repeater*/
static int set_sta_mac_repeater(int radio)
{
    char buf_addrmode[32];
    char buf_repeater[32];
    int addrmode;
    int mac_repeater;
    char mac_repeater_s[32];
    char addrmode_s[32];
    char cmd[128];
    char TempBuf_opmode[32];

    ezplib_get_attr_val("system_mode", 0, "name", TempBuf_opmode, 32, EZPLIB_USE_CLI);
    if(!strcmp(TempBuf_opmode, "wisp0"))
        {
            printf("MAC Repeater setting is not needed in WISP Mode,disable it.\n");
            sprintf(cmd, "iwpriv apclii0 set MACRepeaterEn=0");
            system(cmd);
            printf("CMD in set_sta_mac_repeater is %s\n", cmd);
            return T_SUCCESS;
        }

    /*Get Addr Mode*/
    ezplib_get_attr_val("wl0_apcli_rule", 0, "addrmode", buf_addrmode, 32, EZPLIB_USE_CLI);
    /*Get STA Channel From NVRAM*/
    strcpy(addrmode_s, buf_addrmode);
    addrmode = atoi(addrmode_s);

    /*Get MAC Repeater*/
    ezplib_get_attr_val("wl0_apcli_rule", 0, "macrepeater", buf_repeater, 32, EZPLIB_USE_CLI);
    /*Get STA Channel From NVRAM*/
    strcpy(mac_repeater_s, buf_repeater);
    mac_repeater = atoi(mac_repeater_s);

    if(0 == addrmode)
        {
            if(1 == mac_repeater)
                {
                    sprintf(cmd, "iwpriv apclii0 set MACRepeaterEn=%d", MAC_REPEATER_ENABLE);
                }
            else
                {
                    sprintf(cmd, "iwpriv apclii0 set MACRepeaterEn=%d", MAC_REPEATER_DISABLE);
                }
        }
    else
        {
            sprintf(cmd, "iwpriv apclii0 set MACRepeaterEn=%d", MAC_REPEATER_DISABLE);
        }

    system(cmd);
    printf("CMD in set_sta_mac_repeater is %s\n", cmd);
    return T_SUCCESS;
}
//#if 0
static int create_wpa_supplicant_conf(int radio)
{
    FILE *fp;
    char buf_ssid[33];
    char ssid[33];
    char buf_secmode[32];
    char secmode[32];
    char buf_wpacrypto[32];
    char wpacrypto[32];
    char buf_wpaauth[32];
    char wpaauth[32];
    char buf_wpauser[32];
    char wpauser[32];
    char buf_wpapasswd[64];
    char wpapasswd[64];
    char proto[32];
    char pairwise[32];
    char eap[32];
    char cmd[128];
    char buffer[11][128];//Modified by Mario Huang, 64 is not enough for shared key string
    char buffer_write[128];
    int size, i;
    /*Get STA SSID From NVRAM*/
    ezplib_get_attr_val("wl0_apcli_rule", 0, "ssid", buf_ssid, 33, EZPLIB_USE_CLI);
    strcpy(ssid, buf_ssid);

    ezplib_get_attr_val("wl0_apcli_rule", 0, "secmode", buf_secmode, 32, EZPLIB_USE_CLI); 
    strcpy(secmode, buf_secmode);

    ezplib_get_attr_val("wl0_wpa_auth_rule", 0, "wpa_crypto", buf_wpacrypto, 32, EZPLIB_USE_CLI); 
    strcpy(wpacrypto, buf_wpacrypto);

    ezplib_get_attr_val("wl0_wpa_auth_rule", 0, "wpa_auth", buf_wpaauth, 32, EZPLIB_USE_CLI); 
    strcpy(wpaauth, buf_wpaauth);

    ezplib_get_attr_val("wl0_wpa_auth_rule", 0, "wpa_user", buf_wpauser, 32, EZPLIB_USE_CLI); 
    strcpy(wpauser, buf_wpauser);

    ezplib_get_attr_val("wl0_wpa_auth_rule", 0, "wpa_passwd", buf_wpapasswd, 64, EZPLIB_USE_CLI); 
    strcpy(wpapasswd, buf_wpapasswd);

    if(!strcmp(secmode, "wpa"))
        {
            sprintf(proto, "WPA");
        }
    else
        {
            sprintf(proto, "RSN");
        }

    if(!strcmp(wpacrypto, "aes"))
        {
            sprintf(pairwise, "CCMP");
        }
    else
        {
            sprintf(pairwise, "TKIP");
        }

    if(!strcmp(wpaauth, "1"))
        {
            sprintf(eap, "PEAP");
        }
    else
        {
            sprintf(eap, "TTLS");
        }

    sprintf(cmd, "rm -rf /var/run/wpa_supplicant.conf");
    system(cmd);
    memset(cmd, 0, sizeof(cmd));
 
    sprintf(cmd, "touch /var/run/wpa_supplicant.conf");
    system(cmd);
    memset(cmd, 0, sizeof(cmd));
	
    if(!( fp = fopen("/var/run/wpa_supplicant.conf", "wr")))
        {
            printf("Open wpa_supplicant.conf error\n");
            return T_FAILURE;
        }

    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer[0], "network={\n");
    sprintf(buffer[1], "	ssid=\"%s\"\n", ssid);
    sprintf(buffer[2], "	scan_ssid=1\n");
    sprintf(buffer[3], "	key_mgmt=WPA-EAP\n");
    sprintf(buffer[4], "	proto=%s\n", proto);
    sprintf(buffer[5], "	pairwise=%s\n", pairwise);
    sprintf(buffer[6], "	eap=%s\n", eap);
    sprintf(buffer[7], "	identity=\"%s\"\n", wpauser);
    sprintf(buffer[8], "	password=\"%s\"\n", wpapasswd);
    sprintf(buffer[9], "	phase2=\"auth=MSCHAPV2\"\n");
    sprintf(buffer[10], "}");
    for(i=0; i<11; i++)
        {
            memset(buffer_write, 0, sizeof(buffer_write));
            strcpy(buffer_write, buffer[i]);
            printf("buffer_write:%s\n",buffer_write);
            size = fwrite(buffer_write, strlen(buffer_write), 1, fp);
            if (size < 1) 
                {
                    printf("Write wpa_supplicant.conf error\n");
                }   
        }
    fclose(fp);
    return T_SUCCESS;
}

static int start_apcli_apd_daemon(int radio)
{
    char cmd[128] = {0};
    int secmode;
    char TempBuf_opmode[32] = {0};
    char apcli_br[16] = {0};
    /*Get the operation mode*/
    ezplib_get_attr_val("system_mode", 0, "name", TempBuf_opmode, 32, EZPLIB_USE_CLI);
    if(!strcmp(TempBuf_opmode, "sta0"))
        {
           sprintf(apcli_br, "br-lan0");
        }
    else if(!strcmp(TempBuf_opmode, "wisp0"))
        {
           sprintf(apcli_br, "br-wan0");
        }
    else
        {
           printf("ERROR:This operation mode is not supported to start apcli apd daemon\n"); 
           return T_FAILURE;
        }  
     /*Get security mode of station*/
    secmode = get_sta_secmode(radio);
    /*Kill the old daemon first*/
    sprintf(cmd, "killall wpa_supplicant");
    printf("CMD in start_apcli_apd_daemon is %s\n", cmd);
    system(cmd);
    memset(cmd, 0, sizeof(cmd));

    /*Set encryp_type according to security mode and crypto*/
    switch(secmode)
        {
            case SECMODE_WPA:
            case SECMODE_WPA2:
                sprintf(cmd, "/usr/sbin/wpa_supplicant  -iapclii0 -b %s -c /var/run/wpa_supplicant.conf  -Dralink -B", apcli_br);
                break;
            default:
                printf("ERROR:Apd daemon is only used for WPA or WPA2\n");
                return T_FAILURE;
        }

    printf("%s\n", cmd);
    system(cmd);
    return T_SUCCESS;

}

static int kill_apcli_apd_daemon(int radio)
{
    char cmd[128];
    sprintf(cmd, "killall wpa_supplicant");
    printf("CMD in kill_apcli_apd_daemon is %s\n", cmd);
    system(cmd);
    return T_SUCCESS;

}

static int set_sta_authmode(int radio)
{
    char cmd[128];
    int secmode;
    char encmode[32];
    char buf[32]; 
    /*Get security mode of station*/
    secmode = get_sta_secmode(radio);

    /*Set authmode according to security mode*/
    switch(secmode)
        {
            case SECMODE_OPEN:
                sprintf(cmd, "iwpriv apclii0 set ApCliAuthMode=OPEN");
                break;
            case SECMODE_WEP:
                if(RADIO_2G == radio)
                    {
                        ezplib_get_attr_val("wl0_apcli_sec_wep_rule", 0, "encmode", buf, 32, EZPLIB_USE_CLI); 
                    }
                else
                    {
                        ezplib_get_attr_val("wl1_apcli_sec_wep_rule", 0, "encmode", buf, 32, EZPLIB_USE_CLI); 
                    }
                strcpy(encmode, buf);
                if(!strcmp(encmode, "shared"))
                    {
                        sprintf(cmd, "iwpriv apclii0 set ApCliAuthMode=SHARED");
                    }
                else
                    {
                        sprintf(cmd, "iwpriv apclii0 set ApCliAuthMode=OPEN");
                    }
                break;
            case SECMODE_WPA:
                sprintf(cmd, "iwpriv apclii0 set ApCliAuthMode=WPA");
                break;
            case SECMODE_WPA2:
                sprintf(cmd, "iwpriv apclii0 set ApCliAuthMode=WPA2");
                break;
            case SECMODE_PSK:
                sprintf(cmd, "iwpriv apclii0 set ApCliAuthMode=WPAPSK");
                break;
            case SECMODE_PSK2:
                sprintf(cmd, "iwpriv apclii0 set ApCliAuthMode=WPA2PSK");
        }
    printf("CMD in set_sta_authmode is %s\n", cmd);
    system(cmd);
    return T_SUCCESS;
}

static int set_sta_encryp_type(int radio)
{
    char cmd[128];
    int secmode;
    char buf[32]; 
    char crypto[32];
     /*Get security mode of station*/
    secmode = get_sta_secmode(radio);

  
    /*Set encryp_type according to security mode and crypto*/
    switch(secmode)
        {
            case SECMODE_OPEN:
                sprintf(cmd, "iwpriv apclii0 set ApCliEncrypType=NONE");
                break;
            case SECMODE_WEP:
                sprintf(cmd, "iwpriv apclii0 set ApCliEncrypType=WEP");
                break;
            case SECMODE_PSK:
                /*Get crypto*/
                if(RADIO_2G == radio)
                    {
                        ezplib_get_attr_val("wl0_apcli_sec_wpa_rule", 0, "crypto", buf, 32, EZPLIB_USE_CLI); 
                    }
                else
                    {
                        ezplib_get_attr_val("wl1_apcli_sec_wpa_rule", 0, "crypto", buf, 32, EZPLIB_USE_CLI); 
                    }
                strcpy(crypto, buf);

                if(!strcmp(crypto, "aes"))
                    {
                        sprintf(cmd, "iwpriv apclii0 set ApCliEncrypType=AES");
                    }
                else
                    {
                        sprintf(cmd, "iwpriv apclii0 set ApCliEncrypType=TKIP");
                    }
                break;
            case SECMODE_PSK2:
                /*Get crypto*/
                if(RADIO_2G == radio)
                    {
                        ezplib_get_attr_val("wl0_apcli_sec_wpa2_rule", 0, "crypto", buf, 32, EZPLIB_USE_CLI); 
                    }
                else
                    {
                        ezplib_get_attr_val("wl1_apcli_sec_wpa2_rule", 0, "crypto", buf, 32, EZPLIB_USE_CLI); 
                    }
                strcpy(crypto, buf);

                if(!strcmp(crypto, "aes"))
                    {
                        sprintf(cmd, "iwpriv apclii0 set ApCliEncrypType=AES");
                    }
                else
                    {
                        sprintf(cmd, "iwpriv apclii0 set ApCliEncrypType=TKIP");
                    }
                break;
            case SECMODE_WPA:
            case SECMODE_WPA2:
                /*Get crypto*/
                if(RADIO_2G == radio)
                    {
                        ezplib_get_attr_val("wl0_wpa_auth_rule", 0, "wpa_crypto", buf, 32, EZPLIB_USE_CLI); 
                    }
                else
                    {
                        ezplib_get_attr_val("wl1_wpa_auth_rule", 0, "wpa_crypto", buf, 32, EZPLIB_USE_CLI); 
                    }
                strcpy(crypto, buf);

                if(!strcmp(crypto, "aes"))
                    {
                        sprintf(cmd, "iwpriv apclii0 set ApCliEncrypType=AES");
                    }
                else
                    {
                        sprintf(cmd, "iwpriv apclii0 set ApCliEncrypType=TKIP");
                    }
                break;

        }

    printf("CMD in set_sta_encryp_type is %s\n", cmd);
    system(cmd);
    return T_SUCCESS;
}

static int set_sta_wpa_psk(int radio)
{
    char key[256] = {0};
    char buf[256] = {0};
    char cmd[512];
    int secmode;
	int i = 0;
	int j = 0;
    /*Get security mode of station*/
    secmode = get_sta_secmode(radio);

    /*Set wpa psk according to security mode and crypto*/
    switch(secmode)
        {
            /*Deleted by Mario Huang, WPAPSK is not need in 802.1X mode*/
            //case SECMODE_WPA:
            case SECMODE_PSK:
                /*Get wpa psk*/
                if(RADIO_2G == radio)
                    {
                        ezplib_get_attr_val("wl0_apcli_sec_wpa_rule", 0, "key", buf, 256, EZPLIB_USE_CLI); 
                    }
                else
                    {
                        ezplib_get_attr_val("wl1_apcli_sec_wpa_rule", 0, "key", buf, 256, EZPLIB_USE_CLI); 
                    }
				
				for(i=0,j=0;j<strlen(buf);i++,j++) {
					if( '\"' == buf[j] 
					|| '`' == buf[j]
					|| '\\' == buf[j]
					) {
					key[i] = '\\';
					i++;
					}
					key[i] = buf[j];
				}
				
                sprintf(cmd, "iwpriv apclii0 set ApCliWPAPSK=\"%s\"", key);
                break;
            /*Deleted by Mario Huang, WPAPSK is not need in 802.1X mode*/
            //case SECMODE_WPA2:
            case SECMODE_PSK2:
                /*Get wpa psk*/
                if(RADIO_2G == radio)
                    {
                        ezplib_get_attr_val("wl0_apcli_sec_wpa2_rule", 0, "key", buf, 256, EZPLIB_USE_CLI); 
                    }
                else
                    {
                        ezplib_get_attr_val("wl1_apcli_sec_wpa2_rule", 0, "key", buf, 256, EZPLIB_USE_CLI); 
                    }

				for(i=0,j=0;j<strlen(buf);i++,j++) {
					if( '\"' == buf[j] 
					|| '`' == buf[j]
					|| '\\' == buf[j]
					) {
					key[i] = '\\';
					i++;
					}
					key[i] = buf[j];
				}
				
                sprintf(cmd, "iwpriv apclii0 set ApCliWPAPSK=\"%s\"", key);
                break;
            default:
                return T_SUCCESS;
        }

    printf("CMD in set_sta_wpa_psk is %s\n", cmd);
    system(cmd);
    return T_SUCCESS;
}
static int set_sta_default_keyid(int radio)
{
    char keyid_s[32];
    int keyid;
    int secmode;
    char buf[32];
    char cmd[128];
    /*Get security mode of station*/
    secmode = get_sta_secmode(radio);
    /*set sta default keyid only in WEP mode. If is not wep, return success*/
    if(secmode != SECMODE_WEP)
        {
            return T_SUCCESS;
        }

    /*Get key id from NVRAM first*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl0_apcli_sec_wep_rule", 0, "key_index", buf, 32, EZPLIB_USE_CLI); 
        }
    else
        {
            ezplib_get_attr_val("wl1_apcli_sec_wep_rule", 0, "key_index", buf, 32, EZPLIB_USE_CLI); 
        }
    strcpy(keyid_s, buf);
    if(!strcmp(keyid_s, ""))
        {
            keyid = 1;
        }
    else
        {
            keyid = atoi(keyid_s);
        }
    sprintf(cmd, "iwpriv apclii0 set ApCliDefaultKeyID=%d", keyid);
    system(cmd);
    printf("CMD in set_sta_default_keyid is %s\n", cmd);
    return T_SUCCESS;
}

static int set_sta_wep_key(int radio)
{
    char keyid_s[32];
    char buf_keyid[32];
    int keyid;
    char keyid_name[32];
    char buf_key[65] = {0};
    char key[65] = {0};
    char cmd[128];
    int secmode;
	int i = 0;
	int j = 0;
    /*Get security mode of station*/
    secmode = get_sta_secmode(radio);

    /*If is not wep, return success*/
    if(secmode != SECMODE_WEP)
        {
            return T_SUCCESS;
        }

    /*Get key id from NVRAM first*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl0_apcli_sec_wep_rule", 0, "key_index", buf_keyid, 32, EZPLIB_USE_CLI); 
        }
    else
        {
            ezplib_get_attr_val("wl1_apcli_sec_wep_rule", 0, "key_index", buf_keyid, 32, EZPLIB_USE_CLI); 
        }
    printf("keyid is %s\n",buf_keyid);
    strcpy(keyid_s, buf_keyid);
    if(!strcmp(keyid_s, ""))
        {
            keyid = 1;
        }
    else
        {
            keyid = atoi(keyid_s);
        }
    /*Construct key id name*/
    sprintf(keyid_name, "key%d", keyid);

    /*Get key from NVRAM first*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl0_apcli_sec_wep_rule", 0, keyid_name, buf_key, 65, EZPLIB_USE_CLI); 
        }
    else
        {
            ezplib_get_attr_val("wl1_apcli_sec_wep_rule", 0, keyid_name, buf_key, 65, EZPLIB_USE_CLI); 
        }
    //strcpy(key, buf_key);
	for(i=0,j=0;j<strlen(buf_key);i++,j++) {
		if( '\"' == buf_key[j] || '`' == buf_key[j]	|| '\\' == buf_key[j]) {
					key[i] = '\\';
					i++;
		}
		key[i] = buf_key[j];
	}
	
    sprintf(cmd, "iwpriv apclii0 set ApCliDefaultKeyID=%d", keyid);
    system(cmd);

    sprintf(cmd, "iwpriv apclii0 set ApCliKey%d=\"%s\"", keyid, key);
    system(cmd);
    printf("CMD in set_sta_wep_key is %s\n", cmd);
    return T_SUCCESS;
}

static int set_sta_addr_mode(int radio)
{
    char cmd[128];
    char TempBuf_opmode[32];
    char buf[32];
    char addrmode[32];
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl0_apcli_rule", 0, "addrmode", buf, 32, EZPLIB_USE_CLI);
        }
    else
        {
            ezplib_get_attr_val("wl1_apcli_rule", 0, "addrmode", buf, 32, EZPLIB_USE_CLI);
        }
    strcpy(addrmode, buf);

    if(RADIO_2G == radio)
	{
	    ezplib_get_attr_val("system_mode", 0, "name", TempBuf_opmode, 32, EZPLIB_USE_CLI);
	}
    if(!strcmp(TempBuf_opmode, "wisp0"))
        {
            strcpy(addrmode, "0");
        }

    sprintf(cmd, "iwpriv apclii0 set apcliAddrMode=\'%s\'", addrmode);
    system(cmd);
    printf("CMD in set_sta_addr_mode is %s\n", cmd);
    return T_SUCCESS;
}

static int set_sta_enable(int radio, int enable)
{
    char cmd[128];
    sprintf(cmd, "iwpriv apclii0 set ApCliEnable=%d", enable);
    system(cmd);
    printf("CMD in set_sta_enable is %s\n", cmd);
    return T_SUCCESS;
}

/*Get STA Current BSSID*/
int get_sta_assoc_bssid(int radio, char *bssid)
{
    char cmd[128];
    FILE *fin;
    char str[255],c;
    //char bssid[32];
    char ptn[]="Access Point:";
    sprintf(cmd, "iwconfig apclii0 | grep 'Access Point:' > /tmp/sta_bssid");
    system(cmd);

    fin = fopen("/tmp/sta_bssid","r");
    while ((c=fgetc(fin)) != EOF){
        ungetc(c,fin);		
        readline(str,fin);
        //printf("str is %s\n",str);
        if (get_sta_bssid(bssid, str, ptn) == 0)
            break;
    }
    fclose(fin);
    system("rm -f /tmp/sta_bssid");	
    //printf("Current Bssid is %s\n",bssid);
    /*Get STA BSSID From NVRAM*/
    return T_SUCCESS;
}

/*Added By Andy Yu in 2013/10/11: Get STA RSSI */
int get_sta_assoc_rssi(int radio, char *rssi)
{
	FILE *fp;
	char tmpRssi1[16] = {0};
	char tmpRssi2[16] = {0};
	int Rssi1 = 0;
	int Rssi2 = 0;
	
	system("iwpriv apclii0 stat | grep RSSI | awk '{print $3, $4}' > /tmp/rssi.dat");
	if (NULL == (fp = fopen("/tmp/rssi.dat", "r"))) {
		return T_FAILURE;
	} else {
		memset(tmpRssi1, 0x00, sizeof(tmpRssi1));
		memset(tmpRssi2, 0x00, sizeof(tmpRssi2));
		fscanf(fp, "%s%s", tmpRssi1, tmpRssi2);
		
		delspace(tmpRssi1, &tmpRssi1[0]);
		Rssi1 = atoi(tmpRssi1);
		
		delspace(tmpRssi2, &tmpRssi2[0]);
		Rssi2 = atoi(tmpRssi2);
		
		assert(NULL!=rssi);
		if (Rssi1 == 0 && Rssi2 == 0) {
			sprintf(rssi, "--/-- dBm");
		} else if (Rssi1 == 0) {
			sprintf(rssi, "--/%d dBm", Rssi2);
		} else if (Rssi2 == 0) {
			sprintf(rssi, "%d/-- dBm", Rssi1);                       
		} else {
			sprintf(rssi, "%d/%d dBm", Rssi1, Rssi2);            
		} 

	}

	fclose(fp);
	return T_SUCCESS;
}

//Added By Andy Yu in 2013/10/12: Get Ethernet Client List
int get_ethernet_client_list(int radio, ETHERNET_CLIENT_LIST *client_list)
{
	int ret = 0;
	char tmp_buf[64] = { 0 };
	char mode[10] = { 0 };
	char addr_mode[10] = { 0 };
	char macrepeater_enable[10] = { 0 };

	ezplib_get_attr_val("wl0_apcli_rule", 0, "macrepeater", macrepeater_enable, 10, EZPLIB_USE_CLI);
	ezplib_get_attr_val("wl0_apcli_rule", 0, "addrmode", addr_mode, 10, EZPLIB_USE_CLI);
	//ezplib_get_attr_val("wl_mode_rule", 0, "mode", mode, 10, EZPLIB_USE_CLI);
	ezplib_get_attr_val("system_mode", 0, "name", mode, 10, EZPLIB_USE_CLI);
	printf("### %s %s %s ...................................................................\n", mode, addr_mode,
	       macrepeater_enable);

	/* Get br-lan0 MAC */
	ret = get_lan_mac(tmp_buf);
	printf("++++++++++%s+++++++++++\n", tmp_buf);
	if (T_FAILURE == ret) {
		printf("ERROR: Get LAN MAC Error\n");
		return T_FAILURE;
	}
	
	if ((0 == strcmp(mode, "ur")|| 0 == strcmp(mode,"sta0")) && 0 == strcmp(addr_mode, "0") && 0 == strcmp(macrepeater_enable, "1")) {
		int i = 0;
		FILE *fp1 = NULL;
		char buff1[20] = { 0 };
		client_list->client_num = 0;

		/*Chged By Andy Yu in 2013/10/21: Ethernet Client List iwpriv rai0 stat->iwpriv rai0 eth_cli*/
		system("iwpriv rai0 eth_cli > /tmp/stat.dat ");
		system("awk 'n==1{print} $0~/MIMOPS/{n=1}'  /tmp/stat.dat > /tmp/tmp.dat");
		system("awk '{print $1}' /tmp/tmp.dat > /tmp/macrepeater.dat ");

		if (NULL == (fp1 = fopen("/tmp/macrepeater.dat", "r"))) {
			return T_FAILURE;
		}

		while (!feof(fp1)) {
			ret = fscanf(fp1, "%s", buff1);
			if ( ret == EOF) {
				printf("fsacnf End\n");
				break;
			}

			if ((buff1[2] != ':') || (buff1[5] != ':') || (buff1[8] != ':') || (buff1[11] != ':')
			    || (buff1[14] != ':') || (!strncmp(buff1, tmp_buf, 17))) {
				continue;
			}

			memset(client_list->client[i].macaddr, 0x00, sizeof(client_list->client[i].macaddr));
			strcpy(client_list->client[i].macaddr, buff1);
			printf("++++++++client_list->client[i].macaddr = %s\n", client_list->client[i].macaddr);
			memset(buff1, 0, 20);
			i++;
		}
		client_list->client_num = i;
		printf("client_list->client_num = %d+++++\n", client_list->client_num);
		fclose(fp1);
		system("rm  /tmp/tmp.dat");
		system("rm  /tmp/macrepeater.dat");
		return T_SUCCESS;

	} else {
		int i = 0;
		int j = 0;
		FILE *fp = NULL;
		char buff1[20] = { 0 };
		char buff2[64] = { 0 };
		client_list->client_num = 0;

		/*Chged By Andy Yu in 2013/10/21: Ethernet Client List iwpriv rai0 stat->iwpriv rai0 eth_cli*/
		system("iwpriv rai0 eth_cli > /tmp/stat.dat ");
		system("awk 'n==1{print} $0~/MAT information:/{n=1}'  /tmp/stat.dat > /tmp/tmp.dat");

		if (NULL == (fp = fopen("/tmp/tmp.dat", "r"))) {
			return T_FAILURE;
		}

		while (!feof(fp)) {
			ret = fscanf(fp, "%d%s%s", &j, buff1, buff2);
			if (j > 128) {
				printf("Client Max Number is 128\n");
				break;
			}
			if ( ret == EOF) {
				printf("fsacnf End\n");
				break;
			}
			
			if ((buff1[2] != ':') || (buff1[5] != ':') || (buff1[8] != ':') || (buff1[11] != ':')
			    || (buff1[14] != ':') || (!strncmp(buff1, tmp_buf, 17))) {
				continue;
			}

			memset(client_list->client[i].macaddr, 0x00, sizeof(client_list->client[i].macaddr));
			memset(client_list->client[i].ipaddr, 0x00, sizeof(client_list->client[i].ipaddr));
			strcpy(client_list->client[i].macaddr, buff1);
			strcpy(client_list->client[i].ipaddr, buff2);
			printf("++++++++client_list->client[i].macaddr = %s\n+++client_list->client[i].ipaddr = %s\n", 
				client_list->client[i].macaddr, client_list->client[i].ipaddr);
			memset(buff1, 0, 20);
			memset(buff2, 0, 64);
			i++;
		}
		client_list->client_num = i;
		printf("client_list->client_num = %d++++++++\n", client_list->client_num);
		fclose(fp);
		system("rm  /tmp/tmp.dat");
		return T_SUCCESS;
	}
}

/*Get STA Connected AP Informations*/
int get_sta_assoc_ap_info(int radio, AP_INFO *ap_info)
{
    char cmd[128];
    FILE *fp = NULL;
    int Rssi1 = 0;
    int Rssi2 = 0;
    char TMPrssi1[16] = {0}, TMPrssi2[16] = {0}; 
    char tmpBSSID[28] = {0}, tmpSSID[64] = {0}, tmpRSSI1[16] = {0}, tmpRSSI2[16] = {0}, tmpAuth[32] = {0}; 
    char ap_info_buf[128] = {0}, auth_mode_get[32] = {0};
    	
    memset(cmd, 0, sizeof(cmd));
    /*Carry out site survey, restore the result to /tmp/site_survey_middleware.dat file*/	
    sprintf(cmd, "iwpriv apclii0 assoc_ap_info | grep -r 1 > /tmp/connected_ap.dat");
    system(cmd);

	if (NULL == (fp = fopen("/tmp/connected_ap.dat","r")))
	{
		printf("Connected AP is NONE\n");
	}
	else
	{
		fgets(ap_info_buf, 128, fp);

		snprintf(tmpSSID, 35, "%s", ap_info_buf+10);
		printf("tmpSSID is %s\n", tmpSSID);
		delspace2(tmpSSID, ap_info->ssid);

		snprintf(tmpBSSID, 20, "%s", ap_info_buf+45);
		printf("tmpBSSID is %s\n", tmpBSSID);
		delspace_bssid(tmpBSSID, ap_info->bssid);


		snprintf(tmpAuth, 24, "%s", ap_info_buf+65);
		printf("tmpAuth is %s\n", tmpAuth);
		delspace(tmpAuth, auth_mode_get);

		snprintf(tmpRSSI1, 3, "%s", ap_info_buf+89);
		printf("tempRSSI1:%s\n",tmpRSSI1);
		delspace(tmpRSSI1, &TMPrssi1[0]);
		Rssi1 = atoi(TMPrssi1);

		snprintf(tmpRSSI2, 3, "%s", ap_info_buf+97);
		printf("tempRSSI2:%s\n",tmpRSSI2);
		delspace(tmpRSSI2, &TMPrssi2[0]);
		Rssi2 = atoi(TMPrssi2);

		assert(NULL!=ap_info->rssi);
		if (Rssi1 == 0 && Rssi2 == 0) {
			sprintf(ap_info->rssi, "--/-- dBm");
		} else if (Rssi1 == 0) {
			sprintf(ap_info->rssi, "--/%d dBm", Rssi2);
		} else if (Rssi2 == 0) {
			sprintf(ap_info->rssi, "%d/-- dBm", Rssi1);                       
		} else {
			sprintf(ap_info->rssi, "%d/%d dBm", Rssi1, Rssi2);            
		} 



		if(!strcmp(auth_mode_get, "WPAPSK/AES")){
			ap_info->auth_mode = STA_AUTHMODE_WPAPSK;
		}else if(!strcmp(auth_mode_get, "WPAPSK/TKIP")){
			ap_info->auth_mode = STA_AUTHMODE_WPAPSK;
		}else if(!strcmp(auth_mode_get, "WPA1PSKWPA2PSK/TKIPAES")){
			ap_info->auth_mode = STA_AUTHMODE_WPA1PSKWPA2PSK;
		}else if(!strcmp(auth_mode_get, "WPA2PSK/AES")){
			ap_info->auth_mode = STA_AUTHMODE_WPA2PSK;
		}else if(!strcmp(auth_mode_get, "WPA2PSK/TKIP")){
			ap_info->auth_mode = STA_AUTHMODE_WPA2PSK;
		}else if(!strcmp(auth_mode_get, "WEP")){
			ap_info->auth_mode = STA_AUTHMODE_WEP;
		}else if(!strcmp(auth_mode_get, "NONE")){
			ap_info->auth_mode = STA_AUTHMODE_NONE;
		}else if(!strcmp(auth_mode_get, "WPA1PSKWPA2PSK/AES")){
			ap_info->auth_mode = STA_AUTHMODE_WPA1PSKWPA2PSK;
		}else if(!strcmp(auth_mode_get, "WPA1PSKWPA2PSK/TKIP")){
			ap_info->auth_mode = STA_AUTHMODE_WPA1PSKWPA2PSK;
		}else if(!strcmp(auth_mode_get, "WPA/TKIP")){
			ap_info->auth_mode = STA_AUTHMODE_WPA;
		}else if(!strcmp(auth_mode_get, "WPA/AES")){
			ap_info->auth_mode = STA_AUTHMODE_WPA;
		}else if(!strcmp(auth_mode_get, "WPA/TKIPAES")){
			ap_info->auth_mode = STA_AUTHMODE_WPA;
		}else if(!strcmp(auth_mode_get, "WPA2/TKIP")){
			ap_info->auth_mode = STA_AUTHMODE_WPA2;
		}else if(!strcmp(auth_mode_get, "WPA2/AES")){
			ap_info->auth_mode = STA_AUTHMODE_WPA2;
		}else if(!strcmp(auth_mode_get, "WPA2/TKIPAES")){
			ap_info->auth_mode = STA_AUTHMODE_WPA2;
		}else if(!strcmp(auth_mode_get, "WPA1WPA2/TKIPAES")){
			ap_info->auth_mode = STA_AUTHMODE_WPA1WPA2;
		}else if(!strcmp(auth_mode_get, "WPA2PSK/TKIPAES")){
			ap_info->auth_mode = STA_AUTHMODE_WPA2PSK;
		}else if(!strcmp(auth_mode_get, "WPA1WPA2/AES")){
			ap_info->auth_mode = STA_AUTHMODE_WPA1WPA2;
		}else if(!strcmp(auth_mode_get, "WPA1WPA2/TKIP")){
			ap_info->auth_mode = STA_AUTHMODE_WPA1WPA2;
		}else if(!strcmp(auth_mode_get, "WPAPSK/TKIPAES")){
			ap_info->auth_mode = STA_AUTHMODE_WPAPSK;
		}else{
			printf("ERROR:Can not get auth mode,%s\n",auth_mode_get);
			fclose(fp);
			return T_FAILURE;
		}

	}

    fclose(fp);
    system("rm -rf /tmp/connected_ap.dat");
    return T_SUCCESS;
}


/*Get STA Current SSID*/
//int get_sta_connect_ssid(int radio, char *ssid)
int get_sta_assoc_ssid(int radio, char *ssid)
{
    char cmd[128];
    FILE *fin = NULL;
    char str[255],c;
    //char bssid[32];
    char ptn[]="ESSID:";
    sprintf(cmd, "iwconfig apclii0 | grep 'ESSID:' > /tmp/sta_ssid");
    system(cmd);

    fin = fopen("/tmp/sta_ssid","r");
    while ((c=fgetc(fin)) != EOF){
        ungetc(c,fin);		
        readline(str,fin);
        //printf("str is %s\n",str);
        if (get_sta_ssid(ssid, str, ptn) == 0)
            break;
    }
    fclose(fin);
    system("rm -f /tmp/sta_ssid");	
    //printf("Current Ssid is %s\n",ssid);
    /*Get STA BSSID From NVRAM*/
    return T_SUCCESS;
}

/*Get STA Connected AP Wireless Mode*/
int get_sta_assoc_wmode(int radio, char *wmode)
{
    char cmd[128] = {0};
    char bssid[32] = {0};
    char converted_bssid[32] = {0};
    FILE *fp = NULL;
    char *pBssidList = (char*)malloc(256 * sizeof (char));
    char tmpBSSID[28], tmpWMODE[16];
    char bssid_get[28], wireless_mode_get[16];

    get_sta_assoc_bssid(radio, bssid);
    //printf("bssid is %s\n", bssid);
    /*Set apcli site survey as enable*/	
    sprintf(cmd, "iwpriv apclii0 set SiteSurvey=1");
    system(cmd);	
    memset(cmd, 0, sizeof(cmd));
    /*Carry out site survey, restore the result to /tmp/site_survey_middleware.dat file*/	
    convert_bssid(bssid, converted_bssid);
    sprintf(cmd, "iwpriv apclii0 get_site_survey | grep \"%s\" > /tmp/site_survey_middleware.dat", converted_bssid);
    system(cmd);	
    //printf("%s\n", cmd);
    memset(cmd, 0, sizeof(cmd));

    if (NULL == (fp = fopen(SITE_SURVEY_FILE,"r")))
        {
            printf("Ap list is NONE\n");
		free(pBssidList);
		return T_FAILURE;
        }
    else
        {
            //fgets(pBssidList, 256, fp);
            //fgets(pBssidList, 256, fp);

            while(!feof(fp)){
                memset(tmpBSSID, 0x00, sizeof(tmpBSSID));
                memset(tmpWMODE, 0x00, sizeof(tmpWMODE));
                memset(bssid_get, 0x00, sizeof(bssid_get));
                memset(wireless_mode_get, 0x00, sizeof(wireless_mode_get));

                fgets(pBssidList, 256, fp);

                snprintf(tmpBSSID, 19, "%s", pBssidList+39);
                delspace_bssid(tmpBSSID, bssid_get);
                //printf("bssid_get is %s\n", bssid_get);

                snprintf(tmpWMODE, 9, "%s", pBssidList+90);
                delspace(tmpWMODE, wireless_mode_get);
                //printf("wireless_mode_get is %s\n", wireless_mode_get);

                if(!strcmp(bssid, bssid_get))
                    {
                        strcpy(wmode, wireless_mode_get);             
                        //printf("wmode is %s\n", wmode);
                        break;
                    }
            }
        }

    free(pBssidList);
    fclose(fp);
    //sprintf(cmd, "rm -rf /tmp/site_survey_middleware.dat");
    //system(cmd);	

    return T_SUCCESS;
}

/*Get STA Connected AP Security Mode*/
//int get_sta_assoc_authmode(int radio, int *auth_mode)
int get_sta_assoc_authmode(int radio, int *auth_mode, int *enc_type)
{
    char cmd[128] = {0};
    char bssid[32] = {0};
    char converted_bssid[32] = {0};
    FILE *fp = NULL;
    char *pBssidList = (char*)malloc(256 * sizeof (char));
    char tmpBSSID[28], tmpAuth[32];
    char bssid_get[28], auth_mode_get[32];
    
    get_sta_assoc_bssid(radio, bssid);
    //printf("bssid is %s\n", bssid);
    /*Set apcli site survey as enable*/	
    sprintf(cmd, "iwpriv apclii0 set SiteSurvey=1");
    system(cmd);	
    memset(cmd, 0, sizeof(cmd));
    /*Carry out site survey, restore the result to /tmp/site_survey_middleware.dat file*/	
    convert_bssid(bssid, converted_bssid);
    sprintf(cmd, "iwpriv apclii0 get_site_survey | grep \"%s\" > /tmp/site_survey_middleware.dat", converted_bssid);
    system(cmd);
    printf("cmd is %s\n",cmd);
    system("cat /tmp/site_survey_middleware.dat");	
    //printf("%s\n", cmd);
    memset(cmd, 0, sizeof(cmd));

    if (NULL == (fp = fopen(SITE_SURVEY_FILE,"r")))
        {
            printf("Ap list is NONE\n");
		free(pBssidList);
		return T_FAILURE;
        }
    else
        {
            //fgets(pBssidList, 256, fp);
            //fgets(pBssidList, 256, fp);

            while(!feof(fp)){
                memset(tmpBSSID, 0x00, sizeof(tmpBSSID));
                memset(tmpAuth, 0x00, sizeof(tmpAuth));
                memset(bssid_get, 0x00, sizeof(bssid_get));
                memset(auth_mode_get, 0x00, sizeof(auth_mode_get));

                fgets(pBssidList, 256, fp);

                snprintf(tmpBSSID, 19, "%s", pBssidList+39);
                //printf("tmpBSSID is %s\n", tmpBSSID);
                delspace_bssid(tmpBSSID, bssid_get);
                //printf("bssid_get is %s\n", bssid_get);

		snprintf(tmpAuth, 23, "%s", pBssidList+59);
                //printf("tmpAuth is %s\n", tmpAuth);
		delspace(tmpAuth, auth_mode_get);
                //printf("auth_mode_get is %s\n", auth_mode_get);

                if(!strcmp(bssid, bssid_get))
                    {
			if(!strcmp(auth_mode_get, "WPAPSK/AES")){
				*auth_mode = STA_AUTHMODE_WPAPSK;
			}else if(!strcmp(auth_mode_get, "WPAPSK/TKIP")){
				*auth_mode = STA_AUTHMODE_WPAPSK;
			}else if(!strcmp(auth_mode_get, "WPA1PSKWPA2PSK/TKIPAES")){
				*auth_mode = STA_AUTHMODE_WPA1PSKWPA2PSK;
			}else if(!strcmp(auth_mode_get, "WPA2PSK/AES")){
				*auth_mode = STA_AUTHMODE_WPA2PSK;
			}else if(!strcmp(auth_mode_get, "WPA2PSK/TKIP")){
				*auth_mode = STA_AUTHMODE_WPA2PSK;
			}else if(!strcmp(auth_mode_get, "WEP")){
				*auth_mode = STA_AUTHMODE_WEP;
			}else if(!strcmp(auth_mode_get, "NONE")){
				*auth_mode = STA_AUTHMODE_NONE;
			}else if(!strcmp(auth_mode_get, "WPA1PSKWPA2PSK/AES")){
				*auth_mode = STA_AUTHMODE_WPA1PSKWPA2PSK;
			}else if(!strcmp(auth_mode_get, "WPA1PSKWPA2PSK/TKIP")){
				*auth_mode = STA_AUTHMODE_WPA1PSKWPA2PSK;
			}else if(!strcmp(auth_mode_get, "WPA/TKIP")){
				*auth_mode = STA_AUTHMODE_WPA;
			}else if(!strcmp(auth_mode_get, "WPA/AES")){
				*auth_mode = STA_AUTHMODE_WPA;
			}else if(!strcmp(auth_mode_get, "WPA/TKIPAES")){
				*auth_mode = STA_AUTHMODE_WPA;
			}else if(!strcmp(auth_mode_get, "WPA2/TKIP")){
				*auth_mode = STA_AUTHMODE_WPA2;
			}else if(!strcmp(auth_mode_get, "WPA2/AES")){
				*auth_mode = STA_AUTHMODE_WPA2;
			}else if(!strcmp(auth_mode_get, "WPA2/TKIPAES")){
				*auth_mode = STA_AUTHMODE_WPA2;
			}else if(!strcmp(auth_mode_get, "WPA1WPA2/TKIPAES")){
				*auth_mode = STA_AUTHMODE_WPA1WPA2;
			}else if(!strcmp(auth_mode_get, "WPA2PSK/TKIPAES")){
				*auth_mode = STA_AUTHMODE_WPA2PSK;
			}else if(!strcmp(auth_mode_get, "WPA1WPA2/AES")){
				*auth_mode = STA_AUTHMODE_WPA1WPA2;
			}else if(!strcmp(auth_mode_get, "WPA1WPA2/TKIP")){
				*auth_mode = STA_AUTHMODE_WPA1WPA2;
			}else if(!strcmp(auth_mode_get, "WPAPSK/TKIPAES")){
				*auth_mode = STA_AUTHMODE_WPAPSK;
			}else{
				printf("ERROR:Can not get auth mode,%s\n",auth_mode_get);
				free(pBssidList);
				fclose(fp);
				sprintf(cmd, "rm -rf /tmp/site_survey_middleware.dat");
				system(cmd);	
				return T_FAILURE;
			}
                        break;
                    }
            }
        }
    printf("auth_mode is %d,%s\n", *auth_mode, auth_mode_get);
    free(pBssidList);
    fclose(fp);
    //sprintf(cmd, "rm -rf /tmp/site_survey_middleware.dat");
    //system(cmd);	

    return T_SUCCESS;
}

/*Get STA Association Status*/
//int get_sta_association(int radio, int *assoc)
int get_sta_assoc_status(int radio, int *assoc)
{
    char bssid[32] = {0};
    char ptn[]="Not-AssoCiAtED";

    memset(bssid, 0, sizeof(bssid));
    get_sta_assoc_bssid(radio, bssid);
    printf("bssid in get_sta_association is %s\n", bssid);
    if(!strcmp(bssid, ptn))
        {
           *assoc = APCLI_NOT_ASSOCIATED;
        }
    else
        {
           *assoc = APCLI_ASSOCIATED;
        }
    
    //printf("Current ASSOC is %d\n", *assoc);
    /*Get STA BSSID From NVRAM*/
    return T_SUCCESS;
}
/*Set STA Security*/
static int set_sta_security(int radio)
{
    set_sta_authmode(radio);
    set_sta_encryp_type(radio);
    set_sta_wpa_psk(radio);
    set_sta_default_keyid(radio);
    set_sta_wep_key(radio);
    return T_SUCCESS;
}

int set_config_sta(int radio)
{
    int secmode;
    set_sta_ssid(radio);
    set_sta_bssid(radio);
    set_sta_mac_repeater(radio);
    set_wirelessmode_channel(radio);
    set_power(radio);
    set_sta_disallow_tkip(radio, 0);
    //set_sta_channel(radio);
    set_sta_security(radio);
    set_sta_addr_mode(radio);
    set_sta_enable(radio, STA_ENABLE);
    vap_up_down(radio, 0, WLAN_MODE_STA, VAP_UP);
    /*Get security mode of station*/
    secmode = get_sta_secmode(radio);
    if((SECMODE_WPA == secmode) || (SECMODE_WPA2 == secmode))
        {
           create_wpa_supplicant_conf(radio);
           start_apcli_apd_daemon(radio);
        }
    else
        {
           kill_apcli_apd_daemon(radio);
        }
    /*Added by Mario Huang for debug*/
    return T_SUCCESS;
}



/*****************************************************************/
/*AP SCAN                                                        */ 
/*****************************************************************/
int get_ap_list(int radio, SCAN_AP_LIST *ap_list)
{
	char cmd[128] = {0};
    int ap_id = 0;
	FILE *fp = NULL;
    int num = 0;
	char *pBssidList = (char*)malloc(65536 * sizeof (char));        
	char TMPrssi[16] = {0};
	char TMPchannel[16] = {0};
	char TMPauth[32] = {0};
	char tmpBSSID[28] = {0};
	char tmpSSID[64] = {0};
	char tmpRSSI[16] = {0};
	char tmpChannel[16] = {0};

	char tmpAuth[32] = {0};
/*	char tmpEncry[20] = {0}; mask by frank to avoid warning*/
	char tmpNetworkType[24] = {0};

	/*Chged site survey CMD by Andy Yu: 20140221*/
	sprintf(cmd, "iwpriv apclii0 set SiteSurvey=1; iwpriv apclii0 get_site_survey");
    system(cmd);	
	printf("CMD in get_ap_list is %s\n", cmd);
	
    ap_list->ap_num = 0;
	if (NULL == (fp = fopen(SITE_SURVEY_FILE,"r")))
	{
		printf("Ap list is NONE\n");
		free(pBssidList);
		return T_FAILURE;		
	}
	else
	{
		while(!feof(fp)){
			memset(tmpBSSID, 0x00, sizeof(tmpBSSID));
			memset(tmpRSSI, 0x00, sizeof(tmpRSSI));
			memset(tmpSSID, 0x00, sizeof(tmpSSID));
			memset(tmpChannel, 0x00, sizeof(tmpChannel));
			memset(tmpAuth, 0x00, sizeof(tmpAuth));
			memset(tmpNetworkType, 0x00, sizeof(tmpNetworkType));

			num++;
			if (NULL == (fgets(pBssidList, 65536, fp)))
			{
				printf("ap_list->ap_num+++++++++++++%d\n", ap_list->ap_num);
				break;
			}
                        /*Clear the memory*/
			memset(ap_list->ap[ap_id].ssid, 0x00, sizeof(ap_list->ap[ap_id].ssid));
			memset(ap_list->ap[ap_id].bssid, 0x00, sizeof(ap_list->ap[ap_id].bssid));
			memset(ap_list->ap[ap_id].rssi_str, 0x00, sizeof(ap_list->ap[ap_id].rssi_str));

			snprintf(tmpChannel, 3, "%s", pBssidList); 
			delspace(tmpChannel, &TMPchannel[0]);
			ap_list->ap[ap_id].channel = atoi(TMPchannel);
			if (ap_list->ap[ap_id].channel ==  0){
				break;
			}

			snprintf(tmpSSID, 35, "%s", pBssidList+4);
			//delspace1(tmpSSID, ap_list->ap[ap_id].ssid);
                        printf("tmpSSID is %s\n", tmpSSID);
			delspace2(tmpSSID, ap_list->ap[ap_id].ssid);

			snprintf(tmpBSSID, 19, "%s", pBssidList+39);
                        /*Print for debug*/
                        //printf("tempBSSID:%s\n",tmpBSSID);
			delspace(tmpBSSID, ap_list->ap[ap_id].bssid);


			snprintf(tmpAuth, 23, "%s", pBssidList+59);
                        /*Print for debug*/
                        printf("tempAUTH:%s\n",tmpAuth);
			delspace(tmpAuth, &TMPauth[0]);
                        printf("TMPauth:%s\n",TMPauth);

			snprintf(tmpRSSI, 9, "%s", pBssidList+82);
                        /*Print for debug*/
                        //printf("tempRSSI:%s\n",tmpRSSI);
			delspace(tmpRSSI, &TMPrssi[0]);
		    ap_list->ap[ap_id].rssi = atoi(TMPrssi);
			sprintf(ap_list->ap[ap_id].rssi_str, "%d%%", ap_list->ap[ap_id].rssi);              
			
			snprintf(tmpNetworkType, 3, "%s", pBssidList+106);
                        /*Print for debug*/
                        //printf("tmpNetworkType:%s\n",tmpNetworkType);
			if(!strcmp(TMPauth, "WPAPSK/AES")){
				//sprintf(tmpAuth, "WPAPSK");
				//sprintf(tmpEncry, "AES");	
				ap_list->ap[ap_id].auth_mode = STA_AUTHMODE_WPAPSK;
				ap_list->ap[ap_id].encry = STA_ENCRY_AES;
			}else if(!strcmp(TMPauth, "WPAPSK/TKIP")){
				//sprintf(tmpAuth, "WPAPSK");
				//sprintf(tmpEncry, "TKIP");	
				ap_list->ap[ap_id].auth_mode = STA_AUTHMODE_WPAPSK;
				ap_list->ap[ap_id].encry = STA_ENCRY_TKIP;
			}else if(!strcmp(TMPauth, "WPA1PSKWPA2PSK/TKIPAES")){
				//sprintf(tmpAuth, "WPA1PSKWPA2PSK");
				//sprintf(tmpEncry, "TKIPAES");
				ap_list->ap[ap_id].auth_mode = STA_AUTHMODE_WPA1PSKWPA2PSK;
				ap_list->ap[ap_id].encry = STA_ENCRY_TKIPAES;
			}else if(!strcmp(TMPauth, "WPA2PSK/AES")){
				//sprintf(tmpAuth, "WPA2PSK");
				//sprintf(tmpEncry, "AES");
				ap_list->ap[ap_id].auth_mode = STA_AUTHMODE_WPA2PSK;
				ap_list->ap[ap_id].encry = STA_ENCRY_AES;
			}else if(!strcmp(TMPauth, "WPA2PSK/TKIP")){
				//sprintf(tmpAuth, "WPA2PSK");
				//sprintf(tmpEncry, "TKIP");
				ap_list->ap[ap_id].auth_mode = STA_AUTHMODE_WPA2PSK;
				ap_list->ap[ap_id].encry = STA_ENCRY_TKIP;
			}else if(!strcmp(TMPauth, "WEP")){
				//sprintf(tmpAuth, "Unknown");
				//sprintf(tmpEncry, "WEP");
				ap_list->ap[ap_id].auth_mode = STA_AUTHMODE_WEP;
				ap_list->ap[ap_id].encry = STA_ENCRY_WEP;
			}else if(!strcmp(TMPauth, "NONE")){
				//sprintf(tmpAuth, "NONE");
				//sprintf(tmpEncry, "Not Use");
				ap_list->ap[ap_id].auth_mode = STA_AUTHMODE_NONE;
				ap_list->ap[ap_id].encry = STA_ENCRY_NONE;
			}else if(!strcmp(TMPauth, "WPA1PSKWPA2PSK/AES")){
				//sprintf(tmpAuth, "WPA1PSKWPA2PSK");
				//sprintf(tmpEncry, "AES");
				ap_list->ap[ap_id].auth_mode = STA_AUTHMODE_WPA1PSKWPA2PSK;
				ap_list->ap[ap_id].encry = STA_ENCRY_AES;
			}else if(!strcmp(TMPauth, "WPA1PSKWPA2PSK/TKIP")){
				//sprintf(tmpAuth, "WPA1PSKWPA2PSK");
				//sprintf(tmpEncry, "TKIP");
				ap_list->ap[ap_id].auth_mode = STA_AUTHMODE_WPA1PSKWPA2PSK;
				ap_list->ap[ap_id].encry = STA_ENCRY_TKIP;
			}else if(!strcmp(TMPauth, "WPA/TKIP")){
				//sprintf(tmpAuth, "WPA");
				//sprintf(tmpEncry, "TKIP");
				ap_list->ap[ap_id].auth_mode = STA_AUTHMODE_WPA;
				ap_list->ap[ap_id].encry = STA_ENCRY_TKIP;
			}else if(!strcmp(TMPauth, "WPA/AES")){
				//sprintf(tmpAuth, "WPA");
				//sprintf(tmpEncry, "AES");
				ap_list->ap[ap_id].auth_mode = STA_AUTHMODE_WPA;
				ap_list->ap[ap_id].encry = STA_ENCRY_AES;
			}else if(!strcmp(TMPauth, "WPA/TKIPAES")){
				//sprintf(tmpAuth, "WPA");
				//sprintf(tmpEncry, "TKIPAES");
				ap_list->ap[ap_id].auth_mode = STA_AUTHMODE_WPA;
				ap_list->ap[ap_id].encry = STA_ENCRY_TKIPAES;
			}else if(!strcmp(TMPauth, "WPA2/TKIP")){
				//sprintf(tmpAuth, "WPA2");
				//sprintf(tmpEncry, "TKIP");
				ap_list->ap[ap_id].auth_mode = STA_AUTHMODE_WPA2;
				ap_list->ap[ap_id].encry = STA_ENCRY_TKIP;
			}else if(!strcmp(TMPauth, "WPA2/AES")){
				//sprintf(tmpAuth, "WPA2");
				//sprintf(tmpEncry, "AES");
				ap_list->ap[ap_id].auth_mode = STA_AUTHMODE_WPA2;
				ap_list->ap[ap_id].encry = STA_ENCRY_AES;
			}else if(!strcmp(TMPauth, "WPA2/TKIPAES")){
				//sprintf(tmpAuth, "WPA2");
				//sprintf(tmpEncry, "TKIPAES");
				ap_list->ap[ap_id].auth_mode = STA_AUTHMODE_WPA2;
				ap_list->ap[ap_id].encry = STA_ENCRY_TKIPAES;
			}else if(!strcmp(TMPauth, "WPA1WPA2/TKIPAES")){
				//sprintf(tmpAuth, "WPA1WPA2");
				//sprintf(tmpEncry, "TKIPAES");
				ap_list->ap[ap_id].auth_mode = STA_AUTHMODE_WPA1WPA2;
				ap_list->ap[ap_id].encry = STA_ENCRY_TKIPAES;
			}else if(!strcmp(TMPauth, "WPA2PSK/TKIPAES")){
				//sprintf(tmpAuth, "WPA2PSK");
				//sprintf(tmpEncry, "TKIPAES");
				ap_list->ap[ap_id].auth_mode = STA_AUTHMODE_WPA2PSK;
				ap_list->ap[ap_id].encry = STA_ENCRY_TKIPAES;
			}else if(!strcmp(TMPauth, "WPA1WPA2/AES")){
				//sprintf(tmpAuth, "WPA1WPA2");
				//sprintf(tmpEncry, "AES");
				ap_list->ap[ap_id].auth_mode = STA_AUTHMODE_WPA1WPA2;
				ap_list->ap[ap_id].encry = STA_ENCRY_AES;
			}else if(!strcmp(TMPauth, "WPA1WPA2/TKIP")){
				//sprintf(tmpAuth, "WPA1WPA2");
				//sprintf(tmpEncry, "TKIP");
				ap_list->ap[ap_id].auth_mode = STA_AUTHMODE_WPA1WPA2;
				ap_list->ap[ap_id].encry = STA_ENCRY_TKIP;
			}else if(!strcmp(TMPauth, "WPAPSK/TKIPAES")){
				//sprintf(tmpAuth, "WPAPSK");
				//sprintf(tmpEncry, "TKIPAES");
				ap_list->ap[ap_id].auth_mode = STA_AUTHMODE_WPAPSK;
				ap_list->ap[ap_id].encry = STA_ENCRY_TKIPAES;
			}
			//strcpy(radiocheck, "");

			if(!strcmp(tmpNetworkType, "In")){
				ap_list->ap[ap_id].network_type = NETWORK_TYPE_IN;
			}else if (!strcmp(tmpNetworkType, "Ad")){
				ap_list->ap[ap_id].network_type = NETWORK_TYPE_AD;
			}
            ap_list->ap_num++;
            if(ap_list->ap_num == 128)
				{
                    break;
                }
			ap_id++;
		}
	}

	free(pBssidList);
	fclose(fp);

	return T_SUCCESS; 	
}
/*****************************************************************/
/*Op Mode Switch                                                 */
/*****************************************************************/
#if 0
int set_device_mode(int radio, int op_mode)
{
    int device_mode;
    char radio_name[VAP_NAME_LEN];
    char cmd[128];
    int ret;
    switch(op_mode)
        {
            case OP_MODE_NORMAL:
            case OP_MODE_AP:
                device_mode = 0;
                break;  
            case OP_MODE_STA0:
                device_mode = 1;
                break;  
            case OP_MODE_WISP:
                device_mode = 2;
                break;  
            default:
                printf("Error:Such operation mode is not supported current, please check it\n");
                return T_FAILURE;
        }   
    /*Construct AP*/
    ret = construct_main_ap(radio_name, radio);
    if(T_FAILURE == ret)
        {
            printf("ERRO:Construct main AP failure!\n");
            return T_FAILURE;
        }
    sprintf(cmd, "iwpriv %s set DeviceMode=%d", radio_name, device_mode);
    printf("CMD in set_device_mode is %s\n", cmd);
    system(cmd);
    return T_SUCCESS;
}
#endif
int set_apcli_disconnect_ap(int radio)
{
    char apcli_name[8];
    char cmd[128];
    int ret;
    ret = construct_vap(apcli_name, radio, 0, WLAN_MODE_STA);
    if(T_FAILURE == ret)
        {
            printf("ERROR:Construct VAP Failure!\n");
            return T_FAILURE;
        }

    sprintf(cmd, "ifconfig %s down && ifconfig %s up && iwpriv %s set ApCliEnable=0", apcli_name, apcli_name, apcli_name);
    printf("CMD in set_apcli_disconnect_ap is %s\n", cmd);
    system(cmd);
    return T_SUCCESS;
}

int config_apcli(int radio)
{
    int apcli_enable = 0;
    char apcli_enable_s[32];
    char buf[32];
    int secmode;
    /*Disconnect with AP*/
    ezplib_get_attr_val("wl0_apcli_rule", 0, "enable", buf, 32, EZPLIB_USE_CLI);
    strcpy(apcli_enable_s, buf);
    apcli_enable = atoi(apcli_enable_s);

    if(SSID_DISABLE == apcli_enable)
        {
            set_apcli_disconnect_ap(radio);
        }
    /*STA UP*/
    vap_up_down(radio, 0, WLAN_MODE_STA, VAP_UP);
    /*Set SSID*/
    set_sta_ssid(radio);
    /*Set BBSSID*/
    set_sta_bssid(radio);
    /*STA Disabllow TKIP*/
    set_sta_disallow_tkip(radio, 0);
    /*Set Security*/
    set_sta_security(radio);
    /*Set STA AddrMode*/
    set_sta_addr_mode(radio);
    /*Set ApCli Enable*/
    set_sta_enable(radio, VAP_UP); 
    /*STA Down*/
    vap_up_down(radio, 0, WLAN_MODE_STA, VAP_DOWN);
    /*STA Up*/
    vap_up_down(radio, 0, WLAN_MODE_STA, VAP_UP);

    /*Get security mode of station*/
    secmode = get_sta_secmode(radio);
    if((SECMODE_WPA == secmode) || (SECMODE_WPA2 == secmode))
        {
           printf("\n\n******(SECMODE_WPA == secmode) || (SECMODE_WPA2 == secmode)*****\n\n");
           create_wpa_supplicant_conf(radio);
           start_apcli_apd_daemon(radio);
        }
    else
        {
           kill_apcli_apd_daemon(radio);
        }

    return T_SUCCESS;
}

int set_apcli_basic(int radio)
{
    set_sta_mac_repeater(radio);
    set_wirelessmode_channel(radio);
    set_power(radio);
    return T_SUCCESS;
}



