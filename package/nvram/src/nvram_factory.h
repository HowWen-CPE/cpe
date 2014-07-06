#ifndef __NVRAM_FACTORY_H_
#define __NVRAM_FACTORY_H_

#if defined(PRODUCT_GP_WP688)
#include "config_wp688.h"
#elif defined(PRODUCT_GP_WP868)
#include "config_wp868.h"
#elif defined(PRODUCT_GP_WP838)
#include "config_wp838.h"
#else
#error "Unsupported product name!!!!" 
#endif

#endif//# __NVRAM_FACTORY_H_
