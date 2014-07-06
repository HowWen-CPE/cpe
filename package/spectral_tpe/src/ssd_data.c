/*
 * =====================================================================================
 *
 *       Filename:  ssd_data.c
 *
 *    Description:  Spectral Scan data handler 
 *
 *        Version:  1.0
 *        Created:  11/22/2011 04:44:34 PM
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

#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>

#include "ssd_defs.h"
#include "ssd_proto.h"
#include "ssd_data.h"
#include "ssd_utils.h"
#include "ssd_interf.h"
#include "classifier.h"


/*
 * Function     : ssd_update_silent_samp_msg
 * Description  : updates the silent message, which is sent when there is no data form
 *                spectral driver
 * Input params : pointer to ssd_info_t, current frequency
 * Return       : void
 *
 */
void ssd_update_silent_samp_msg(ssd_info_t *pinfo, int freq)
{
    SPECTRAL_SAMP_MSG *ss_msg = GET_SILENT_SAMP_MSG_PTR(pinfo);
    ss_msg->freq = freq;
    ss_msg->freq_loading = 0;
}

/*
 * Function     : ssd_init_silent_samp_msg
 * Description  : initializes the silent samp msg to default values
 * Input params : pointer to ssd_info_t
 * Return       : void
 *
 */
void ssd_init_silent_samp_msg(ssd_info_t *pinfo)
{
    int i = 0;
    ssd_config_t            *pconfig     = GET_CONFIG_PTR(pinfo);
    struct INTERF_SRC_RSP   *interf_resp = NULL;
    SPECTRAL_SAMP_MSG       *ss_msg      = GET_SILENT_SAMP_MSG_PTR(pinfo);
    SPECTRAL_SAMP_DATA      *ss_data     = &ss_msg->samp_data;

    /* reset the frequency information */
    ss_msg->signature = SPECTRAL_SIGNATURE;
    ss_msg->freq = 0;
    ss_msg->freq_loading = 0;

    ss_data->spectral_data_len = sizeof(SPECTRAL_SAMP_MSG);

    /*
     * When sending a "fake/silent" SAMP message, set all the RSSI fields
     * in the SAMP message to the minpwr level passed in at the command line.
     * Also set each bin power level to 255
     */
    ss_data->spectral_rssi              = pconfig->minpower;
    ss_data->spectral_combined_rssi     = pconfig->minpower;
    ss_data->spectral_upper_rssi        = pconfig->minpower;
    ss_data->spectral_lower_rssi        = pconfig->minpower;
    ss_data->spectral_max_scale         = 0;
    ss_data->spectral_bwinfo            = 0;
    ss_data->spectral_tstamp            = 0;
    ss_data->spectral_max_index         = 0;
    ss_data->spectral_max_mag           = 0;
    ss_data->spectral_max_exp           = 0;
    ss_data->spectral_last_tstamp       = 0;
    ss_data->spectral_upper_max_index   = 0;
    ss_data->spectral_lower_max_index   = 0;
    ss_data->spectral_nb_lower          = 0;
    ss_data->spectral_nb_upper          = 0;
    ss_data->bin_pwr_count              = BIN_POWER_COUNT;

    /* reset the power bin information */
    for (i = 0; i < BIN_POWER_COUNT; i++) {
        ss_data->bin_pwr[i] = GUI_IGNORE_THIS_BIN_VALUE;
    }

    /* reset the interference data */
    interf_resp = (struct INTERF_SRC_RSP*)(&ss_data->interf_list);
    memset((void *)(interf_resp), 0, sizeof(struct INTERF_SRC_RSP));
    interf_resp->count = htons(0);
}

/*
 * Function     : ssd_clear_max_rssi_data
 * Description  : clears the max rssi related msg
 * Input params : pointer ssd_info_t
 * Return       : void
 *
 */
void ssd_clear_max_rssi_data(ssd_info_t* pinfo)
{
    ssd_samp_info_t *psinfo = GET_SAMP_INFO_PTR(pinfo);

    memset(&psinfo->lower_max_rssi_msg, 0, sizeof(SPECTRAL_SAMP_MSG));
    memset(&psinfo->upper_max_rssi_msg, 0, sizeof(SPECTRAL_SAMP_MSG));

    psinfo->lower_maxrssi       = DEFAULT_MAX_RSSI;
    psinfo->upper_maxrssi       = DEFAULT_MAX_RSSI;
    psinfo->lower_last_tstamp   = 0;
    psinfo->upper_last_tstamp   = 0;
}


