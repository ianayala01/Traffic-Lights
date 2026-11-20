FIRSTNAME=Ian
LASTNAME=Ayala
HW=1

ROOTNAME=$(LASTNAME)_$(FIRSTNAME)_HW$(HW)

CC=gcc
CFLAGS= -g -I.
DEPS=

RUNOPTIONS =pigpio

LIBS =

ifeq ($(RUNOPTIONS),pigpio)
	SRC = $(ROOTNAME)_pigpio.c
	LIBS = -lpigpio -lrt -pthread -Wall
	TARGET = assignment1pigpio
else
	SRC = $(ROOTNAME)_dra.c
	LIBS =
	TARGET = assignment1b
endif

OBJ = $(TARGET).o

$(OBJ): $(SRC) $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm *.o assignment1pigpio assignment1b

run: $(TARGET)
	sudo ./$(TARGET)
