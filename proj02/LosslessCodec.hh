/*! \file b.cpp
    \brief This file refers to the project's third challenge. It demonstrates the developed image codecs that relie on
        predictive coding. To run the program successfuly you need one image file in .ppm, a file name for the compressed file and a file name for the decompressed file.
        Eg.
        Encode  -> ./LosslessCodecEncode.o path/to/img.ppm path/to/img.bin
        Decode  -> ./LosslessCodecDecode.o path/to/img.bin path/to/img.ppm
*/

#include <string>
#include <fstream>
#include <vector>
#include <bitset>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>

#include "Golomb.hh"
#include "BitStream.hh"

using namespace std;
using namespace cv;


class LosslessCodec {
    public:
        void encode(string path_to_img, string path_to_bin);
        void decode(string path_to_bin, string path_to_img);
    private:
        void toYUV(Mat img, Mat* yuv_channels);
        int calculate_m(Mat mat);
        double calculate_entropy(Mat mat);
        string toByte(string bits);
        string removePadding(string bits);
        Mat predictorEnc(Mat img);
        Mat predictorDec(Mat err);
};

void LosslessCodec::toYUV(Mat img, Mat* yuv_channels) {

    // convert to YUV colorspace
    cvtColor(img, img, COLOR_RGB2YUV);
    
    // split YUV channels into 3 arrays
    split(img, yuv_channels);

    
    Mat tmp_u (img.size().height/2, img.size().width/2, CV_8UC1);
    Mat tmp_v (img.size().height/2, img.size().width/2, CV_8UC1);

    int u_i = 0, v_i = 0;
    int u_j = 0, v_j = 0;

    // Convert to YUV4:2:0
    for (int i = 0; i < img.size().height; i+=2) {
        for (int j = 0; j < img.size().width; j+=2) {
            tmp_u.at<uchar>(u_i, u_j++) = yuv_channels[1].at<uchar>(i,j);
            tmp_v.at<uchar>(v_i, v_j++) = yuv_channels[2].at<uchar>(i,j);
        }
        u_i++; v_i++;
        u_j = v_j = 0;
    }

    yuv_channels[1] = tmp_u;
    yuv_channels[2] = tmp_v;
}

Mat LosslessCodec::predictorEnc(Mat img) {

    Mat error (img.size().height, img.size().width, CV_8UC1);
    int a, b, c, x;
    for (int i = 0; i < img.size().height; i++) {
        for (int j = 0; j < img.size().width; j++) {
            // canto superior esquerdo
            if (i == 0 && j == 0) {
                a = 0;
                b = 0;
                c = 0;
            } 
            else if (i == 0 && j != 0) { // linha cima
                a = (int) img.at<uchar>(i,j-1);
                b = 0;
                c = 0;
            }
            else if (i != 0 && j == 0) { // coluna esquerda
                a = 0;
                b = (int) img.at<uchar>(i-1,j);
                c = 0;
            }
            else {
                a = (int) img.at<uchar>(i,j-1);
                b = (int) img.at<uchar>(i-1,j);
                c = (int) img.at<uchar>(i-1,j-1);
            }

            // Prediction
            if (c >= max(a,b)) {
                x = min(a,b);
            }
            else if (c <= min(a,b)) {
                x = max(a,b);
            }
            else {
                x = a + b - c;
            }
            error.at<uchar>(i,j) = (uchar) ( ((int) img.at<uchar>(i,j)) - x);
            // cout << (int) error.at<uchar>(i,j) << endl;
        }
    }
    return error;
}

Mat LosslessCodec::predictorDec(Mat err) {

    Mat img (err.size().height, err.size().width, CV_8UC1);
    int a, b, c, x;
    for (int i = 0; i < img.size().height; i++) {
        for (int j = 0; j < img.size().width; j++) {
            // canto superior esquerdo
            if (i == 0 && j == 0) {
                a = 0;
                b = 0;
                c = 0;
            } 
            else if (i == 0 && j != 0) { // linha cima
                a = (int) img.at<uchar>(i,j-1);
                b = 0;
                c = 0;
            }
            else if (i != 0 && j == 0) { // coluna esquerda
                a = 0;
                b = (int) img.at<uchar>(i-1,j);
                c = 0;
            }
            else {
                a = (int) img.at<uchar>(i,j-1);
                b = (int) img.at<uchar>(i-1,j);
                c = (int) img.at<uchar>(i-1,j-1);
            }

            // Prediction
            if (c >= max(a,b)) {
                x = min(a,b);
            }
            else if (c <= min(a,b)) {
                x = max(a,b);
            }
            else {
                x = a + b - c;
            }
            img.at<uchar>(i,j) = (uchar) ( ((int) err.at<uchar>(i,j)) + x);
        }
    }
    return img;
}

