INCLUDE_DIRS := Includes
CC 			 := g++
CFLAGS 		 := -I $(INCLUDE_DIRS)

OBJS 		 := avlClass.o customErrorClass.o Koball_CC2.o

.PHONY: clean all

all: $(OBJS) Koball_CodeChallengeTwo

clean:
	rm  $(OBJS) Koball_CodeChallengeTwo

Koball_CodeChallengeTwo: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

customErrorClass.o: customErrorClass.cpp
	$(CC) -c -o $@ $(CFLAGS) $<

avlClass.o: avlClass.cpp
	$(CC) -c -o $@ $(CFLAGS) $<

Koball_CC2.o: Koball_CC2.cpp
	$(CC) -c -o $@ $(CFLAGS) $<