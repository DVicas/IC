#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>

#include <sndfile.h>
#include <sndfile.hh>

using namespace std;

int main(int argc, char *argv[]) {

    SndfileHandle f1, f2;

    f1 = SndfileHandle("/home/diogo/Documents/IC/proj01/sample01.wav");
    f2 = SndfileHandle("/home/diogo/Documents/IC/IC/oi.wav");

    short readCount;
    short buffer1[f1.channels()], buffer2[f2.channels()];
    short og, cp;
    double mean1, mean2 = 0;

    while((readCount = f1.readf(buffer1, 1)) > 0) {
        f2.readf(buffer2, 1);
        og = (buffer1[0] + buffer1[1])/2;
        cp = (buffer2[0] + buffer2[1])/2;
        mean1 += pow(og, 2);
        mean2 += pow(og - cp, 2);
    }
    
    double snr = 10 * log10(mean1/ mean2);
    cout << "SNR : " << snr << endl; 
 
    return 0;
}