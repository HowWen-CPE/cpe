EXEC = flashtest_wp838
EXTRA_CFLAGS := -I.
all: $(EXEC)

	
$(EXEC):  flash_api.c	
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ flash_api.c 


romfs:
	$(ROMFSINST) /bin/$(EXEC)

clean:
	-rm -f $(EXEC) *.elf *.gdb *.o

