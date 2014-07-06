all: libethread.so ethread 
libethread.so: libethread.c
	$(CC) $(CFLAGS) -fpic -shared -o $@ $^
ethread: ethread.c
	$(CC) $(CFLAGS) -o $@ $^ -L. -lethread

clean:
	rm -f *so* *.o  ethread

