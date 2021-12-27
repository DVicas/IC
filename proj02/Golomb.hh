/*! \file Golomb.hh
    \brief Golomb implemented class. Used for encoding numbers and decoding golomb codes.

*/
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <bitset>

using namespace std;
int bin2d(int);

class Golomb {
    public:
        Golomb();
        string EncodeNumbers(int i, int m);
        short DecodeNumbers(string code, int m);
    private:
        int zzz;
};

Golomb::Golomb() {
    /**
    * Golomb class constructor.
    */
}

string Golomb::EncodeNumbers(int i, int m) {  
    /**
    * EncodeNumbers() will encode an arbitrary positive number i with an arbitrary number m that influences the calculations of, i.e, 
    * the quocient and the remainder.
    */ 
    int q, r;
    string str; 

    q = floor(i/m);
    r = i - q*m;

    for (int j = 0; j < q; j++) {
        str = str + '0';
    }
    str = str + "1";

    // check if m is a power of 2
    if (ceil(log2(m)) != floor(log2(m))) {
        int b = ceil(log2(m));
        int n_bits;

        if (r<b){
            n_bits = b-1;
        }
        else{
            r = r+pow(2,b)-m;
            n_bits = b;
        }

        int aux = 0;

        string auxStr = bitset<64>(r).to_string();
        
        string reverse = string();

        for (int j=63; j>63-n_bits; j--){
            reverse=auxStr[j]+reverse;
        }
        str+=reverse;
    }
    
    else{
        int aux = 0;

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
    */
    int k = ceil(log2(m));
    int t = pow(2, k) - m;
    short s;
    int r2;
    int r;

    int sep = (int) bits.find("1");
    string in_q = bits.substr(0, sep);
    string in_r = bits.substr(sep + 1);
    if (in_r.length() == 1) 
        in_r = '0' + in_r;
    //quotient
    int q = in_q.size();
    //store and remove last bit (of the remainder) to have k - 1 bits
    char last = in_r.back();
    int l_bit = last - '0';
    in_r.pop_back(); 
    if(in_r.length() == 0){
        r = 0;
    }else{
        r = stoi(in_r, 0, 2);//decimal conversion of the k - 1 bits
    }
    
    
    if(r < t){
        s = q * m + r;
    }else{
        r2 = r * 2 + l_bit;
        s = q * m + r2 - t;
        r = r + l_bit;
    }

    return s;
}