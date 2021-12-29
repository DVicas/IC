/*! \file b_ex1.cpp
    \brief This file refers to the project's second challenge. It demonstrates the developed audio codec that relies on
        predictive coding.
*/
#include <fstream>
#include <iostream>
#include <sndfile.h>
#include <map>
#include <math.h>
#include "BitStream.hh"
#include "Golomb.hh"
#include <vector>
#include <string>
#include <chrono>

using namespace std;

vector<int> code_info;  /**< vector with the length of each written code; used for decoding purposes */
short* mono_buffer; /**< mono channel buffer for generic purposes */
int num_items; /**< static number of items of the mono channel ( half of the wav fil samples ) */
SF_INFO sfinfo; /**< audio file data structure */

int predictor(char* audio_file);
short folding(short residual);
short defolding(short n);
int calculate_m(int sum);
void lossless_encode(short* error_buffer, int m);
void lossless_decode(int m, char* audio_file);
void lossy_encode(short* error_buffer, int m, int optimal_bits);
void lossy_decode(int m, char* audio_file, int optimal_bits);
void histograms(short* buffer, short* error_buffer);

int main(int argc, char* argv[]){

    if(argc != 3){
        cout << "WRONG NUMBER OF ARGUMENTS!" << endl;
        return 1;
    }
    //encoding
    int m = predictor(argv[1]);

    //decoding
    lossless_decode(m, argv[2]);

    return 1;
}


int predictor(char* audio_file){
    /**
    * reads audio file, creates single channel buffer (avg of stereo), calculates
    * the residual values (uses folding to get only positive values) based on predictive coding and calculates optimal m; it then encodes the value
    * with its respective Golomb code and writes to file using BitStream class.
    * It also compares the original buffer with the residual buffer (with calculated entropies); The residual
    * buffer will always have less entropy.
    */
    SNDFILE* file;
    sfinfo.format=0;

    file=sf_open(audio_file,SFM_READ,&sfinfo);


    int all_items = (int)sfinfo.frames * sfinfo.channels;
    short* buffer = (short *)malloc(all_items * sizeof(short));
    mono_buffer = (short *)malloc(sfinfo.frames * sizeof(short));
    short* error_buffer = (short *)malloc(sfinfo.frames * sizeof(short));
    int sum = 0;
    int rd_data = sf_read_short(file, buffer, all_items);
    num_items = sfinfo.frames;

    for(int i = 0; i < rd_data; i += 2){
        mono_buffer[i / 2] = (buffer[i] + buffer[i + 1]) / 2; 
    }

    for(int i = 0; i < num_items; i++){
        short f_;
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
        //folding
        error_buffer[i] = folding(error_buffer[i]);

        sum += error_buffer[i]; 
    }

    //calculating histograms and entropy
    histograms(mono_buffer, error_buffer);
    //calculating optimal m
    int m = calculate_m(sum);
    //encoding
    lossless_encode(error_buffer, m);
        cout << "COMPRESSING COMPLETE" << endl;
    return m;
}

short folding(short residual){
    /**
    * Transforms an abritrary number into a positive number. If the given residual 
    * is negative it will turn into a positive odd number, else it will fold the number into his respective double number
    */ 
    if (residual >= 0){
        residual = residual * 2;   
    }else{

        residual = residual * (-2) - 1;
}
    return residual;
}

short defolding(short n){
    /**
    * Defolding() will do the inverse of the folding() function.
    */ 
    if(n % 2 == 0){
        n /= 2;
    }else{
        n = (n + 1) / (-2);
    }
    return n;
}

int calculate_m(int sum){
    /**
    * This function will calculate the 'optimal' m with the formula -> ceil(-1/log2(mean/(mean+1)))
    */ 
    double mean = (double) sum / num_items; 
    int m = ceil(-1/log2(mean/(mean+1)));
    cout << "-----------MEAN----------" << endl;
    cout << "mean -> " << mean << endl;
    cout << "--------OPTIMAL M--------" << endl;  
    cout << "optimal m -> " << m << endl;

    return m;
}

