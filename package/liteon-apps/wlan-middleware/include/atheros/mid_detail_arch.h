#ifndef __MID_DETAIL_ARCH_H__
#define __MID_DETAIL_ARCH_H__


#define AP_NAME_2G          "ath"
#define WDS_NAME_2G         "wds"
#define STA_NAME_2G         "sta"
#define AP_NAME_5G          "ath"
#define WDS_NAME_5G         "wds"
#define STA_NAME_5G         "sta"

#define MAX_STA_NUM    64



/*---------ACL Function---------*/
#define ACL_UNCHANGED_MAC	0
#define ACL_ADD_MAC			1
#define ACL_DELETED_MAC		2

#define ATH_ACL_DISABLE    	0
#define ATH_ACL_ALLOW    	1
#define ATH_ACL_DENY    	2
#define ATH_ACL_FLUSH    	3

/*---------DFS Function---------*/
#define DFS_GET_NOL			14
#define DFS_IS_CAC_DONE		20
#define SIOCGATHPHYERR      0x89F5


#endif
