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
        void DecodeNumbers(int m);

};

Golomb::Golomb() {

}

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

void Golomb::DecodeNumbers(int m) {

}