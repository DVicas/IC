#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "BitStream.hh"

using namespace std;

int main(int argc, char* argv[]){
    vector<int> buff;
    BitStream bstream("teste.txt"," ");
    for(int x: bstream.getBuffer()){
        cout << x;
    }
    bstream.close();
    return 1;
}