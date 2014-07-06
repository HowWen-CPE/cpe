/*
 *	Wireless Tools
 *
 *		Jean II - HPLB '99 - HPL 99->07
 *
 * This tool can access various piece of information on the card
 * not part of iwconfig...
 * You need to link this code against "iwlist.c" and "-lm".
 *
 * This file is released under the GPL license.
 *     Copyright (c) 1997-2007 Jean Tourrilhes <jt@hpl.hp.com>
 */

#include "iwlib.h"		/* Header */
#include <sys/time.h>

#define SPECTRAL_SCAN_FLAG 1

#ifdef SPECTRAL_SCAN_FLAG

#define FINAL_RESULT_FLAG 1

int initProbeScan();
void printProbeScan();
void releaseProbeScan();
channel_avgrssi_pair_t getProbeResult2_4G();
channel_avgrssi_pair_t getProbeResult5G();
#endif

/* global variable */
static int debug = 0;
static int channel_scan_timeout = 8000000;
static int get_data_one_time = 50000;
scan_database_t scan_results_head = NULL;

#ifdef FINAL_RESULT_FLAG
channel_avgrssi_pair_t final_results_2_4g = NULL;
channel_avgrssi_pair_t final_results_5g = NULL;
#endif

#define TOTAL_DUAL_BAND_CHANNELS     254
#define TOTAL_SCAN_2_4G_BAND                   13
#define SCAN_2_4G_START_CHANNEL                     1
#define TOTAL_SCAN_5G_BAND			    24
#define SCAN_5G_START_CHANNEL			    36
#define SCAN_5G_END_CHANNEL			   165

/****************************** TYPES ******************************/

/*
 * Scan state and meta-information, used to decode events...
 */
typedef struct iwscan_state
{
  /* State */
  int			ap_num;		/* Access Point number 1->N */
  int			val_index;	/* Value in table 0->(N-1) */
} iwscan_state;

/*
 * Bit to name mapping
 */
typedef struct iwmask_name
{
  unsigned int	mask;	/* bit mask for the value */
  const char *	name;	/* human readable name for the value */
} iwmask_name;

/*
 * Types of authentication parameters
 */
typedef struct iw_auth_descr
{
  int				value;		/* Type of auth value */
  const char *			label;		/* User readable version */
  const struct iwmask_name *	names;		/* Names for this value */
  const int			num_names;	/* Number of names */
} iw_auth_descr;

/**************************** CONSTANTS ****************************/

#define IW_SCAN_HACK		0x8000

#define IW_EXTKEY_SIZE	(sizeof(struct iw_encode_ext) + IW_ENCODING_TOKEN_MAX)

/* ------------------------ WPA CAPA NAMES ------------------------ */
/*
 * This is the user readable name of a bunch of WPA constants in wireless.h
 * Maybe this should go in iwlib.c ?
 */

#ifndef WE_ESSENTIAL
#define IW_ARRAY_LEN(x) (sizeof(x)/sizeof((x)[0]))

//static const struct iwmask_name iw_enc_mode_name[] = {
//  { IW_ENCODE_RESTRICTED,	"restricted" },
//  { IW_ENCODE_OPEN,		"open" },
//};
//#define	IW_ENC_MODE_NUM		IW_ARRAY_LEN(iw_enc_mode_name)

static const struct iwmask_name iw_auth_capa_name[] = {
  { IW_ENC_CAPA_WPA,		"WPA" },
  { IW_ENC_CAPA_WPA2,		"WPA2" },
  { IW_ENC_CAPA_CIPHER_TKIP,	"CIPHER-TKIP" },
  { IW_ENC_CAPA_CIPHER_CCMP,	"CIPHER-CCMP" },
};
#define	IW_AUTH_CAPA_NUM	IW_ARRAY_LEN(iw_auth_capa_name)

static const struct iwmask_name iw_auth_cypher_name[] = {
  { IW_AUTH_CIPHER_NONE,	"none" },
  { IW_AUTH_CIPHER_WEP40,	"WEP-40" },
  { IW_AUTH_CIPHER_TKIP,	"TKIP" },
  { IW_AUTH_CIPHER_CCMP,	"CCMP" },
  { IW_AUTH_CIPHER_WEP104,	"WEP-104" },
};
#define	IW_AUTH_CYPHER_NUM	IW_ARRAY_LEN(iw_auth_cypher_name)

static const struct iwmask_name iw_wpa_ver_name[] = {
  { IW_AUTH_WPA_VERSION_DISABLED,	"disabled" },
  { IW_AUTH_WPA_VERSION_WPA,		"WPA" },
  { IW_AUTH_WPA_VERSION_WPA2,		"WPA2" },
};
#define	IW_WPA_VER_NUM		IW_ARRAY_LEN(iw_wpa_ver_name)

static const struct iwmask_name iw_auth_key_mgmt_name[] = {
  { IW_AUTH_KEY_MGMT_802_1X,	"802.1x" },
  { IW_AUTH_KEY_MGMT_PSK,	"PSK" },
};
#define	IW_AUTH_KEY_MGMT_NUM	IW_ARRAY_LEN(iw_auth_key_mgmt_name)

static const struct iwmask_name iw_auth_alg_name[] = {
  { IW_AUTH_ALG_OPEN_SYSTEM,	"open" },
  { IW_AUTH_ALG_SHARED_KEY,	"shared-key" },
  { IW_AUTH_ALG_LEAP,		"LEAP" },
};
#define	IW_AUTH_ALG_NUM		IW_ARRAY_LEN(iw_auth_alg_name)

static const struct iw_auth_descr	iw_auth_settings[] = {
  { IW_AUTH_WPA_VERSION, "WPA version", iw_wpa_ver_name, IW_WPA_VER_NUM },
  { IW_AUTH_KEY_MGMT, "Key management", iw_auth_key_mgmt_name, IW_AUTH_KEY_MGMT_NUM },
  { IW_AUTH_CIPHER_PAIRWISE, "Pairwise cipher", iw_auth_cypher_name, IW_AUTH_CYPHER_NUM },
  { IW_AUTH_CIPHER_GROUP, "Pairwise cipher", iw_auth_cypher_name, IW_AUTH_CYPHER_NUM },
  { IW_AUTH_TKIP_COUNTERMEASURES, "TKIP countermeasures", NULL, 0 },
  { IW_AUTH_DROP_UNENCRYPTED, "Drop unencrypted", NULL, 0 },
  { IW_AUTH_80211_AUTH_ALG, "Authentication algorithm", iw_auth_alg_name, IW_AUTH_ALG_NUM },
  { IW_AUTH_RX_UNENCRYPTED_EAPOL, "Receive unencrypted EAPOL", NULL, 0 },
  { IW_AUTH_ROAMING_CONTROL, "Roaming control", NULL, 0 },
  { IW_AUTH_PRIVACY_INVOKED, "Privacy invoked", NULL, 0 },
};
#define	IW_AUTH_SETTINGS_NUM		IW_ARRAY_LEN(iw_auth_settings)

/* Values for the IW_ENCODE_ALG_* returned by SIOCSIWENCODEEXT */
static const char *	iw_encode_alg_name[] = {
	"none",
	"WEP",
	"TKIP",
	"CCMP",
	"unknown"
};
#define	IW_ENCODE_ALG_NUM		IW_ARRAY_LEN(iw_encode_alg_name)

#ifndef IW_IE_CIPHER_NONE
/* Cypher values in GENIE (pairwise and group) */
#define IW_IE_CIPHER_NONE	0
#define IW_IE_CIPHER_WEP40	1
#define IW_IE_CIPHER_TKIP	2
#define IW_IE_CIPHER_WRAP	3
#define IW_IE_CIPHER_CCMP	4
#define IW_IE_CIPHER_WEP104	5
/* Key management in GENIE */
#define IW_IE_KEY_MGMT_NONE	0
#define IW_IE_KEY_MGMT_802_1X	1
#define IW_IE_KEY_MGMT_PSK	2
#endif	/* IW_IE_CIPHER_NONE */

