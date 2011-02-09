CC = g++
LD = g++
CCFLAGS = -g -Wall
LDFLAGS = -g -Wall
OBJ	= VishnuException.o SystemException.o UserException.o UMS.o TMS.o FMS.o IMS.o IMSVishnuException.o UMSVishnuException.o FMSVishnuException.o TMSVishnuException.o main.o InternalVishnuException.o

all	: main



main	: $(OBJ)
		$(LD) $(LDFLAGS) $(OBJ) -o $@

VishnuException.o	: VishnuException.cc
		$(CC) $(CCFLAGS) -c $< -o $@

main.o	: main.cc 
		$(CC) $(CCFLAGS) -c $< -o $@

UMS.o	: UMS.cc
		$(CC) $(CCFLAGS) -c $< -o $@

TMS.o	: TMS.cc
		$(CC) $(CCFLAGS) -c $< -o $@

FMS.o	: FMS.cc
		$(CC) $(CCFLAGS) -c $< -o $@

IMS.o	: IMS.cc
		$(CC) $(CCFLAGS) -c $< -o $@

IMSVishnuException.o	: IMSVishnuException.cc
		$(CC) $(CCFLAGS) -c $< -o $@

InternalVishnuException.o	: InternalVishnuException.cc
		$(CC) $(CCFLAGS) -c $< -o $@

UMSVishnuException.o	: UMSVishnuException.cc
		$(CC) $(CCFLAGS) -c $< -o $@

FMSVishnuException.o	: FMSVishnuException.cc
		$(CC) $(CCFLAGS) -c $< -o $@

TMSVishnuException.o	: TMSVishnuException.cc
		$(CC) $(CCFLAGS) -c $< -o $@

SystemException.o	: SystemException.cc
		$(CC) $(CCFLAGS) -c $< -o $@

UserException.o	: UserException.cc
		$(CC) $(CCFLAGS) -c $< -o $@

clean	:
		rm *~ *.o