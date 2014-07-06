LDFLAGS += -L../nvram -lnvram
LIB_MID_NAME = libwlan-middleware.so
LIB_MID_CFLAGS = $(CFLAGS) \
				 -I ./include

COMMON_HEADS = include/mid_common.h include/mid_detail.h include/mid_func.h
MID_OBJS = mid_detail_ap_get.o mid_detail_ap_set.o mid_detail_sta.o \
			mid_common_nvram.o mid_common_wlan.o mid_func.o

all:$(COMMON_HEADS) $(MID_OBJS)
	$(CC) $(LIB_MID_CFLAGS) -shared \
		-o $(LIB_MID_NAME) $(MID_OBJS) $(LDFLAGS) 

$(MID_OBJS):$(COMMON_HEADS)
	$(CC) -c $*.c -fpic \
	$(LIB_MID_CFLAGS) 

clean:
	rm -rf $(LIB_MID_NAME) $(LIB_MID_NAME)* *.o
