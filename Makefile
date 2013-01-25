# Reference: http://mrbook.org/tutorials/make/

CC=clang++
CFLAGS=-c -Wall -Werror -g -O3 -I/usr/local/include -I/opt/local/include
LDFLAGS=-lglog -lgflags -L/usr/local/lib -L/opt/local/lib
SOURCES=bingo.cc
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=bingo

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cc.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf bingo *.o *.gv *.png
