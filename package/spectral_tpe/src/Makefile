#
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk
#
# Makes spectral apps
#
#ifeq ($(obj),)
#obj := .
#endif

# Default OS is linux
#ifeq ($(OS),)
#OS := linux
#endif


# Important note: Values of CC, AR and LD should be passed to this script.

# Path defines...
#ATHEROSPATH:=$(TOPDIR)/drivers/wlan_modules
ATHEROSPATH:=$(KERNEL_BUILD_DIR)/wifi_QCA_10.1.436/drivers/wlan_modules/
SPECTRAL_DRIVER_PATH	:= $(ATHEROSPATH)/lmac/spectral
ADF_INC					:= ${ATHEROSPATH}/adf/include
ADF_OS_INC				:= ${ATHEROSPATH}/adf/include/os
ASF_INC					:= ${ATHEROSPATH}/asf/include
ATH_DEV_INC				:= ${ATHEROSPATH}/lmac/ath_dev
UMAC_INC					:= ${ATHEROSPATH}/umac/include
COMMON_INC				:= ./common
HAL						:=${ATHEROSPATH}/hal
HAL_INC					:=${ATHEROSPATH}/hal
OS							:=linux
CLASSIFIER_SRC			:= ./common
CLASSIFIER_INC			:= ./common
#BINDIR					= /usr/local/bin
DEPTH					:= ../../drivers/wlan
CHANNEL_SCAN_SRC                   :=./channelscan
CHANNEL_SCAN_INC                   :=./channelscan

# Spectral
#ifeq ($(strip ${ATH_SUPPORT_SPECTRAL}),1)
ALL+= ssd athssd
SPECTRAL_TOOLS+= ssd athssd
#endif

all: ${ALL}

# Add necessary include path
INCS+= -I$(ATHEROSPATH) -I${HAL}/${OS} -I${HAL_INC} -I${COMMON_INC}
INCS+= -I${CLASSIFIER_INC} -I${ADF_INC} -I${ADF_OS_INC} -I${ASF_INC}
INCS+= -I${SPECTRAL_DRIVER_PATH} -I${ATH_DEV_INC} -I.
INCS+= -I${CHANNEL_SCAN_INC}

# Update CFLAGS
CFLAGS= ${INCS} ${COPTS} -Wall

ifeq (${BUILD_STATIC}, y)
LDFLAGS= -static
endif

ifeq (${BUILD_DEBUG}, y)
CFLAGS+= -g
endif

ifeq (${BUILD_OPTIMIZED}, y)
CFLAGS += -Os -pipe -mips32r2 -mtune=mips32r2 -funit-at-a-time
endif

ssd: ssd.c ssd_utils.c ssd_proto.c ssd_data.c ssd_list.c ssd_interf.c ath_classifier.c ${CLASSIFIER_SRC}/classifier.c  ${CHANNEL_SCAN_SRC}/channel_scan.c  ${CHANNEL_SCAN_SRC}/iwlib.c Spectral_speed.c
	${CC} -o ssd ${CFLAGS} ${LDFLAGS} ssd.c ssd_proto.c ssd_data.c ssd_utils.c ssd_list.c ssd_interf.c ${CLASSIFIER_SRC}/classifier_init.c ${CLASSIFIER_SRC}/classifier.c ath_classifier.c ${CHANNEL_SCAN_SRC}/channel_scan.c  ${CHANNEL_SCAN_SRC}/iwlib.c Spectral_speed.c -lm -lrt  -lpthread

athssd: ath_ssd.c ath_classifier.c ${CLASSIFIER_SRC}/classifier.c ath_ssd_cmds.c
	${CC} -o athssd ${CFLAGS} ${LDFLAGS} ath_ssd.c ath_ssd_cmds.c ath_classifier.c -lm

diagtools: logadc.c logspectral.c
	make -f Makefile.logspectraladc

clean:
	@rm -rf ssd athssd
 
athssd_clean:
	@rm -rf athssd