/*
 * Function     : print_samp_msg
 * Description  : print SAMP msg
 * Input params :
 * Return       :
 *
 */
void print_samp_msg(SPECTRAL_SAMP_MSG *pmsg)
{
    printf("freq    = %d\n", pmsg->freq);
}

#define line()          printf("----------------------------------------------------\n")

/*
 * Function     : dbg_print_SAMP_msg
 * Description  : Print contents of SAMP Message
 * Input        : Pointer to SAMP message
 * Output       : Void
 *
 */
void dbg_print_SAMP_msg(SPECTRAL_SAMP_MSG* ss_msg)
{
    int i = 0;

    SPECTRAL_SAMP_DATA *p = &ss_msg->samp_data;
    SPECTRAL_CLASSIFIER_PARAMS *pc = &p->classifier_params;
    struct INTERF_SRC_RSP  *pi = &p->interf_list;

    line();
    printf("Spectral Message\n");
    line();
    printf("Signature   :   0x%x\n", ss_msg->signature);
    printf("Freq        :   %d\n", ss_msg->freq);
    printf("Freq load   :   %d\n", ss_msg->freq_loading);
    printf("CW Inter    :   %d\n", ss_msg->cw_interferer);
    line();
    printf("Spectral Data info\n");
    line();
    printf("data length     :   %d\n", p->spectral_data_len);
    printf("rssi            :   %d\n", p->spectral_rssi);
    printf("combined rssi   :   %d\n", p->spectral_combined_rssi);
    printf("upper rssi      :   %d\n", p->spectral_upper_rssi);
    printf("lower rssi      :   %d\n", p->spectral_lower_rssi);
    printf("bw info         :   %d\n", p->spectral_bwinfo);
    printf("timestamp       :   %d\n", p->spectral_tstamp);
    printf("max index       :   %d\n", p->spectral_max_index);
    printf("max exp         :   %d\n", p->spectral_max_exp);
    printf("max mag         :   %d\n", p->spectral_max_mag);
    printf("last timstamp   :   %d\n", p->spectral_last_tstamp);
    printf("upper max idx   :   %d\n", p->spectral_upper_max_index);
    printf("lower max idx   :   %d\n", p->spectral_lower_max_index);
    printf("bin power count :   %d\n", p->bin_pwr_count);
    line();
    printf("Classifier info\n");
    line();
    printf("20/40 Mode      :   %d\n", pc->spectral_20_40_mode);
    printf("dc index        :   %d\n", pc->spectral_dc_index);
    printf("dc in MHz       :   %d\n", pc->spectral_dc_in_mhz);
    printf("upper channel   :   %d\n", pc->upper_chan_in_mhz);
    printf("lower channel   :   %d\n", pc->lower_chan_in_mhz);
    line();
    printf("Interference info\n");
    line();
    printf("inter count     :   %d\n", pi->count);

    for (i = 0; i < pi->count; i++) {
        printf("inter type  :   %d\n", pi->interf[i].interf_type);
        printf("min freq    :   %d\n", pi->interf[i].interf_min_freq);
        printf("max freq    :   %d\n", pi->interf[i].interf_max_freq);
    }


}

/*
 * Function     : ssd_process_spectral_msg
 * Description  : process the spectral data from spectral driver
 * Input params : pointer to ssd_info_t, pointer to spectral message
 * Return       : void
 *
 */
