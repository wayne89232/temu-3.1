include ../config-host.mak

DEFINES=-I. -I.. -I$(SRC_PATH) -I$(SRC_PATH)/include -I$(SRC_PATH)/x86_64-softmmu -I$(SRC_PATH)/target-i386 -I$(SRC_PATH)/fpu 
DEFINES+=-D_GNU_SOURCE

CFLAGS=-Wall -O4 -g -fPIC -MMD 
LDFLAGS=-g -Wl -shared 
LIBS=

LIBS+=-lm 

PLUGIN = plugin.so

all: main.c 
	gcc -shared -o $(PLUGIN) $(CFLAGS) $(DEFINES)  main.c 
	
# %.o: %.c 
# 	gcc  -c -o $@ $<

# # %.o: %.cpp
# # 	g++ $(DEFINES) -c -o $@ $<

# %.so: main.o 
# 	g++  $^ -o -c $@ 

clean:
	rm -f *.o *.so *~ $(PLUGIN)


