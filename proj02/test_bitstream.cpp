#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "BitStream.hh"

using namespace std;

int main(int argc, char* argv[]){
    vector<int> buff;
    BitStream bstream("teste.txt","out.txt");
    cout << bstream.readBit();
    vector<int> n_bits{1,0,0,1,0};
    bstream.writeBits(n_bits);
    //for(int x: bstream.getBuffer()){
    //    cout << x;
    //}
    bstream.close();
    return 1;
}