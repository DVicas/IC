CC= g++
FLAGS= -g -Wall `pkg-config --cflags --libs opencv4`


LosslessCodecEncode.o: test_lossEncode.cpp LosslessCodec.hh
	$(CC) $(FLAGS) $< -o $@

LosslessCodecDecode.o: test_lossDecode.cpp LosslessCodec.hh
	$(CC) $(FLAGS) $< -o $@

clean: 
	rm *.o
