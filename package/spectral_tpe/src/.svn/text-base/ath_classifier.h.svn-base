/*
 * =====================================================================================
 *
 *       Filename:  ath_classifier.h
 *
 *    Description:  Classifier
 *
 *        Version:  1.0
 *        Created:  12/26/2011 11:16:42 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  S.Karthikeyan (),
 *        Company:  Qualcomm Atheros
 *
 *        Copyright (c) 2012 Qualcomm Atheros, Inc.
 *        All Rights Reserved
 *        Qualcomm Atheros Confidential and Proprietary
 *
 *
 * =====================================================================================
 */

#ifndef _ATH_CLASSIFIER_H_
#define _ATH_CLASSIFIER_H_

#include "spectral_types.h"
#include "spectral_data.h"
#include <stdbool.h>

/*
 * TODO : MAX_FFT_BINS Should be changed to accomdate 802.11ac
 *        chips like Peregrine, ROme
 */

#define SPECTRAL_DBG_LOG_SAMP   1024
#define MAX_FFT_BINS            512

#ifndef TRUE
#define TRUE    (1)
#endif

#ifndef FALSE
#define FALSE   !(TRUE)
#endif

#define NUM_FFT_BINS_20MHZ  64
#define NUM_FFT_BINS_40MHZ  128
#define NUM_FFT_BINS_80MHZ  256

#define CLASSIFIER_DEBUG 0
#if CLASSIFIER_DEBUG
#define cinfo(fmt, args...) do {\
    printf("classifier: %s (%4d) : " fmt "\n", __func__, __LINE__, ## args); \
    } while (0)
#else
#define cinfo(fmt, args...)
#endif



/* XXX : This should be kept in sync with driver values */
enum ieee80211_cwm_width {
    IEEE80211_CWM_WIDTH20,
    IEEE80211_CWM_WIDTH40,
    IEEE80211_CWM_WIDTH80,
    IEEE80211_CWM_WIDTHINVALID = 0xff    /* user invalid value */
};

typedef u_int32_t DETECT_MODE;

#define LOG_NONE        (0)
#define LOG_MWO         (1)
#define LOG_CW          (2)
#define LOG_WIFI        (3)
#define LOG_FHSS        (4)
#define LOG_ALL         (5)

#define SPECT_CLASS_DETECT_NONE          0
#define SPECT_CLASS_DETECT_MWO           0x1
#define SPECT_CLASS_DETECT_CW            0x2
#define SPECT_CLASS_DETECT_WiFi          0x4
#define SPECT_CLASS_DETECT_CORDLESS_24   0x8
#define SPECT_CLASS_DETECT_CORDLESS_5    0x10
#define SPECT_CLASS_DETECT_BT            0x20
#define SPECT_CLASS_DETECT_FHSS          0x40
#define SPECT_CLASS_DETECT_ALL           0xff

#define NUM_FHSS_BINS (10)

#define CLASSIFIER_HASHSIZE 32
#define MAC_ADDR_LEN        6
#define CLASSIFIER_HASH(addr)   \
    ((((const u_int8_t *)(addr))[MAC_ADDR_LEN - 1] ^ \
      ((const u_int8_t *)(addr))[MAC_ADDR_LEN - 2] ^ \
      ((const u_int8_t *)(addr))[MAC_ADDR_LEN - 3] ^ \
      ((const u_int8_t *)(addr))[MAC_ADDR_LEN - 4]) % CLASSIFIER_HASHSIZE)

typedef struct _fhss_detect_param_ {
    u_int32_t start_ts;
    u_int32_t last_ts;
    u_int32_t delta;
    u_int16_t freq_bin;
    u_int16_t rssi;
    u_int16_t num_samp;
    u_int16_t in_use;
} spectral_fhss_param;

typedef struct _mwo_detect_param_ {
    u_int32_t start_ts;
    u_int32_t last_ts;
    u_int32_t delta;
    u_int32_t off_time;
    u_int16_t rssi;
    u_int16_t num_samp;
    u_int16_t in_use;
} spectral_mwo_param;
#define NUM_MWO_BINS (10)

typedef enum _spectral_scan_band {
    SCAN_NONE   = 0,
    SCAN_24GHZ  = 1,
    SCAN_5GHZ   = 2,
    SCAN_ALL    = 3,
} SPECTRAL_SCAN_BAND;

