#include "stylusdetect.h"

void StylusDetect::handleFrame(cv::Mat &frame)
{
    this->frame = frame;

    bgr = frame.clone();
    cvtColor(frame, frame, CV_BGR2HSV);
    orig = frame.clone();
    trackGreen = frame.clone(); // frame versions in HSV
    trackRed = frame.clone();

    processLedTracking();

    imshow(this->windowName, bgr);

    if(RECORD){
        outputVideo.write(this->bgr);
    }
}

void StylusDetect::processLedTracking()
{
   /* Filter HSV image to isolate leds color
    * This value are importante
    */
    inRange(trackGreen, Scalar(lowH[LED_GREEN], lowS[LED_GREEN], lowV[LED_GREEN]),
            Scalar(highH[LED_GREEN], highS[LED_GREEN], highV[LED_GREEN]), trackGreen);

    inRange(trackRed, Scalar(lowH[LED_RED], lowS[LED_RED], lowV[LED_RED]),
            Scalar(highH[LED_RED], highS[LED_RED], highV[LED_RED]), trackRed);

    /* Optimize threshold to improve object detection
     */
    optimizeThreshold(this->trackGreen);
    optimizeThreshold(this->trackRed);

    /* Leds are now isolated, we can find their positions
     */
    Point positionLedGreen = findObjectPosition(trackGreen);
    Point positionLedRed = findObjectPosition(trackRed, true);
    circle(this->bgr, positionLedGreen, 20, Scalar(0,255,0), 5);
    circle(this->bgr, positionLedRed, 20, Scalar(0,0,255), 5);

    if(DEBUG){
        imshow("trackGreen", trackGreen);
        imshow("trackRed", trackRed);
    }
}

/**
 * @brief StylusDetect::findObjectPosition
 *  Find the position of a LED.
 *  We use Moments to calculate the center and the area
 * @param imgThresholded
 * @param trackLight: if trackLight is true, we will detect if the LED is ON or OFF
 * @return The center Point of the LED
 */
Point StylusDetect::findObjectPosition(Mat& imgThresholded, bool trackLight){

    Moments oMoments = moments(imgThresholded);
    Point center;
    double dM01 = oMoments.m01;
    double dM10 = oMoments.m10;
    double dArea = oMoments.m00;

    if(dArea){
        int posX = dM10/dArea;
        int posY = dM01/dArea;
        center = Point(posX, posY);
    }

    if(trackLight){

        this->lightRoi = cv::RotatedRect(center, Size(60,60), 0);
        Point tl = this->lightRoi.boundingRect().tl();
        Point br = this->lightRoi.boundingRect().br();
        Rect frameRect(0,0, frame.cols, frame.rows);

       if (tl.inside(frameRect) && br.inside(frameRect)){
           this->trackLight = this->orig(this->lightRoi.boundingRect());
           inRange(this->trackLight, Scalar(94,0,0), Scalar(255,255,255), this->trackLight);

           optimizeThreshold(this->trackLight);

           Canny(this->trackLight, this->trackLight, 100, 255);
           Moments m = moments(this->trackLight);

           double mAera = m.m00;

           if(mAera != 0){
               // LEDs ON
                putText(this->bgr, "ON", Point(center.x + 100, center.y + 100), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,255,0),3);
           } else {
               putText(this->bgr, "OFF", Point(center.x + 100, center.y + 100), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),3);
           }

           if(DEBUG)
                imshow("trackLight", this->trackLight);

       } else {
           qDebug() << "roi out of range";
       }
    }

    return center;
}

/**
 * @brief StylusDetect::drawRotatedRect draw a rotated rect on a frame
 * @param frame
 * @param rect
 */
void StylusDetect::drawRotatedRect(Mat &frame, RotatedRect &rect ){
    Point2f vertices[4];
    rect.points(vertices);
    for (int i = 0; i < 4; i++)
        line(frame, vertices[i], vertices[(i+1)%4], Scalar(0,255,0),1);
}

/**
 * @brief StylusDetect::optimizeThreshold Remove small object (noise) on a thresholded frame
 * @param frame
 */
void StylusDetect::optimizeThreshold(Mat &frame){

    //morphological opening (remove small objects from the foreground)
    erode(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate( frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    //morphological closing (fill small holes in the foreground)
    dilate( frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    erode(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
}
