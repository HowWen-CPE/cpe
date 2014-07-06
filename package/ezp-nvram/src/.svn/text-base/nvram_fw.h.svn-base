#ifndef _NVRAM_FW_H_
#define _NVRAM_FW_H_

typedef enum
{
    NVRAM_FW_0_0_0 = 0,
    NVRAM_FW_1_6_5,
    NVRAM_FW_1_6_6,
    NVRAM_FW_1_6_7,
    NVRAM_FW_1_7_0,
    NVRAM_FW_1_7_1,
    NVRAM_FW_1_7_2,
    NVRAM_FW_1_7_3,
    NVRAM_FW_1_7_4,
    NVRAM_FW_1_7_5,
    NVRAM_FW_1_7_6,
    NVRAM_FW_1_7_7,
    NVRAM_FW_1_7_8,
    NVRAM_FW_1_7_9,
    NVRAM_FW_1_7_10,
    NVRAM_FW_1_7_11,
    NVRAM_FW_2_0_0,
    NVRAM_FW_2_0_1,
    NVRAM_FW_2_0_2,
    NVRAM_FW_2_0_3,
    NVRAM_FW_2_0_4,
    NVRAM_FW_2_0_5,
    /* Added by Liteon, 2013-7-9 */
   /* Firmware from v0.1.6 to v0.1.7 */
    NVRAM_FW_2_0_6,
    NVRAM_FW_2_0_7,
    /* Ended Liteon */
} nvram_fw_version;

struct nvram_fw_tuple {
    char *fw_str;
    nvram_fw_version fw_version;
    int (*fw_upgrade_func)(void); 
    int (*fw_downgrade_func)(void); 
};


#endif /* _NVRAM_FW_H_ */

