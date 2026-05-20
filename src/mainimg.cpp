#include <opencv2/opencv.hpp>
#include <tracker.h>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    FaceTracker A = FaceTracker();
    namedWindow("ffs", WINDOW_AUTOSIZE);
    while (true) {
        vector<float> pos = A.retrieveFacePos();
        cout << "x:" << pos[0] << " y:" << pos[1] << endl;
        Mat Aa = A.getFrame();
        cv::imshow("ffs", Aa);
        cv::waitKey(30);
    }

    destroyAllWindows();
    return 0;
}
