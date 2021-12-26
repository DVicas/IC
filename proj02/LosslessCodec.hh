#include <string>
#include <fstream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>


using namespace std;
using namespace cv;

class LosslessCodec {
    public:
        void encode(string path);
    private:
        void toYUV(Mat img, Mat* yuv_channels);
};

void LosslessCodec::toYUV(Mat img, Mat* yuv_channels) {

    cvtColor(img, img, COLOR_RGB2YUV);
    split(img, yuv_channels);

    /****** DEBUG ******/
    cout << yuv_channels << endl;    
    cout << yuv_channels[0].size() << endl;
    cout << yuv_channels[1].size() << endl;
    cout << yuv_channels[2].size() << endl;

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
    imshow("Y", yuv_channels[0]);
    imshow("U", yuv_channels[1]);
    imshow("V", yuv_channels[2]);
    waitKey(0);

    cout << yuv_channels[0].size() << endl;
    cout << yuv_channels[1].size() << endl;
    cout << yuv_channels[2].size() << endl;

    // imshow("tmp_u", tmp_u);
    // imshow("mat_v", mat_v);
    // waitKey(0);

    // Mat mat_y (img.size().height,img.size().width,CV_8UC1);
    // Mat mat_u (img.size().height,img.size().width,CV_8UC1);
    // Mat mat_v (img.size().height,img.size().width,CV_8UC1);
    // float r, g, b;
    // int y, u, v;
    // for (int i = 0; i < img.size().height; i++) {
    //     for (int j = 0; j < img.size().width; j++) {
    //         r = img.at<Vec3b>(i,j)[0] ;
    //         g = img.at<Vec3b>(i,j)[1] ;
    //         b = img.at<Vec3b>(i,j)[2] ;

    //         // y = 16 + 65.481*r + 128.553*g + 24.966*b;
    //         // y = 0.299*r + 0.587*g + 0.114*b;
    //         // u = 128 -0.147*r - 0.289*g + 0.436*b;
    //         // v = 128 + 0.550*r - 0.515*g - 0.1*b;
    //         y = 0.299*r + 0.587*g + 0.114*b;
    //         u = 128 - 0.168736*r - 0.331264*g + 0.5*b;
    //         v = 128 + 0.5*r - 0.418688*g - 0.081312*b;

    //         mat_y.at<uchar>(i,j) = y;
    //         // if (i%2 == 0 && j%2 == 0) {
    //             mat_u.at<uchar>(i,j) = u;
    //             mat_v.at<uchar>(i,j) = v;
    //         // }
    //     }
    // }

    // Mat channels[3] = {mat_y, mat_u, mat_v};
    // Mat output;
    // merge(channels, 3, output);
    // imshow("test", output);
    // waitKey(0);
    //  cout << (int) img.at<Vec3b>(i,j)[1] << endl;

}


void LosslessCodec::encode(string path) {
    Mat img = imread(path);
    if (img.empty()) {
        cout << "Error with image" << endl;
    }

    Mat channels[3];
    toYUV(img, channels);

    cout << channels << endl;
 }