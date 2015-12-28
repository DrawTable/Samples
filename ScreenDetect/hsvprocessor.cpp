#include "hsvprocessor.h"

// This function has to be static
// defined in main.cpp
void refresh(int, void*);
void onMouse( int event, int x, int y, int, void*);
void changeImage(int, void*);

HSVProcessor::HSVProcessor(string filename)
{
    // Create windows
    mainWindow = "main window";
    debugWindow = "debug";
    controlWindow = "control";
    warpedWindow = "warped";
    currentImage = 0;
    resetLastPoints();
    namedWindow(debugWindow, WINDOW_KEEPRATIO);
    namedWindow(mainWindow, WINDOW_KEEPRATIO);
    namedWindow(warpedWindow, WINDOW_KEEPRATIO);
    namedWindow(controlWindow, WINDOW_KEEPRATIO);
    setMouseCallback(mainWindow, onMouse);

    // add trackbars for hsv range
    initTrackBars();

    // handle the image
    processImage(filename);
}


void HSVProcessor::processImage(string filename)
{
    // init the origingal
    orig_img = imread(filename);
    contoured = Mat::zeros(orig_img.rows, orig_img.cols, CV_8UC3);
    toHsv();
}


// convert to hsv color space
void HSVProcessor::toHsv()
{
    cvtColor(orig_img, hsv, CV_BGR2HSV);
    threshold();
}

void HSVProcessor::threshold()
{
    // Threshold the image to
    inRange(hsv, Scalar(lowH, lowS, lowV), Scalar(highH,highS,highV), thresholded);
    optimizeThreshold();
    findScreen(thresholded);
    show();
}

void HSVProcessor::show()
{
    imshow(debugWindow, contoured);
    imshow(mainWindow, orig_img);
    imshow(warpedWindow, warped);
    imshow(controlWindow, thresholded);
}

void HSVProcessor::optimizeThreshold()
{
    //morphological opening (remove small objects from the foreground)
    erode(thresholded, thresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate( thresholded, thresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    //morphological closing (fill small holes in the foreground)
    dilate( thresholded, thresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    erode(thresholded, thresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    GaussianBlur(thresholded, thresholded, Size(3,3), 0);
}

bool contour_compare_area(const vector<Point> c1, const vector<Point> c2){
    return contourArea(c1) > contourArea(c2);
}



void HSVProcessor::findScreen(Mat &thresholded)
{
    vector<vector<Point>> contours;
    vector<Point> approx;

    Mat img;
    thresholded.copyTo(img);

    findContours(img, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    qDebug() << contours.size() << " contours found";

    if(contours.size() > 0){
        // sort contours by area
        std::sort(contours.begin(),contours.end(), contour_compare_area);

        // find the screen contours
        int n = contours.size();
        for(int i=0; i < n; ++i){

            // approximate the contour
            approxPolyDP(contours.at(i), approx, 0.05 * arcLength(contours.at(i), true), true );

            // our screen has 4 point when approximated
            if(approx.size() > 0){
                qDebug() << "found a contours with for points (" << i << ")";

                // draw screen
                polylines(contoured, approx, true, Scalar(0,0,255), 3);
                transformImage(approx);
                break;
            }
        }
    }
}

void HSVProcessor::transformImage(vector<Point> rect)
{
    Point tl,tr, bl, br;
    int width, height;
    // order Points : top-left, top-right, bottom-left, bottom-right
    rect = getOrderedPoints(rect);
    tl = rect.at(0);
    tr = rect.at(1);
    br = rect.at(2);
    bl = rect.at(3);

    // find max width
    int widthA  = std::sqrt( std::pow( std::abs(br.y - bl.y) ,2) + std::pow( std::abs(br.x - bl.x),2));
    int widthB  = std::sqrt( std::pow( std::abs(tr.y - tl.y) ,2) + std::pow( std::abs(tr.x - tl.x),2));
    width = std::max(widthA, widthB);

    int heightA  = std::sqrt( std::pow( std::abs(tl.y - bl.y) ,2) + std::pow( std::abs(tl.x - bl.x),2));
    int heightB  = std::sqrt( std::pow( std::abs(tr.y - br.y) ,2) + std::pow( std::abs(tr.x - br.x),2));
    height = std::max(heightA, heightB);

    qDebug() << "max width:" << width;
    qDebug() << "max height:" << height;

    Point2f src[4];
    src[0] = rect.at(0);
    src[1] = rect.at(1);
    src[2] = rect.at(2);
    src[3] = rect.at(3);

    Point2f dst[4];

    dst[0] = Point(0,0);
    dst[1] = Point(width-1, 0);
    dst[2] = Point(width-1, height-1);
    dst[3] = Point(0, height-1);

    transformMatrix = getPerspectiveTransform(src, dst);
    warpPerspective(orig_img, warped, transformMatrix, Size(width, height));
}

void HSVProcessor::drawTransFormedPoint(Point2f p)
{
    vector<Point2f> src;
    vector<Point2f> dst;
    src.push_back(p);
    perspectiveTransform(src, dst, transformMatrix);
    cout << "a: " << src.at(0) << " -> b:" << dst.at(0) << endl;

    if((int)lastPointOrig.x > 0 && (int)lastPointWarped.x > 0){
        line(warped, lastPointWarped, dst.at(0), Scalar(0,0,255), 2);
        line(orig_img, lastPointOrig, src.at(0), Scalar(0,0,255), 2);
    }

    lastPointOrig = src.at(0);
    lastPointWarped = dst.at(0);

}

void HSVProcessor::resetLastPoints()
{
    lastPointOrig = Point(-1,-1);
    lastPointWarped = Point(-1,-1);
}

void HSVProcessor::initTrackBars()
{
    // green
    lowH = 41;      // 34
    lowS = 26;      // 26
    lowV = 128;     // 71

    highH = 90;     // 84
    highS = 255;    // 255
    highV = 255;    // 255

    // creation des slider dans la fenetre "control"
    createTrackbar("lowH",  "control", &lowH, 179, refresh);
    createTrackbar("lowS",  "control", &lowS, 255, refresh);
    createTrackbar("lowV",  "control", &lowV, 255, refresh);

    createTrackbar("highH", "control", &highH, 179, refresh);
    createTrackbar("highS", "control", &highS, 255, refresh);
    createTrackbar("highV", "control", &highV, 255, refresh);

    createTrackbar("Image", mainWindow, &currentImage, 20, changeImage);
}

bool compare_points(const Point& p1, const Point& p2){
    return (p1.x + p1.y) < (p2.x + p2.y);
}

vector<Point> HSVProcessor::getOrderedPoints(vector<Point> rect)
{
    Point tl, tr, bl, br;
    sort(rect.begin(), rect.end(), compare_points);
    tl = rect.at(0);
    br = rect.at(3);
    int diff1 = tl.y - rect.at(1).y;
    int diff2 = tl.y - rect.at(2).y;

    if(diff1 < diff2){
        tr = rect.at(2);
        bl = rect.at(1);
    } else {
        tr = rect.at(1);
        bl = rect.at(2);
    }

    rect.at(0) = tl;
    rect.at(1) = tr;
    rect.at(2) = br;
    rect.at(3) = bl;

    return rect;
}
