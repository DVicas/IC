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
        void writeBits(string bits);
        string readBits(int bits);
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
            for(int i = 0; i < 8; i++){
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
void BitStream::writeBits(string bits){
    //BitStream write n bits
    int bitcount = 0;
    char curr_byte = 0;

    if(out.is_open()){
        for(int i = 0; i < bits.length(); i++){
            curr_byte = curr_byte << 1 | bits[i];
            bitcount++;
            if (bitcount == 8){
                out << curr_byte;
                curr_byte = 0;
                bitcount = 0;
            }
        }
    }else{
        cout << "No output file to write into!" << endl;
    }
}
string BitStream::readBits(int bits){
    //BitStream read n bits

    if(in.is_open()){
        vector<int> auxiliar;
        string nbits;
        for(int i = 0; i < buffer.size() - bits; i++){
            auxiliar.push_back(buffer[i]);
        }
        for(int x = buffer.size() - 10; x < buffer.size(); x++){
            nbits += buffer[x];
        }
        buffer = auxiliar;
        return nbits;
        
    }else{
        cout << "No input file to read!" << endl;
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