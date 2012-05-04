.phony	: all

all	: testConnect testCallback

ZMQ_INC = ./ZMQHelper
ZMQ_LIB = ./ZMQHelper
FLAG = -Wall -g

testConnect	: DIET_client_Tests.cpp diet.o
		g++ ${FLAG} DIET_client_Tests.cpp diet.o -I${ZMQ_INC} -L${ZMQ_LIB}/mdclient.opp -lzmq -o testConnect -lboost_regex

diet.o	:	DIET_client.cpp
		g++ ${FLAG} -c DIET_client.cpp -o diet.o -I${ZMQ_INC}  	 


testCallback	: ServerUMSTests.cpp internalApi.cpp ServerUMS.cpp
		g++ ${FLAG}  ServerUMSTests.cpp internalApi.cpp ServerUMS.cpp -o testCallback  -I${ZMQ_INC} -L${ZMQ_LIB}/mdclient.opp -lzmq 


