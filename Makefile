CC = g++
CFLAGS = -g -lpthread
targets = WebServer
vpath %.h headers
vpath %.cpp sources
obj = main.o Http.o Server.o 


main: $(obj)
	$(CC) $(CFLAGS)  -o $(targets) $(obj)

main.o: main.cpp Server.h
	$(CC) $(CFLAGS) -c main.cpp
Http.o: Http.h
	$(CC) $(CFLAGS) -c sources/Http.cpp
Server.o: Server.h
	$(CC) $(CFLAGS) -c sources/Server.cpp
# main.o: Server.h

# Http.o: Http.h

# Server.o: Server.h


.PHONY : clean

clean:
	rm -f $(targets) $(obj)