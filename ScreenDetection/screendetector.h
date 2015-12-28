#ifndef SCREENDETECTOR_H
#define SCREENDETECTOR_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

#define DEBUG true      // show windows for debug

/**
 * @brief The ScreenDetector class Find a screen within an single image
 *
 * Finding the projected screen is the first part of the calibration process.
 * The screen is detected by tracking his background color (Generally green).
 *
 * ScreenDetector determine the coordinates of the four points that represent
 * the four corners of the screen and produce a transformation matrix.
 * Thanks to this transformation matrix we are able to match coordinates taken
 * by the camera with the real coordinates of the computers that is projecting the screen.
 */
class ScreenDetector
{
    typedef vector<Point> Contour;
    typedef vector<Contour> Contours;

public:
    /**
     * @brief The Error class shows error to the user
     *
     * Usage: Pass an error object as reference to a ScreenDetector method
     * and check Error::hasError() before using the returned value.
     */
    class Error{
        friend class ScreenDetector;
        bool error = false;
        string title;
        string message;
        void setError(string title, string message){
            this->message = message;
            this->title = title;
            this->error = true;
        }
    public:
        bool hasError() {return error;}
        string getErrorTitle() const {return title;}
        string getErrorMessage() const {return message;}
    };

    ScreenDetector(Mat img);
    void setColorRange(const Scalar hsvMin, const Scalar HsvMax);
    Mat getTransformationMatrix(Error&);
    static Point transformPoint(Point point, Mat transformMatrix);

private:
    Mat img,                  // original image containing a screen
        hsv,                  // image converted to HSV color space
        thresholded;          // image filtered by the screen's background color

    Scalar hsvMin, hsvMax;    // hsv color range of the green screen

    const int erodeDilateSize = 5;
    const double approximateEpsilon = 0.05;

    vector<Point> getOrderedPoints(vector<Point> rect);
    Mat transformImage(vector<Point> rect);
};

#endif // SCREENDETECTOR_H