void LosslessCodec::encode(string path_to_img, string path_to_bin) {
    Mat img = imread(path_to_img);
    if (img.empty()) {
        cout << "Error with image" << endl;
    }

    Mat channels[3];
    toYUV(img, channels);

    //one for each channel (3 channels) // Y = [0] / U = [1] / V = [2]
    Mat error[3];
    double entropy = 0;
    for (int i = 0; i < 3; i++) {   //fill error matrices and calc entropy 
        error[i] = predictorEnc(channels[i]);
        entropy += calculate_entropy(channels[i]);
    }
    entropy = entropy / 3; //3 channels
    cout << "Avg. Entropy: " << entropy << endl << endl;


    BitStream bs = BitStream("", path_to_bin);
    Golomb g;
    int m[3], nbytes[3] = {0};
    int val;
    string bits, golomb;
    for (int k = 0; k < 3; k++) {
        m[k] = calculate_m(error[k]);

        for (int i = 0; i < error[k].size().height; i++) {
            for (int j = 0; j < error[k].size().width; j++){   
                val = error[k].at<uchar>(i,j);
                golomb = g.EncodeNumbers(val, m[k]);
                bits = toByte(golomb);
                bs.writeBits(bits);

                nbytes[k] += bits.length() / 8; 
            }
        }
    }

    // BOTTOMER 
    bs.writeBits(bitset<8>(m[0]).to_string());
    bs.writeBits(bitset<8>(m[1]).to_string());
    bs.writeBits(bitset<8>(m[2]).to_string());
    bs.writeBits(bitset<16>(img.size().height).to_string());
    bs.writeBits(bitset<16>(img.size().width).to_string());
    bs.writeBits(bitset<24>(nbytes[0]).to_string());
    bs.writeBits(bitset<24>(nbytes[1]).to_string());
    bs.writeBits(bitset<24>(nbytes[2]).to_string());

}