/* Values for the IW_IE_CIPHER_* in GENIE */
static const char *	iw_ie_cypher_name[] = {
	"none",
	"WEP-40",
	"TKIP",
	"WRAP",
	"CCMP",
	"WEP-104",
};
#define	IW_IE_CYPHER_NUM	IW_ARRAY_LEN(iw_ie_cypher_name)

/* Values for the IW_IE_KEY_MGMT_* in GENIE */
static const char *	iw_ie_key_mgmt_name[] = {
	"none",
	"802.1x",
	"PSK",
};
#define	IW_IE_KEY_MGMT_NUM	IW_ARRAY_LEN(iw_ie_key_mgmt_name)

#endif	/* WE_ESSENTIAL */

/************************* WPA SUBROUTINES *************************/

#ifndef WE_ESSENTIAL
/*------------------------------------------------------------------*/
/*
 * Print all names corresponding to a mask.
 * This may want to be used in iw_print_retry_value() ?
 */
static void 
iw_print_mask_name(unsigned int			mask,
		   const struct iwmask_name	names[],
		   const unsigned int		num_names,
		   const char *			sep)
{
  unsigned int	i;

  /* Print out all names for the bitmask */
  for(i = 0; i < num_names; i++)
    {
      if(mask & names[i].mask)
	{
	  /* Print out */
	  printf("%s%s", sep, names[i].name);
	  /* Remove the bit from the mask */
	  mask &= ~names[i].mask;
	}
    }
  /* If there is unconsumed bits... */
  if(mask != 0)
    printf("%sUnknown", sep);
}

/*------------------------------------------------------------------*/
/*
 * Print the name corresponding to a value, with overflow check.
 */
static void
iw_print_value_name(unsigned int		value,
		    const char *		names[],
		    const unsigned int		num_names)
{
  if(value >= num_names)
    printf(" unknown (%d)", value);
  else
    printf(" %s", names[value]);
}

/*------------------------------------------------------------------*/
/*
 * Parse, and display the results of an unknown IE.
 *
 */
static void 
iw_print_ie_unknown(unsigned char *	iebuf,
		    int			buflen)
{
  int	ielen = iebuf[1] + 2;
  int	i;

  if(ielen > buflen)
    ielen = buflen;

  printf("Unknown: ");
  for(i = 0; i < ielen; i++)
    printf("%02X", iebuf[i]);
  printf("\n");
}

/*------------------------------------------------------------------*/
/*
 * Parse, and display the results of a WPA or WPA2 IE.
 *
 */
static inline void 
iw_print_ie_wpa(unsigned char *	iebuf,
		int		buflen)
{
  int			ielen = iebuf[1] + 2;
  int			offset = 2;	/* Skip the IE id, and the length. */
  unsigned char		wpa1_oui[3] = {0x00, 0x50, 0xf2};
  unsigned char		wpa2_oui[3] = {0x00, 0x0f, 0xac};
  unsigned char *	wpa_oui;
  int			i;
  uint16_t		ver = 0;
  uint16_t		cnt = 0;

  if(ielen > buflen)
    ielen = buflen;

#ifdef DEBUG
  /* Debugging code. In theory useless, because it's debugged ;-) */
  printf("IE raw value %d [%02X", buflen, iebuf[0]);
  for(i = 1; i < buflen; i++)
    printf(":%02X", iebuf[i]);
  printf("]\n");
#endif

  switch(iebuf[0])
    {
    case 0x30:		/* WPA2 */
      /* Check if we have enough data */
      if(ielen < 4)
	{
	  iw_print_ie_unknown(iebuf, buflen);
 	  return;
	}

      wpa_oui = wpa2_oui;
      break;

    case 0xdd:		/* WPA or else */
      wpa_oui = wpa1_oui;
 
      /* Not all IEs that start with 0xdd are WPA. 
       * So check that the OUI is valid. Note : offset==2 */
      if((ielen < 8)
	 || (memcmp(&iebuf[offset], wpa_oui, 3) != 0)
	 || (iebuf[offset + 3] != 0x01))
 	{
	  iw_print_ie_unknown(iebuf, buflen);
 	  return;
 	}

      /* Skip the OUI type */
      offset += 4;
      break;

    default:
      return;
    }
  
  /* Pick version number (little endian) */
  ver = iebuf[offset] | (iebuf[offset + 1] << 8);
  offset += 2;

  if(iebuf[0] == 0xdd)
    printf("WPA Version %d\n", ver);
  if(iebuf[0] == 0x30)
    printf("IEEE 802.11i/WPA2 Version %d\n", ver);

  /* From here, everything is technically optional. */

  /* Check if we are done */
  if(ielen < (offset + 4))
    {
      /* We have a short IE.  So we should assume TKIP/TKIP. */
      printf("                        Group Cipher : TKIP\n");
      printf("                        Pairwise Cipher : TKIP\n");
      return;
    }
 
  /* Next we have our group cipher. */
  if(memcmp(&iebuf[offset], wpa_oui, 3) != 0)
    {
      printf("                        Group Cipher : Proprietary\n");
    }
  else
    {
      printf("                        Group Cipher :");
      iw_print_value_name(iebuf[offset+3],
			  iw_ie_cypher_name, IW_IE_CYPHER_NUM);
      printf("\n");
    }
  offset += 4;

  /* Check if we are done */
  if(ielen < (offset + 2))
    {
      /* We don't have a pairwise cipher, or auth method. Assume TKIP. */
      printf("                        Pairwise Ciphers : TKIP\n");
      return;
    }

  /* Otherwise, we have some number of pairwise ciphers. */
  cnt = iebuf[offset] | (iebuf[offset + 1] << 8);
  offset += 2;
  printf("                        Pairwise Ciphers (%d) :", cnt);

  if(ielen < (offset + 4*cnt))
    return;

  for(i = 0; i < cnt; i++)
    {
      if(memcmp(&iebuf[offset], wpa_oui, 3) != 0)
 	{
 	  printf(" Proprietary");
 	}
      else
	{
	  iw_print_value_name(iebuf[offset+3],
			      iw_ie_cypher_name, IW_IE_CYPHER_NUM);
 	}
      offset+=4;
    }
  printf("\n");
 
  /* Check if we are done */
  if(ielen < (offset + 2))
    return;

  /* Now, we have authentication suites. */
  cnt = iebuf[offset] | (iebuf[offset + 1] << 8);
  offset += 2;
  printf("                        Authentication Suites (%d) :", cnt);

  if(ielen < (offset + 4*cnt))
    return;

  for(i = 0; i < cnt; i++)
    {
      if(memcmp(&iebuf[offset], wpa_oui, 3) != 0)
 	{
 	  printf(" Proprietary");
 	}
      else
	{
	  iw_print_value_name(iebuf[offset+3],
			      iw_ie_key_mgmt_name, IW_IE_KEY_MGMT_NUM);
 	}
       offset+=4;
     }
  printf("\n");
 
  /* Check if we are done */
  if(ielen < (offset + 1))
    return;

  /* Otherwise, we have capabilities bytes.
   * For now, we only care about preauth which is in bit position 1 of the
   * first byte.  (But, preauth with WPA version 1 isn't supposed to be 
   * allowed.) 8-) */
  if(iebuf[offset] & 0x01)
    {
      printf("                       Preauthentication Supported\n");
    }
}
 
/*------------------------------------------------------------------*/
/*
 * Process a generic IE and display the info in human readable form
 * for some of the most interesting ones.
 * For now, we only decode the WPA IEs.
 */
