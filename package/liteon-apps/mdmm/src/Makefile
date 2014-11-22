#
# boarddata
#

all: md mm

md: md.c 
	$(CC) $(CFLAGS) -o $@ $^ 

mm: mm.c 
	$(CC) $(CFLAGS) -o $@ $^ 

clean:
	rm -f *.o *.a *.so md mm
