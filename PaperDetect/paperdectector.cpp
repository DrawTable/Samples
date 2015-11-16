#include "paperdectector.h"
#include "utils.h"

PaperDectector::PaperDectector(const char * file) :
    VideoController(file),
    lowH(0), lowS(0), lowV(0), highH(179), highS(255), highV(255),
    lastX(-1), lastY(-1)
{
    namedWindow("control", CV_WINDOW_KEEPRATIO);

    // orange
    // min(75, 94, 117) max(80,181,160)
    lowH = 75;
    lowS = 94;
    lowV = 117;

    highH = 80;
    highS = 181;
    highV = 160;

    // creattion des slider dans la fenetre "control"
    createTrackbar("lowH",  "control", &lowH, 179);
    createTrackbar("lowS",  "control", &lowS, 255);
    createTrackbar("lowV",  "control", &lowV, 255);

    createTrackbar("highH", "control", &highH, 179);
    createTrackbar("highS", "control", &highS, 255);
    createTrackbar("highV", "control", &highV, 255);
}

void PaperDectector::handleFrame(Mat &frame)
{
    Mat imgHSV, imgThresholded;

    // convert in HSV
    cvtColor(frame,imgHSV, CV_BGR2HSV);

    // Threshold the image to
    inRange(imgHSV, Scalar(lowH, lowS, lowV), Scalar(highH,highS,highV), imgThresholded);

    //morphological opening (remove small objects from the foreground)
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    //morphological closing (fill small holes in the foreground)
    dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    Mat imgLines = Mat::zeros(frame.size(), CV_8UC3);

    Moments oMoments = moments(imgThresholded);
    double dM01 = oMoments.m01;
    double dM10 = oMoments.m10;
    double dArea = oMoments.m00;

    if(dArea > 10000){
        int posX = dM10/dArea;
        int posY = dM01/dArea;

        if(lastX >= 0 && lastY >=0 && posX >= 0 && posY >= 0){
            line(imgLines, Point(posX,posY), Point(lastX, lastY), Scalar(0,255,0),2);
        }

        lastX = posX;
        lastY = posY;
    }

    frame = frame + imgLines;
    imshow(windowName, frame);
    imshow("control", imgThresholded);

    if(waitKey(33) == int('p')){
        this->togglePause();
    }
}