static inline void
iw_print_gen_ie(unsigned char *	buffer,
		int		buflen)
{
  int offset = 0;

  /* Loop on each IE, each IE is minimum 2 bytes */
  while(offset <= (buflen - 2))
    {
      printf("                    IE: ");

      /* Check IE type */
      switch(buffer[offset])
	{
	case 0xdd:	/* WPA1 (and other) */
	case 0x30:	/* WPA2 */
	  iw_print_ie_wpa(buffer + offset, buflen);
	  break;
	default:
	  iw_print_ie_unknown(buffer + offset, buflen);
	}
      /* Skip over this IE to the next one in the list. */
      offset += buffer[offset+1] + 2;
    }
}
#endif	/* WE_ESSENTIAL */

/***************************** SCANNING *****************************/
/*
 * This one behave quite differently from the others
 *
 * Note that we don't use the scanning capability of iwlib (functions
 * iw_process_scan() and iw_scan()). The main reason is that
 * iw_process_scan() return only a subset of the scan data to the caller,
 * for example custom elements and bitrates are ommited. Here, we
 * do the complete job...
 */

/*------------------------------------------------------------------*/
/*
 * Print one element from the scanning results
 */
static inline void
print_scanning_token(struct stream_descr *	stream,	/* Stream of events */
		     struct iw_event *		event,	/* Extracted token */
		     struct iwscan_state *	state,
		     struct iw_range *	iw_range,	/* Range info */
		     int		has_range)
{
  char		buffer[128];	/* Temporary buffer */

  /* Now, let's decode the event */
  switch(event->cmd)
    {
    case SIOCGIWAP:
      printf("          Cell %02d - Address: %s\n", state->ap_num,
	     iw_saether_ntop(&event->u.ap_addr, buffer));
      state->ap_num++;
      break;
    case SIOCGIWNWID:
      if(event->u.nwid.disabled)
	printf("                    NWID:off/any\n");
      else
	printf("                    NWID:%X\n", event->u.nwid.value);
      break;
    case SIOCGIWFREQ:
      {
	double		freq;			/* Frequency/channel */
	int		channel = -1;		/* Converted to channel */
	freq = iw_freq2float(&(event->u.freq));
	/* Convert to channel if possible */
	if(has_range)
	  channel = iw_freq_to_channel(freq, iw_range);
	iw_print_freq(buffer, sizeof(buffer),
		      freq, channel, event->u.freq.flags);
	printf("                    %s\n", buffer);
      }
      break;
    case SIOCGIWMODE:
      /* Note : event->u.mode is unsigned, no need to check <= 0 */
      if(event->u.mode >= IW_NUM_OPER_MODE)
	event->u.mode = IW_NUM_OPER_MODE;
      printf("                    Mode:%s\n",
	     iw_operation_mode[event->u.mode]);
      break;
    case SIOCGIWNAME:
      printf("                    Protocol:%-1.16s\n", event->u.name);
      break;
    case SIOCGIWESSID:
      {
	char essid[IW_ESSID_MAX_SIZE+1];
	memset(essid, '\0', sizeof(essid));
	if((event->u.essid.pointer) && (event->u.essid.length))
	  memcpy(essid, event->u.essid.pointer, event->u.essid.length);
	if(event->u.essid.flags)
	  {
	    /* Does it have an ESSID index ? */
	    if((event->u.essid.flags & IW_ENCODE_INDEX) > 1)
	      printf("                    ESSID:\"%s\" [%d]\n", essid,
		     (event->u.essid.flags & IW_ENCODE_INDEX));
	    else
	      printf("                    ESSID:\"%s\"\n", essid);
	  }
	else
	  printf("                    ESSID:off/any/hidden\n");
      }
      break;
    case SIOCGIWENCODE:
      {
	unsigned char	key[IW_ENCODING_TOKEN_MAX];
	if(event->u.data.pointer)
	  memcpy(key, event->u.data.pointer, event->u.data.length);
	else
	  event->u.data.flags |= IW_ENCODE_NOKEY;
	printf("                    Encryption key:");
	if(event->u.data.flags & IW_ENCODE_DISABLED)
	  printf("off\n");
	else
	  {
	    /* Display the key */
	    iw_print_key(buffer, sizeof(buffer), key, event->u.data.length,
			 event->u.data.flags);
	    printf("%s", buffer);

	    /* Other info... */
	    if((event->u.data.flags & IW_ENCODE_INDEX) > 1)
	      printf(" [%d]", event->u.data.flags & IW_ENCODE_INDEX);
	    if(event->u.data.flags & IW_ENCODE_RESTRICTED)
	      printf("   Security mode:restricted");
	    if(event->u.data.flags & IW_ENCODE_OPEN)
	      printf("   Security mode:open");
	    printf("\n");
	  }
      }
      break;
    case SIOCGIWRATE:
      if(state->val_index == 0)
	printf("                    Bit Rates:");
      else
	if((state->val_index % 5) == 0)
	  printf("\n                              ");
	else
	  printf("; ");
      iw_print_bitrate(buffer, sizeof(buffer), event->u.bitrate.value);
      printf("%s", buffer);
      /* Check for termination */
      if(stream->value == NULL)
	{
	  printf("\n");
	  state->val_index = 0;
	}
      else
	state->val_index++;
      break;
    case SIOCGIWMODUL:
      {
	unsigned int	modul = event->u.param.value;
	int		i;
	int		n = 0;
	printf("                    Modulations :");
	for(i = 0; i < IW_SIZE_MODUL_LIST; i++)
	  {
	    if((modul & iw_modul_list[i].mask) == iw_modul_list[i].mask)
	      {
		if((n++ % 8) == 7)
		  printf("\n                        ");
		else
		  printf(" ; ");
		printf("%s", iw_modul_list[i].cmd);
	      }
	  }
	printf("\n");
      }
      break;
    case IWEVQUAL:
      iw_print_stats(buffer, sizeof(buffer),
		     &event->u.qual, iw_range, has_range);
      printf("                    %s\n", buffer);
      break;
#ifndef WE_ESSENTIAL
    case IWEVGENIE:
      /* Informations Elements are complex, let's do only some of them */
      iw_print_gen_ie(event->u.data.pointer, event->u.data.length);
      break;
#endif	/* WE_ESSENTIAL */
    case IWEVCUSTOM:
      {
	char custom[IW_CUSTOM_MAX+1];
	if((event->u.data.pointer) && (event->u.data.length))
	  memcpy(custom, event->u.data.pointer, event->u.data.length);
	custom[event->u.data.length] = '\0';
	printf("                    Extra:%s\n", custom);
      }
      break;
    default:
      printf("                    (Unknown Wireless Token 0x%04X)\n",
	     event->cmd);
   }	/* switch(event->cmd) */
}

