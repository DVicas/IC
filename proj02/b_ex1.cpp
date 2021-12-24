#include <fstream>
#include <iostream>
#include <sndfile.h>
#include <map>
#include <math.h>
#include "BitStream.hh"
#include "Golomb.hh"
#include <vector>
#include <string>

using namespace std;

void predictor(char* audio_file);
int calculate_m(int sum, int num_items);
void encode(int num_items, short* error_buffer, int m);
void histograms(short* buffer, short* error_buffer, int num_items);

int main(int argc, char* argv[]){

    if(argc != 2){
        cout << "WRONG NUMBER OF ARGUMENTS!" << endl;
        return 1;
    }
    predictor(argv[1]);        
    return 1;
}
//reads audio file, creates single channel (avg of stereo), predicts and calculates
//the residual values and calculates optimal m; it then encodes the value
//with its respective golomb code and writes to file using made classes
void predictor(char* audio_file){
    
    SNDFILE* file;
    SF_INFO sfinfo;
    sfinfo.format=0;

    file=sf_open(audio_file,SFM_READ,&sfinfo);

    int num_items = (int)sfinfo.frames * sfinfo.channels;
    short* buffer = (short *)malloc(num_items * sizeof(short));
    short* mono_buffer = (short *)malloc(sfinfo.frames * sizeof(short));
    short* error_buffer = (short *)malloc(sfinfo.frames * sizeof(short));
    int sum = 0;
    int rd_data = sf_read_short(file, buffer, num_items);

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
        sum += abs(error_buffer[i]); //mal
        //cout << "Residual -> " << error_buffer[i] << " Fn -> " << buffer[i] << endl;
    }

    //calculating histograms and entropy
    histograms(mono_buffer, error_buffer, sfinfo.frames);
    //calculating optimal m
    int m = calculate_m(sum, sfinfo.frames);
    //encoding
    encode(sfinfo.frames, error_buffer, m);

}

int calculate_m(int sum, int num_items){
    //calculating the mean to find the "optimal" m -> log2(log(2) * (mean / sqrt(2)))
    double mean = (double) sum / num_items; //what if the mean is negative?
    int m = log2(log(2) * (mean / sqrt(2)));
    cout << "-----------MEAN----------" << endl;
    cout << mean << endl;
    cout << "--------OPTIMAL M--------" << endl;  
    cout << "optimal m -> " << m << endl;

    return m;
}

void encode(int num_items, short* error_buffer, int m){
    //for each value in the error buffer, calculate the golomb code and write to file via bitstream
    Golomb g;
    BitStream bstream(" ", "golomb_output.wav");
    string code;

    for(int i = 0; i < num_items; i++){
        code = g.EncodeNumbers(error_buffer[i], m);
        code.erase(remove(code.begin(), code.end(), ' '), code.end());
        bstream.writeBits(code);
    }
    bstream.close();
}

void histograms(short* buffer, short* error_buffer, int num_items){
    //HISTOGRAMS
    map<short,int> error_buff_histo;
    map<short,int> buff_histo;
    map<short,int>::iterator it;
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
        entropy += p * log(p);
    }
    cout << "ENTROPY -> " << entropy * -1 << endl;
    
    entropy = 0;
    p = 0;
    
    cout<<"--------error BUFFER--------"<<endl;
    for(it = error_buff_histo.begin(); it != error_buff_histo.end(); it++){
        //cout << it->first << " " << it->second << endl;
        p = (double)it->second / (num_items);
        entropy += p * log(p);
    }
    cout << "ENTROPY -> " << entropy * -1 << endl;
}