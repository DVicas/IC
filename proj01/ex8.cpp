#include <iostream>
#include <string>
#include <cstring>
#include <stdio.h>

#include <sndfile.h>
#include <sndfile.hh>

using namespace std;

int main(int argc, char *argv[]) {

    if (argc != 4) cout << "Wrong arguments: InputFile OutputFile N_Bits_to_Shift" << endl;
    
    int n_shift = argv[4];
    if (n_shift > 15 && n_shift < 1) {
        cout << "Bits to shift must be between 0 and 15" << endl;
        return -1;
    }

    SF_INFO inf;
    inf.format = 0;
    SNDFILE* fin = sf_open(argv[1], SFM_READ, &inf);
    if (sf_error(fin) != 0) {
        sf_close (fin);
		cout "Could not open input file" << endl;
		return -1;
    }

    SNDFILE* fout = sf_open(argv[2], SFM_WRITE, &inf);
    if (sf_error(fout) != 0) {
        sf_close (fout) ;
		cout << "Could not open output file" << endl;
		return -1;
    }

    short readCount;
    short ptr[inf.channels];
    while((readCount = (short) sf_readf_short (fin, ptr, 1)) > 0){
        ptr[0] = (ptr[0] >> n_shift) << n_shift; 
        ptr[1] = (ptr[1] >> n_shift) << n_shift;

        sf_writef_short (fout, ptr, readCount);
    }

    return 0;
}