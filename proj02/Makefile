CC= g++
CFLAGS= -g -Wall `pkg-config --cflags opencv4`
LFLAGS= `pkg-config --libs opencv4`

LosslessCodec.o: test_lossCodec.cpp LosslessCodec.hh
	$(CC) $(CFLAGS) $(LFLAGS) $< -o $@

clean: 
	rm *.o