void lossless_encode(short* error_buffer, int m){
    //for each value in the error buffer, calculate the golomb code and write to file via bitstream
    /**
    * lossless_encode() will encode each value in the error (or residual) buffer. It calculates its golomb code
    * and writes it into a binary file via BitStream class. This function uses a truncation mechanism
    * that allows the bits to be written only if they are multiples of 8 (bytes) or have a code length
    * bigger than 8 (byte). If there are remainder bits we will just add 0's to the end of the file.
    */ 
    Golomb g;
    BitStream bstream("", "golomb_output.bin");
    string code;
    string byte;
    auto start = chrono::steady_clock::now();
    for(int i = 0; i < num_items; i++){
        code = g.EncodeNumbers(error_buffer[i], m);
        byte.append(code); 
        code_info.push_back(code.length()); //add length of code to vector
        while(byte.length() >= 8){
            if(byte.length() % 8 == 0){
                bstream.writeBits(byte.substr(0, byte.length()));
                byte = "";
            }else if(byte.length() > 8 ){
                bstream.writeBits(byte.substr(0, 8));
                byte.erase(0,8);
                }
            }
    }
    while(byte.length() != 8){
        byte += "0";
    }
    bstream.writeBits(byte);
    bstream.close();
    auto end = chrono::steady_clock::now();
    cout << "Elapsed time in milliseconds: "
        << chrono::duration_cast<chrono::milliseconds>(end - start).count()
        << " ms" << endl;
}
void lossless_decode(int m, char* audio_file){
    /**
    * lossless_decode() will read an encoded file and decodes each code with the help of 
    * the code_info vector composed by the length of each code by order.
    * It reads each code with length x using the BitStream readBits() method, and "defolds" the integer
    * to get the 'real' value. Afterwards it reverses the residual encoding formula to decode the residual.
    * Finally, it will write the decoded data into an audio file with name given as arguments. 
    */ 
    auto start = chrono::steady_clock::now();
    SNDFILE* file;
    sfinfo.channels = 1;

    file=sf_open(audio_file,SFM_WRITE,&sfinfo);

    short* buffer = (short *)malloc(num_items * sizeof(short)); 
    BitStream bstream("golomb_output.bin", "");
    Golomb g;
    for(int i = 0; i < num_items; i++){
        string code = bstream.readBits(code_info.at(i)); //reads code
        short n = g.DecodeNumbers(code, m); //decodes the code with length i
        n = defolding(n); //defolds residual value
        short f_;
        if(i > 2){
            f_ = (3 * buffer[i - 1]) - (3 * buffer[i - 2]) + buffer[i - 3];
        }else if(i == 2){
            f_ = (2 * buffer[i - 1]) - buffer[i - 2];
        }else if(i == 1){
            f_ = buffer[i - 1]; 
        }else{
            f_ = 0; 
        }
        buffer[i] = n + f_;
    }
    cout << "DECOMPRESSING COMPLETE" << endl;
    int rd_data = sf_write_short(file, buffer, num_items); 
    sf_close(file);
    bstream.close();
    auto end = chrono::steady_clock::now();
    cout << "Elapsed time in milliseconds: "
        << chrono::duration_cast<chrono::milliseconds>(end - start).count()
        << " ms" << endl;
}
void lossy_encode(short* error_buffer, int m, int entropy){
    // void lossyCoding(int frames, int nbits){
    //     // TODO calcular nbits otimo

    //     int ptr[frames];
    //     for(int i=0 ; i<frames ; i++){
    //         ptr[i]=(bufferMono[i] >>  nbits) << nbits;
    //     }
    // }
    
}
void lossy_decode(int m, char* audio_file, int entropy){

}

void histograms(short* buffer, short* error_buffer){
    /**
    * calculating histograms and respective entropies, being the error_buffer < buffer
    */ 
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