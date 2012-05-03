.phony	: all

all	: testConnect testCallback

ZMQ_INC = /home/phi/Dev/ZMQ/zeromqTest/src/base
ZMQ_LIB = /home/phi/Dev/ZMQ/zeromqTest/src/base

testConnect	: DIET_client_Tests.cpp diet.o
		g++ -Wall -g DIET_client_Tests.cpp diet.o -I${ZMQ_INC} -L${ZMQ_LIB}/mdclient.opp -lzmq -o testConnect

diet.o	:	DIET_client.c
		g++ -Wall -g -c DIET_client.c -o diet.o -I${ZMQ_INC}  	 


testCallback	: ServerUMSTests.cpp internalApi.cpp ServerUMS.cpp
		g++ -Wall -g  ServerUMSTests.cpp internalApi.cpp ServerUMS.cpp -o testCallback  -I${ZMQ_INC} -L${ZMQ_LIB}/mdclient.opp -lzmq


