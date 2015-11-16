include ../config-host.mak

# DEFINES=-I. -I.. 

PLUGIN = plugin.so

all: main.c 
	gcc -shared -o $(PLUGIN) -fPIC main.c
	
# %.o: %.c 
# 	gcc  -c -o $@ $<

# # %.o: %.cpp
# # 	g++ $(DEFINES) -c -o $@ $<

# %.so: main.o 
# 	g++  $^ -o -c $@ 

clean:
	rm -f *.o *.so *~ $(PLUGIN)


