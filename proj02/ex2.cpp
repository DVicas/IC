#include "BitStream.h"

int main() {

    BitStream bs("Teste bits", "w");

    //010100111
    bs.WriteBit(0);
    bs.WriteBit(1);
    bs.WriteBit(0);
    bs.WriteBit(1);
    bs.WriteBit(0);
    bs.WriteBit(0);
    bs.WriteBit(1);
    bs.WriteBit(1);
    bs.WriteBit(1);

    bs.Close();
}