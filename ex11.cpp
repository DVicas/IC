#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <cmath>

using namespace std;
int size;
int * readPPM(const char* file){

    FILE * fIn;
    fIn = fopen(file, "rb");

    int width;
    int height;
    int RGB;
    int j=0;
    char type[3];
    
    fscanf(fIn, "%s", type);
    fscanf(fIn, "%d\n %d\n", &width, &height);
    fscanf(fIn, "%d\n", &RGB);

    size = 3*width*height;
    unsigned char* pixels = new unsigned char[size];
    int* mean = new int[size/3];

    while (fread(pixels, sizeof(unsigned char), size, fIn));

    for (int i = 0; i < size;i=i+3 ){
        mean[j] = (pixels[i]+pixels[i+1]+pixels[i+2])/3;
        j++;
    }

    fclose(fIn);
    
    return mean;
}

int main(int argc, char* argv[]){

    if (argc != 3){
        cout << "Error: Incorrect usage - first argument - Image to copy, second argument - new image." << endl;
        return -1;
    }

    int* mean_ori   = readPPM(argv[1]);
    int* mean_noise = readPPM(argv[2]);
    
    float mean_error = 0;
    float max_error = 0;
    int max_val = 255;

    for (int i=0; i < size/3; i++){
        mean_error = mean_error + pow((mean_ori[i]-mean_noise[i]),2);
        if (pow((mean_ori[i]-mean_noise[i]),2) > max_error) max_error = pow((mean_ori[i]-mean_noise[i]),2);
    }
    mean_error = mean_error/(size/3);  // MSE 1/N * SUM((Ori-Noise)*(Ori-Noise))
    float snr = 10*log10(max_val*max_val/mean_error);  //  PSNR = 10log(A*A/e*e)

    cout << "PSNR: " << snr << endl;
    cout << "Maximum per pixel absolute error: " << max_error << endl;

    return 0;

}