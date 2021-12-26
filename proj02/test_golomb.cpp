#include "Golomb.hh"

using namespace std;

int main() {

    Golomb g;
    int i;
    cout << g.EncodeNumbers(503, 13) << endl;
    i = g.DecodeNumbers("000100", 5);
    cout << "---------------" << endl;
    cout << i << endl;
    i = g.DecodeNumbers("0000001111", 5);
    cout << "---------------" << endl;
    cout << i << endl;
    i = g.DecodeNumbers("0000000000000000000000000000000000000000000000000000000000000000000000000000011000", 13);
    cout << "---------------" << endl;
    cout << i << endl;
    i = g.DecodeNumbers("00100", 5);
    cout << "---------------" << endl;
    cout << i << endl;

    return 0;
}