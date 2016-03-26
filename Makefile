# CC = gcc
CFLAGS = -Wall -g   
LDFLAGS = -lpthread 

A3OBJS = diskinfo.o diskobj.o

all: diskinfo 

diskinfo: $(A3OBJS)
	$(CXX) $(CFLAGS) -o diskinfo $(A3OBJS) $(LDFLAGS) 

clean: 
	rm -rf $(A3OBJS) diskinfo 