void ssd_process_spectral_msg(ssd_info_t *pinfo, SPECTRAL_SAMP_MSG* msg)
{
    SPECTRAL_SAMP_DATA *ss_data;

    static int initialized = FALSE;
    static int curr_freq = CHANNEL_01_FREQ;

    u_int32_t is_ht2040         = 0;
    u_int32_t prev_time_stamp   = 0;
    u_int32_t primary_rssi      = 0;


    int narrowband_lwr  = 0;
    int narrowband_upr  = 0;
    int peak_index      = 0;

    //printf("~~~ssd_process_spectral_msg 1~~~");
    if (msg->signature != SPECTRAL_SIGNATURE) {
        return;
    }

    ss_data = &msg->samp_data;

    is_ht2040       = ss_data->spectral_data_len > 100 ? 1:0;
    prev_time_stamp = ss_data->spectral_last_tstamp;

    /* mute compiler warning */
    prev_time_stamp = prev_time_stamp;
    narrowband_upr  = ss_data->spectral_nb_upper;
    narrowband_lwr  = ss_data->spectral_nb_lower;

    peak_index  =   ss_data->spectral_lower_max_index;
    primary_rssi =  ss_data->spectral_lower_rssi;


    if ((pinfo->config.eacs) && (msg->cw_interferer == 1)) {
        not_yet(); //daniel comment:we can realize as 854 just system call
        return;
    }


    /*
     * Init the classifier data only once or when the frequency changes
     */

    if (!initialized || (msg->freq != curr_freq)) {
        init_bandinfo(&pinfo->lwrband, &pinfo->uprband, ENABLE_CLASSIFIER_PRINT);
        ms_init_classifier(&pinfo->lwrband, &pinfo->uprband, &ss_data->classifier_params);
        initialized = TRUE;
        curr_freq = msg->freq;
    }
    //printf("~~~ssd_process_spectral_msg 2~~~");
    /* classify the spectral data */
    classifier(&pinfo->lwrband, ss_data->spectral_tstamp, ss_data->spectral_last_tstamp, primary_rssi,
               narrowband_lwr, peak_index);

    if (is_ht2040) {
        peak_index = ss_data->spectral_upper_max_index;
        primary_rssi = ss_data->spectral_upper_rssi;
        classifier(&pinfo->uprband, ss_data->spectral_tstamp, ss_data->spectral_last_tstamp, primary_rssi,
                   narrowband_upr, peak_index);
    }

    prev_time_stamp = ss_data->spectral_tstamp;
    update_11g_interf_detection(pinfo);

    /*daniel delete begin*/
    #if 0
    ss_process_interf();//dxm add this ,this is may be for his UI,we don't need this in GP
    //call cjson/ update_interf ()
    //need to use
    //ath_ssd.c update_interf_info() similar to update_interf()
    //but can not use reason
    //1)ath_ssd.c not use 
    //2)wp854:WEB_INTERF_RSP ,wp838:INTERF_RSP.wp838 member less than wp854
    #endif
    /*daniel delete end*/

    if (pinfo->config.eacs) {
        not_yet();//daniel comment:we can realize as 854 just system call
    }

    return;
}

/*
 * Function     : get_next_index
 * Description  :
 * Input params :
 * Return       :
 *
 */
int get_next_index(int cur_index)
{
    if (cur_index < (MAX_SAVED_SAMP_MESSAGES - 1))
        return (cur_index + 1);

    if (cur_index == (MAX_SAVED_SAMP_MESSAGES - 1))
        return 0;

    return -1;
}

/*
 * Function     : ssd_save_maxrssi_samp_message
 * Description  :
 * Input params :
 * Return       :
 *
 */
void ssd_save_maxrssi_samp_message(ssd_info_t *pinfo, SPECTRAL_SAMP_MSG* msg)
{
    ssd_samp_msg_buffers_t *p = NULL;
    ssd_samp_info_t *psinfo = &pinfo->sampinfo;

    if (msg->freq == GUI_LOWER_FREQ) {
        p = &pinfo->lower_msg_pool;
    } else if (msg->freq == GUI_UPPER_FREQ) {
        p = &pinfo->upper_msg_pool;
    }

    assert(p != NULL);
    put_msg(pinfo, (char *)msg, sizeof(SPECTRAL_SAMP_MSG), p);

    if (psinfo->num_samp_saved < MAX_SAVED_SAMP_MESSAGES)
        psinfo->num_samp_saved++;

    if ((psinfo->num_samp_saved >= psinfo->num_samp_to_save) &&
        (psinfo->total_channel_switches == 1)) {
        ssd_switch_channel(pinfo);
        ssd_start_scan(pinfo);
    }

}

/*
 * Function     : ssd_save_this_spectral_msg
 * Description  :
 * Input params :
 * Return       :
 *
 */