static inline void
save_scanning_data(struct stream_descr *	stream,	/* Stream of events */
		     struct iw_event *		event,	/* Extracted token */
		     struct iwscan_state *	state,
		     struct iw_range *	iw_range,	/* Range info */
		     int		has_range,
		     char*          mac,
		     int*             channel_number,
		     int*             signal_level,
		     int*             noise_floor_level,
		     int*		denominator,
		     int*		numerator)
{
  char		buffer[128];	/* Temporary buffer */
  double		freq;			/* Frequency/channel */
  int		channel = -1;		/* Converted to channel */
  char * tmp_buf = buffer;

  /* Now, let's decode the event */
  switch(event->cmd)
    {
    case SIOCGIWAP:
      iw_saether_ntop(&event->u.ap_addr, buffer);
      memcpy(mac, buffer, 23);
      break;
    case SIOCGIWFREQ:
	freq = iw_freq2float(&(event->u.freq));
	/* Convert to channel if possible */
	if(has_range) 
	  channel = iw_freq_to_channel(freq, iw_range);

	if (channel < 0) {
		iw_calcu_freq(buffer, sizeof(buffer),
		      freq, &channel, event->u.freq.flags);
		printf("buffer: %s\n", buffer);
		if (!strncmp(buffer, "5.785", 5))
			*channel_number = 157;
		else if (!strncmp(buffer, "5.825", 5))
			*channel_number = 165;
	} else
		*channel_number = channel;
	
      break;
    case IWEVQUAL:
    {
	  int		len;
	   int		buflen;
	   const iwqual *	qual = &event->u.qual;

	  if(has_range && ((qual->level != 0)
			   || (qual->updated & (IW_QUAL_DBM | IW_QUAL_RCPI))))
	    {
	      /* Deal with quality : always a relative value */
	      if(!(qual->updated & IW_QUAL_QUAL_INVALID))
		{
		  *denominator =  qual->qual;
		  *numerator= iw_range->max_qual.qual;
		  tmp_buf += len;
		  buflen -= len;
		}

	      /* Check if the statistics are in RCPI (IEEE 802.11k) */
	      if(qual->updated & IW_QUAL_RCPI)
		{
		  /* Deal with signal level in RCPI */
		  /* RCPI = int{(Power in dBm +110)*2} for 0dbm > Power > -110dBm */
		  if(!(qual->updated & IW_QUAL_LEVEL_INVALID))
		    {
		      double	rcpilevel = (qual->level / 2.0) - 110.0;
		      *signal_level = rcpilevel;
		      tmp_buf += len;
		      buflen -= len;
		    }

		  /* Deal with noise level in dBm (absolute power measurement) */
		  if(!(qual->updated & IW_QUAL_NOISE_INVALID))
		    {
		      double	rcpinoise = (qual->noise / 2.0) - 110.0;
		     *noise_floor_level = rcpinoise;
		    }
		}
	      else
		{
		  /* Check if the statistics are in dBm */
		  if((qual->updated & IW_QUAL_DBM)
		     || (qual->level > iw_range->max_qual.level))
		    {
		      /* Deal with signal level in dBm  (absolute power measurement) */
		      if(!(qual->updated & IW_QUAL_LEVEL_INVALID))
			{
			  int	dblevel = qual->level;
			  /* Implement a range for dBm [-192; 63] */
			  if(qual->level >= 64)
			    dblevel -= 0x100;
			  *signal_level = dblevel;
			  tmp_buf += len;
			  buflen -= len;
			}

		      /* Deal with noise level in dBm (absolute power measurement) */
		      if(!(qual->updated & IW_QUAL_NOISE_INVALID))
			{
			  int	dbnoise = qual->noise;
			  /* Implement a range for dBm [-192; 63] */
			  if(qual->noise >= 64)
			    dbnoise -= 0x100;
			  *noise_floor_level = dbnoise;
			}
		    }
		  else
		    {
		      /* Deal with signal level as relative value (0 -> max) */
		      if(!(qual->updated & IW_QUAL_LEVEL_INVALID))
			{
		           *signal_level = qual->level;
			  tmp_buf += len;
			  buflen -= len;
			}

		      /* Deal with noise level as relative value (0 -> max) */
		      if(!(qual->updated & IW_QUAL_NOISE_INVALID))
			{
		      	  *noise_floor_level =  qual->noise;
			}
		    }
		}
	    }
	  else
	    {
	      /* We can't read the range, so we don't know... */
		       *signal_level = qual->level;
		       *noise_floor_level =  qual->noise;
	    }
	}
      break;
   }
}

void print_sst(scan_summary_t sst)
{
	int i = 0;

	for(i = 0; i < TOTAL_DUAL_BAND_CHANNELS; i++ ) {
		printf("i=%d, channel: %d, rssi: %d, noise floor: %d, channel count: %d\n",i, sst->channel[i], sst->max_rssi[i], sst->noise_floor[i], sst->cell[i].count);
	}
}

#ifdef FINAL_RESULT_FLAG
void print_final_results(channel_avgrssi_pair_t cap)
{
	channel_avgrssi_pair_t rs = cap;

    if (NULL == cap)
    {
        return;
    }
	rs = rs->next;

    #if 0
    if(rs != NULL)
    {
         printf("GoodChannel:=%d, avg rssi=%d\n",cap->channel, cap->avg_rssi);
    }
    #endif
    
	while (rs != NULL) {
		printf("channel:=%d, avg rssi=%d\n", rs->channel, rs->avg_rssi);
		rs = rs->next;
	}
}

void free_linklist_final_results(channel_avgrssi_pair_t cap)
{
	channel_avgrssi_pair_t header = cap;
	channel_avgrssi_pair_t rs = header->next;
	channel_avgrssi_pair_t tmp_p = rs;

	while(rs != NULL) {
		rs = rs->next;
		free(tmp_p);
		tmp_p = rs;
	}

	free(header);
}
#endif

void print_results(struct scan_database * results)
{
	struct scan_database * rs;

	if (results == NULL)
		return;

	rs = results;

	for (;rs != NULL; rs = rs ->cell_next) {
			printf("AP number: %d, Mac addr: %s, Frequency: %d, Signal Level: %d, noise floor: %d, quality: %d/%d\n",rs->cell_number, 
					rs->cell_mac_addr, rs->cell_frequency, rs->cell_signal_level, rs->cell_noise_floor_level,
					rs->cell_quality_pair.denominator, rs->cell_quality_pair.numerator);
	}
}

void free_linklist_results(struct scan_database * results)
{
	struct scan_database * rs, *tmp_cell, *header;

	if(results == NULL)
		return;

	header = results;
	tmp_cell = rs = results->cell_next;

	while(rs != NULL) {
		tmp_cell = rs;
		rs = rs->cell_next;
		if (rs != NULL) {
			header->cell_next = tmp_cell->cell_next;
			rs->cell_pre = tmp_cell->cell_pre;
		} 
		//printf("AP number: %d, free node: %s\n", rs->cell_number, rs->cell_mac_addr);
		
		free(tmp_cell);
	}

	/* free header node */
	if (header != NULL)
		free(header);
}

int get_channel_noisefloor(struct scan_database * results, int channel)
{
	struct scan_database *rs = NULL;
	int noise_floor = -100;

	rs = results;

	rs = rs->cell_next;

	while ( rs != NULL) {
		if (rs->cell_frequency == channel) {
				noise_floor = rs->cell_noise_floor_level;
				break;
		}
		rs = rs->cell_next;
	}

	return noise_floor;
}

int  get_channel_maxrssi(struct scan_database * results, int channel, int *count)
{
	struct scan_database *rs = NULL;
	int max_rssi = -100;
	int cnt = 0;

	rs = results;
	rs = rs->cell_next;
	
	while ( rs != NULL) {
		if (rs->cell_frequency == channel) {
			//printf("frequency: %d, channel, %d, signal level: %d, max rssi: %d\n",rs->cell_frequency, channel, rs->cell_signal_level, max_rssi);
			if (rs->cell_signal_level > max_rssi)
				max_rssi = rs->cell_signal_level;

			cnt ++;
		}
		rs = rs->cell_next;
	}

	*count = cnt;

	//printf("max rssi: %d, count: %d\n", max_rssi, *count);

	return max_rssi;
}

int find_average_rssi(scan_summary_t sst, const int *chanlist, int chancount, int centChan)
{
#define NF_WEIGHT_FACTOR 2
    int chan;
    int i;//, j;
    int average = -100;
    int totalrssi = 0; /* total rssi for all channels so far */

    if (chancount <= 0) {
        /* return a large enough number for not to choose this channel */
        return 0xffffffff;
    }

    for (i = 0; i < chancount; i++) {
        chan = chanlist[i];
        if (chan > 0 && chan < 14) 
            totalrssi += sst->max_rssi[chan -1];
	else
	   totalrssi += sst->max_rssi[chan - 1];
    }
    
    average = totalrssi/chancount;
    //printf("totalrssi: %d, average: %d\n", totalrssi, average);

    /* add the weighted noise floor */
    average += sst->noise_floor[centChan] * NF_WEIGHT_FACTOR;

    return (average);
#undef NF_WEIGHT_FACTOR
}