/* Thresholds that are different between legacy (11n)
   and 11ac chipsets */
typedef struct _classifier_thresholds {
    /* CW interference detection parameters */
    u_int32_t cw_int_det_thresh;
    
    /* Wi-Fi detection parameters */
    int wifi_det_min_diff;
    
    /* FHSS detection parameters */
    u_int32_t fhss_sum_scale_down_factor;
} CLASSIFIER_THRESHOLDS;

typedef struct _CLASSIFIER_DATA_STRUCT {

    int is_valid;                           /* indicates if the contents are valid */
    u_int8_t macaddr[MAC_ADDR_LEN];         /* associated MAC address */

    DETECT_MODE spectral_detect_mode;
    SPECTRAL_SCAN_BAND band;

    u_int16_t sm_init_done;
    u_int16_t cur_freq;

    /* MWO detect */
    u_int32_t mwo_burst_idx;
    u_int32_t mwo_burst_found;
    u_int32_t mwo_cur_burst_cnt;
    u_int32_t mwo_burst_start_time;
    u_int32_t mwo_pre_burst_time;
    u_int32_t mwo_thresh;
    u_int32_t mwo_rssi;

    spectral_mwo_param mwo_param[NUM_MWO_BINS];
    u_int16_t mwo_cur_bin;

    /* Differentiated thresholds to be used,
       populated as per whether legacy (11n)
       or advanced (11ac) Spectral capability
       is available */
    CLASSIFIER_THRESHOLDS thresholds;

    /* CW detection */
    u_int16_t cw_burst_found;
    u_int32_t cw_start_time;
    u_int32_t cw_last_found_time;
    u_int16_t cw_rssi;
    u_int16_t cw_num_detected;

    /* WiFi detection */
    u_int32_t spectral_num_wifi_detected;
    u_int32_t spectral_wifi_ts;
    u_int32_t wifi_rssi;

    /* FHSS detection */
    spectral_fhss_param fhss_param[NUM_FHSS_BINS];
    u_int16_t fhss_cur_bin;

    /* Overall detection */
    DETECT_MODE current_interference;

    u_int32_t mwo_detect_ts;
    u_int16_t mwo_num_detect;

    u_int32_t cw_detect_ts;
    u_int16_t cw_num_detect;

    u_int32_t wifi_detect_ts;
    u_int16_t wifi_num_detect;

    u_int32_t fhss_detect_ts;
    u_int32_t fhss_num_detect;
    u_int32_t fhss_dwell_time;

    u_int32_t dsss_detect_ts;
    u_int32_t dsss_num_detect;

    /* Total count of the detected interference */
    u_int32_t cw_cnt;
    u_int32_t wifi_cnt;
    u_int32_t fhss_cnt;
    u_int32_t mwo_cnt;

    /* Debug stuff only */
    u_int16_t spectral_log_first_time;
    u_int16_t spectral_num_samp_log;
    u_int16_t commit_done;
    u_int8_t *spectral_bin_bufSave;
    int32_t *spectral_data_misc;
    u_int16_t spectral_log_num_bin;
    u_int16_t last_samp;
    u_int16_t log_mode;
} CLASSIFER_DATA_STRUCT;


#define IS_MWO_DETECTED(p)          ((p->current_interference & SPECT_CLASS_DETECT_MWO)?1:0)
#define IS_CW_DETECTED(p)           ((p->current_interference & SPECT_CLASS_DETECT_CW)?1:0)
#define IS_WiFi_DETECTED(p)         ((p->current_interference & SPECT_CLASS_DETECT_WiFi)?1:0)
#define IS_CORDLESS_24_DETECTED(p)  ((p->current_interference & SPECT_CLASS_DETECT_CORDLESS_24)?1:0)
#define IS_CORDLESS_5_DETECTED(p)   ((p->current_interference & SPECT_CLASS_DETECT_CORDLESS_5)?1:0)
#define IS_BT_DETECTED(p)           ((p->current_interference & SPECT_CLASS_DETECT_BT)?1:0)
#define IS_FHSS_DETECTED(p)         ((p->current_interference & SPECT_CLASS_DETECT_FHSS)?1:0)

