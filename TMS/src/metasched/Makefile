LIBS=-L/opt/install/lib -lboost_system -lzmq -ljansson
INCLUDES=-I/opt/install/include
SERVEREXEC=server
CLIENTEXEC=client
CC=g++
CFLAGS=-Wall

all: $(SERVEREXEC) $(CLIENTEXEC)

$(CLIENTEXEC): TestClient.o JsonObject.o
	$(CC) $(CFLAGS) $(LIBS) TestClient.o JsonObject.o -o $(CLIENTEXEC)

TestClient.o: TestClient.cpp JsonObject.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(LIBS) -c TestClient.cpp JsonObject.cpp

$(SERVEREXEC): TestServer.o JsonObject.o
	$(CC) $(CFLAGS) $(LIBS) TestServer.o JsonObject.o -o $(SERVEREXEC)

TestServer.o: TestServer.cpp JsonObject.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(LIBS) -c TestServer.cpp JsonObject.cpp

clean:
	rm -rf *.o $(CLIENTEXEC) $(SERVEREXEC)
