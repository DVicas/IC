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
    short readcount;

    // Open input file
    inf.format = 0;
    fin = sf_open(argv[1], SFM_READ, &inf);
    if (fin == NULL) {
        sf_close (fin) ;
		cout << "Could not open input file" << endl;
		return -1;
    }

    // Check file format
    if (!sf_format_check (&inf)) {	
        sf_close (fin) ;
		cout << "Invalid encoding" << endl;
		return -1;
	};
    
    // Open output file
    fout = sf_open(argv[2], SFM_WRITE, &inf);
    if (fout == NULL) {
        sf_close (fout) ;
		cout << "Could not open output file" << endl;
		return -1;
    }

    // Read input file frame by frame and write to output 
    // buffer_size = frames_to_read * channels, we only want 1 frame at a time so buffer_size = 1 * channels
    short buffer[inf.channels];                             
    while ((readcount = (short) sf_readf_short(fin, buffer, 1)) > 0)
		sf_writef_short (fout, buffer, readcount) ;

    sf_close(fin);
    sf_close(fout);

    return 0;
}