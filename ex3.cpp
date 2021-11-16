#include <iostream>
#include <typeinfo>
#include <string>
#include <cstring>
#include <stdio.h>

#include <sndfile.h>

using namespace std;

int main(int argc, char *argv[]) {

    if (argc != 3) cout << "Wrong arguments" << endl;
    SNDFILE *fin, *fout;
    SF_INFO inf;
    int readcount;

    // Open input file
    inf.format = 0;
    fin = sf_open(argv[1], SFM_READ, &inf);
    if (fin == NULL) {
        sf_close (fin) ;
		printf ("Could not open input file\n") ;
		return -1;
    }

    // Define file format and check
    inf.format = SF_FORMAT_WAV | SF_FORMAT_PCM_32;
    if (!sf_format_check (&inf)) {	
        sf_close (fin) ;
		printf ("Invalid encoding\n") ;
		return -1;
	};
    
    // Open output file
    fout = sf_open(argv[2], SFM_WRITE, &inf);
    if (fout == NULL) {
        sf_close (fout) ;
		printf ("Could not open output file\n") ;
		return -1;
    }

    // Read input file frame by frame and write to output 
    float buffer[inf.frames*inf.channels];
    while ((readcount = (int) sf_readf_float(fin, buffer, 1)) > 0)
		sf_writef_float (fout, buffer, readcount) ;

    sf_close(fin);
    sf_close(fout);

    return 0;
}