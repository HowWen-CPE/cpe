#ifndef _EZPCOM_LIB_H
#define _EZPCOM_LIB_H

/* 
 * THE VENDOR TABLE 
 */

#define EZP_COM_VENDOR_MAP(X)                                   \
    X(AUTO, "Auto", 0x3789, 1)                                  \
    X(HUAWEI, "Huawei", 0x12d1, 1)                              \
    X(HUAWEI2, "Huawei (Alternative)", 0x1033, 1)               \
    X(ZTE, "ZTE", 0x19d2, 1)                                    \
    X(EMPTY, "", 0x0, 0)                                        \


/* 
 * THE PRODUCT TABLE TO A SPECIFIC VENDOR
 * (1) Device Name
 * (2) Default Device, Target Vendor, Target Device, Target Class, MessageEndpoint, * ResponseEndpoint,
 * (3) DetachStorageOnly, Mode, Interface, 
 * (4) Device Num, Data, Ctrl,
 * (5) MessageContent, Shown
 *
 * XXX: 
 * I. For the multiple-message devices, the entries (other than the last 
 * message) should be set with 0xF0. The last entries should be set with 0x00.
 * 
 * II. Also, in the first message, flag should be set. Others should be set
 * 0.
 *
 * III. For the express card, the "Default Device" is set as 0xFFFF.
 * 
 */

/* Empty definition. */
#define EZP_COM_MAP_EMPTY(X)                        \
    X("",                                           \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,          \
            0, 0, 0x0,                              \
            0, 0, 0, "",                            \
            NULL, 0)                                \


#define EZP_COM_MAP_AUTO(X)                       \
    X("Auto",                                       \
            0x5233, EMPTY, 0x0, 0x0, 0x0, -1,       \
            0, 0, 0x0,                              \
            0, 0, 0, "",                         \
            NULL, 1 )                               \
    X("",                                           \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,          \
            0, 0, 0x0,                              \
            0, 0, 0, "",                            \
            NULL, 0)                                \

