#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "BitStream.hh"

using namespace std;

int main(int argc, char* argv[]){
   
    BitStream bstream(" ","xd.txt");
    bstream.writeBits("01000001");
    bstream.writeBits("01001110");
    bstream.writeBits("01000100");
    bstream.close();
    return 1;
}