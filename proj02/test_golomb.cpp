/*! \file test_golomb.cpp
    \brief Golomb class file tester. Some useful encoding and decoding.

*/
#include "Golomb.hh"

using namespace std;

int main() {

    Golomb g;
    cout << g.EncodeNumbers(1225, 676) << endl;
    cout << "---------------" << endl;
    cout << g.EncodeNumbers(10, 6) << endl;
    cout << "---------------" << endl;
    cout << g.DecodeNumbers("011110000001", 676) << endl;
    cout << "---------------" << endl;
    g.DecodeNumbers("00111", 4);
    cout << "---------------" << endl;
    g.DecodeNumbers("01110", 5);
    cout << "---------------" << endl;
    g.DecodeNumbers("00100", 5);
    cout << "---------------" << endl;

    return 0;
}