void LosslessCodec::decode(string path_to_bin, string path_to_img) {
    
    int m[3], nbytes[3];
    int height, width;
    int i = 0, j = 0;

    BitStream bs = BitStream(path_to_bin, "");
    Golomb g;

    vector<int> img_info = bs.getBuffer();

    u_int aux = 0;
    vector<int>::iterator it = img_info.end() - 16*8;


    cout << "-----Bitstream Header-----\n" << endl;

    // Get each channel m value 
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 8; ++it, j++) {
            aux = (aux << 1) | *it;
        }
        m[i] = aux;
        aux = 0;
    }
    cout << "Y m: " << m[0] << endl;
    cout << "U m: " << m[1] << endl;
    cout << "V m: " << m[2] << endl;

    // Get img height and width
    for (j = 0; j < 16; ++it, j++) {
        aux = (aux << 1) | *it;
    }
    height = aux;
    cout << "Height: " << height << endl;

    aux = 0;
    for (j = 0; j < 16; ++it, j++) {
        aux = (aux << 1) | *it;
    }
    width = aux;
    cout << "Width: " << width << endl;

    aux = 0;
    // Get number of bytes per channel
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 24; ++it, j++) {
            aux = (aux << 1) | *it;
        }
        nbytes[i] = aux;
        aux = 0;
    }
    cout << "Y bytes: " << nbytes[i] << endl;
    cout << "U bytes: " << nbytes[i] << endl;
    cout << "V bytes: " << nbytes[i] << endl;



    // create error matrices
    Mat errorY(height, width, CV_8UC1);
    Mat errorU(height/2, width/2, CV_8UC1);
    Mat errorV(height/2, width/2, CV_8UC1);


    vector<int> vals[3];                            //store values for 3 channels


    bool unary_flag = 1;                            
    string bits = "";
    int tmp;
    for (int i = 0; i < 3; i++) {
        for (unsigned int j = 0; j < nbytes[i] ; j++) {
            bits = bits + bs.readBits(8);
            if (unary_flag) {                       // we're reading the unary part
                // while(bits.back() != '1') {
                //     bits = bits + bs.readBits(8);
                // }
                if (bits.back() == '0') {           // read the rest of the unary
                    continue;
                }
                unary_flag = 0;                     // the next byte is the binary part
            } else {
                tmp = g.DecodeNumbers(removePadding(bits), m[i]);

                // cout << "bits: " << bits << endl;
                // cout << "no pad: " << removePadding(bits) << endl;
                // cout << "golomb: " << g.DecodeNumbers(removePadding(bits), m[i]) << endl;

                vals[i].push_back(tmp);
                bits = "";
                unary_flag = 1;
            }
        }
        unary_flag = 1;
        bits = "";
    }

    //Put the values in the specific channel
    it = vals[0].begin();
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            errorY.at<uchar>(i,j) = *(it++);
        }
    }

    it = vals[1].begin();
    for (i = 0; i < height/2; i++) {
        for (j = 0; j < width/2; j++) {
            errorU.at<uchar>(i,j) = *(it++);
        }
    }

    it = vals[2].begin();
    for (i = 0; i < height/2; i++) {
        for (j = 0; j < width/2; j++) {
            errorV.at<uchar>(i,j) = *(it++);
        }
    }

    Mat img[3];
    img[0] = predictorDec(errorY);
    img[1] = predictorDec(errorU);
    img[2] = predictorDec(errorV);

    
    Mat tmp_u (height, width, CV_8UC1);
    Mat tmp_v (height, width, CV_8UC1);

    int u_i = 0, v_i = 0;
    int u_j = 0, v_j = 0;
    for (int i = 0; i < height; i+=2) {                                     // Expand YUV4:2:0 to YUV4:4:4
        for (int j = 0; j < width; j+=2) {
            tmp_u.at<uchar>(i,j) = img[1].at<uchar>(u_i,u_j+1);
            tmp_u.at<uchar>(i+1,j) = img[1].at<uchar>(u_i,u_j+1);
            tmp_u.at<uchar>(i,j+1) = img[1].at<uchar>(u_i,u_j+1);
            tmp_u.at<uchar>(i+1,j+1) = img[1].at<uchar>(u_i,u_j+1);

            tmp_v.at<uchar>(i,j) = img[2].at<uchar>(v_i,v_j+1);
            tmp_v.at<uchar>(i+1,j) = img[2].at<uchar>(v_i,v_j+1);
            tmp_v.at<uchar>(i,j+1) = img[2].at<uchar>(v_i,v_j+1);
            tmp_v.at<uchar>(i+1,j+1) = img[2].at<uchar>(v_i,v_j+1);
            u_j++;v_j++;
        }
        u_i++; v_i++;
        u_j = v_j = 0;
    }

    img[1] = tmp_u;
    img[2] = tmp_v;

    Mat yuv, rgb;
    merge(img, 3, yuv);
    cvtColor(yuv,rgb,COLOR_YUV2RGB);

    imwrite(path_to_img, rgb);
}

int LosslessCodec::calculate_m(Mat mat) {
    double mean;
    int sum = 0;

    for (int i = 0; i < mat.size().height; i++ ) {
        for (int j = 0; j < mat.size().width; j++) {
            sum += (int) mat.at<uchar>(i,j);
        }
    }
    mean = sum / (mat.size().height * mat.size().width);

    return ceil(-1/log2(mean/(mean+1)));
}

double LosslessCodec::calculate_entropy(Mat mat) {
    
    int bins[256] = {0};
    int val;
    for(int i = 0; i < mat.size().height; i++) {
        for (int j = 0; j < mat.size().width; j++) {
            val = (int) mat.at<uchar>(i,j);           
            bins[val]++;
        }
    }

    double size = mat.size().width * mat.size().height;
    double entropy = 0;
    for(int i = 0; i < 256; i++) {
        if (bins[i] > 0) {
            entropy += (bins[i]/size) * (log(bins[i]/size));
        }
    }

    return entropy*(-1);
}

string LosslessCodec::toByte(string bits) {

    int sep = (int) bits.find("1");
    string q = bits.substr(0, sep+1);
    string r = bits.substr(sep+1);

    while(q.length() % 8 != 0) {
        q = "1" + q;
    }

    while (r.length() % 8 != 0) {
        r = "0" + r; 
    }

    return q + r;
}

string LosslessCodec::removePadding(string bits) {

    string q = bits.substr(0, bits.length()-8);
    string r = bits.substr(bits.length()-8);
    int n;

    n = q.find("0");

    if (n ==  string::npos) {
        q = "1";
    } else {
        q = q.substr(n);
    }
    return q + r;
}