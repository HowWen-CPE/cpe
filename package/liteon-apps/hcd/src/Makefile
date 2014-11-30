PROG = hcd
HCD_CFLAGS = $(CFLAGS) -I ./
HCD_HEADS = cmd.h  hcd.h
HCD_OBJS = hcd.o cmd.o

$(PROG): $(HCD_OBJS)
	$(CC) $(HCD_CFLAGS) $(EXTRA_CFLAGS) \
	$(LDFLAGS) $(INBAND_LIB) \
	-o $(PROG) cmd.o hcd.o
$(HCD_OBJS):
	$(CC) -c $*.c -fpic \
	$(HCD_CFLAGS)
clean:
	rm -f *.o hcd
