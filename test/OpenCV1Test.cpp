#include <iostream>

#include "opencv2/opencv.hpp"

using namespace cv;

int main() {
    auto* capture = new VideoCapture();
    capture->open(0);
    if (!capture->isOpened()) {
        std::cout << "video not exist" << std::endl;
        return -1;
    }
    while (true) {
        Mat frame;
        capture->read(frame);
        if (frame.empty()) {
            break;
        }
        imshow("video", frame);
        waitKey(30);
    }
    capture->release();
    delete capture;
    return 0;
}
