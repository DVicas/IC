#include <iostream>
#include <fstream>
#include <string>
#include "BitStream.hh"

using namespace std;

int main(int argc, char* argv[]){
    vector<int> buff;
    BitStream bstream("teste.txt");
    bstream.readBit();
    buff = bstream.getBuffer();
    for(int x: buff){
        cout << x;
    }
    bstream.close();
    return 1;
}