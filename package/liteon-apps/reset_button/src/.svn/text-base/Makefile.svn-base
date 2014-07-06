EXEC = reset_button

all: $(EXEC)

reset_button: reset_button.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -f *.o $(EXEC)

