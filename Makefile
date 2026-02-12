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
else
	SRC = $(ROOTNAME)_DRA.c
	LIBS =
endif

OBJ = $(ROOTNAME)_$(RUNOPTIONS).o

$(OBJ): $(SRC) $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ROOTNAME)_$(RUNOPTIONS): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm *.o $(ROOTNAME)_pigpio $(ROOTNAME)_DRA

run: $(ROOTNAME)_$(RUNOPTIONS)
	sudo ./$(ROOTNAME)_$(RUNOPTIONS)
