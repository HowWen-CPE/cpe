EXEC = mii_mgr
OBJS = mii_mgr.o


all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)


clean:
	-rm -f $(EXEC) *.elf *.gdb *.o

