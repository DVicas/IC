#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "LosslessCodec.hh"


using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {

    LosslessCodec g = LosslessCodec();
    // g.encode(argv[1]);

    g.decode(argv[1]);
    // Mat img = imread(argv[1], 0);

    // if (img.empty()) {
    //     cout << "Wrong path" << endl;
    // }

    // imshow("teste.bgr", img);

    // waitKey(0);

    // destroyWindow("teste.bgr");

    return 0;
}