void ssd_save_this_spectral_msg(ssd_info_t *pinfo, SPECTRAL_SAMP_MSG* msg)
{

    ssd_samp_info_t *psinfo = GET_SAMP_INFO_PTR(pinfo);
    int32_t time_stamp  = msg->samp_data.spectral_tstamp;
    int16_t rssi        = msg->samp_data.spectral_rssi;

    if (msg->freq == GUI_LOWER_FREQ) {
        if (psinfo->lower_last_tstamp == 0)
            psinfo->lower_last_tstamp = time_stamp;

        if (rssi > psinfo->lower_maxrssi) {
            psinfo->lower_maxrssi = rssi;
            memcpy(&psinfo->lower_max_rssi_msg, msg, sizeof(SPECTRAL_SAMP_MSG));
        }

        if ((time_stamp - psinfo->lower_last_tstamp) >= pinfo->config.maxhold_interval) {
            ssd_save_maxrssi_samp_message(pinfo, &psinfo->lower_max_rssi_msg);
            psinfo->lower_maxrssi = DEFAULT_MAX_RSSI;
            psinfo->lower_last_tstamp = 0;
        }
    }

    if (msg->freq == GUI_UPPER_FREQ) {
        if (psinfo->upper_last_tstamp == 0)
            psinfo->upper_last_tstamp = time_stamp;

        if (rssi > psinfo->upper_maxrssi) {
            psinfo->upper_maxrssi = rssi;
            memcpy(&psinfo->upper_max_rssi_msg, msg, sizeof(SPECTRAL_SAMP_MSG));
        }

        if ((time_stamp - psinfo->upper_last_tstamp) >= pinfo->config.maxhold_interval) {
            ssd_save_maxrssi_samp_message(pinfo, &psinfo->upper_max_rssi_msg);
            psinfo->upper_maxrssi = DEFAULT_MAX_RSSI;
            psinfo->upper_last_tstamp = 0;
        }
    }

    return;
}


/*
 * Function     : ssd_build_single_SAMP_response
 * Description  : prepare SAMP message for GUI
 * Input params :
 * Return       :
 *
 */
u_int8_t* ssd_build_single_SAMP_response(ssd_info_t *pinfo,
                                         uint16_t freq,
                                         int *num_bytes,
                                         u_int8_t *bin_pwr,
                                         int pwr_count,
                                         u_int8_t rssi,
                                         int8_t lower_rssi,
                                         int8_t upper_rssi,
                                         u_int8_t max_scale,
                                         int16_t max_mag,
                                         struct INTERF_SRC_RSP *interf)
{
    int resp_pkt_size;
    int i;

    struct SAMPLE_RSP *SAMP_rsp     = NULL;
    struct FREQ_PWR_RSP *pwr_resp   = NULL;
    struct INTERF_SRC_RSP *src_resp = NULL;
    struct TLV *tlv;

    float pwr_val;
    float calc_maxmag = log10f(max_mag);
    float bin_sum = 0;
    float bin_pwr_val;

    static u_int8_t resp_num = 0;
    u_int8_t val;

    resp_pkt_size = sizeof(struct TLV) +
                    sizeof(struct SAMPLE_RSP) +
                    sizeof(struct INTERF_SRC_RSP) +
                    (128)*(sizeof(struct FREQ_PWR_RSP)) +
                    sizeof(struct INTERF_RSP);

    tlv = (struct TLV*)malloc(resp_pkt_size);
    if (!tlv)
        return NULL;

    tlv->tag = SAMP_RESPONSE;
    tlv->len = resp_pkt_size - 3;

    SAMP_rsp = (struct SAMPLE_RSP *)tlv->value;
    SAMP_rsp->freq = htons(freq);
    SAMP_rsp->sample_count = htons(SAMPLE_COUNT);

    pwr_resp = (struct FREQ_PWR_RSP*)SAMP_rsp->samples;

    lower_rssi = convert_rssi_to_dbm(lower_rssi);
    upper_rssi = convert_rssi_to_dbm(upper_rssi);
    rssi       = convert_rssi_to_dbm(rssi);

    *num_bytes = resp_pkt_size;

    if (pinfo->config.power_per_bin) {
        for (i = 0; i < pwr_count; i++) {
            val = (u_int8_t)(*(bin_pwr + i));
            bin_sum += powf(val, 2);
        }
    }

