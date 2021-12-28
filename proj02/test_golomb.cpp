/*! \file test_golomb.cpp
    \brief Golomb class file tester. Some useful encoding and decoding.

*/
#include "Golomb.hh"

using namespace std;

int main() {

    Golomb g;
    cout << g.EncodeNumbers(273, 74) << endl;
    cout << g.DecodeNumbers("0001110011", 74) << endl;
    cout << "---------------" << endl;
    cout << g.EncodeNumbers(15, 5) << endl;
    cout << g.DecodeNumbers("000100", 5) << endl;
    cout << "---------------" << endl;
    cout << g.EncodeNumbers(14, 5) << endl;
    cout << g.DecodeNumbers("001111", 5) << endl;
    cout << "---------------" << endl;
    cout << g.EncodeNumbers(13, 5) << endl;
    cout << g.DecodeNumbers("001110", 5) << endl;
    cout << "---------------" << endl;
    cout << g.EncodeNumbers(12, 5) << endl;
    cout << g.DecodeNumbers("00110", 5) << endl;
    cout << "---------------" << endl;
    cout << g.EncodeNumbers(11, 5) << endl;
    cout << g.DecodeNumbers("00101", 5) << endl;
    cout << "---------------" << endl;
    cout << g.EncodeNumbers(10, 5) << endl;
    cout << g.DecodeNumbers("00100", 5) << endl;
    cout << "---------------" << endl;
    cout << g.EncodeNumbers(9, 5) << endl;
    cout << g.DecodeNumbers("01111", 5) << endl;
    cout << "---------------" << endl;
    cout << g.EncodeNumbers(8, 5) << endl;
    cout << g.DecodeNumbers("01110", 5) << endl;
    cout << "---------------" << endl;
    cout << g.EncodeNumbers(7, 5) << endl;
    cout << g.DecodeNumbers("0110", 5) << endl;
    cout << "---------------" << endl;
    cout << g.EncodeNumbers(6, 5) << endl;
    cout << g.DecodeNumbers("0101", 5) << endl;
    cout << "---------------" << endl;
    cout << g.EncodeNumbers(5, 5) << endl;
    cout << g.DecodeNumbers("0100", 5) << endl;
    cout << "---------------" << endl;
    cout << g.EncodeNumbers(4, 5) << endl;
    cout << g.DecodeNumbers("1111", 5) << endl;
    cout << "---------------" << endl;
    cout << g.EncodeNumbers(3, 5) << endl;
    cout << g.DecodeNumbers("1110", 5) << endl;
    cout << "---------------" << endl;
    cout << g.EncodeNumbers(2, 5) << endl;
    cout << g.DecodeNumbers("110", 5) << endl;
    cout << "---------------" << endl;
    cout << g.EncodeNumbers(1, 5) << endl;
    cout << g.DecodeNumbers("101", 5) << endl;
    cout << "---------------" << endl;


    // cout << "---------------" << endl;
    // cout << g.EncodeNumbers(10, 6) << endl;
    // cout << "---------------" << endl;
    // cout << g.DecodeNumbers("011110000001", 676) << endl;
    // cout << "---------------" << endl;
    // g.DecodeNumbers("00111", 4);
    // cout << "---------------" << endl;
    // g.DecodeNumbers("01110", 5);
    // cout << "---------------" << endl;
    // g.DecodeNumbers("00100", 5);
    // cout << "---------------" << endl;

    return 0;
}