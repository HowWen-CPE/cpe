#ifndef __FW_WP838_H_
#define __FW_WP838_H_

typedef enum
{
	NVRAM_FW_0_0_0 = 0,
} nvram_fw_version;

/*********DO NOT MODIFY BELOW***********/
struct nvram_fw_tuple {
    char *fw_str;
    nvram_fw_version fw_version;
    int (*fw_upgrade_func)(void); 
    int (*fw_downgrade_func)(void); 
};
/*********DO NOT MODIFY ABOVE***********/

#endif