struct centerchan {
	int count;				 /* number of chans to average the rssi */
	const int *chanlist;   /* the possible beacon channels around center chan */
};

#define X(a)    sizeof(a)/sizeof(a[0]), a


void find_11ng_best_channel(scan_summary_t sst, int* channel, int* rssi, int bw)
{
	int best_channel = 0, chan = 0, i = 0, band = 0, avg_rssi = -100, min_avg_rssi = 0;
	#ifdef FINAL_RESULT_FLAG
	channel_avgrssi_pair_t cap = NULL, tmp_cap_p = NULL;
	channel_avgrssi_pair_t header = final_results_2_4g;

	tmp_cap_p = header;
	#endif

	if (sst == NULL)
		return;
	/* debug */
	//print_sst(sst);

	/* 20MHz*/
	if (1) {
		/*
		* The following center chan data structures are invented to allow calculating
		* the average rssi in 20Mhz band for a certain center chan. 
		*
		* We would then pick the band which has the minimum rsi of all the 20Mhz bands.
		*/

		/* For 20Mhz band with center chan 1, we would see beacons only on channels 1,2. */
		static const int center1[] = { 1, 2 };

		/* For 20Mhz band with center chan 6, we would see beacons on channels 4,5,6 & 7. */
		static const int center6[] = { 4, 5, 6, 7 };

		/* For 20Mhz band with center chan 11, we would see beacons on channels 9,10 & 11. */
		static const int center11[] = { 9, 10, 11 };

		struct centerchan centerchans[] = {
		    { X(center1) },
		    { X(center6) },
		    { X(center11) }
		};

		for(i = 0; i < TOTAL_SCAN_2_4G_BAND; i++ ) {
		    chan = sst->channel[i];
		    
		    if ((chan != 1) && (chan != 6) && (chan != 11)) {
		        /* Don't bother with other center channels except for 1, 6 & 11 */
		        continue;
		    }

		    switch (chan) {
		        case 1:
		            band = 0;
		            break;
		        case 6:
		            band = 1;
		            break;
		        case 11:
		            band = 2;
		            break;
		        default: 
		            band = 0;
		            break;
		    }
			/* debug */
			//printf("channel: %d ", chan);
		     #ifdef FINAL_RESULT_FLAG
		     cap = (struct channel_avgrssi_pair *)malloc(sizeof(struct channel_avgrssi_pair));
		     if (cap == NULL) {
			    printf("allocate memory fail\n");
				return;
		     }
			 #endif

		    /* find the average rssi for this 20Mhz band */
		    avg_rssi = find_average_rssi(sst, centerchans[band].chanlist, centerchans[band].count, chan);

#ifdef FINAL_RESULT_FLAG
		    /* store data of every channel */
		     cap->channel = chan;
		     cap->avg_rssi = avg_rssi;
		     cap->next = NULL;

		     tmp_cap_p->next = cap;
		     cap->pre = tmp_cap_p;
		     tmp_cap_p = cap;
#endif

		    if (avg_rssi < min_avg_rssi) {
		        min_avg_rssi = avg_rssi;
		        best_channel = i + 1;
		    }

		    //printf("channel: %d, avg_rssi: %d, min_avg_rssi: %d, best_channel: %d\n", chan, avg_rssi, min_avg_rssi, best_channel);
	    }
    }/* 40MHz*/
    else {
	   printf("40MHz");
    }
    
#ifdef FINAL_RESULT_FLAG
	final_results_2_4g->avg_rssi = min_avg_rssi;
	final_results_2_4g->channel = best_channel;
#endif

   *channel = best_channel;
   *rssi = min_avg_rssi;
}

void find_11na_best_channel(struct scan_database * results, scan_summary_t sst, int* channel, int* rssi, int bw)
{
	scan_summary_t tmp_sst = NULL;
	int i = 0, j = 0, channel_exist = 0;
	int chan = 0;
	int avg_rssi = -100, min_avg_rssi = 0, best_channel = 0;

#ifdef FINAL_RESULT_FLAG
	channel_avgrssi_pair_t cap = NULL, tmp_cap_p = NULL;
	//channel_avgrssi_pair_t header = final_results_5g;
	tmp_cap_p = final_results_5g;
#endif

	if (sst == NULL)
		return;

	tmp_sst = sst;

	
	/*
	* The following center chan data structures are invented to allow calculating
	* the average rssi in 20Mhz band for a certain center chan. 
	*
	* We would then pick the band which has the minimum rsi of all the 20Mhz bands.
	*/
	/* For 20Mhz band */
	static const int centerchannel[] = { 36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 149, 153, 157, 161, 165 };

	for(i = SCAN_5G_START_CHANNEL; i < SCAN_5G_END_CHANNEL ; i++ ) {
		chan = sst->channel[i-1];
		channel_exist = 0;

		if (chan < SCAN_5G_START_CHANNEL)
			continue;

		for (j = 0; j < TOTAL_SCAN_5G_BAND; j ++) {
			if (chan == centerchannel[j]) {
				channel_exist = 1;
				break;
			}
		}

		if (!channel_exist)
			continue;
		
#ifdef FINAL_RESULT_FLAG
		cap = (struct channel_avgrssi_pair *)malloc(sizeof(struct channel_avgrssi_pair));

		if (cap == NULL) {
			printf("allocate memory fail\n");
			return ;
		}
#endif

		/* find the average rssi for this 20Mhz band */
		avg_rssi = find_average_rssi(sst, &chan, 1, chan);

#ifdef FINAL_RESULT_FLAG
		/* store data of every channel */
		cap->avg_rssi = avg_rssi;
		cap->channel = chan;
		cap->next = NULL;

		tmp_cap_p->next = cap;
		cap->pre = tmp_cap_p;
		tmp_cap_p = cap;
#endif

		 if (avg_rssi < min_avg_rssi) {
		        min_avg_rssi = avg_rssi;
		        best_channel = chan;
		    }

		 //printf("channel: %d, avg_rssi: %d, min_avg_rssi: %d, best_channel: %d\n", chan, avg_rssi, min_avg_rssi, best_channel);
	}

#ifdef FINAL_RESULT_FLAG
        final_results_5g->avg_rssi = min_avg_rssi;
        final_results_5g->channel = best_channel;
#endif
    
	 *channel = best_channel;
   	*rssi = min_avg_rssi;
}

int radio_mode(char *ifname, int* bw)
{
	char * buf[64], *tmp_p = NULL;
	FILE *file;
	int mode = -1;
	int bandwdith = 0;
	
	if (ifname == NULL)
		return -1;

	tmp_p = (char *)buf;

	memset(buf, 0, sizeof(buf));

	sprintf(tmp_p, "iwconfig %s | grep Frequency | awk '{print $2}' | cut -c 11", ifname);

	file = popen(tmp_p, "r");

	if (file != NULL) {
		memset(buf, 0, sizeof(buf));

		if (fgets(tmp_p, 8, file) != NULL) {
				mode = atoi(tmp_p);
		}
	} else
		return -1;

	pclose(file);

	memset(buf, 0, sizeof(buf));

	sprintf(tmp_p, "iwconfig %s | grep Rate | awk '{print $2}' | cut -c 6-8", ifname);

	file = popen(tmp_p, "r");

	if (file != NULL) {
		memset(buf, 0, sizeof(buf));

		if (fgets(tmp_p, 8, file) != NULL) {
			bandwdith = atoi(tmp_p);
		}
	}
	pclose(file);

	if (bandwdith == 450 || bandwdith == 300)
		bandwdith = 40;
	else
		bandwdith = 20;

	*bw = bandwdith;

	return mode;
}

