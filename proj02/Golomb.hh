#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <bitset>

using namespace std;

class Golomb {
    public:
        Golomb(int n, int x);
        void EncodeNumbers();
        void DecodeNumbers();
    private:
        int i;
        int m;
};

Golomb::Golomb(int n, int x) {
    i = n;
    m = x;
}

void Golomb::EncodeNumbers() {   
    int q, r;
    string str = string(); 

    //if (i>=0) i = i*2;
    //else i = i*(-2)-1;

    q = floor(i/m);
    r = i - q*m;

    for (int j = 0; j < q; j++) {
        str = str + '0';
    }
    str = str + "1 ";

    // check if m is a power of 2
    if (ceil(log2(m)) != floor(log2(m))) {
        int b = ceil(log2(m));
        int n_bits;

        if (r<b){
            n_bits = b-1;
        }
        else{
            r = r+pow(2,b);
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


    cout << str << endl;
    
    
}

void Golomb::DecodeNumbers() {

}