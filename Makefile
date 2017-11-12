
CFLAGS=-fPIC -std=gnu++11 -O3 -march=mips32r2 -I/usr/local/include/libfreenect
LDFLAGS=-lm -lopencv_core -lopencv_imgproc -lopencv_highgui -lfreenect

kinect-depth-png: kinect.cpp kinect-depth-png.cpp
	clang++ $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	rm -f *.o kinect-depth-png