void find_best_channel(struct scan_database * results, int mode, int bw)
{
	int i = 0, count = 0, max_rssi = -100, noise_floor = -100, best_channel = 0, rssi = 0, channel_number = 1;
	struct scan_database  *rs;
	scan_summary_t sst;

	rs = results;
	//print_results(results);
	
	if (results == NULL || mode < 0) { 
		printf("results is null or mode error\n");
		return;
	}

	sst = (struct scan_summary* )malloc(sizeof(struct scan_summary));

	if (sst == NULL) {
		printf("allocate memory fail\n");
		return;
	}
	//printf("debug 1\n");
	/* init channel */
	for(i = 0; i < 254; i ++) {
		sst->channel[i] = i + 1;
		sst->cell[i].channel = i + 1;
		sst->cell[i].count = 0;
		sst->max_rssi[i] = 0;
		sst->noise_floor[i] = 0;
	}

	rs = rs->cell_next;
	//printf("debug 2\n");

	/* calculate max rssi for every channel */
	while( channel_number > 0 && channel_number < TOTAL_DUAL_BAND_CHANNELS){
			/* get one channel max rssi */
			max_rssi = get_channel_maxrssi(results, channel_number, &count);

			/* max rssi */
			sst->max_rssi[channel_number -1] = max_rssi;

			/* total channel for this max rssi */
			sst->cell[channel_number - 1].count= count;

			/* noise floor */
			noise_floor = get_channel_noisefloor(results, channel_number);
			sst->noise_floor[channel_number -1] = noise_floor;
			
			channel_number ++;
	}

	/* print information for debug */
	//printf("debug 3\n");
	/*if(1 == debug)
	{
	    printf("probescan result:\n");
	    print_sst(sst);
	}*/

	/* 2.4G */
	if (mode == 2) {
		find_11ng_best_channel(sst, &best_channel, &rssi, bw);
	} /* 5g*/
	else if (mode == 5) {
		find_11na_best_channel(results, sst, &best_channel, &rssi, bw);
	} else {
		printf("Unknown mode: %d\n", mode);
		return ;
	}

	//printf("debug 4\n");

	//printf("Best channel: %d, avg rssi: %d\n", best_channel, rssi);

	if (sst != NULL)
		free(sst);
	
}

/*------------------------------------------------------------------*/
/*
 * Perform a scanning on one device
 */
