#include "screendetector.h"

int main(int argc, char *argv[])
{
    // get an image
    Mat img = imread("/Users/paulnta/Documents/HEIG4/PDG/tests/IMG_1883.JPG");
    ScreenDetector sd(img);

    // get the transform matrix
    ScreenDetector::Error err;
    Mat transformMatrix = sd.getTransformationMatrix(err);
    // always check error before using the transformatrix
    if(err.hasError()){
        cerr << err.getErrorTitle() << ":\n"<< err.getErrorMessage() << endl;
        return -1;
    }

    // test if we can transform a point
    Point p1(1756, 1010);
    Point p2 = ScreenDetector::transformPoint(p1, transformMatrix);
    cout << p1 << " -> " << p2 << endl;

    // test the result of a point outside of the screen
    p1 = Point(0, 0);
    p2 = ScreenDetector::transformPoint(p1, transformMatrix);
    cout << p1 << " -> " << p2 << endl; // will be p1 and p2 are < 0

    waitKey(0); // wait for user key press
    return 0;
}

