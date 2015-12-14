#include <QCoreApplication>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

string windowName = "mainWindow";

void detectScreen(){

    Mat img = imread("/Users/paulnta/Documents/HEIG4/PDG/tests/calibration06.JPG");
    imshow(windowName, img);
}

int main(int argc, char *argv[])
{

    namedWindow(windowName, WINDOW_KEEPRATIO);
    detectScreen();

    waitKey(0);
    return 0;
}

