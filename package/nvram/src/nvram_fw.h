#ifndef _NVRAM_FW_H_
#define _NVRAM_FW_H_
#include "nvram.h"

#if defined(PRODUCT_GP_WP688)
#include "fw_wp688.h"
#elif defined(PRODUCT_GP_WP868)
#include "fw_wp868.h"
#elif defined(PRODUCT_GP_WP838)
#include "fw_wp838.h"
#endif

#endif /* _EZPLIB_FW_H_ */
