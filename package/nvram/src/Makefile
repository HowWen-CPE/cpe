CLI_FILENAME = nvram

ifeq ($(LITEON_APP_PATH), )

else 
 	CLI_PATH=$(LITEON_APP_PATH)/$(CLI_FILENAME)
	include $(LITEON_APP_PATH)/rules.gcc
endif

LIB_VERMAJOR = 0
LIB_VERMINOR = 1
LIB_FILENAME = libnvram.so

LIB_CFLAGS  = $(CFLAGS) -shared -fPIC -DDEBUG

CLI_CFLAGS  = $(CFLAGS)	-DDEBUG
CLI_LDFLAGS = $(LDFLAGS)

CLI_OBJ = cli.o
LIB_OBJ = crc.o nvram.o nvram_public.o nvram_fw.o nvram_rule.o

all: cli libnvram

cli: libnvram
	$(CC) $(CLI_CFLAGS) -c -o cli.o cli.c
	$(CC)  $(CLI_LDFLAGS) $(CLI_OBJ) \
	$(LIB_FILENAME) \
		-o $(CLI_FILENAME)

cli.o: cli.c
	$(CC) $(CLI_CFLAGS) -c -o $@ $<

libnvram:crc.c nvram.c nvram_public.c nvram_fw.c nvram_rule.c attr_2_desc.h rule_2_attr.h
	$(CC) $(LIB_CFLAGS) -c -o crc.o crc.c
	$(CC) $(LIB_CFLAGS) -c -o nvram.o nvram.c
	$(CC) $(LIB_CFLAGS) -c -o nvram_public.o nvram_public.c
	$(CC) $(LIB_CFLAGS) -c -o nvram_fw.o nvram_fw.c
	$(CC) $(LIB_CFLAGS) -c -o nvram_rule.o nvram_rule.c
	$(CC) $(LIB_CFLAGS)  \
		-o $(LIB_FILENAME) $(LIB_OBJ)

clean:
	rm -f $(CLI_FILENAME) $(LIB_FILENAME)* *.o

ifeq ($(LITEON_APP_PATH), )

else 
install:
	$(call install_program,$(CLI_PATH)/$(CLI_FILENAME),$(APP_INSTALL_ROOT))
	$(call install_library,$(LIB_FILENAME))
endif
