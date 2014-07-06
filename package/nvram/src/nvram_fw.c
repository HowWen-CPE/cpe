#include "nvram.h"

#if defined(PRODUCT_GP_WP688)
#include "fw_wp688.c"
#elif defined(PRODUCT_GP_WP868)
#include "fw_wp868.c"
#elif defined(PRODUCT_GP_WP838)
#include "fw_wp838.c"
#endif
