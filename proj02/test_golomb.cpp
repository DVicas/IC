#include "Golomb.hh"

using namespace std;

int main() {

    Golomb g;
    cout << g.EncodeNumbers(14340, 8);
    g.DecodeNumbers("000100", 5);
    cout << "---------------" << endl;
    g.DecodeNumbers("00111", 4);
    cout << "---------------" << endl;
    g.DecodeNumbers("01110", 5);
    cout << "---------------" << endl;
    g.DecodeNumbers("00100", 5);
    cout << "---------------" << endl;

    return 0;
}