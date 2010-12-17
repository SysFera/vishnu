CC 	= g++
CCFLAGS = -Wall
LD 	= g++
LDFLAGS = -Wall
DEBUG	= -g
INC	= -I/opt/mysql/include

.phony	: all clean

all	: MYSQLDatabase.o POSTGREDatabase.o DbFactory.o

MYSQLDatabase.o	: MYSQLDatabase.cc
		$(CC) $(CCFLAGS) $(INC) -c $< -o $@

POSTGREDatabase.o	: POSTGREDatabase.cc
		$(CC) $(CCFLAGS) -c $< -o $@

DbFactory.o	: DbFactory.cc
		$(CC) $(CCFLAGS) $(INC) -c $< -o $@


debug	: CCFLAGS += $(DEBUG)

clean	:
		rm *~ *.o