static int
print_scanning_info(int		skfd,
		    char *	ifname,
		    char *	args[],		/* Command line args */
		    int		count)		/* Args count */
{
  struct iwreq		wrq;
  struct iw_scan_req    scanopt;		/* Options for 'set' */
  int			scanflags = 0;		/* Flags for scan */
  unsigned char *	buffer = NULL;		/* Results */
  int			buflen = IW_SCAN_MAX_DATA; /* Min for compat WE<17 */
  struct iw_range	range;
  int			has_range;
  struct timeval	tv;				/* Select timeout */
 // int			timeout = 15000000;		/* 15s */
  int			timeout = 8000000;		/* 3s */

  /* Avoid "Unused parameter" warning */
  args = args; count = count;

  /* Debugging stuff */
  if((IW_EV_LCP_PK2_LEN != IW_EV_LCP_PK_LEN) || (IW_EV_POINT_PK2_LEN != IW_EV_POINT_PK_LEN))
    {
      fprintf(stderr, "*** Please report to jt@hpl.hp.com your platform details\n");
      fprintf(stderr, "*** and the following line :\n");
      fprintf(stderr, "*** IW_EV_LCP_PK2_LEN = %zu ; IW_EV_POINT_PK2_LEN = %zu\n\n",
	      IW_EV_LCP_PK2_LEN, IW_EV_POINT_PK2_LEN);
    }

  /* Get range stuff */
  has_range = (iw_get_range_info(skfd, ifname, &range) >= 0);

  /* Check if the interface could support scanning. */
  if((!has_range) || (range.we_version_compiled < 14))
    {
      fprintf(stderr, "%-8.16s  Interface doesn't support scanning.\n\n",
	      ifname);
      return(-1);
    }

  scan_results_head = (struct scan_database *)malloc(sizeof(struct scan_database));

  if(scan_results_head == NULL) {
      fprintf(stderr, "Memory malloc fail.\n\n");
      return(-1);
  }

  memset(scan_results_head, 0, sizeof(struct scan_database));

#if 0
#ifdef FINAL_RESULT_FLAG
  final_results_2_4g = (struct channel_avgrssi_pair *)malloc(sizeof(struct channel_avgrssi_pair));
  final_results_5g = (struct channel_avgrssi_pair *)malloc(sizeof(struct channel_avgrssi_pair));

  if (final_results_2_4g== NULL ||final_results_5g == NULL ) {
	printf("allocate memory fail\n");
	return -1;
  } else {
	final_results_2_4g->avg_rssi = 0;
	final_results_2_4g->channel = 0;
	final_results_2_4g->next = NULL;
	final_results_2_4g->pre = NULL;
	final_results_5g->avg_rssi = 0;
	final_results_5g->channel =0;
	final_results_5g->next = NULL;
	final_results_5g->pre = NULL;
  }
#endif
#endif

  /* Init timeout value -> 250ms between set and first get */
  tv.tv_sec = 0;
  //tv.tv_usec = 250000;
  tv.tv_usec = 20000;

  /* Clean up set args */
  memset(&scanopt, 0, sizeof(scanopt));

  /* Parse command line arguments and extract options.
   * Note : when we have enough options, we should use the parser
   * from iwconfig... */
  while(count > 0)
    {
      /* One arg is consumed (the option name) */
      count--;
      
      /*
       * Check for Active Scan (scan with specific essid)
       */
      if(!strncmp(args[0], "essid", 5))
	{
	  if(count < 1)
	    {
	      fprintf(stderr, "Too few arguments for scanning option [%s]\n",
		      args[0]);
	      return(-1);
	    }
	  args++;
	  count--;

	  /* Store the ESSID in the scan options */
	  scanopt.essid_len = strlen(args[0]);
	  memcpy(scanopt.essid, args[0], scanopt.essid_len);
	  /* Initialise BSSID as needed */
	  if(scanopt.bssid.sa_family == 0)
	    {
	      scanopt.bssid.sa_family = ARPHRD_ETHER;
	      memset(scanopt.bssid.sa_data, 0xff, ETH_ALEN);
	    }
	  /* Scan only this ESSID */
	  scanflags |= IW_SCAN_THIS_ESSID;
	}
      else if (!strncmp(args[0], "last", 4)){
	/* Check for last scan result (do not trigger scan) */
	/* Hack */
	    scanflags |= IW_SCAN_HACK;
	    args ++;
	    count --;
      	} else if(!strncmp(args[0], "timeout", 7) && args[1] != NULL){
		channel_scan_timeout = atoi(args[1]);
		args = args + 2;
		count = count -2;
		/*printf("timeout: %d\n", channel_scan_timeout);*/
	} else if (!strncmp(args[0], "onetime", 7) && args[1] != NULL) {
		get_data_one_time = atoi(args[1]);
		args = args + 2;
		count = count -2;
		/*printf("onetime: %d\n", get_data_one_time);*/
	} else if (!strncmp(args[0], "debug", 5) && args[1] != NULL){
		debug = atoi(args[1]);
		args = args +2;
		count = count -2;
		/*printf("debug: %d\n", debug);*/
	}else {
		 fprintf(stderr, "Invalid scanning option [%s]\n", args[0]);
            	return(-1);
	}

      /* Next arg */
     // args++;
    }

   if (get_data_one_time >= channel_scan_timeout) {
		channel_scan_timeout = get_data_one_time;
   }

   timeout = channel_scan_timeout;
    tv.tv_usec = get_data_one_time;

  /* Check if we have scan options */
  if(scanflags)
    {
      wrq.u.data.pointer = (caddr_t) &scanopt;
      wrq.u.data.length = sizeof(scanopt);
      wrq.u.data.flags = scanflags;
    }
  else
    {
      scanopt.flags = 0x3; /* channel scan for liteon app */
      wrq.u.data.pointer = (caddr_t) &scanopt;
      wrq.u.data.length = sizeof(scanopt);
      wrq.u.data.flags = scanflags;
    }

  /* If only 'last' was specified on command line, don't trigger a scan */
  if(scanflags == IW_SCAN_HACK)
    {
      /* Skip waiting */
      tv.tv_usec = 0;
    }
  else
    {
      /* Initiate Scanning */
      if(iw_set_ext(skfd, ifname, SIOCSIWSCAN, &wrq) < 0)
	{
	  if((errno != EPERM) || (scanflags != 0))
	    {
	      fprintf(stderr, "%-8.16s  Interface doesn't support scanning : %s\n\n",
		      ifname, strerror(errno));
	      return(-1);
	    }
	  /* If we don't have the permission to initiate the scan, we may
	   * still have permission to read left-over results.
	   * But, don't wait !!! */

	  tv.tv_usec = 0;
	}
    }
  timeout -= tv.tv_usec;

  /* Forever */
  while(1)
    {
      fd_set		rfds;		/* File descriptors for select */
      int		last_fd;	/* Last fd */
      int		ret;

      /* Guess what ? We must re-generate rfds each time */
      FD_ZERO(&rfds);
      last_fd = -1;

      /* In here, add the rtnetlink fd in the list */

      /* Wait until something happens */
      ret = select(last_fd + 1, &rfds, NULL, NULL, &tv);

      /* Check if there was an error */
      if(ret < 0)
	{
	  if(errno == EAGAIN || errno == EINTR)
	    continue;
	  fprintf(stderr, "Unhandled signal - exiting...\n");
	  return(-1);
	}

      /* Check if there was a timeout */
      if(ret == 0)
	{
	  unsigned char *	newbuf;

	realloc:
	  /* (Re)allocate the buffer - realloc(NULL, len) == malloc(len) */
	  newbuf = realloc(buffer, buflen);
	  if(newbuf == NULL)
	    {
	      if(buffer)
		free(buffer);
	      fprintf(stderr, "%s: Allocation failed\n", __FUNCTION__);
	      return(-1);
	    }
	  buffer = newbuf;

	  /* Try to read the results */
	  wrq.u.data.pointer = buffer;
	  wrq.u.data.flags = 1;
	  wrq.u.data.length = buflen;
	  if(iw_get_ext(skfd, ifname, SIOCGIWSCAN, &wrq) < 0)
	    {
	      /* Check if buffer was too small (WE-17 only) */
	      if((errno == E2BIG) && (range.we_version_compiled > 16))
		{
		  /* Some driver may return very large scan results, either
		   * because there are many cells, or because they have many
		   * large elements in cells (like IWEVCUSTOM). Most will
		   * only need the regular sized buffer. We now use a dynamic
		   * allocation of the buffer to satisfy everybody. Of course,
		   * as we don't know in advance the size of the array, we try
		   * various increasing sizes. Jean II */

		  /* Check if the driver gave us any hints. */
		  if(wrq.u.data.length > buflen)
		    buflen = wrq.u.data.length;
		  else
		    buflen *= 2;

		  /* Try again */
		  goto realloc;
		}

	      /* Check if results not available yet */
	      if(errno == EAGAIN)
		{
		 //printf("Resulsts not available, restart timer: 50ms\n");
		  /* Restart timer for only 100ms*/
		  tv.tv_sec = 0;
		  //tv.tv_usec = 100000;
		  tv.tv_usec = 20000;
		  timeout -= tv.tv_usec;
		  if(timeout > 0)
		    continue;	/* Try again later */
		}

	      /* Bad error */
	      free(buffer);
	      fprintf(stderr, "%-8.16s  Failed to read scan data : %s\n\n",
		      ifname, strerror(errno));
	      return(-2);
	    }
	  else
	    /* We have the results, go to process them */
	    break;
	}

      /* In here, check if event and event type
       * if scan event, read results. All errors bad & no reset timeout */
    }

  if(wrq.u.data.length)
    {
      struct iw_event		iwe;
      struct stream_descr	stream;
      struct iwscan_state	state = { .ap_num = 1, .val_index = 0 };
      int			ret, count = 0;
      struct scan_database *cell_scan_db = NULL, *p = NULL;//, *q = NULL;
      p = scan_results_head;
      char* mac_addr[32];
      char* tmp_mac_p = NULL;
      int signal_level, noise_floor, channel, denominator,numerator;

     tmp_mac_p = (char *)mac_addr;
    if(debug == 3) {
	      /* Debugging code. In theory useless, because it's debugged ;-) */
	      int	i;
	      printf("Scan result %d [%02X", wrq.u.data.length, buffer[0]);
	      for(i = 1; i < wrq.u.data.length; i++)
		  printf(":%02X", buffer[i]);
	      printf("]\n");
     }
     if (debug == 2) 
      	printf("%-8.16s  Scan completed :\n", ifname);
	 
      iw_init_event_stream(&stream, (char *) buffer, wrq.u.data.length);
      do
	{
		  /* Extract an event and print it */
	  ret = iw_extract_event_stream(&stream, &iwe,
					range.we_version_compiled);
	  if(ret > 0) {		  	
		    /*print_scanning_token(&stream, &iwe, &state,
					 &range, has_range);*/
			
		   save_scanning_data(&stream, &iwe, &state,
					 &range, has_range, tmp_mac_p, &channel, &signal_level, &noise_floor, &denominator, &numerator);
		   switch(iwe.cmd)
		   {
			case SIOCGIWAP:
				cell_scan_db = (struct scan_database* )malloc(sizeof(struct scan_database));
				if (cell_scan_db == NULL) {
					printf("allocate memory fail\n");
					return -1;
				}
				memset(cell_scan_db, 0, sizeof(struct scan_database));
				memcpy(cell_scan_db->cell_mac_addr, mac_addr, 23);
				break;
			case SIOCGIWFREQ:
				 cell_scan_db->cell_frequency = channel;
				break;
			case IWEVQUAL:
				 cell_scan_db->cell_noise_floor_level = noise_floor;
				 cell_scan_db->cell_signal_level = signal_level;
				 cell_scan_db->cell_quality_pair.denominator = denominator;
				 cell_scan_db->cell_quality_pair.numerator = numerator;
				 
				 if(strcmp(cell_scan_db->cell_mac_addr, "")) {
			   	   	count ++;
					/* first element not used, it viewed as linklist header */
				   	if (p == scan_results_head) {
						p->cell_next = cell_scan_db;
						p->cell_pre = NULL;
						cell_scan_db->cell_next = NULL;
						cell_scan_db->cell_pre = p;
						p = p->cell_next;
				   	} else {
						p->cell_next = cell_scan_db;
						cell_scan_db->cell_next = NULL;
						cell_scan_db->cell_pre = p;
						p=p->cell_next;
				     	}
				   	p->cell_number = count;
    				}
				break;
		     }
	    }
	}while(ret > 0);
	
      #if 0
      printf("\n");
      #endif
    }
  else
    printf("%-8.16s  No scan results\n\n", ifname);

  free(buffer);

  /* Debug printf information */
  if(1 == debug)
  {
      print_results(scan_results_head);
  }
  
  {
  	int mode = -1, bw = 0;
	mode = radio_mode(ifname, &bw);
	//printf("mode: %s, bw: %d\n", mode == 2?"2.4G":"5G", bw);
  	find_best_channel(scan_results_head, mode, bw);
  }

  /* free dynamic memory */
  if (scan_results_head != NULL) {
  	free_linklist_results(scan_results_head);
  }

#if 0
#ifdef FINAL_RESULT_FLAG
  print_final_results(final_results_2_4g);
  print_final_results(final_results_5g);


  if (final_results_2_4g != NULL)
  	free_linklist_final_results(final_results_2_4g);

  if (final_results_5g != NULL)
  	free_linklist_final_results(final_results_5g);
#endif
#endif
  
  return(0);
}

