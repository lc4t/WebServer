CC = g++
CFLAGS = -g -lpthread
targets = WebServer
obj = main.o Request.o Server.o Response.o



main: $(obj)
	$(CC) $(CFLAGS)  -o $(targets) $(obj)

main.o: headers/Server.h
	$(CC) $(CFLAGS) -c main.cpp

Server.o: headers/Server.h
	$(CC) $(CFLAGS) -c sources/Server.cpp

Request.o: headers/Request.h
	$(CC) $(CFLAGS) -c sources/Request.cpp

Response.o: headers/Response.h
	$(CC) $(CFLAGS) -c sources/Response.cpp


# main.o: Server.h

# Request.o: Request.h

# Server.o: Server.h


.PHONY : clean

clean:
	rm -f $(targets) $(obj)