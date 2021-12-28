#include <string>
#include <fstream>

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
        void encode(string path);
        void decode();
    private:
        void toYUV(Mat img, Mat* yuv_channels);
        int calculate_m(Mat mat);
        double calculate_entropy(Mat mat);
        Mat predictor(Mat img);
};

void LosslessCodec::toYUV(Mat img, Mat* yuv_channels) {

    // convert to YUV colorspace
    cvtColor(img, img, COLOR_RGB2YUV);
    
    // split YUV channels into 3 arrays
    split(img, yuv_channels);

    /****** DEBUG ******/
    // cout << yuv_channels << endl;    
    // cout << yuv_channels[0] << endl;
    // cout << yuv_channels[1] << endl;
    // cout << yuv_channels[2] << endl;

    // imshow("Y", channels[0]);
    // imshow("U", channels[1]);
    // imshow("V", channels[2]);
    // waitKey(0);


    // ofstream file;
    // file.open("yuv");
    // file << "---   Y   ---\n" << channels[0] << "\n---  U  ---\n" << channels[1] << "\n---  V  ---\n" << channels[2];

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

    /***** MORE DEBUG ******/
    // imshow("Y", yuv_channels[0]);
    // imshow("U", yuv_channels[1]);
    // imshow("V", yuv_channels[2]);
    // waitKey(0);

    // cout << yuv_channels[0].size() << endl;
    // cout << yuv_channels[1].size() << endl;
    // cout << yuv_channels[2].size() << endl;
}

Mat LosslessCodec::predictor(Mat img) {

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


void LosslessCodec::encode(string path) {
    Mat img = imread(path);
    if (img.empty()) {
        cout << "Error with image" << endl;
    }

    Mat channels[3];
    toYUV(img, channels);

    //one for each channel (3 channels) // Y = [0] / U = [1] / V = [2]
    Mat error[3];
    double entropy = 0;
    for (int i = 0; i < 3; i++) {   //fill error matrices and calc entropy 
        error[i] = predictor(channels[i]);
        entropy += calculate_entropy(channels[i]);
    }
    entropy = entropy / 3; //3 channels
    cout << "entropy " << entropy << endl;
    

    Golomb g;
    BitStream bs = BitStream("", "image.bin");
    int m, val;
    string bits, code, byte;
    for (int k = 0; k < 3; k++) {
        m = calculate_m(error[k]);

        cout << m << endl;
        
        for (int i = 0; i < error[k].size().height; i++) {
            for (int j = 0; j < error[k].size().width; j++){   
                val = error[k].at<uchar>(i,j);
                bits = g.EncodeNumbers(val, m);
                bs.writeBits(bits);
            }
        }
    }
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