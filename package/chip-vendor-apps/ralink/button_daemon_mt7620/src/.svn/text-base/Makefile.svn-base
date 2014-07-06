EXEC = button_daemon

all: $(EXEC)

button_daemon: button_daemon.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -f *.o $(EXEC)

