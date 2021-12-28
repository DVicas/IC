/*! \file BitStream.hh
    \brief Useful class to manipulate bits.

*/
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
        int z = 0;
        fstream in, out;
};

BitStream::BitStream(string inFile,string outFile){
    /**
    * BitStream constructor, this initializes two file streams if the given arguments are not empty; if the strings are empty
    * the stream will not be opened. If an input file is given it will immediately read all of the bits and store them
    * in an integer buffer.
    */
    if(not inFile.empty()){
        in.open(inFile);
        char c;
        while(in.get(c)){
            for(int i = 7; i >= 0; i--){
                buffer.push_back(((c >> i) & 1));
            }
        }
    }
    if(not outFile.empty()){
        out.open(outFile, fstream::out);
    }
}
void BitStream::writeBit(int bit){
    /**
    * this function will write on bit on a file
    */ 
    if(out.is_open()){
        out << bit;
        
    }else{
        cout << "No outfile to write to!" << endl;
    }
}
int BitStream::readBit(){
    /**
    * this function will read and return one bit (MSB) from a given input file  
    */
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
    /**
    * Writes the given bits on a file by storing them up to a byte, this requires external 'truncation' mechanisms; 
    */
    int bitcount = 0;
    char curr_byte ;

    if(out.is_open()){
        for(int i = 0; i < bits.length(); i++){
            curr_byte = curr_byte << 1 | (bits[i] - '0');
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
    /**
    * This function will read @bits amount of bits by taking them out of the BitStream buffer and, consequently, erasing 
    * them after the operation is done.
    */

    if(in.is_open()){
        string nbits;

        int y = z + bits;

        for(; z < y; z++){
             nbits += to_string(buffer[z]);
        }
        return nbits;
        
    }else{
        cout << "No input file to read!" << endl;

        return "";
    }
}
vector<int> BitStream::getBuffer(){
    /**
    *  getBuffer() will return the bit buffer from the BitStream class
    */
    return buffer;
}
void BitStream::close(void){
    /**
    * Useful method to reallocate memory
    */

    if(in.is_open()){
        in.close();
    }
    if(out.is_open()){
        out.close();
    }


}