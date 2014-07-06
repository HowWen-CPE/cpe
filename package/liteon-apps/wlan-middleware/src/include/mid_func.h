#ifndef __MID_FUNC_H_
#define __MID_FUNC_H_
#include	<stdlib.h>
#include	<stdio.h>
#include <string.h>

int set_wlan_basic(int radio);
int set_wlan_advance(int radio);
int set_operation_mode(int radio, int mode_prev, int mode_curr);
int set_enable_radio_before_network(int radio, int mode_prev, int mode_curr);
#endif //End of __MID_FUNC_H_