    for (i = 0; i < pwr_count; i++) {
        if ((i == 0) && (pinfo->config.index_data == 1)) {
            pwr_resp->pwr = resp_num++;
            pwr_resp++;
            continue;
        }

        val = (u_int8_t)(*(bin_pwr + i));

        if (val == 0)
            val = 0x1;

        if (val == 255) {
            pwr_val = pinfo->config.minpower;
            if (pwr_val < 0)
                pwr_val = abs(pwr_val);
            else
                pwr_val = 255;
            pwr_resp->pwr = (u_int8_t)pwr_val;
            pwr_resp++;
            continue;
        }

        pwr_val = (float)val;

        if (pinfo->config.power_per_bin) {
            if (i <= 63)
                pwr_val = lower_rssi;
            else
                pwr_val = upper_rssi;
            pwr_val /= 10;
            if (pwr_val < 0)
                bin_pwr_val = 1/powf(10, abs(pwr_val));
            else
                bin_pwr_val = powf(10, pwr_val);

            bin_pwr_val *= ((powf(val, 2))/bin_sum);
            bin_pwr_val = 10 * (log10f(bin_pwr_val));

            if (bin_pwr_val < - 127) {
                bin_pwr_val = -127;
            }

            if (bin_pwr_val < pinfo->config.minpower)
                bin_pwr_val = pinfo->config.minpower;

            if (bin_pwr_val < 0)
                bin_pwr_val = abs(bin_pwr_val);
            else
                bin_pwr_val = 255;

            pwr_resp->pwr = (u_int8_t)bin_pwr_val;
            pwr_resp++;
            continue;
        }

        if (pinfo->config.rssi_only) {
            if (i <= 63)
                pwr_val = lower_rssi;
            else
                pwr_val = upper_rssi;
            
            if (pwr_val < 0)
                pwr_val = abs(pwr_val);
            else
                pwr_val = 255;

        } else {
            if (pinfo->config.raw_fft) {
                pwr_val *= max_scale;
                pwr_val = log10f(pwr_val);
                pwr_val -= calc_maxmag;
                pwr_val *= 20;

                if (pinfo->config.use_rssi) {
                    if (i <= 63) 
                        pwr_val += lower_rssi;
                    else
                        pwr_val += upper_rssi;
                }

                if (pwr_val < pinfo->config.minpower)
                    pwr_val = pinfo->config.minpower;

                if (pwr_val < 0)
                    pwr_val = abs(pwr_val);
                else
                    pwr_val = 255;
            }

        }

        if (pinfo->config.scale) {
            pwr_val *= 0.392;
        }

        if (pinfo->config.flip) {
            val = (u_int8_t)pwr_val;
        } else {
            val = (u_int8_t)(100 - pwr_val);
        }

        pwr_resp->pwr = val;
        pwr_resp++;
    }

    src_resp = (struct INTERF_SRC_RSP*)pwr_resp;
    memset((void*)src_resp, 0, sizeof(struct INTERF_SRC_RSP));
    src_resp->count = htons(0);

    if (pinfo->config.classify) {
        ssd_add_11g_interference_report(pinfo, src_resp);
    }
    if (src_resp->count) {
        src_resp->count = htons(src_resp->count);
    }

    *num_bytes = resp_pkt_size;
    return (unsigned char*)tlv;
}
/*
 * Function     :
 * Description  :
 * Input params :
 * Return       :
 *
 */
void ssd_send_normal_spectral_data_to_client(ssd_info_t *pinfo, SPECTRAL_SAMP_MSG *ss_msg, int client_fd)
{
    int num_bytes = 0;
    int send_err = 0;
    u_int8_t *rsp_msg   = NULL;
    SPECTRAL_SAMP_DATA *ss_data = NULL;
    ssd_config_t *pconfig = GET_CONFIG_PTR(pinfo);
    ssd_samp_info_t *psinfo = GET_SAMP_INFO_PTR(pinfo);

    if (psinfo->num_samp_sent < psinfo->num_resp_reqd) {
        ss_data = &ss_msg->samp_data;
        rsp_msg = ssd_build_single_SAMP_response(pinfo,
                                                 ss_msg->freq, &num_bytes,
                                                 ss_data->bin_pwr,
                                                 ss_data->bin_pwr_count,
                                                 ss_data->spectral_combined_rssi,
                                                 ss_data->spectral_lower_rssi,
                                                 ss_data->spectral_upper_rssi,
                                                 ss_data->spectral_max_scale,
                                                 ss_data->spectral_max_mag,
                                                 &ss_data->interf_list);

        if (pconfig->atheros_ui) {
            send_err = send(client_fd, ss_msg, sizeof(SPECTRAL_SAMP_MSG), 0);
        } else {
            send_err = send(client_fd, rsp_msg, num_bytes, 0);
        }

        if (send_err == -1)
            perror("send");

        psinfo->num_samp_sent++;

        if (!(pconfig->classify) && (psinfo->num_samp_sent == psinfo->num_resp_reqd)) {
            if ((psinfo->total_channel_switches % 2) != 0) {
                ssd_switch_channel(pinfo);
                ssd_start_scan(pinfo);
            }
        }

    }
}

