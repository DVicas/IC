#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class BitStream{
    //bitstream class
    public:
    
        BitStream(string inFile = "",string outFile = "");
        void writeBit(void);
        void readBit(void);
        vector<int> getBuffer(void);
        void close(void);

    private:
        vector<int> buffer;
        fstream in;
        fstream out;
};

BitStream::BitStream(string inFile,string outFile){
    //bitstream constructor
    if(not inFile.empty()){
        in.open(inFile);
    }
    if(not outFile.empty()){
        out.open(outFile);
    }
}
void BitStream::writeBit(){
    //BitStream writeBit method
}
void BitStream::readBit(){
    //BitStream readBit
    char c;
    while(in.get(c)){
        for(int i = 7; i >=0; i--){
            buffer.push_back(((c >> i) & 1));
        }
    }

}
vector<int> BitStream::getBuffer(){
    return buffer;
}
void BitStream::close(void){
    //BitStream close method
    if(in.is_open()){
        in.close();
    }
    if(out.is_open()){
        out.close();
    }


}