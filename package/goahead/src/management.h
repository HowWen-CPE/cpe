#ifndef __MANAGEMENT_H__
#define __MANAGEMENT_H__

#define PROC_IF_STATISTIC	"/proc/net/dev"
#define PROC_MEM_STATISTIC	"/proc/meminfo"


#define TXBYTE		0
#define TXPACKET	1
#define RXBYTE		2
#define RXPACKET	3
#define COLLS		4

//#define WIFI_STATUS "iwpriv rai0 stat"

#define SYSTEM_COMMAND_LOG "/var/system_command.log"

extern int 		ejArgs(int argc, char_t **argv, char_t *fmt, ...);
void formDefineManagement(void);
void management_init(void);
char *setNthValueLong(int index, char *old_values, char *new_value);
char *subStringByIndex(char *string, int begin, int end);
#endif
