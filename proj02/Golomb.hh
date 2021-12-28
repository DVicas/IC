/*! \file Golomb.hh
    \brief Golomb implemented class. Used for encoding numbers and decoding golomb codes.

*/
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <bitset>

using namespace std;


class Golomb {
    public:
        string EncodeNumbers(int i, int m);
        short DecodeNumbers(string code, int m);
    private:
};

string Golomb::EncodeNumbers(int i, int m) {  
    /**
    * EncodeNumbers() will encode an arbitrary positive number i with an arbitrary number m that influences the calculations of, i.e, 
    * the quocient and the remainder. External folding mechanisms are needed
    */ 
    int q, r;
    string str; 

    q = floor(i/m);
    r = i - q*m;

    for (int j = 0; j < q; j++) {
        str = str + '0';
    }
    str = str + "1";

    int aux = 0;
    // check if m is a power of 2
    if (ceil(log2(m)) != floor(log2(m))) {
        int b = ceil(log2(m));
        int n_bits;

        if (r < pow(2, b) - m){
            n_bits = b - 1;
        }
        else{
            r = r + pow(2,b) - m;
            n_bits = b;
        }

        string auxStr = bitset<64>(r).to_string();
        
        string reverse = string();

        for (int j=63; j>63-n_bits; j--){
            reverse=auxStr[j]+reverse;
        }
        str+=reverse;
    }
    
    else{

        string auxStr = bitset<64>(r).to_string();

        for (int j=0; j<64; j++){
            if(auxStr[j]=='1' && aux==0){
                aux=1;
                str+=auxStr[j];
            }
            else if (aux==1)
            {
                str+=auxStr[j];
            }
            
        }
    }
    return str;
}

short Golomb::DecodeNumbers(string bits, int m) {
    /**
    * DecodeNumbers() will decode a given Golomb code with its respective arbitrary m.
    * External folding mechanisms are needed.else{

    }
    */
    short s;
    int r2;
    int r;

    int sep = (int) bits.find("1");
    string in_q = bits.substr(0, sep);
    string in_r = bits.substr(sep + 1);

    //quotient
    int q = in_q.size();
    
    r = stoi(in_r, 0, 2);

    if(ceil(log2(m)) != floor(log2(m))){
        //not power of 2
        int b = ceil(log2(m));

        if(r >= pow(2, b) - m){
            r = r - pow(2, b) + m;
        }
    }
    
    return q*m + r;
    
}