#define SET_INTERFERENCE(p, type)   (p->current_interference |=  type)
#define CLR_INTERFERENCE(p, type)   (p->current_interference &= ~(type))



/* Function declarations */
extern void classifier_process_spectral_msg(SPECTRAL_SAMP_MSG *msg, CLASSIFER_DATA_STRUCT *pclas, u_int16_t log_type);
extern void print_detected_interference(CLASSIFER_DATA_STRUCT* pclas);
extern void print_spect_int_stats(void);
extern const char* ether_sprintf(const u_int8_t *mac);
extern void init_classifier_data(const u_int8_t* macaddr, bool is_advncd_spectral);
extern CLASSIFER_DATA_STRUCT* get_classifier_data(const u_int8_t* macaddr);
extern int check_wifi_signal(CLASSIFER_DATA_STRUCT* pclas, u_int32_t num_bins, u_int8_t* pfft_bins, u_int32_t ch_width);

/* --- Default spectral classifier parameters */

/* MWO parameters */
/* Minimum and maximum frequency to check for MWO interference */
#define MIN_MWO_FREQ                (2437)
#define MAX_MWO_FREQ                (2482)
#define MIN_RSS_TO_DETECT           (5)
#define MWO_INT_BIN_SUM_SIZE        (3)
#define MWO_INT_DET_THRESH          (200)
#define MWO_MAX_GAP_WITHIN_BURST    (6*1000)
#define MWO_MAX_BURST_TIME          (12*2000)
#define MWO_MIN_DUTY_CYCLE          (6*1000)
#define MWO_MAX_DUTY_CYCLE          (12*1000)
#define MWO_SECOND_DET_THRESH       (1500*1000)
#define MWO_DETECT_CONFIRM_COUNT    (2)
#define MWO_CONFIRM_MISSING_TIME    (10000*1000)

/* CW interference detection parameters */
#define CW_RSSI_THRESH              (10)
#define CW_INT_BIN_SUM_SIZE         (3) /* Should always be 3. Some design changes needed for other number */
#define CW_INT_DET_THRESH           (200)
#define CW_INT_FOUND_TIME_THRESH    (15*1000)
#define CW_INT_FOUND_MIN_CNT        (50)
#define CW_INT_MISSING_THRESH       (1000)
#define CW_INT_CONFIRM_WIN          (100*1000)
#define CW_INT_CONFIRM_MISSING_WIN  (2000*1000)
#define CW_SUM_SCALE_DOWN_FACTOR    (2)

/* WiFI detection parameters */
#define WIFI_DET_MIN_RSSI           (10)
#define WIFI_DET_MIN_DIFF           (200)
#define WIFI_BIN_WIDTH              (4)
#define WIFI_DET_CONFIRM_WIN        (500*1000)
#define WIFI_DET_RESET_TIME         (5000*1000)
#define WIFI_MIN_NUM_DETECTS        (2)

/* FHSS detection parameters */
#define FHSS_DET_THRESH             (10)
#define FHSS_INT_BIN_SUM_SIZE       (3)  /* NOTE: Should always be 3, else will require design change */
#define FHSS_CENTER_THRESH          (100)
#define FHSS_MIN_DWELL_TIME         (500)
#define FHSS_SINGLE_BURST_TIME      (15*1000)
#define FHSS_LACK_OF_BURST_TIME     (3*150*1000)
#define FHSS_DETECTION_CONFIRM_WIN  (5*1000*1000)
#define FHSS_DETECTION_RESET_WIN    (10*1000*1000)
#define FHSS_SUM_SCALE_DOWN_FACTOR  (2)


/* Differentiated values for 11ac chipsets having advanced spectral
   capability */

/* CW interference detection parameters */
#define ADVNCD_CW_INT_DET_THRESH           (60)

/* WiFI detection parameters */
#define ADVNCD_WIFI_DET_MIN_DIFF           (20)

/* FHSS detection parameters */
#define ADVNCD_FHSS_SUM_SCALE_DOWN_FACTOR  (0)

/* Macros to access thresholds */

#define ACCESS_THRESHOLD(pclas)                   (pclas->thresholds)

