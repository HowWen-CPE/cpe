#
# boarddata
#

all: libboarddata.so boarddata

libboarddata.so: boarddata.c
	$(CC) $(CFLAGS) -fpic -shared -o $@ $^

boarddata: main.c
	$(CC) $(CFLAGS) -o $@ $^ -L. -lboarddata

clean:
	rm -f *.o *.a *.so boarddata
