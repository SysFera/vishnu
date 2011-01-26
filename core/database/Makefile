CC 	= g++
CCFLAGS = -Wall
LD 	= g++
LDFLAGS = -Wall
DEBUG	= -g
INC	= -I/usr/include/postgresql
LIB 	= -L/usr/lib/ -lpq


.phony	: all clean

all	: main main.o POSTGREDatabase.o DbFactory.o

#MYSQLDatabase.o	: MYSQLDatabase.cc
#		$(CC) $(CCFLAGS) $(INC) -c $< -o $@

Database.o       : Database.cc Database.hh
		$(CC) $(CCFLAGS) $(INC) $(LIB) -c $< -o $@

POSTGREDatabase.o	: POSTGREDatabase.cc
		$(CC) $(CCFLAGS) $(INC) $(LIB) -c $< -o $@

DbFactory.o	: DbFactory.cc POSTGREDatabase.hh
		$(CC) $(CCFLAGS) $(INC) -c $< -o $@

main.o: main.cc POSTGREDatabase.hh DbFactory.hh 
	$(CC) $(CCFLAGS) $(INC) $(LIB) -c $< -o $@


main: main.o POSTGREDatabase.o DbFactory.o Database.o
	$(CC) -o $@ $(CCFLAGS) $(INC) $(LIB) main.o POSTGREDatabase.o DbFactory.o Database.o


debug	: CCFLAGS += $(DEBUG)

clean	:
		rm *~ *.o

#g++ -Wall -I/usr/include/postgresql -L/usr/lib/ -lpq -c main.cc -o main.o
#g++ -Wall -I/usr/include/postgresql -L/usr/lib/ -lpq -c POSTGREDatabase.cc -o POSTGREDatabase.o
#g++ -Wall -I/usr/include/postgresql -c DbFactory.cc -o DbFactory.o
#
