#include <QCoreApplication>
#include <opencv2/opencv.hpp>

#include "hsvprocessor.h"

using namespace std;
using namespace cv;

int currentImage = 1;
int nImage = 20;
bool drawing = false;

string images[] = {
    "/Users/paulnta/Documents/HEIG4/PDG/tests/calibration01.JPG",
    "/Users/paulnta/Documents/HEIG4/PDG/tests/IMG_1880.JPG",
    "/Users/paulnta/Documents/HEIG4/PDG/tests/IMG_1881.JPG",
    "/Users/paulnta/Documents/HEIG4/PDG/tests/IMG_1882.JPG",
    "/Users/paulnta/Documents/HEIG4/PDG/tests/IMG_1883.JPG",
    "/Users/paulnta/Documents/HEIG4/PDG/tests/IMG_1884.JPG",
    "/Users/paulnta/Documents/HEIG4/PDG/tests/IMG_1885.JPG",
    "/Users/paulnta/Documents/HEIG4/PDG/tests/IMG_1886.JPG",
    "/Users/paulnta/Documents/HEIG4/PDG/tests/IMG_1887.JPG",
    "/Users/paulnta/Desktop/IMG_1873.JPG",
    "/Users/paulnta/Desktop/IMG_1874.JPG",
    "/Users/paulnta/Desktop/IMG_1875.JPG",
    "/Users/paulnta/Desktop/IMG_1876.JPG",
    "/Users/paulnta/Desktop/IMG_1877.JPG",
    "/Users/paulnta/Desktop/IMG_1878.JPG",
    "/Users/paulnta/Documents/HEIG4/PDG/tests/calibration02.JPG",
    "/Users/paulnta/Documents/HEIG4/PDG/tests/calibration03.JPG",
    "/Users/paulnta/Documents/HEIG4/PDG/tests/calibration04.JPG",
    "/Users/paulnta/Documents/HEIG4/PDG/tests/calibration05.JPG",
    "/Users/paulnta/Documents/HEIG4/PDG/tests/calibration06.JPG"
};


HSVProcessor hsvProc(images[currentImage]);

void refresh(int, void*){
    hsvProc.threshold();
}

void changeImage(int, void*){
    hsvProc.currentImage++;
    hsvProc.initImage(images[hsvProc.currentImage % nImage]);
}

void onMouse( int event, int x, int y, int, void*){

    if(event == EVENT_LBUTTONDOWN){
        drawing = true;
    } else if (event == EVENT_LBUTTONUP){
        drawing = false;
        hsvProc.resetLastPoints();
    }

    if(drawing){
        hsvProc.drawTransFormedPoint(Point2f(x,y));
        hsvProc.show();
    }
}

int main(int argc, char *argv[])
{
    hsvProc.show();
    waitKey(0);
    return 0;
}

