CC = g++
targets = WebServer
vpath %.h headers
vpath %.cpp sources
obj = main.o Server.o


main: $(obj)
	$(CC) $(obj) -o $(targets) -lpthread

main.o: Server.h

Server.o: Server.h

.PHONY : clean

clean:
	rm -f $(targets) $(obj)