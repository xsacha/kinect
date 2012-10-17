all: kinect-depth-ascii

CFLAGS=-O3 -I/usr/local/include/libfreenect
LDFLAGS=-L/usr/local/lib -lpng -lfreenect

kinect-depth-ascii: kinect.o image.o kinect-depth-ascii.c
	cc $(CFLAGS) $(LDFLAGS) -o kinect-depth-ascii $^

kinect.o: kinect.h kinect.c
	cc $(CFLAGS) -c kinect.c

image.o: image.h image.c
	cc $(CFLAGS) -c image.c

clean:
	rm -f *.o kinect-depth-ascii
