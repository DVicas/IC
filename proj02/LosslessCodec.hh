#include <string>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>


using namespace std;
using namespace cv;

class LosslessCodec {
    public:
        LosslessCodec(string);
        void Encode();
};

LosslessCodec::LosslessCodec(string path) { 
    Mat img = imread(path);

    if (img.empty()) {
        cout << "Wrong path" << endl;
    }

    imwrite("teste.ppm", img);    
}

void LosslessCodec::Encode() { }