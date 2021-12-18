#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <bitset>

using namespace std;

class Golomb {
    public:
        Golomb();
        void EncodeNumbers(int i, int m);
        int DecodeNumbers(string bits, int m);

};

Golomb::Golomb() { }

void Golomb::EncodeNumbers(int i, int m) {   
    int q, r;
    string str = " ";  

    // check if m is a power of 2
    if (ceil(log2(m)) != floor(log2(m))) {
        int b = ceil(log10(m));
    }


    q = floor(i/m);
    r = i - q*m;

    for (int j = 0; j < q; j++) {
        str = str + '0';
    }
    str = str + "1 ";

    str += bitset<2>(r).to_string();

    cout << str << endl;
    
}

int Golomb::DecodeNumbers(string bits, int m) {
    
    cout << "Bits: " << bits << endl;
    cout << "m: " << m << endl;
    
    int b, x;

    int sep = (int) bits.find(" ");
    string in_q = bits.substr(0, sep);
    string in_r = bits.substr(sep+1);

    int q = in_q.size() - 1 ;
        
    int r = 0; 
    int n = 0;
    string::iterator it;
    for(it = in_r.begin(); it != in_r.end(); it++) {
        if(*it == '1') {
            r += pow(2, n);
        }
        n++;
    }

    cout << "q: " << q << endl;
    cout << "r: " << r << endl;

    x = pow(b, 2);

    return 0;
}