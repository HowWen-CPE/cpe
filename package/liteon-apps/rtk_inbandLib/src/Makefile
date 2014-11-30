LIB_MID_NAME = librtk_inbandLib.so
LIB_MID_CFLAGS = $(CFLAGS) -I ./

COMMON_HEADS =  ioh.h inband_if.h
MID_OBJS = inband_api.o inband_if.o ioh.o

all:$(COMMON_HEADS) $(MID_OBJS)
	$(CC) $(LIB_MID_CFLAGS) -shared \
		-o $(LIB_MID_NAME) $(MID_OBJS) $(LDFLAGS) 

$(MID_OBJS):$(COMMON_HEADS)
	$(CC) -c $*.c -fpic \
	$(LIB_MID_CFLAGS) 

clean:
	rm -rf $(LIB_MID_NAME) $(LIB_MID_NAME)* *.o
