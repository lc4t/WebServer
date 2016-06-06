CC = g++
CFLAGS = -g -lpthread -std=c++11
targets = WebServer
obj = main.o Request.o Manage.o Server.o StaticPage.o Page.o PHP.o



main: $(obj)
	$(CC) $(CFLAGS) -o $(targets) $(obj)

main.o: headers/Server.h
	$(CC) $(CFLAGS) -c main.cpp

Server.o: headers/Server.h
	$(CC) $(CFLAGS) -c sources/Server.cpp

Manage.o: headers/Manage.h
	$(CC) $(CFLAGS) -c sources/Manage.cpp

Request.o: headers/Request.h
	$(CC) $(CFLAGS) -c sources/Request.cpp

Page.o: headers/Page.h
		$(CC) $(CFLAGS) -c sources/Page.cpp

StaticPage.o: headers/StaticPage.h
	$(CC) $(CFLAGS) -c sources/StaticPage.cpp

PHP.o: headers/PHP.h
	$(CC) $(CFLAGS) -c sources/PHP.cpp





# main.o: Server.h

# Request.o: Request.h

# Server.o: Server.h


.PHONY : clean r

clean:
	rm -f $(obj)

r:
	pkill WebServer;rm -f $(targets) $(obj);make

rn:
	pkill WebServer;rm -f $(targets) $(obj);make;./WebServer
