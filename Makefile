DEBUG=-g -O0
#DEBUG=-O3
INCLUDES=-I/usr/local/include -I. -I/usr/include/opencv -I/usr/include/opencv2
CPPFLAGS=$(DEBUG) -Wall $(INCLUDE) -Winline -pipe
LDFLAGS=-L/usr/local/lib -L/usr/lib -lopencv_legacy -lopencv_highgui -lopencv_core -lopencv_ml -lopencv_video \
		-lopencv_imgproc -lopencv_calib3d -lopencv_objdetect -lwiringPi -lpthread -lm -lstdc++
TARGET=snake_camera

SRC=ServoDriver.cpp SnakeCamera.cpp
HEADER=ServoDriver.hpp
OBJS=$(SRC:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)
	
.cpp.o: $(HEADER)
	$(CXX) -c $(CPPFLAGS) $< -o $@ $(INCLUDES)

test: $(HEADER) ServoDriver.cpp Test.cpp
	$(CXX) -o $@ $^ $(INCLUDES) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(TARGET) $(TEST)

