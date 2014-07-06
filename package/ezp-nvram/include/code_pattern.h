#define CODE_ID		"U2ND"
#define BOOT_PATTERN	"EST"
#define UP_PMON		1
#define UP_MAC		2
#define UP_CODE		3
#define UP_PIGGY	4
#define UP_EOU_KEY	5
#define UP_SN		6
#define UP_LANG		7

#define LINKSYS		7
#define CISCO		90

#define ENGLISH 	1
#define JAPANESE	2
#define GERMAN		3
#define FRENCH		4
#define KOREAN		5

#define	USA		1
#define	JAPAN		2
#define	EUROPE		3
#define WW		4
#define GERMANY		5
#define	KOREA		6
#define	FRANCE		7

#define	WRT54G		1
#define	WRT54GS		2
#define	WRT54V5		3
#define	DPN		4
#define	RTA41		5
#define WRT54GL         6
#define WRT54GSV4	7


#define LINKSYS_MODEL	WRT54GL
/***************************************
 * define country                      *
 * LOCALE=COUNTRY =                    *
 ***************************************/
#define COUNTRY		LOCALE
#define LOCALE		USA
//#define LOCALE		JAPAN
//#define LOCALE		EUROPE
//#define LOCALE		WW
//#define LOCALE		GERMANY	
//#define LOCALE		FRANCE
//#define LOCALE		KOREA

/***************************************
 * define model name and code pattern  *
 * MODEL_NAME =                        *
 * CODE_PATTERN =                      *
 ***************************************/
	#define UI_STYLE	CISCO
		#define CODE_PATTERN   "W54G"
		#if LOCALE == GERMANY
			#define MODEL_NAME	"WRT54G-DE"
			#define MODEL_VERSION	""
		#elif LOCALE == FRANCE
			#define MODEL_NAME	"WRT54G-FR"
			#define MODEL_VERSION	""
		#elif LOCALE == JAPAN
			#define MODEL_NAME	"WRT54G-JP"
			#define MODEL_VERSION	""
		#else
			#define MODEL_NAME	"WRT54GL"
			#define MODEL_VERSION	"V1"
		#endif
		#define	CT_VENDOR		"LINKSYS"
		/*Barry add for Control Intel Flash Downgrade code*/
		#define INTEL_FLASH_SUPPORT_VERSION_FROM "v1.41.8"
		#define BCM4712_CHIP_SUPPORT_VERSION_FROM "v1.50.0"
		#define INTEL_FLASH_SUPPORT_BOOT_VERSION_FROM "v1.3"
		#define BCM4712_CHIP_SUPPORT_BOOT_VERSION_FROM "v2.0"

/***************************************
 * define language                     *
 * LANGUAGE =                          *
 * LANG_SEL=EN                         *
 * HTTP_CHARSET =		       *
 ***************************************/
#if LOCALE == JAPAN
	#define	LANGUAGE	JAPANESE
	#define	HTTP_CHARSET	"shift-jis"
#elif LOCALE == GERMANY
	#define LANGUAGE	GERMAN
	#define	HTTP_CHARSET	"iso-8859-1"
#elif LOCALE == FRANCE
	#define LANGUAGE	FRENCH
	#define	HTTP_CHARSET	"iso-8859-1"
#elif LOCALE == KOREA
	#define LANGUAGE 	KOREAN
	#define	HTTP_CHARSET	"euc-kr"
#else
	#define LANGUAGE 	ENGLISH
	#define	HTTP_CHARSET	"iso-8859-1"
#endif

/***************************************
 * define wireless max channel         *
 * WL_MAX_CHANNEL =                    *
 ***************************************/
#if LOCALE == JAPAN || LOCALE == EUROPE || LOCALE == GERMANY || LOCALE == FRANCE
	#define	WL_MAX_CHANNEL	"13"
#else
	#define WL_MAX_CHANNEL	"11"
#endif

/***************************************
 * define web file path                *
 * WEB_PAGE =                          *
 ***************************************/
#if UI_STYLE ==  CISCO
	#define WEB_PAGE	"cisco_wrt54g_m"
#endif

/***************************************
 * check LOCALE
 ***************************************/
 #if LOCALE != JAPAN && LOCALE != USA && LOCALE != EUROPE && LOCALE != GERMANY && LOCALE != FRANCE
	#error	"The LOCALE for LINKSYS is error, must be USA, EUROPE, JAPAN, GERMANY or FRANCE"
 #endif
