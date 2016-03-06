# Compiler
CC   = g++
OPTS = -std=c++11 -ffast-math

#Libraries
DLIBS =  -Wl,-Bdynamic -lusb-1.0 -lGL -lSDL2 -lfreenect2 -lopencv_core -lopencv_videoio -lopencv_imgproc -lopencv_features2d -lzbar
SLIBS = -Wl,-Bstatic -lGLEW 
INCS = -I./include -I./src/ -I./include/libfreenect2

#Directories
LDIR = -L./depends -L./depends/libusb -L./depends/libfreenect2 -Wl,-rpath=./depends/lib -Wl,-rpath=./depends/libfreenect2
SRCS = $(shell find src -name '*.cpp')
DIRS = $(shell find src -type d | sed './src/./g' ) 
OBJS = $(patsubst src/%.cpp,obj/%.o,$(SRCS))


NaiBrain: $(OBJS)
	$(CC) $(LDIR) $(OPTS) $(OBJS) $(SLIBS) $(DLIBS) $(INCS) -o $@ -Wl,-as-needed 

obj/%.o: src/%.cpp
	-mkdir -p $(dir $@)
	-mkdir -p $(dir $(dir $@))
	-mkdir -p $(dir $(dir $(dir $@)))
	$(CC) $(OPTS) -c $< $(INCS) -o $@

clean:
	rm -r ./obj ./NaiBrain
