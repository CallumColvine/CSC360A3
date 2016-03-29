# CC = gcc
CFLAGS = -Wall -g   
LDFLAGS = -lpthread 

A3OBJSINFO = diskinfo.o diskobj.o
A3OBJSLIST = disklist.o
A3OBJSGET = diskget.o
A3OBJSPUT = diskput.o

all: diskinfo disklist diskget diskput

diskinfo: $(A3OBJSINFO)
	$(CXX) $(CFLAGS) -o diskinfo $(A3OBJSINFO) $(LDFLAGS) 

disklist: $(A3OBJSLIST)
	$(CXX) $(CFLAGS) -o disklist $(A3OBJSLIST) $(LDFLAGS) 

diskget: $(A3OBJSGET)
	$(CXX) $(CFLAGS) -o diskget $(A3OBJSGET) $(LDFLAGS) 

diskput: $(A3OBJSPUT)
	$(CXX) $(CFLAGS) -o diskput $(A3OBJSPUT) $(LDFLAGS) 

clean: 
	rm -rf $(A3OBJSINFO) diskinfo disklist diskget diskput
