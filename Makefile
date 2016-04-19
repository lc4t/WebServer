CC = g++
CFLAGS = -g -lpthread
targets = WebServer
vpath %.h headers
vpath %.cpp sources
obj = main.o Request.o Server.o Response.o


main: $(obj)
	$(CC) $(CFLAGS)  -o $(targets) $(obj)

main.o: main.cpp Server.h 
	$(CC) $(CFLAGS) -c main.cpp
Request.o: Request.h
	$(CC) $(CFLAGS) -c sources/Request.cpp
Server.o: Server.h Response.h
	$(CC) $(CFLAGS) -c sources/Server.cpp
Response.o: Response.h Request.h
	$(CC) $(CFLAGS) -c sources/Response.cpp
# main.o: Server.h

# Request.o: Request.h

# Server.o: Server.h


.PHONY : clean

clean:
	rm -f $(targets) $(obj)