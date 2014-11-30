PROG = inband
inband_CFLAGS = $(CFLAGS) -I ./
inband_HEADS = 
inband_OBJS = main.o

$(PROG): $(inband_OBJS)
	$(CC) $(inband_CFLAGS) $(EXTRA_CFLAGS) \
	$(LDFLAGS)  $(INBAND_LIB) \
	-o $(PROG) main.o
$(inband_OBJS):
	$(CC) -c $*.c -fpic \
	$(inband_CFLAGS)
clean:
	rm -f *.o inband
