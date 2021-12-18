#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <cmath>

using namespace std;

int main(int argc, char* argv[]){

    if (argc != 2){
        cout << "Error: Incorrect usage - first argument - Image to copy, second argument - new image." << endl;
        return -1;
    }

    FILE * fIn;
    fIn = fopen(argv[1], "rb");

    int width;
    int height;
    int RGB;
    int j=0;
    char type[3];
    
    fscanf(fIn, "%s", type);
    fscanf(fIn, "%d\n %d\n", &width, &height);
    fscanf(fIn, "%d\n", &RGB);

    int size = 3*width*height;
    unsigned char* pixels = new unsigned char[size];
    int* mean = new int[size/3];

    while (fread(pixels, sizeof(unsigned char), size, fIn));
    
    
    return 0;

}