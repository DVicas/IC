#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <cmath>

using namespace std;

int main(int argc, char* argv[]){

  if (argc != 3){
    cout << "Error: Incorrect usage - first argument - Image to copy, second argument - new image." << endl;
    return -1;
  }

  FILE * fIn;
  fIn = fopen(argv[1], "rb");

  int width;
  int height;
  int RGB;
  int j=0;
  ofstream newimage;
  
  char type[3];

  fscanf(fIn, "%s", type);
  fscanf(fIn, "%d\n %d\n", &width, &height);
  fscanf(fIn, "%d\n", &RGB);

  int size = 3*width*height;
  unsigned char* pixels = new unsigned char[size];
  unsigned char red[size/3], green[size/3], blue[size/3];

  while (fread(pixels, sizeof(unsigned char), size, fIn));

  for (int i = 0; i < size; i=i+3 ){
    red[j] = (pixels[i] >> 1) << 1;
    green[j] = (pixels[i+1] >> 1) << 1;
    blue[j] = (pixels[i+2]  >> 1) << 1;
    j++;
  }

  fclose(fIn);

  newimage.open(argv[2]);
  newimage << type << endl;
  newimage << width << " " << height << endl;
  newimage << RGB << endl;

  for (int i=0;i < size/3;i++){
     newimage << (char) red[i] << (char) green[i]  << (char) blue[i];
  }

  return 0;

}