/*********************** FREQUENCIES/CHANNELS ***********************/

/*------------------------------------------------------------------*/
/*
 * Print the number of channels and available frequency for the device
 */
static int
print_freq_info(int		skfd,
		char *		ifname,
		char *		args[],		/* Command line args */
		int		count)		/* Args count */
{
  struct iwreq		wrq;
  struct iw_range	range;
  double		freq;
  int			k;
  int			channel;
  char			buffer[128];	/* Temporary buffer */

  /* Avoid "Unused parameter" warning */
  args = args; count = count;

  /* Get list of frequencies / channels */
  if(iw_get_range_info(skfd, ifname, &range) < 0)
      fprintf(stderr, "%-8.16s  no frequency information.\n\n",
		      ifname);
  else
    {
      if(range.num_frequency > 0)
	{
	  printf("%-8.16s  %d channels in total; available frequencies :\n",
		 ifname, range.num_channels);
	  /* Print them all */
	  for(k = 0; k < range.num_frequency; k++)
	    {
	      freq = iw_freq2float(&(range.freq[k]));
	      iw_print_freq_value(buffer, sizeof(buffer), freq);
	      printf("          Channel %.2d : %s\n",
		     range.freq[k].i, buffer);
	    }
	}
      else
	printf("%-8.16s  %d channels\n",
	       ifname, range.num_channels);

      /* Get current frequency / channel and display it */
      if(iw_get_ext(skfd, ifname, SIOCGIWFREQ, &wrq) >= 0)
	{
	  freq = iw_freq2float(&(wrq.u.freq));
	  channel = iw_freq_to_channel(freq, &range);
	  iw_print_freq(buffer, sizeof(buffer),
			freq, channel, wrq.u.freq.flags);
	  printf("          Current %s\n\n", buffer);
	}
    }
  return(0);
}

/************************* COMMON UTILITIES *************************/
/*
 * This section was initially written by Michael Tokarev <mjt@tls.msk.ru>
 * but heavily modified by me ;-)
 */

/*------------------------------------------------------------------*/
/*
 * Map command line arguments to the proper procedure...
 */
typedef struct iwlist_entry {
  const char *		cmd;		/* Command line shorthand */
  iw_enum_handler	fn;		/* Subroutine */
  int			max_count;
  const char *		argsname;	/* Args as human readable string */
} iwlist_cmd;

static const struct iwlist_entry iwlist_cmds[] = {
  { "scanning",		print_scanning_info,	-1, "[essid NNN] [last]" },
  //{ "channel",		print_freq_info,	0, NULL },
  { NULL, NULL, 0, 0 },
};

/*------------------------------------------------------------------*/
/*
 * Find the most appropriate command matching the command line
 */
static inline const iwlist_cmd *
find_command(const char *	cmd)
{
  const iwlist_cmd *	found = NULL;
  int			ambig = 0;
  unsigned int		len = strlen(cmd);
  int			i;

  /* Go through all commands */
  for(i = 0; iwlist_cmds[i].cmd != NULL; ++i)
    {
      /* No match -> next one */
      if(strncasecmp(iwlist_cmds[i].cmd, cmd, len) != 0)
	continue;

      /* Exact match -> perfect */
      if(len == strlen(iwlist_cmds[i].cmd))
	return &iwlist_cmds[i];

      /* Partial match */
      if(found == NULL)
	/* First time */
	found = &iwlist_cmds[i];
      else
	/* Another time */
	if (iwlist_cmds[i].fn != found->fn)
	  ambig = 1;
    }

  if(found == NULL)
    {
      fprintf(stderr, "iwlist: unknown command `%s' (check 'iwlist --help').\n", cmd);
      return NULL;
    }

  if(ambig)
    {
      fprintf(stderr, "iwlist: command `%s' is ambiguous (check 'iwlist --help').\n", cmd);
      return NULL;
    }

  return found;
}

/*------------------------------------------------------------------*/
/*
 * Display help
 */
static void iw_usage(int status)
{
  FILE *		f = status ? stderr : stdout;
  int			i;

  for(i = 0; iwlist_cmds[i].cmd != NULL; ++i)
    {
      fprintf(f, "%s [interface] %s %s\n",
	      (i ? "             " : "Usage: channel_scan"),
	      iwlist_cmds[i].cmd,
	      iwlist_cmds[i].argsname ? iwlist_cmds[i].argsname : "");
    }

  exit(status);
}

/******************************* MAIN ********************************/

/*------------------------------------------------------------------*/
/*
 * The main !
 */
#ifdef SPECTRAL_SCAN_FLAG
int
probe_scan_main(int	argc,
     char **	argv)
#else
int
main(int	argc,
     char **	argv)
#endif
{
  int skfd;			/* generic raw socket desc.	*/
  char *dev;			/* device name			*/
  char *cmd;			/* command			*/
  char **args;			/* Command arguments */
  int count;			/* Number of arguments */
  const iwlist_cmd *iwcmd;

  if(argc < 2)
    iw_usage(1);

  /* Those don't apply to all interfaces */
  if((argc == 2) && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")))
    iw_usage(0);
  if((argc == 2) && (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version")))
    return(iw_print_version_info("iwlist"));

  if(argc == 2)
    {
      cmd = argv[1];
      dev = NULL;
      args = NULL;
      count = 0;
    }
  else
    {
      cmd = argv[2];
      dev = argv[1];
      args = argv + 3;
      count = argc - 3;
    }

  /* find a command */
  iwcmd = find_command(cmd);
  if(iwcmd == NULL)
    return 1;

  /* Check arg numbers */
  if((iwcmd->max_count >= 0) && (count > iwcmd->max_count))
    {
      fprintf(stderr, "iwlist: command `%s' needs fewer arguments (max %d)\n",
	      iwcmd->cmd, iwcmd->max_count);
      return 1;
    }

  /* Create a channel to the NET kernel. */
  if((skfd = iw_sockets_open()) < 0)
    {
      perror("socket");
      return -1;
    }
  /* do the actual work */
  if (dev)
    (*iwcmd->fn)(skfd, dev, args, count);
  else
    iw_enum_devices(skfd, iwcmd->fn, args, count);

  /* Close the socket. */
  iw_sockets_close(skfd);
  
  return 0;
}

#ifdef SPECTRAL_SCAN_FLAG
int initProbeScan()
{
  final_results_2_4g = (struct channel_avgrssi_pair *)malloc(sizeof(struct channel_avgrssi_pair));
  final_results_5g = (struct channel_avgrssi_pair *)malloc(sizeof(struct channel_avgrssi_pair));

  if (final_results_2_4g== NULL ||final_results_5g == NULL ) {
	printf("allocate memory fail\n");
	return -1;
  } else {
	final_results_2_4g->avg_rssi = 0;
	final_results_2_4g->channel = 0;
	final_results_2_4g->next = NULL;
	final_results_2_4g->pre = NULL;
	final_results_5g->avg_rssi = 0;
	final_results_5g->channel =0;
	final_results_5g->next = NULL;
	final_results_5g->pre = NULL;
  }
  return 0;
}

void printProbeScan()
{
    print_final_results(final_results_2_4g);
    print_final_results(final_results_5g);
}

void releaseProbeScan()
{
    if (final_results_2_4g != NULL)
    {
      free_linklist_final_results(final_results_2_4g);
      final_results_2_4g = NULL;
    }

    if (final_results_5g != NULL)
    {
      free_linklist_final_results(final_results_5g);
      final_results_5g = NULL;
    }
}

channel_avgrssi_pair_t getProbeResult2_4G()
{
    return final_results_2_4g;
}

channel_avgrssi_pair_t getProbeResult5G()
{
    return final_results_5g;
}

#endif

