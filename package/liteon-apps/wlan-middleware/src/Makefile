LDFLAGS += -L../nvram -lnvram
#LDFLAGS += -L../vlanmid -lvlanmid
LIB_MID_NAME = libwlan-middleware.so
LIB_MID_CFLAGS = $(CFLAGS) \
				 -I ./include

COMMON_HEADS = include/mid_detail.h \
			   include/mid_detail_arch.h \
			   include/mid_common.h \
			   include/mid_common_nvram.h 

MID_OBJS = mid_func.o\
		   mid_func_arch.o\
		   mid_common_nvram.o\
		   mid_common_wlan.o\
		   mid_detail_ap_get.o \
		   mid_detail_ap_set.o \
		   mid_detail_sta.o \
		   mid_detail_wds.o

all:$(COMMON_HEADS) $(MID_OBJS)
	$(CC) $(LIB_MID_CFLAGS) -shared \
		-o $(LIB_MID_NAME) $(MID_OBJS) $(LDFLAGS) 

clean:
	rm -rf $(LIB_MID_NAME) $(LIB_MID_NAME)* *.o

#$(MID_OBJS):$(COMMON_HEADS)
#	$(CC) -c $*.c -fpic \
#	$(LIB_MID_CFLAGS) 

mid_func.o: $(COMMON_HEADS) include/mid_func.h

mid_func_arch.o: $(COMMON_HEADS) include/mid_func.h

mid_common_nvram.o: $(COMMON_HEADS) include/mid_common_nvram.h

mid_common_wlan.o: $(COMMON_HEADS) include/mid_common.h

mid_detail_ap_get.o: $(COMMON_HEADS) include/mid_detail_ap_get.h

mid_detail_ap_set.o: $(COMMON_HEADS) include/mid_detail_ap_set.h

mid_detail_sta.o: $(COMMON_HEADS) include/mid_detail_sta.h

mid_detail_wds.o: $(COMMON_HEADS) include/mid_detail_wds.h


.c.o:
	$(CC) $(LIB_MID_CFLAGS) -fpic \
		-c -o $@ $< 
