#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class BitStream{
    //bitstream class
    public:
    
        BitStream(string inFile = "",string outFile = "");
        void writeBit(int bit);
        int readBit(void);
        void writeBits(vector<int> n_bits);
        vector<int> readBits(int bits);
        vector<int> getBuffer(void);
        void close(void);

    private:
        vector<int> buffer;
        fstream in, out;
};

BitStream::BitStream(string inFile,string outFile){
    //bitstream constructor
    if(not inFile.empty()){
        in.open(inFile);
        char c;
        while(in.get(c)){
            for(int i = 7; i >=0; i--){
                buffer.push_back(((c >> i) & 1));
            }
        }
    }
    if(not outFile.empty()){
        out.open(outFile, fstream::out | fstream::app);
    }
}
void BitStream::writeBit(int bit){
    //BitStream writeBit method
    if(out.is_open()){
        out << bit;
        
    }else{
        cout << "No outfile to write to!" << endl;
    }
}
int BitStream::readBit(){
    //BitStream readBit
    if(in.is_open()){
        vector<int> auxiliar;
        for(int i = 1; i < buffer.size(); i++){
            auxiliar.push_back(buffer[i]);
        }
        int bit = buffer.at(0);
        buffer = auxiliar;
        return bit;
        
    }else{
        cout << "No input file to read!" << endl;
        return 2;
    }
}
void BitStream::writeBits(vector<int> n_bits){
    //BitStream write n bits
    if(out.is_open()){
        for(int x: n_bits){
            out << x;
        }
    }else{
        cout << "No output file to write into!" << endl;
    }
}
vector<int> BitStream::readBits(int bits){
    //BitStream read n bits
    vector<int> nbits;

    if(in.is_open()){
        vector<int> auxiliar;
        for(int i = bits; i < buffer.size(); i++){
            auxiliar.push_back(buffer[i]);
        }
        for(int x=0; x < bits; x++){
            nbits.push_back(buffer[x]);
        }
        buffer = auxiliar;
        return nbits;
        
    }else{
        cout << "No input file to read!" << endl;
        return nbits;
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