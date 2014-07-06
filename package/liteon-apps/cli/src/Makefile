PROG = cli
CLI_CFLAGS = $(CFLAGS) -I ./include
CLI_HEADS = clicbk.h  clicbklib.h  cli.h
CLI_OBJS = cli.o clicbk/clicbk.o clicbk/clicbklib.o

$(PROG): $(CLI_OBJS)
	$(CC) $(CLI_CFLAGS) $(EXTRA_CFLAGS) \
	$(READLINE_LIB) \
	$(LDFLAGS) $(NVRAM_LIB) \
	$(ETHREAD_LIB) \
	$(WLANMW_LIB) \
	-o $(PROG) cli.o clicbk.o clicbklib.o
$(CLI_OBJS):
	$(CC) -c $*.c -fpic \
	$(CLI_CFLAGS)
clean:
	rm -f *.o cli