/*
 * Function     :
 * Description  :
 * Input params :
 * Return       :
 *
 */
void ssd_prepare_spectral_data_to_client(ssd_info_t *pinfo, SPECTRAL_SAMP_MSG* ss_msg, int client_fd)
{
    ssd_samp_info_t *psinfo = GET_SAMP_INFO_PTR(pinfo);

    int32_t time_stamp  = ss_msg->samp_data.spectral_tstamp;
    int16_t rssi        = ss_msg->samp_data.spectral_rssi;

    if (psinfo->num_samp_sent < psinfo->num_resp_reqd) {
        alarm(0);
    }

    if (ss_msg->freq == GUI_LOWER_FREQ) {
        /* this is the first spectral report received for this channel
         * in this maxhold interval
         */
        if (psinfo->lower_last_tstamp == 0)
            psinfo->lower_last_tstamp = time_stamp;

        if (rssi > psinfo->lower_maxrssi) {
            psinfo->lower_maxrssi = rssi;
            memcpy(&psinfo->lower_max_rssi_msg, ss_msg, sizeof(SPECTRAL_SAMP_MSG));
        }

        if ((time_stamp - psinfo->lower_last_tstamp) >= pinfo->config.maxhold_interval) {
            ssd_send_normal_spectral_data_to_client(pinfo, &psinfo->lower_max_rssi_msg, client_fd);
            psinfo->lower_maxrssi = DEFAULT_MAX_RSSI;
            psinfo->lower_last_tstamp = 0;
        }

    }

    if (ss_msg->freq == GUI_UPPER_FREQ) {
        /* this is the first spectral report received for this channel
         * in this maxhold interval
         */

        if (psinfo->upper_last_tstamp == 0) 
            psinfo->upper_last_tstamp = time_stamp;

        if (rssi > psinfo->upper_maxrssi) {
            psinfo->upper_maxrssi = rssi;
            memcpy(&psinfo->upper_max_rssi_msg, ss_msg, sizeof(SPECTRAL_SAMP_MSG));
        }

        if ((time_stamp - psinfo->upper_last_tstamp) >= pinfo->config.maxhold_interval) {
            ssd_send_normal_spectral_data_to_client(pinfo, &psinfo->upper_max_rssi_msg, client_fd);
            psinfo->upper_maxrssi = DEFAULT_MAX_RSSI;
            psinfo->upper_last_tstamp = 0;
        }
    }
}

/*
 * Function     :
 * Description  :
 * Input params :
 * Return       :
 *
 */
int ssd_can_send_saved_msg(ssd_info_t *pinfo)
{
    ssd_samp_msg_buffers_t *plower = &pinfo->lower_msg_pool;
    ssd_samp_msg_buffers_t *pupper = &pinfo->upper_msg_pool;

    if (plower->count && pupper->count)
        return 1;
    else
        return 0;

}

/*
 * Function     :
 * Description  :
 * Input params :
 * Return       :
 *
 */
void ssd_send_samp_data_to_client(ssd_info_t *pinfo, SPECTRAL_SAMP_MSG* ss_msg, int client_fd)
{

    int num_bytes = 0;
    int send_err = 0;
    ssd_config_t *pconfig = GET_CONFIG_PTR(pinfo);
    SPECTRAL_SAMP_DATA *ss_data = &ss_msg->samp_data;
    u_int8_t* rsp = NULL;
    ssd_samp_info_t *psinfo = GET_SAMP_INFO_PTR(pinfo);

    rsp = ssd_build_single_SAMP_response(pinfo,
                                         ss_msg->freq, &num_bytes,
                                         ss_data->bin_pwr,
                                         ss_data->bin_pwr_count,
                                         ss_data->spectral_combined_rssi,
                                         ss_data->spectral_lower_rssi,
                                         ss_data->spectral_upper_rssi,
                                         ss_data->spectral_max_scale,
                                         ss_data->spectral_max_mag,
                                         &ss_data->interf_list);


    if (pconfig->atheros_ui) {
        send_err = send(client_fd, ss_msg, sizeof(SPECTRAL_SAMP_MSG), 0);
    } else {
        send_err = send(client_fd, rsp, num_bytes, 0);
    }

    /* mute compiler warning */
    send_err = send_err;

    psinfo->num_samp_sent++;
}

