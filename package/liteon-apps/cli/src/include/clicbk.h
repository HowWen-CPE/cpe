#ifndef CLI_CBK_H_
#define CLI_CBK_H_

#define VERSION_BOOTVERSION   "Boot ROM: Version 0.0.3"
#define VERSION_AP                      "AP Software: Version 1.0.0"
#define BUILD_TIME                      "Built on Aug 10 2007,  19:41:40"

#include "cli.h"
extern struct parse_token_s cmdTbl[];
extern int g_curRadio;
extern int g_curVap;

#define UP 1
#define DOWN 0

//#define VAP_COMMONNAME               "wifi0"
#ifdef SUPPORT_WALLAP
#define VAP_NUMBERS                       8
#else
#define VAP_NUMBERS                       4
#endif
//#define VAP_NAME                               "ath"
#define WLAN_MAX_DEV	2

#define T_SUCCESS  0
#define T_FAILURE  -1

#define T_TRUE            1
#define T_FALSE           0

#define MAX_TXPOWER 10

#define OUTPUT_FORMAT   "%-20d%s\n"

typedef struct zone_s
{
	int cityId;
	int zoneId;
	char name[70];
} zone_t;

#define ENABLE  "enable"
#define DISABLE "disable"
#define CLEAR     "clear"
#define EXTERNAL "external"
#define INTERNAL "original"
#define WLAN      "wlan"
#define VIRTUALAP "virtualap"
#define LAN "lan"
#define SWITCHQOS "switchqos"
#define SWITCHRATE "switchrate"
#define PORT8021X "port8021x"

#define  FULL "full"
#define  HALF "half"
#define  QUARTER "quarter"
#define  EIGHTH  "eighth"
#define  MIN	 "min"


#define EXTANT_1  "590"
#define EXTANT_2  "591"
#define EXTANT_3  "596"
#define EXTANT_4  "598"

//#define EXTANT_1_DESC  "Use 3CWE590 3Com 2/2dBi Dual-Band Panel Antenna"
//#define EXTANT_2_DESC  "Use 3CWE591 3Com 6/8dBi Dual-Band Omni Antenna"
#define EXTANT_3_DESC  "Use 18/20dBi Dual-Band Panel Antenna"
//#define EXTANT_4_DESC  "Use 3CWE598 3Com 8/10dBi Dual-Band Panel Antenna"

#define ACCESSPOINT "accesspoint"
#define WIRELESSBRIDGE "wirelessbridge"

#define FAC_DEFAULT_PATH "/etc/httpd/facdef.sh"

enum radioBand
{
	std20MHz = 0,
	autoRadioBand,
	wide40MHz
};

#define EXT_NONE 0
#define EXT_UP 1
#define EXT_LOW -1

int quitCmdHandler(CLI * pCli, char *p, struct parse_token_s *pNxtTbl);
int antennaCliCmdx(CLI * pCli, char *p, struct parse_token_s *pNxtTbl);
int ipMaskCliCmdSet(CLI * pCli, char *p, struct parse_token_s *pNxtTbl);
int ipAddrCliCmdSet(CLI * pCli, char *p, struct parse_token_s *pNxtTbl);
int ipMaskCliCmdGet(CLI * pCli, char *p, struct parse_token_s *pNxtTbl);
int ipAddrCliCmdGet(CLI * pCli, char *p, struct parse_token_s *pNxtTbl);
int antennaCliCmdGet(CLI * pCli, char *p, struct parse_token_s *pNxtTbl);


int systemNameSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int systemNameGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);

int txChainSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);

int versionHandler(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int radarCmdHandler(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int pingCmdHandler(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int multipingCmdHandler(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int rfpingCmdHandler(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int wmodeCmdHandler(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);

int LedctrlWlanHandler(const char *, const char *);
int LedctrlCmdHandler(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);

int countryCodeGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int operationModeGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);

int dhcpcGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int ipaddrGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int ipAddrSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int ipmaskGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int ipmaskSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int accountGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int accountSet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl);
int connectRssiThresholdSet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl);
int connectRssiThresholdGet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl);
int disconnectRssiThresholdGet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl);
int disconnectRssiThresholdSet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl);
int rssiGet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl);
int macGet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl);
int ssidGet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl);
int dhcpGet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl);
int dhcpSet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl);
int rssithrSet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl);

int ipgatewayGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int httpGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);

/*Thomas add 24/07/2007*/
int dhcpcCliCmdx(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);

int wiredRadiusAddrSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);//Added by Mario Huang for wired 802.1x radius server IP address set
int wiredRadiusPortSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);//Added by Mario Huang for wired 802.1x radius server Port number set
int wiredRadiusSecretSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);//Added by Mario Huang for wired 802.1x radius server secret set
int wiredRadiusReauthSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);//Added by Mario Huang for wired 802.1x reauth enable and disable
int wiredRadiusPeriodSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);//Added by Mario Huang for wired 802.1x reauth period set
int wiredEAPTimeoutSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);//Added by Mario Huang for wired 802.1x EAP Timeout set
int wiredAdminModeSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);//Added by Mario Huang for wired 802.1x radius admin mode set
int wiredForceReauthSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);//Added by Mario Huang for wired 802.1x force reauth
int wiredForceReinitSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);//Added by Mario Huang for wired 802.1x force reinit
int gatewaySet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int httpserverportSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int httpsserverportSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);

int operationmodeCliCmdx(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int securitySet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int securityGet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl);

int getsecurity(CLI * pCli, char *pToken,struct parse_token_s *pNxtTbl);
int associationtimeoutGet(CLI * pCli, char *pToken,struct parse_token_s *pNxtTbl);
int associationtimeoutSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int distanceSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int fragmentthresholdSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int aggregationmsduGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int aggregationmpduGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int aggregationmsduCliCmdx(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int aggregationmpduCliCmdx(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int profilevlanGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int profilevlanCliCmdx(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);

int currentpowerSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);

int configWlanX(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int configVap(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
//Deleted by Mario Huang, Close it temporary
#if 0
#if defined(WallAP)//by frank
int configLan(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);

int modeGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int nativeVlanGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int tagNativeVlanGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int allowVId1_Get(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int allowVId2_Get(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);

int modeCliCmdx(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int nativeVlanSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int tagNativeVlanCliCmdx(CLI * pCli, 
		char *pToken, struct parse_token_s *pNxtTbl);
int allowVId1_Set(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int allowVId2_Set(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);

int mngVlanGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int mngVlanSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);

int statisticsCliCmdx(CLI * pCli, 
		char *pToken, struct parse_token_s *pNxtTbl);
int getStatistics(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int setStatistics(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int statGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int statResetGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);

int switchRateSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int switchRateGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);


int switchQosSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int switchQosGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);

int portlinkstatusGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);

#endif
#endif
/****************************************************************/
#define CLI_INDEX_NONE         -1
#define CLI_INDEX_SSID         0
#define CLI_INDEX_DEVICEID     1
#define CLI_INDEX_SECURITY     2
#define CLI_INDEX_NETMASK      3
#define CLI_INDEX_IPADDR       4
#define CLI_INDEX_DISCONNRSSITHR   5
#define CLI_INDEX_CONNRSSITHR  6
#define CLI_INDEX_ACCOUNT      7
#define CLI_INDEX_DHCP         8

#define CLI_NAME_SSID          "ssid"
#define CLI_NAME_DEVICEID      "deviceid"
#define CLI_NAME_SECURITY      "security"
#define CLI_NAME_NETMASK       "netmask"
#define CLI_NAME_IPADDR        "ipaddr"
#define CLI_NAME_DISCONNRSSITHR    "disconnrssithr"
#define CLI_NAME_CONNRSSIHR    "connrssithr"
#define CLI_NAME_ACCOUNT       "account"
#define CLI_NAME_DHCP		   "dhcp"

//security
#define SECURITY_AUTH_MODE     "authmode"
#define SECURITY_ENC_TYPE      "enctype"
#define SECURITY_key           "key"

//read/write
#define ONLYREAD              1
#define READWRITE             0

#define NONE                  0
#define IS_CHANGED            1

/*************************************************************** */
#define GLOBAL_CONFIGS_COUNT 20
#define GLOBAL_ITEM_NAME_LENGTH 32
#define GLOBAL_ITEM_TATAL_PARAMS 8
#define GLOBAL_PARAM_NAME_LENGTH 32
#define GLOBAL_VALUE_LENGTH 128

struct param_pair {
	char param_name[GLOBAL_PARAM_NAME_LENGTH]; // param name
	char value[GLOBAL_PARAM_NAME_LENGTH];     //param value
};

struct item_config {
	char name[GLOBAL_ITEM_NAME_LENGTH];       //name of config item
	struct param_pair params[GLOBAL_ITEM_TATAL_PARAMS]; //numbers of param
};

typedef struct global_config {
	struct item_config *item;  //config item
	int flag;                  //0: initial value, 1: modified
	int readonly;              //0: no, 1: yes
	int item_index;            //item index
} global_config_t;

static global_config_t global_configs[GLOBAL_CONFIGS_COUNT];
void init_global_config();
void free_global_config();
void debug_global_config();



int vapactiveCliCmdx(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int ssidSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);


/*Added by Mario Huang for wired 802.1X start*/
int get_wired8021x_mode(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int wiredRadiusAddrGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int wiredRadiusPortGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int wiredRadiusSecretGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int wiredRadiusReauthGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int wiredRadiusPeriodGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int wiredEAPTimeoutGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int wiredAdminModeGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int wiredAuthStatusGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
/*Added by Mario Huang for wired 802.1X end*/
int staSeparationGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int vapVlanTagGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int vapssidsuppressGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int vapVlanIDGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int vapVlanPriorityGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int multibssidGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);


int staSeparationCliCmdx(CLI * pCli, char *pToken,
		struct parse_token_s *pNxtTbl);
int vapVlanTagCliCmdx(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int vapssidsuppressCliCmdx(CLI * pCli, char *pToken,
		struct parse_token_s *pNxtTbl);
int vapVlanIDSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);

int vapVlanPrioritySet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);


int isolationCliCmdx(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int multibssidCliCmdx(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int ssidsuppressCliCmdx(CLI * pCli, char *pToken,
		struct parse_token_s *pNxtTbl);

int externalAntennaCliCmdx(CLI * pCli, char *pToken,
		struct parse_token_s *pNxtTbl);

int rtsthresholdGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int dot11dGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int fragmentthresholdGet(CLI * pCli, char *pToken,
		struct parse_token_s *pNxtTbl);
int distanceGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);




int beaconintervalSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int dtimSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int dot11dCliCmdx(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int powerCliCmdx(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int rtsthresholdSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);


int snmpGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int snmpcontactGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int rwSnmpSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int roSnmpSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int snmpContactSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int snmpStatusCliCmdx(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);




int sntpGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int timeofdayGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int timezoneGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);

int sntpCliCmdx(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int sntpserver1Set(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int sntpserver2Set(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int timeofdaySet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);


int timeofzoneSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);


int daylightsavingCliCmdx(CLI * pCli, char *pToken,
		struct parse_token_s *pNxtTbl);
int daylighttimeSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);

int syslogGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);



int syslogCliCmdx(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int syslogserverSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int syslogserverportSet(CLI * pCli, char *pToken,
		struct parse_token_s *pNxtTbl);
int sysloglevelSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);




int uptimeGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int onlywiredlanGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int rebootCmdHandler(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int applycfgCmdHandler(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int cancelcfgCmdHandler(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int accessCmdHandler(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int debugCmdHandler(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int factorydefaultSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int httpsCliCmdx(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int countryCodeCliCmdSet(CLI * pCli, char *pToken,
		struct parse_token_s *pNxtTbl);
int dot11gSetCmdHandler(CLI * pCli, char *pToken,
		struct parse_token_s *pNxtTbl);
int dot11bgnSetCmdHandler(CLI * pCli, char *pToken,
		struct parse_token_s *pNxtTbl);
int dot11aSetCmdHandler(CLI * pCli, char *pToken,
		struct parse_token_s *pNxtTbl);
int dot11anSetCmdHandler(CLI * pCli, char *pToken,
		struct parse_token_s *pNxtTbl);
int wirelessModeGetCmdHandler(CLI * pCli, char *pToken,
		struct parse_token_s *pNxtTbl);

int wirelessStationListGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int set_radio_status(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int set_wired8021x_mode(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);//Added by Mario Huang for wired 802.1x enable and disable

int standardBandSetCmdHandler(CLI * pCli, char *pToken,
		struct parse_token_s *pNxtTbl);
int wideBandSetCmdHandler(CLI * pCli, char *pToken,
		struct parse_token_s *pNxtTbl);
int autoBandSetCmdHandler(CLI * pCli, char *pToken,
		struct parse_token_s *pNxtTbl);
int channelSetCmdHandler(CLI * pCli, char *pToken,
		struct parse_token_s *pNxtTbl);
int transmitpowerSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int extchanSetCmdHandler(CLI * pCli, char *pToken,
		struct parse_token_s *pNxtTbl);
int externalAntennaGetCmdHandler(CLI * pCli, char *pToken,
		struct parse_token_s *pNxtTbl);
int txrxchainGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);

int applyIfAddress(void);
int applyWlanAdvSettings(void);
int applySntpSettings(void);
int applySyslogSettings(void);
int applyWlanBasicSettings(void);
int applySnmpSettings(void);
int applyHttpdSettings(void);
int applyOperationModeSettings(void);

int snSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int snGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);

int  TestNandFlash(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl);
int memtest(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl);
int enableBootAbort(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl);	
int disableBootAbort(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl);
int bootAbortGet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl);

int  GetMfgState(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl);
int  SetMfgState(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl);

int GetBoardData(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl);
int InitBoardData(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl);

/* Janelai add @ 2008.6.27. Add CLI command for super user: "set led G/O" */
int ledColorSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);

int ledColorSetstatus(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);

/* Janelai add @ 2008.6.27. Add CLI command for super user: "set rb ON/OFF" */
int rbFunctionSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);

int flashTestHandler(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);

/* Janelai add @ 2008.6.27. Add CLI command for super user: "set eth0 10/100/1000" */
int eth0SpeedSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);
int eth1SpeedSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl);

//Added by Mario Huang
int lan0SpeedGet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl);
#endif
