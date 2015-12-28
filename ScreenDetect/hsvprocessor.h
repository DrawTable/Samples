#ifndef HSVPROCESSOR_H
#define HSVPROCESSOR_H

#include "common.h"

class HSVProcessor
{
    Mat orig_img, hsv, thresholded, contoured, warped;
    Mat transformMatrix;
    int lowH, lowS, lowV, highH, highS, highV;


    void toHsv();
    void initTrackBars();
    vector<Point> getOrderedPoints(vector<Point> rect);

    // drawing
    Point2f lastPointWarped;
    Point2f lastPointOrig;

public:
    string mainWindow;
    string debugWindow;
    string controlWindow;
    string warpedWindow;

    HSVProcessor(string filename);
    int currentImage;
    void processImage(string filename);
    void threshold();
    void show();
    void optimizeThreshold();
    void findScreen(Mat& thresholded);
    void transformImage(vector<Point> rect);
    void drawTransFormedPoint(Point2f p);
    void resetLastPoints();
signals:

public slots:
};

#endif // HSVPROCESSOR_H
