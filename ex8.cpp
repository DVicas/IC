#include <iostream>
#include <string>
#include <cstring>
#include <stdio.h>

#include <sndfile.h>
#include <sndfile.hh>

using namespace std;

int main(int argc, char *argv[]) {

    if (argc != 3) cout << "Wrong arguments" << endl;

    SF_INFO inf;
    inf.format = 0;
    SNDFILE* fin = sf_open(argv[1], SFM_READ, &inf);
    if (sf_error(fin) != 0) {
        sf_close (fin) ;
		printf ("Could not open input file\n") ;
		return -1;
    }

    SNDFILE* fout = sf_open(argv[2], SFM_WRITE, &inf);
    if (sf_error(fout) != 0) {
        sf_close (fout) ;
		printf ("Could not open output file\n") ;
		return -1;
    }

    short readCount;
    short ptr[inf.channels];
    while((readCount = (short) sf_readf_short (fin, ptr, 1)) > 0){
        ptr[0] = (ptr[0] >> 10) << 10; 
        ptr[1] = (ptr[1] >> 10) << 10;

        sf_writef_short (fout, ptr, readCount);
    }

    return 0;
}