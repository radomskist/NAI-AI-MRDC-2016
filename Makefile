# Compiler
CC   = g++
OPTS = -std=c++11

#Libraries
DLIBS =  -Wl,-Bdynamic -lusb-1.0 -lSDL2 -lGL -lfreenect2
INCS = -I./include -I./src/

#Directories
LDIR = -L./depends -L./depends/libusb/lib -Wl,-rpath=./depends/libusb/lib -Wl,-rpath=./depends
SRCS = $(shell find src -name '*.cpp')
DIRS = $(shell find src -type d | sed './src/./g' ) 
OBJS = $(patsubst src/%.cpp,obj/%.o,$(SRCS))


NaiBrain: $(OBJS)
	$(CC) $(LDIR) $(OPTS) $(OBJS) $(DLIBS) $(INCS) -o $@ -Wl,-as-needed 

obj/%.o: src/%.cpp
	-mkdir -p $(dir $@)
	-mkdir -p $(dir $(dir $@))
	-mkdir -p $(dir $(dir $(dir $@)))
	$(CC) $(OPTS) -c $< $(INCS) -o $@

