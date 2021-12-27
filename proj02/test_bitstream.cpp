/*! \file test_bitstream.cpp
    \brief BitStream class file tester.

*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "BitStream.hh"

using namespace std;

int main(int argc, char* argv[]){
   
    BitStream bstream("x.txt","xd.txt");
    // for(int x : bstream.getBuffer()){
    //      cout << x;
    //  }
    string x = bstream.readBits(8);
    cout << x;
    // bstream.writeBits("01100001");
    // cout << bstream.readBit();
    // cout << bstream.readBit();
    bstream.close();
    return 1;
}