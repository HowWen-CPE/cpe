#
# (C) Copyright 2011-2012
# .
#

SRCS	:= $(obj)crc32.c  board_data.c  main.c
HEADERS	:= board_data.h

CPPFLAGS := -Wall $(CFLAGS)

ifeq ($(MTD_VERSION),old)
CPPFLAGS += -DMTD_OLD
endif

all:	$(obj)print_env

$(obj)print_env:	$(SRCS) $(HEADERS)
	$(CC) $(CPPFLAGS) $(SRCS) -o $(obj)print_env

clean:
	rm -f $(obj)print_env
