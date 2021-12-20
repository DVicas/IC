#include <fstream>
#include <iostream>
#include <sndfile.h>
#include <map>
#include <math.h>
#include "BitStream.hh"
#include "Golomb.hh"

using namespace std;

void predictor(char* audio_file);
void histograms(int* buffer, int* error_buffer, int num_items);

int main(int argc, char* argv[]){

    if(argc!=2){
        cout<<"WRONG NUMBER OF ARGUMENTS!"<<endl;
        return 1;
    }
    predictor(argv[1]);        
    return 1;
}

void predictor(char* audio_file){
    
    SNDFILE* file;
    SF_INFO sfinfo;
    sfinfo.format=0;

    file=sf_open(audio_file,SFM_READ,&sfinfo);

    int num_items = (int)sfinfo.frames * sfinfo.channels;
    int* buffer = (int *)malloc(num_items * sizeof(int));
    int* mono_buffer = (int *)malloc(sfinfo.frames * sizeof(int));
    int* error_buffer = (int *)malloc(sfinfo.frames * sizeof(int));
    int sum = 0;

    int rd_data = sf_read_int(file, buffer, num_items);

    for(int i = 0; i < rd_data; i += 2){
        mono_buffer[i / 2] = (buffer[i] + buffer[i + 1]) / 2; 
    }

    for(int i = 0; i < sfinfo.frames; i++){
        int f_;
        if(i > 2){
            f_ = (3 * mono_buffer[i - 1]) - (3 * mono_buffer[i - 2]) + mono_buffer[i - 3];
        }else if(i == 2){
            f_ = (2 * mono_buffer[i - 1]) - mono_buffer[i - 2];
        }else if(i == 1){
            f_ = mono_buffer[i - 1]; 
        }else{
            f_ = 0;
        }  
        error_buffer[i] = mono_buffer[i] - f_;
        sum += error_buffer[i];
        //cout << "Residual -> " << error_buffer[i] << " Fn -> " << buffer[i] << endl;
    }

    //calculating histograms and entropy
    histograms(mono_buffer, error_buffer, sfinfo.frames);

    //calculating the mean to find the "optimal" m -> log2(log(2)*(mean/sqrt(2)))
    double mean = (double) sum / sfinfo.frames;
    int m = log2(log(2) * (mean / sqrt(2)));
    cout << "optimal m -> " << m << endl;

    //for each value in the error buffer, calculate the golomb code and write to file via bitstream
    Golomb g;
    BitStream bstream(" ", "golomb_output.txt");

    for(int i = 0; i < sfinfo.frames; i++){
        // code_vector = g.EncodeNumbers(error_buffer[i], m);
        //bstream.writeBits(code_vector)
    }

}

void histograms(int* buffer, int* error_buffer, int num_items){
    //HISTOGRAMS
    map<int,int> error_buff_histo;
    map<int,int> buff_histo;
    map<int,int>::iterator it;
    double entropy = 0;
    double p = 0;

    for(int i = 0; i < num_items; i++){
        error_buff_histo[error_buffer[i]]++;
        buff_histo[buffer[i]]++;
    }

    cout<<"--------BUFFER--------"<<endl;
    for(it = buff_histo.begin(); it != buff_histo.end(); it++){
        //cout << it->first << " " << it->second << endl;
        p = (double)it->second / (num_items);
        entropy += p*log(p);
    }
    cout << "ENTROPY -> " << entropy*-1 << endl;
    
    entropy = 0;
    p = 0;
    
    cout<<"--------error BUFFER--------"<<endl;
    for(it = error_buff_histo.begin(); it != error_buff_histo.end(); it++){
        //cout << it->first << " " << it->second << endl;
        p = (double)it->second / (num_items);
        entropy += p*log(p);
    }
    cout << "ENTROPY -> " << entropy*-1 << endl;
}