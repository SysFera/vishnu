BASE_LIBS=-L/usr/lib/ -lzmq -L/opt/install/lib -lboost_filesystem -lboost_system -L/opt/install/lib -ljansson
XML_BASE_LIBS=`xmlrpc-c-config c++2 client --libs` -lxerces-c
INCLUDES=-I/opt/install/include
SERVEREXEC=server
CLIENTEXEC=client
CC=g++
CFLAGS=-Wall

all: $(SERVEREXEC) $(CLIENTEXEC)

$(CLIENTEXEC): TestClient.o JsonObject.o
	$(CC) $(CFLAGS) $(BASE_LIBS) TestClient.o JsonObject.o -o $(CLIENTEXEC) $(BASE_LIBS)

TestClient.o: TestClient.cpp JsonObject.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(BASE_LIBS) -c TestClient.cpp JsonObject.cpp

$(SERVEREXEC): TestServer.o JsonObject.o
	$(CC) $(CFLAGS) TestServer.o JsonObject.o -o $(SERVEREXEC) $(BASE_LIBS) 

TestServer.o: TestServer.cpp JsonObject.cpp
	$(CC) $(INCLUDES) $(CFLAGS) -c TestServer.cpp JsonObject.cpp

OneHostPool:

	$(CC) $(INCLUDES) $(CFLAGS) OneRPCManager.cpp OneHostPool.cpp Main.cpp  $(BASE_LIBS) $(XML_BASE_LIBS)

clean:
	rm -rf *.o $(CLIENTEXEC) $(SERVEREXEC)
