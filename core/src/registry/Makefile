CC	= g++
LD	= g++
CFLAGS	= -Wall -g
LDFLAGS = -Wall -g
OBJ	= main.o Mapper_impl.o Mapper.o MapperRegistry.o
INC	= -I/home/keo/Bureau/depot_git_edf/vishnu/core/exception
LK	= -L/home/keo/Bureau/depot_git_edf/vishnu/core/database/build -lexception-vishnu

all	: main

main	: $(OBJ)
		$(LD) $(LDFLAGS) -o $@ $(OBJ) $(LK)

main.o	: main.cc
		$(CC) $(CCFLAGS) -c $< -o $@ $(INC)

Mapper_impl.o	: Mapper_impl.cc
		$(CC) $(CCFLAGS) -c $< -o $@ $(INC)

Mapper.o	: Mapper.cc
		$(CC) $(CCFLAGS) -c $< -o $@ $(INC)

MapperRegistry.o	: MapperRegistry.cc
		$(CC) $(CCFLAGS) -c $< -o $@ $(INC)
