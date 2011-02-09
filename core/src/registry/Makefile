CC	= g++
LD	= g++
CFLAGS	= -Wall -g
LDFLAGS = -Wall -g
OBJ	= main.o Mapper_impl.o Mapper_impl2.o Mapper.o MapperRegistry.o
INC	= -I/home/keo/Bureau/depot_git_edf/vishnu/core/src/exception
LK	= -L/home/keo/Bureau/depot_git_edf/vishnu/core/lib -lexception-vishnu

all	: main

main	: $(OBJ)
		$(LD) $(LDFLAGS) -o $@ $(OBJ) $(LK)

main.o	: main.cpp
		$(CC) $(CCFLAGS) -c $< -o $@ $(INC)

Mapper_impl.o	: Mapper_impl.cpp
		$(CC) $(CCFLAGS) -c $< -o $@ $(INC)

Mapper_impl2.o	: Mapper_impl2.cpp
		$(CC) $(CCFLAGS) -c $< -o $@ $(INC)

Mapper.o	: Mapper.cpp
		$(CC) $(CCFLAGS) -c $< -o $@ $(INC)

MapperRegistry.o	: MapperRegistry.cpp
		$(CC) $(CCFLAGS) -c $< -o $@ $(INC)

clean	:
		rm *~ *.o