/*
 * Function     :
 * Description  :
 * Input params :
 * Return       :
 *
 */
void ssd_send_saved_msg_to_client(ssd_info_t *pinfo, int client_fd, ssd_samp_msg_buffers_t *p, int freq)
{

    int index = 0;
    int result = 0;
    SPECTRAL_SAMP_MSG *ss_msg = NULL;

    if ((result = get_msg(pinfo, &index, p)) == SSD_LIST_OK) {
        ss_msg = &p->buf[index].msg;
        ssd_send_samp_data_to_client(pinfo, ss_msg, client_fd);
    } else {
        ssd_update_silent_samp_msg(pinfo, freq);
        ssd_send_samp_data_to_client(pinfo, &pinfo->silent_msg, client_fd);
    }

    return;
}

/*
 * Function     : ssd_send_classified_spectral_data_to_client
 * Description  : Send the classified data to gui
 * Input params :
 * Return       :
 *
 */
void ssd_send_classified_spectral_data_to_client(ssd_info_t *pinfo, SPECTRAL_SAMP_MSG* msg, int client_fd)
{
    ssd_samp_info_t *psinfo = GET_SAMP_INFO_PTR(pinfo);

    if (pinfo->config.classify) {
        if (ssd_can_send_saved_msg(pinfo)) {
            ssd_send_saved_msg_to_client(pinfo, client_fd, &pinfo->lower_msg_pool, pinfo->lower_msg_pool.freq);
            ssd_send_saved_msg_to_client(pinfo, client_fd, &pinfo->upper_msg_pool, pinfo->upper_msg_pool.freq);
            psinfo->num_samp_sent++;
        }

        if (pinfo->stats.alarm >= 5) {
            ssd_switch_channel(pinfo);
            ssd_start_scan(pinfo);
        }
    }
}


/*
 * Function     : ssd_handle_spectral_data
 * Description  : Receive the spectral data from spectral driver
 *                Process the spectral data from spectral driver
 * Input params : pointer to ssd_info_t,buffer, len
 *                socket fd,
 * Return       :
 *
 */