/* We intentionally use 0x as TargetProduct. */
#define EZP_COM_MAP_HUAWEI(X)                       \
    X("Generic",                                  \
            0x1003, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0,                              \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1 )                               \
    X("E156G",       \
            0x1003, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0,                              \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                \
    X("E160E",       \
            0x1003, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0,                              \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                \
    X("E160G",       \
            0x1003, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0,                              \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                \
    X("E161",       \
            0x1446, HUAWEI, 0x0, 0x0, 0x1, -1,       \
            0, 0, 0x0,                              \
            3, 0, 0, "ttyUSB",                         \
            "55534243000000000000000000000011060000000000000000000000000000", 1) \
    X("E169",                                    \
            0x1001, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0,                              \
            3, 0, 0, "ttyUSB",                         \
            NULL, 1 )                               \
    X("E1692",                                    \
            0x1446, HUAWEI, 0x0, 0x0, 0x1, -1,       \
            0, 0, 0x0,                              \
            3, 0, 2, "ttyUSB",                         \
            "55534243000000000000000000000011060000000000000000000000000000", 1) \
    X("E169U",                                    \
            0x1446, HUAWEI, 0x0, 0x0, 0x1, -1,       \
            0, 0, 0x0,                              \
            3, 0, 2, "ttyUSB",                         \
            "55534243000000000000000000000011060000000000000000000000000000", 1) \
    X("E170",       \
            0x1003, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0,                              \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                \
    X("E172",       \
            0x1003, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0,                              \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                \
    X("E1750",                                    \
            0x1446, HUAWEI, 0x0, 0x0, 0x1, -1,       \
            0, 0, 0x0,                              \
            3, 0, 2, "ttyUSB",                         \
            "55534243000000000000000000000011060000000000000000000000000000", 1) \
    X("E180",       \
            0x1414, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0,                              \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                \
    X("E219",       \
            0x1003, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0,                              \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                \
    X("E220",       \
            0x1003, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0,                              \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                \
    X("E226",       \
            0x1003, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0,                              \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                \
    X("E270",       \
            0x1003, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0,                              \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                \
    X("E800",       \
            0x1003, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0,                              \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                \
    X("E1762",       \
            0x1003, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0,                              \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                \
    X("EC1260",       \
            0x140b, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0,                              \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                \
    X("ET128",       \
            0x1da1, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0,                              \
            3, 2, 0, "ttyACM",                         \
            NULL, 1) \
    X("K1550",       \
            0x1446, HUAWEI, 0x0, 0x0, 0x1, -1,       \
            0, 0, 0x0,                              \
            3, 0, 0, "ttyUSB",                         \
            "55534243000000000000000000000011060000000000000000000000000000", 1) \
    X("K3715",       \
            0x1001, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0,                              \
            3, 0, 0, "ttyUSB",                         \
            NULL, 1) \
    X("E870",                                       \
            0xFFFF, HUAWEI, 0x1003, 0x0, 0x0, -1,       \
            0, 0, 0x0,                              \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                \
    X("",                                           \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,          \
            0, 0, 0x0,                              \
            0, 0, 0, "",                            \
            NULL, 0)                                \


#define EZP_COM_MAP_HUAWEI2(X)                      \
    X("E630",                                       \
            0x0035, HUAWEI, 0x1003, 0x0, 0x0, -1,   \
            1, 0, 0x0,                              \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                  \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0,                              \
            0, 0, 0, "",                            \
            NULL, 0) \


/*
 * In Australia, pre-paid MF626 is buggy! When the data is on, any access to
 * the control tty causes the modem panic! So in "Generic" and
 * "MF628+/MF626/MF636/MF637" both have the same data and tty nums.
 */ 
/*
 * In South Africa, K3565-Z does the following steps:
 * # Standard SCSI eject
 * #usb_modeswitch -v ${VID} -p ${PID} -M
 * 5553424312345678000000000000061b000000020000000000000000000000 -R 1
 * # Storage passthrough
 * usb_modeswitch -v ${VID} -p ${PID} -M
 * 55534243123456782400000080000C85000000240000000000000000000000 -R 1
 * NB: It seems some ZTE devices require USB reset. e.g. ZTE MF628 from TWN
 * Mobile. We leverage 0x80 at Mode to indicate reset.
 */

#define EZP_COM_MAP_ZTE(X)                      \
    X("Generic",                                       \
            0x2000, ZTE, 0x0031, 0x0, 0x01, -1,   \
            0, 0, 0x0,                              \
            3, 2, 2, "ttyUSB",                         \
            "55534243123456782000000080000c85010101180101010101000000000000", 1) \
    X("MF622",                                       \
            0x2000, ZTE, 0x0002, 0x0, 0x04, -1,   \
            0, 0, 0x0,                              \
            3, 2, 1, "ttyUSB",                         \
            "55534243f8f993882000000080000a85010101180101010101000000000000", 1)\
    X("MF622 (version 2)",                                       \
            0x2000, ZTE, 0x0001, 0x0, 0x0, -1,   \
            1, 0, 0x0,                              \
            3, 2, 1, "ttyUSB",                         \
            NULL, 1) \
    X("MF628",                                       \
            0x2000, ZTE, 0x0015, 0x0, 0x08, -1,   \
            0, 0x80, 0x0,                              \
            3, 2, 1, "ttyUSB",                         \
            "5553424312345678000000000000061b000000030000000000000000000000", 1) \
    X("MF628+",                                       \
            0x2000, ZTE, 0x0031, 0x0, 0x01, -1,   \
            0, 0, 0x0,                              \
            3, 2, 2, "ttyUSB",                         \
            "55534243123456782000000080000c85010101180101010101000000000000", 1) \
    X("MF626",                                       \
            0x2000, ZTE, 0x0031, 0x0, 0x01, -1,   \
            0, 0, 0x0,                              \
            3, 2, 2, "ttyUSB",                         \
            "55534243123456782000000080000c85010101180101010101000000000000", 1) \
    X("MF633",                                       \
            0x2000, ZTE, 0x0031, 0x0, 0x01, -1,   \
            0, 0, 0x0,                              \
            3, 2, 2, "ttyUSB",                         \
            "55534243123456782000000080000c85010101180101010101000000000000", 1) \
    X("MF636",                                       \
            0x2000, ZTE, 0x0031, 0x0, 0x01, -1,   \
            0, 0, 0x0,                              \
            3, 2, 2, "ttyUSB",                         \
            "55534243123456782000000080000c85010101180101010101000000000000", 1) \
    X("MF637",                                       \
            0x2000, ZTE, 0x0031, 0x0, 0x01, -1,   \
            0, 0, 0x0,                              \
            3, 2, 2, "ttyUSB",                         \
            "55534243123456782000000080000c85010101180101010101000000000000", 1) \
    X("MF638",                                       \
            0x2000, ZTE, 0x0037, 0x0, 0x01, -1,   \
            0, 0, 0x0,                              \
            3, 2, 1, "ttyUSB",                         \
            "55534243123456782000000080000c85010101180101010101000000000000", 1) \
    X("MU351",                                       \
            0x0003, ZTE, 0x0037, 0x0, 0x01, -1,   \
            0, 0, 0x0,                              \
            3, 2, 1, "ttyUSB",                         \
            "55534243123456782000000080000c85010101180101010101000000000000", 1) \
    X("K3565-Z",                                       \
            0x2000, ZTE, 0x0063, 0x0, 0x01, -1,   \
            0, 0, 0x0,                              \
            5, 3, 1, "ttyUSB",                         \
            "55534243123456782000000080000c85010101180101010101000000000000", 1) \
    X("ONDA MT503HS",                                       \
            0x2000, ZTE, 0x0002, 0x0, 0x08, -1,   \
            0, 0, 0x0,                              \
            3, 2, 1, "ttyUSB",                         \
            "55534243b0c8dc812000000080000a85010101180101010101000000000000", 1) \
    X("ONDA MT505UP",                                       \
            0x2000, ZTE, 0x0002, 0x0, 0x03, -1,   \
            0, 0, 0x0,                              \
            3, 2, 1, "ttyUSB",                         \
            "55534243123456780000010080000a28000000001c00002000000000000000", 1) \
    X("ONDA MT525UP",                                       \
            0x2000, ZTE, 0x0002, 0x0, 0x0, -1,   \
            0, 4, 0x0,                              \
            3, 2, 1, "ttyUSB",                         \
            NULL, 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0,                              \
            0, 0, 0, "",                            \
            NULL, 0) \



#define EZP_COM_VENDOR_NAME_CONVERTER(VNAME,NAME,VID,SHOWN) EZP_COM_VID_##VNAME,
typedef enum {
    EZP_COM_VENDOR_MAP(EZP_COM_VENDOR_NAME_CONVERTER)
} ezp_com_vendor_name_type;

struct ezp_com_vendor_map_entry_type
{
    ezp_com_vendor_name_type vn;
    char *desc;
    int DefaultVendor;
    char shown;
};

struct ezp_com_map_entry_type
{
    char *desc;
    int DefaultProduct;
    ezp_com_vendor_name_type TargetVendorType;
    int TargetProduct;
    int TargetClass;
    int MessageEndpoint;
    int ResponseEndpoint;
    char DetachStorageOnly;
    /* 
     * Huawei(1), Sierra(2), Sony(3), Bandluxe(4), MessageNext(0xF0)
     */
    char Mode; 
    int Interface;
    /* XXX: I intentionally ignore Configuration, AltSetting. */
    int DeviceNum; 
    int DataTTY; 
    int CtrlTTY;
    char *DevName;
    char *MessageContent;
    /* show in GUI or not */
    char Flag;
};

#define EZP_COM_VENDOR_MAP_CONVERTER(VNAME,NAME,VID,SHOWN) \
    {EZP_COM_VID_##VNAME, NAME, VID, SHOWN},

static struct ezp_com_vendor_map_entry_type ezp_com_vendor_map[] = {
    EZP_COM_VENDOR_MAP(EZP_COM_VENDOR_MAP_CONVERTER)
};

#define EZP_COM_MAP_CONVERTER(NAME,PID,TVNAME,TPID,TCLS,EP,REP,DT,MODE,INTF,DNUM,DTTY,CTTY,DEVNMAE,MSG,FLAG) \
   {NAME, PID, EZP_COM_VID_##TVNAME, TPID, TCLS, EP, REP, DT, MODE, INTF, DNUM, DTTY, CTTY, DEVNMAE, MSG, FLAG},


#define EZP_COM_MAP_FOR_EACH_VENDOR_CONVERTER(VNAME,NAME,VID,SHOWN) \
static struct ezp_com_map_entry_type ezp_com_map_##VNAME[] = { \
    EZP_COM_MAP_##VNAME(EZP_COM_MAP_CONVERTER) \
}; 

/* Declare tables to each vendor. */
EZP_COM_VENDOR_MAP(EZP_COM_MAP_FOR_EACH_VENDOR_CONVERTER)


#define EZP_COM_POINTER_MAP_CONVERTER(VNAME,NAME,VID,SHOWN) &ezp_com_map_##VNAME[0],


static struct ezp_com_map_entry_type *ezp_com_pointer_map[] = {
    EZP_COM_VENDOR_MAP(EZP_COM_POINTER_MAP_CONVERTER)
};

/* ------------------------------------ */
/* For location & ISP information */
#define EZP_COM_LOCATION_MAP(X)                         \
    X(TAIWAN, "Taiwan", 1)                              \
    X(AUSTRALIA, "Australia", 1)                        \
    X(CANADA, "Canada", 1)                                    \
    X(CHINA, "China", 1)                                \
    X(HONGKONG, "Hongkong", 1)                          \
    X(INDIA, "India", 1)                                    \
    X(MEXICO, "Mexico", 1)                              \
    X(NETHERLANDS, "Netherland", 1)                     \
    X(NORWAY, "Norway", 1)                              \
    X(PHILIPPINES, "Philippines", 1)                    \
    X(RUSSIA, "Russia", 1)                              \
    X(SOUTH_AFRICA, "South Africa", 1)                  \
    X(USA, "USA", 1)                                    \
    X(EMPTY, "", 0)

/*
    X(AUSTRIA, "Austria", 1)                            \
    X(BELGIUM, "Belgium", 1)                            \
    X(BRASIL, "Brasil", 1)                              \
    X(CANADA, "Canada", 1)                              \
    X(CHILE, "Chile", 1)                                \
    X(CROATIA, "Croatia", 1)                            \
    X(CZECH, "Czech", 1)                                \
    X(DENMARK, "Denmark", 1)                            \
    X(EGYPT, "Egypt", 1)                                \
    X(ESTONIA, "Estonia", 1)                            \
    X(FINLAND, "Finland", 1)                            \
    X(FRANCE, "France", 1)                              \
    X(GERMANY, "Germany", 1)                            \
    X(GREECE, "Greece", 1)                              \
    X(HUNGARY, "Hungary", 1)                            \
    X(INDIA, "India", 1)                                \
    X(INDONESIA, "Indonesia", 1)                        \
    X(IRELAND, "Ireland", 1)                            \
    X(ITALY, "Italy", 1)                                \
    X(JAPAN, "Japan", 1)                                \
    X(LITHUANIA, "Lithuania", 1)                        \
    X(LUXEMBOURG, "Luxembourg", 1)                      \
    X(MACEDONIAN, "Macedonian", 1)                      \
    X(MALAYSIA, "Malaysia", 1)                          \
    X(MEXICO, "Mexico", 1)                              \
    X(NETHERLANDS, "Netherlands", 1)                    \
    X(NEW_ZEALAND, "New Zealand", 1)                    \
    X(POLAND, "Poland", 1)                              \
*/


/* 
 * THE ISP TABLE TO A SPECIFIC LOCATION
 * (1) APN
 * (2) PIN
 * (3) User Name
 * (4) Passwd
 * (5) Dialup String
 *
 */

/* Empty definition. */
#define EZP_COM_ISP_MAP_EMPTY(X)                                            \
    X("", "", "", "", "", "", "", 0)

#define EZP_COM_ISP_MAP_TAIWAN(X)                                           \
    X("Chunghwa Telecom", "internet", "", "chap", "", "", "*99#", 1 )       \
    X("Far EasTone (internet)", "internet", "", "chap", "", "", "*99#", 1 ) \
    X("Far EasTone (fetims)", "fetims", "", "chap", "", "", "*99#", 1 )     \
    X("Taiwan Cellular", "internet", "", "chap", "", "", "*99#", 1 )        \
    X("Vibo", "internet", "", "chap", "", "", "*99#", 1 )                   \
    X("", "", "", "", "", "", "", 0)                                        \


#define EZP_COM_ISP_MAP_CHINA(X)                                            \
    X("China Telecom", "", "", "chap", "", "", "#777", 1 )              \
    X("China Mobile", "cmnet", "", "chap", "", "", "*99#", 1 )              \
    X("China Unicom", "", "", "chap", "", "", "*99#", 1 )                   \
    X("", "", "", "", "", "", "", 0)                                        \


#define EZP_COM_ISP_MAP_HONGKONG(X)                                         \
    X("3 HK", "ipc.three.com.hk", "", "chap", "", "", "*99#", 1 )           \
    X("CSL", "internet", "", "chap", "", "", "*99#", 1 )                    \
    X("New World", "ineternet", "", "chap", "", "", "*99#", 1 )             \
    X("PCCW", "pccw", "", "chap", "", "", "*99#", 1 )                       \
    X("People", "internet", "", "chap", "", "", "*99#", 1 )                 \
    X("SmarTone", "internet", "", "chap", "", "", "*99#", 1 )               \
    X("Sunday", "internet", "", "chap", "", "", "*99#", 1 )                 \
    X("", "", "", "", "", "", "", 0)                                        \

#define EZP_COM_ISP_MAP_INDIA(X)                                     \
    X("TATA", "TATA", "", "chap", "internet", "internet", "#777", 1 )             \
    X("", "", "", "", "", "", "", 0)                                        \

#define EZP_COM_ISP_MAP_MEXICO(X)                                           \
    X("Movistar", "internet.movistar.mx", "", "chap", "", "", "*99#", 1 )   \
    X("Telcel", "internet.itelcel.com", "", "chap", "", "", "*99#", 1 )     \
    X("", "", "", "", "", "", "", 0)                                        \

#define EZP_COM_ISP_MAP_NETHERLANDS(X)                                      \
    X("T-Mobile", "internet", "", "chap", "", "", "*99#", 1 )               \
    X("KPN", "internet", "", "chap", "", "", "*99#", 1 )                    \
    X("Telfort", "internet", "", "chap", "", "", "*99#", 1 )                \
    X("Vodafone", "internet", "", "chap", "", "", "*99#", 1 )               \
    X("", "", "", "", "", "", "", 0)                                        \

#define EZP_COM_ISP_MAP_NORWAY(X)                                           \
    X("Telenor Mobil", "Telenor", "", "chap", "", "", "*99***1#", 1 )       \
    X("Netcom Mobil", "Netcom", "", "chap", "", "", "*99***1#", 1 )         \
    X("", "", "", "", "", "", "", 0)                                        \


#define EZP_COM_ISP_MAP_PHILIPPINES(X)                                      \
    X("Globe", "www.globe.com.ph", "", "chap", "globe", "globe", "*99#", 1 ) \
    X("Smart", "internet", "", "chap", "", "", "*99#", 1 )                  \
    X("Sun Cellula", "minternet", "", "chap", "", "", "*99#", 1 )           \
    X("", "", "", "", "", "", "", 0)                                        \

#define EZP_COM_ISP_MAP_RUSSIA(X)                                           \
    X("BeeLine", "internet.beeline.ru", "", "chap", "beeline", "beeline", "*99#", 1 )  \
    X("Megafon (NWGSM)", "internet.nw", "", "chap", "", "", "*99#", 1 )     \
    X("MTS", "internet.mts.ru", "", "chap", "mts", "mts", "*99#", 1 )       \
    X("PrimTel", "internet.primtel.ru", "", "chap", "", "", "*99#", 1 )     \
    X("", "", "", "", "", "", "", 0)                                        \

#define EZP_COM_ISP_MAP_AUSTRALIA(X)                                        \
    X("iburst", "internet", "", "chap", "", "", "*99#", 1 )                 \
    X("Bigpond", "telstra.bigpond", "", "chap", "", "", "*99#", 1 )         \
    X("Dodo", "dodolns1", "", "chap", "", "", "*99#", 1 )                   \
    X("Exetel", "exetel1", "", "chap", "", "", "*99#", 1 )                  \
    X("Internode", "splns333a1", "", "chap", "", "", "*99#", 1 )            \
    X("Optus", "internet", "", "chap", "", "", "*99#", 1 )                  \
    X("Telstra", "telstra.internet", "", "chap", "", "", "*99#", 1 )        \
    X("Three", "3netaccess", "", "chap", "", "", "*99#", 1 )                \
    X("Three Prepaid", "3services", "", "chap", "", "", "*99#", 1 )         \
    X("TPG", "internet", "", "chap", "", "", "*99#", 1 )                    \
    X("Virgin", "virginbroadband", "", "pap", "", "", "*99***1#", 1)        \
    X("Vodafone", "vfinternet.au", "", "chap", "", "", "*99#", 1 )          \
    X("", "", "", "", "", "", "", 0)                                        \

#define EZP_COM_ISP_MAP_SOUTH_AFRICA(X)                                     \
    X("MTN", "", "", "chap", "", "", "*99#", 1 )                            \
    X("Vodacom", "internet", "", "chap", "", "", "*99***16#", 1 )           \
    X("Cell-C", "internet", "", "chap", "Cellcis", "Cellcis", "*99***16#", 1 ) \
    X("Neotel", "", "", "chap", "", "", "#777", 1 ) \
    X("", "", "", "", "", "", "", 0)                                        \

#define EZP_COM_ISP_MAP_CANADA(X)                                     \
    X("Rogers", "internet.com", "", "chap", "", "", "*99#", 1 )             \
    X("", "", "", "", "", "", "", 0)                                        \

#define EZP_COM_ISP_MAP_USA(X)                                              \
    X("AT&T", "proxy", "", "", "chap", "", "*99#", 1 )                      \
    X("Bell Mobility", "", "", "chap", "", "", "*99#", 1 )                  \
    X("Cellular One", "cellular1wap", "", "chap", "", "", "*99#", 1 )       \
    X("Cincinnati Bell", "wap.gocbw.com", "", "chap", "cbw", "", "*99#", 1 ) \
    X("T-Mobile(T-Zone)", "wap.voicestream.com", "", "chap", "", "", "*99#", 1 ) \
    X("T-Mobile(internet)", "internet2.voicestream.com","","chap", "","","*99#",1)  \
    X("Verizon", "", "", "chap", "", "","*99#", 1)                          \
    X("", "", "", "", "", "", "", 0)                                        \

#define EZP_COM_LOCATION_NAME_CONVERTER(LNAME,NAME,SHOWN) EZP_COM_LID_##LNAME,
typedef enum {
    EZP_COM_LOCATION_MAP(EZP_COM_LOCATION_NAME_CONVERTER)
} ezp_com_location_name_type;

struct ezp_com_location_map_entry_type
{
    ezp_com_location_name_type ln;
    char *desc;
    char shown;
};

struct ezp_com_isp_map_entry_type
{
    char *desc;
    char *apn;
    char *pin;
    char *auth;
    char *user_name;
    char *passwd;
    char *dialup_str;
    char shown;
};

#define EZP_COM_LOCATION_MAP_CONVERTER(LNAME,NAME,SHOWN) \
    {EZP_COM_LID_##LNAME, NAME, SHOWN},

static struct ezp_com_location_map_entry_type ezp_com_location_map[] = {
    EZP_COM_LOCATION_MAP(EZP_COM_LOCATION_MAP_CONVERTER)
};

#define EZP_COM_ISP_MAP_CONVERTER(NAME,APN,PIN,AUTH,USER,PW,DIALUP,SHOWN) \
   {NAME, APN, PIN, AUTH, USER, PW, DIALUP, SHOWN},

#define EZP_COM_ISP_MAP_FOR_EACH_LOCATION_CONVERTER(LNAME,NAME,SHOWN) \
static struct ezp_com_isp_map_entry_type ezp_com_isp_map_##LNAME[] = { \
    EZP_COM_ISP_MAP_##LNAME(EZP_COM_ISP_MAP_CONVERTER) \
}; 

/* Declare tables to each location. */
EZP_COM_LOCATION_MAP(EZP_COM_ISP_MAP_FOR_EACH_LOCATION_CONVERTER)


#define EZP_COM_ISP_POINTER_MAP_CONVERTER(LNAME,NAME,SHOWN) &ezp_com_isp_map_##LNAME[0],


static struct ezp_com_isp_map_entry_type *ezp_com_isp_map[] = {
    EZP_COM_LOCATION_MAP(EZP_COM_ISP_POINTER_MAP_CONVERTER)
};

#endif