#define GET_MIN_MWO_FREQ(pclas)                   (MIN_MWO_FREQ)
#define GET_MAX_MWO_FREQ(pclas)                   (MAX_MWO_FREQ)
#define GET_MIN_RSS_TO_DETECT(pclas)              (MIN_RSS_TO_DETECT)
#define GET_MWO_INT_BIN_SUM_SIZE(pclas)           (MWO_INT_BIN_SUM_SIZE)
#define GET_MWO_INT_DET_THRESH(pclas)             (MWO_INT_DET_THRESH)
#define GET_MWO_MAX_GAP_WITHIN_BURST(pclas)       (MWO_MAX_GAP_WITHIN_BURST)
#define GET_MWO_MAX_BURST_TIME(pclas)             (MWO_MAX_BURST_TIME)
#define GET_MWO_MIN_DUTY_CYCLE(pclas)             (MWO_MIN_DUTY_CYCLE)
#define GET_MWO_MAX_DUTY_CYCLE(pclas)             (MWO_MAX_DUTY_CYCLE)
#define GET_MWO_SECOND_DET_THRESH(pclas)          (MWO_SECOND_DET_THRESH)
#define GET_MWO_DETECT_CONFIRM_COUNT(pclas)       (MWO_DETECT_CONFIRM_COUNT)
#define GET_MWO_CONFIRM_MISSING_TIME(pclas)       (MWO_CONFIRM_MISSING_TIME)

#define GET_CW_RSSI_THRESH(pclas)                 (CW_RSSI_THRESH)
#define GET_CW_INT_BIN_SUM_SIZE(pclas)            (CW_INT_BIN_SUM_SIZE)
#define GET_CW_INT_DET_THRESH(pclas)              (ACCESS_THRESHOLD((pclas)).cw_int_det_thresh)
#define GET_CW_INT_FOUND_TIME_THRESH(pclas)       (CW_INT_FOUND_TIME_THRESH)
#define GET_CW_INT_FOUND_MIN_CNT(pclas)           (CW_INT_FOUND_MIN_CNT)
#define GET_CW_INT_MISSING_THRESH(pclas)          (CW_INT_MISSING_THRESH)
#define GET_CW_INT_CONFIRM_WIN(pclas)             (CW_INT_CONFIRM_WIN)
#define GET_CW_INT_CONFIRM_MISSING_WIN(pclas)     (CW_INT_CONFIRM_MISSING_WIN)
#define GET_CW_SUM_SCALE_DOWN_FACTOR(pclas)       (CW_SUM_SCALE_DOWN_FACTOR)

#define GET_WIFI_DET_MIN_RSSI(pclas)              (WIFI_DET_MIN_RSSI)
#define GET_WIFI_DET_MIN_DIFF(pclas)              (ACCESS_THRESHOLD((pclas)).wifi_det_min_diff)
#define GET_WIFI_BIN_WIDTH(pclas)                 (WIFI_BIN_WIDTH)
#define GET_WIFI_DET_CONFIRM_WIN(pclas)           (WIFI_DET_CONFIRM_WIN)
#define GET_WIFI_DET_RESET_TIME(pclas)            (WIFI_DET_RESET_TIME)

#define GET_FHSS_DET_THRESH(pclas)                (FHSS_DET_THRESH)
#define GET_FHSS_INT_BIN_SUM_SIZE(pclas)          (FHSS_INT_BIN_SUM_SIZE)
#define GET_FHSS_CENTER_THRESH(pclas)             (FHSS_CENTER_THRESH)
#define GET_FHSS_MIN_DWELL_TIME(pclas)            (FHSS_MIN_DWELL_TIME)
#define GET_FHSS_SINGLE_BURST_TIME(pclas)         (FHSS_SINGLE_BURST_TIME)
#define GET_FHSS_LACK_OF_BURST_TIME(pclas)        (FHSS_LACK_OF_BURST_TIME)
#define GET_FHSS_DETECTION_CONFIRM_WIN(pclas)     (FHSS_DETECTION_CONFIRM_WIN)
#define GET_FHSS_DETECTION_RESET_WIN(pclas)       (FHSS_DETECTION_RESET_WIN)
#define GET_FHSS_SUM_SCALE_DOWN_FACTOR(pclas)     (ACCESS_THRESHOLD((pclas)).fhss_sum_scale_down_factor)

#endif  /* _ATH_CLASSIFIER_H_ */

