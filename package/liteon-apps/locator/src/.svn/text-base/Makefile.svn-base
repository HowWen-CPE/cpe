#CC=$(CROSS_COMPILE)gcc
#AR=$(CROSS_COMPILE)ar
all: locator

locator: locator.o
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(NVRAM_LIB) $(WLANMW_LIB) -o $@ $^
#$(NVRAM_LIB) 

clean:
	rm -f *.o locator
