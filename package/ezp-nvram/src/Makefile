# $Id: Makefile 4726 2006-09-02 18:37:11Z nbd $
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/kernel.mk

EXTRA_CFLAGS := -c -I. -I../include
LIBSHARED_OBJS := shutils.o wl.o wl_linux.o linux_timer.o base64.o strings.o files.o process.o crc32.o
#LIBNVRAM_OBJS := nvram_linux.o ezp-lib.o
LIBNVRAM_OBJS := crc32.o nvram_env.o flash_api.o nvram_linux.o ezp-lib.o

#NVDIR = $(BUILD_DIR)/ralink-nvram
#SPECIAL_CFLAGS+= -Os -pipe -mips32 -mtune=mips32 -funit-at-a-time  -fhonour-copts
#SPECIAL_CFLAGS+= -I /home/daniel/liteon_project/disk2/CPE/trunk/staging_dir/mipsel/usr/include -I /home/daniel/liteon_project/disk2/CPE/trunk/build_dir/mipsel/ezp-nvram
#SPECIAL_CFLAGS+= -I$(NVDIR) -I$(LINUX_DIR)/drivers/char
#LDFLAGS += -Wl,--fatal-warnings -L/opt/toolchain-mipsel_gcc4.2.4/lib -Wl,--fatal-warnings -L/opt/toolchain-mipsel_gcc4.2.4/lib 
#LDFLAGS += -L $(NVDIR) -lnvram_ralink
#LIBNVRAM = $(BUILD_DIR)/ralink-nvram/libnvram_ralink-0.9.28.so
#CONF_H	= $(LINUX_DIR)/include/linux/autoconf.h
SPECIAL_CFLAGS+=-I $(LINUX_DIR)/include
all: libshared.so libnvram.so nvram

#%.o: %.c
#	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^
shutils.o: shutils.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^
wl.o: wl.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^
wl_linux.o: wl_linux.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^
linux_timer.o: linux_timer.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^
base64.o: base64.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^
strings.o: strings.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^
files.o: files.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^
process.o: process.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^
crc32.o: crc32.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^	
nvram_env.o: nvram_env.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS)  $(SPECIAL_CFLAGS) -o $@ $^	
flash_api.o: flash_api.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^	
nvram_linux.o: nvram_linux.c 
#	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(SPECIAL_CFLAGS) $(LDFLAGS) -o $@ $^ -lc -lgcc
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^	
ezp-lib.o: ezp-lib.c 
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^	


libshared.so: $(LIBSHARED_OBJS)
	$(CC) -shared -o $@ $^

libnvram.so: $(LIBNVRAM_OBJS)
	$(CC) -shared -o $@ $^

nvram: main.o
	$(CC) -o $@ $^ -L. -lnvram

clean:
	rm -f *.o *.so nvram
