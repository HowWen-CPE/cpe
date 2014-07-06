EXEC = sw_rst_daemon

all: $(EXEC)

sw_rst_daemon: sw_rst_daemon.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -f *.o $(EXEC)

