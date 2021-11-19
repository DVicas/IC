#include <iostream>
#include <string>
#include <stdio.h>

#include <sndfile.h>
#include <sndfile.hh>

using namespace std;
//96,32
int main(int argc, char *argv[]) {

    SndfileHandle f1, f2;

    f1 = SndfileHandle("/home/diogo/Documents/IC/proj01/sample02.wav");
    f2 = SndfileHandle("/home/diogo/Documents/IC/proj01/sample_copy.wav");

    short readCount;
    short buffer[f1.channels()]; 
    
    f1.readf(buffer, 1);
    cout << hex << buffer[0] << endl;
    cout << hex <<buffer[1] << endl;
    f2.readf(buffer, 1);
    cout << buffer[0] << endl;
    cout << buffer[1] << endl;

    while((readCount = f1.readf(buffer, 1)) > 0) {
        
    }

    return 0;
}