void ssd_handle_spectral_data(ssd_info_t *pinfo,
                              u_int8_t* buf,
                              int *recvd_bytes,
                              int client_fd,
                              int nl_spectral_fd)
{
    struct nlmsghdr *nlh = NULL;
    struct msghdr   msg;

    SPECTRAL_SAMP_MSG *ss_msg   = NULL;
    SPECTRAL_SAMP_DATA *ss_data = NULL;

    if (!(nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(sizeof(SPECTRAL_SAMP_MSG))))) {
        perror("no memory");
        return;
    }


    /* init nlmsg data */
    memset(nlh, 0, NLMSG_SPACE(sizeof(SPECTRAL_SAMP_MSG)));
    nlh->nlmsg_len      = NLMSG_SPACE(sizeof(SPECTRAL_SAMP_MSG));
    nlh->nlmsg_pid      = getpid();
    nlh->nlmsg_flags    = 0;

    pinfo->iov.iov_base = (void *)nlh;
    pinfo->iov.iov_len  = nlh->nlmsg_len;

    memset(&pinfo->dst_addr, 0, sizeof(pinfo->dst_addr));

    pinfo->dst_addr.nl_family  = PF_NETLINK;
    pinfo->dst_addr.nl_pid     = 0;
    pinfo->dst_addr.nl_groups  = 1;

    memset(&msg, 0, sizeof(msg));
    msg.msg_name    = (void *)&pinfo->dst_addr;
    msg.msg_namelen = sizeof(pinfo->dst_addr);
    msg.msg_iov     = &pinfo->iov;
    msg.msg_iovlen  = 1;

    

	/*daniel comment begin*/
       /* wp854 spectral_parse_data.c 223line
	fromlen = sizeof(src_addr);
        memset(nlh, 0, NLMSG_SPACE(sizeof(SPECTRAL_SAMP_MSG)));
        memset(&msg, 0, sizeof(msg));
        msg.msg_name = (void *)&dst_addr;
        msg.msg_namelen = sizeof(dst_addr);
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;
	*/
	/*daniel comment end*/




    /* received data from spectral driver */
    recvmsg(nl_spectral_fd, &msg, MSG_WAITALL);

    /* cast to spectral samp msg */
    ss_msg  = (SPECTRAL_SAMP_MSG*)NLMSG_DATA(nlh);
    ss_data = &ss_msg->samp_data;

    /* mute compiler warning */
    ss_data = ss_data;

   //printf("~~0~~\n");

    if (nlh->nlmsg_len) {

        /*
         * Process the spectral data only if the following 
         * are enabled other wise don't process
         * - Classification
         * - EACS
         */
        if (pinfo->config.classify || pinfo->config.eacs) {
			//printf("~~1~~\n");
            ssd_process_spectral_msg(pinfo, ss_msg);
        }
/*daniel add begin*/

        //daniel add below:global_ssam
        if (pinfo->config.classify&& (1 != global_ssam)) {
/*daniel add begin*/

            /*
             * If Classification is enabled then save this
             * message.
             */
            ssd_save_this_spectral_msg(pinfo, ss_msg);
        } 
	/*daniel add begin*/
        else if(global_ssam)
         {
                /*at here, ssamode need to send packet to client,but now don't know
                   how to send this data, pay attention:
                   1, data is very large at a burst;
                   2, the function below want judge the frequency;
                      this should be modified*/              
                void printnetlinkmsg(SPECTRAL_SAMP_MSG* nl_samp_msg);
                //printnetlinkmsg(nl_samp_msg);  
                printnetlinkmsg(ss_msg);  
           }
        /*daniel add end*/

	else {

            /*
             * Else, ship the data to gui
             * TODO : There is a difference of 10 MHz between
             *        the frequency noted by Kernel and Application
             */
            if (ss_msg->freq == (pinfo->config.current_freq + 10))
                ssd_prepare_spectral_data_to_client(pinfo, ss_msg, client_fd);
        }
	/*daniel add begin*/
	/* dxm 2013.9.12 9:55 V1.00.001 [START]*/
            //ssChannelInfoUpdate(global_ChannelInfo,nl_samp_msg);
            ssChannelInfoUpdate(global_ChannelInfo,ss_msg);
            /* dxm 2013.9.12 9:55 V1.00.001 [END]*/
      /*daniel add end*/
    }
	/*daniel add begin*/
	else 
	{


		//SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL2," Received message payload %d bytes freq=%u send_single=%d current_freq=%d\n",\
              //                                nlh->nlmsg_len, nl_samp_msg->freq, send_single, global_current_freq);
              //send_single no use in WP854E_SA
             SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL2," Received message payload %d bytes freq=%u send_single=%d current_freq=%d\n",\
                                              nlh->nlmsg_len, ss_msg->freq, 0, pinfo->config.current_freq);

		
        }
        /*daniel add end*/
    free((char*)nlh);
}



void printnetlinkmsg(SPECTRAL_SAMP_MSG* nl_samp_msg)
{
    SPECTRAL_SAMP_DATA *nl_samp_data;
    char *cTemp;
    int i,len;

    nl_samp_data = &nl_samp_msg->samp_data;

    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL3," freq = %d\r\n", nl_samp_msg->freq);
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL3," freq_loading = %d\r\n", nl_samp_msg->freq_loading);
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL3," cw_interferer = %d\r\n", nl_samp_msg->cw_interferer);

    if(spectral_debug_level < ATH_DEBUG_SPECTRAL3)
    {
        return;
    }
    cTemp = (char*)nl_samp_data;
    len = sizeof(SPECTRAL_SAMP_MSG);
    printf("\n-----------------------------------------------\n");
    for(i=0; i<len; i++)
    {
        if(0 == (i%16))
            printf("\n");
        printf("%2x ",*cTemp);
        cTemp++;
    }
    printf("\n-----------------------------------------------\n");
}



