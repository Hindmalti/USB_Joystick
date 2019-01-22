CC = gcc
CFLAGS = -W -Wall -pedantic -g -I ./usb
LDFLAGS = -lusb-1.0
EXEC = out
SRC = $(wildcard usb/*.c)
OBJ = $(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(OBJ)
cleanall: clean
	rm -rf $(EXEC)
