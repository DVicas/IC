#include <cstring>
#include <iostream>
#include <fstream>
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include <cmath>

using namespace std;

typedef struct{
    int r[256] = {0};
    int g[256] = {0};
    int b[256] = {0};
    int gr[256] = {0};
}Histogram;

typedef struct {
    unsigned char red,green,blue;
} PPMPixel;

typedef struct {
    int x, y;
    PPMPixel *data;
} PPMImage;

#define RGB_COMPONENT_COLOR 255

static PPMImage *readPPM(const char *filename)
{
    char buff[16];
    PPMImage *img;
    FILE *fp;
    int c, rgb_comp_color;
    //open PPM file for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "Unable to open file '%s'\n", filename);
        exit(1);
    }

    //read image format
    if (!fgets(buff, sizeof(buff), fp)) {
        perror(filename);
        exit(1);
    }

    //check the image format
    if (buff[0] != 'P' || buff[1] != '6') {
        fprintf(stderr, "Invalid image format (must be 'P6')\n");
        exit(1);
    }

    //alloc memory form image
    img = (PPMImage *)malloc(sizeof(PPMImage));
    if (!img) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }

    //check for comments
    c = getc(fp);
    while (c == '#') {
        while (getc(fp) != '\n') ;
        c = getc(fp);
    }

    ungetc(c, fp);
    //read image size information
    if (fscanf(fp, "%d %d", &img->x, &img->y) != 2) {
        fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
        exit(1);
    }

    //read rgb component
    if (fscanf(fp, "%d", &rgb_comp_color) != 1) {
        fprintf(stderr, "Invalid rgb component (error loading '%s')\n", filename);
        exit(1);
    }

    //check rgb component depth
    if (rgb_comp_color!= RGB_COMPONENT_COLOR) {
        fprintf(stderr, "'%s' does not have 8-bits components\n", filename);
        exit(1);
    }

    while (fgetc(fp) != '\n') ;
    //memory allocation for pixel data
    img->data = (PPMPixel*)malloc(img->x * img->y * sizeof(PPMPixel));

    if (!img) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }

    //read pixel data from file
    if (fread(img->data, 3 * img->x, img->y, fp) != img->y) {
        fprintf(stderr, "Error loading image '%s'\n", filename);
        exit(1);
    }

    fclose(fp);
    return img;
}
void writePPM(const char *filename, PPMImage *img)
{
    FILE *fp;
    //open file for output
    std::string t = filename;
    std::string tmp = t + ".ppm";
    int n = tmp.length();
    char char_array[n + 1];

    strcpy(char_array, tmp.c_str());
    fp = fopen(char_array, "wb");
    if (!fp) {
        fprintf(stderr, "Unable to open file '%s'\n", filename);
        exit(1);
    }

    //write the header file
    //image format
    fprintf(fp, "P6\n");

    //image size
    fprintf(fp, "%d %d\n",img->x,img->y);

    // rgb component depth
    fprintf(fp, "%d\n",RGB_COMPONENT_COLOR);

    // pixel data
    fwrite(img->data, 3 * img->x, img->y, fp);
    fclose(fp);
}

void changeColorPPMToGreyScale(PPMImage *img)
{
    int i;
    double temp;
    if(img){

        for(i=0;i<img->x*img->y;i++){
            temp = (img->data[i].red *0.3) + (img->data[i].green *0.59) +(img->data[i].blue *0.11);
            img->data[i].red=temp;
            img->data[i].green=temp;
            img->data[i].blue=temp;
        }
    }
}

Histogram histogramImage(const char *filename, PPMImage *img, Histogram hist){
//    int r[256] = {0};
//    int g[256] = {0};
//    int b[256] = {0};
//    int gr[256] = {0};

    //hist = (Histogram *)malloc(sizeof(Histogram));
    int i;
    if(img){
        for(i=0;i<img->x*img->y;i++){
//            r[img->data[i].red]++;
//            g[img->data[i].green]++;
//            b[img->data[i].blue]++;
            hist.r[img->data[i].red]++;
            hist.g[img->data[i].green]++;
            hist.b[img->data[i].blue]++;
        }
    }

    changeColorPPMToGreyScale(img);

    for(i=0;i<img->x*img->y;i++){
//        gr[img->data[i].red]++;
        hist.gr[img->data[i].red]++;

    }

    ofstream MyFile;
    MyFile.open(filename);


    for( int i = 0; i < 256; i++ )  // Yes, this loop range is excessive...
    {
        std::string temp_r = std::to_string(hist.r[i]);
        char const *num_char_red = temp_r.c_str();
        std::string temp_g = std::to_string(hist.g[i]);
        char const *num_char_green = temp_g.c_str();
        std::string temp_b = std::to_string(hist.b[i]);
        char const *num_char_blue = temp_b.c_str();
        std::string temp_gr = std::to_string(hist.gr[i]);
        char const *num_char_grey = temp_gr.c_str();
        MyFile << ""+string(num_char_red)+"\t"+string(num_char_green)+"\t"+string(num_char_blue)+"\t"+string(num_char_grey) <<endl;


    }
    MyFile.close();
    return hist;


}

double entropy_calc(Histogram hist,PPMImage *img){
    double p = 0;
    double entropy=0;
    int size = img->x*img->y;
    for( int i = 0; i < 256; i++ )  // Yes, this loop range is excessive...
    {
        //cout << hist.gr[i] << endl;
        p = (double)hist.gr[i]/size;
        if (p != 0)
            entropy += p*log(p);

    }

    return entropy*-1;
}

int main(int argc, char** argv){

    if(argc==1){
        printf("\nNo Extra Command Line Argument Passed Other Than Program Name");
        exit(1);
    }
    if(argc<2){
        printf("\nMissing arguments");
        exit(1);
    }
    PPMImage *image;
    Histogram hist;
    double entropy;
    image = readPPM(argv[1]);

    hist = histogramImage(argv[2],image,hist);

    entropy = entropy_calc(hist,image);
    cout<<"entropy= "+to_string(entropy)<<endl;
    writePPM(argv[2],image);

}