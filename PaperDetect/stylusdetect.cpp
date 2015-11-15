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
    inRange(trackGreen, lowHSV[LED_GREEN], highHSV[LED_GREEN], trackGreen);
    inRange(trackRed, lowHSV[LED_RED], highHSV[LED_RED], trackRed);

    /* Optimize threshold to improve object detection
     */
    optimizeThreshold(this->trackGreen);
    optimizeThreshold(this->trackRed);

    /* Leds are now isolated, we can find their positions
     */
    Point positionLedGreen = findObjectPosition(trackGreen, LED_GREEN, true);
    Point positionLedRed = findObjectPosition(trackRed, LED_RED, true);

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
Point StylusDetect::findObjectPosition(Mat& imgThresholded, LED_TYPE led, bool trackLight){

    Moments oMoments = moments(imgThresholded);
    Point center;
    double dM01 = oMoments.m01;
    double dM10 = oMoments.m10;
    double dArea = oMoments.m00;

    if(dArea > 0){
        int posX = dM10/dArea;
        int posY = dM01/dArea;
        center = Point(posX, posY);
    }

    /* Track LED's light to improve the position.
     * It's easier to detect the light
     */
    if(trackLight){

        // select the region of interest rectangle by isolating the led in a different window

        this->lightRoi[led] = cv::RotatedRect(center, Size(this->windowTrackLightSize,this->windowTrackLightSize), 0);

        Point tl = this->lightRoi[led].boundingRect().tl();
        Point br = this->lightRoi[led].boundingRect().br();
        Rect frameRect(0,0, frame.cols, frame.rows);

       if (tl.inside(frameRect) && br.inside(frameRect)){ // check if the window is not out of range

           // select the region of interset (roi) from the original hsv image.
           this->trackLight[led] = this->orig(this->lightRoi[led].boundingRect());

           // threshold the image to detect led light
           inRange(this->trackLight[led], Scalar(trackLightHue[led],0,0), Scalar(255,255,255), this->trackLight[led]);
           optimizeThreshold(this->trackLight[led]);


           // detect light contour
           Canny(this->trackLight[led], this->trackLight[led], 100, 255);

           // calculate the exact position of the light
           Moments m = moments(this->trackLight[led]);
           double mArea = m.m00;
           Point lightCenter(double(m.m10/mArea), double(m.m01/mArea));


           if(DEBUG){ // here we can see, the new exact position by drawing a cross
            line(this->trackLight[led], Point(lightCenter.x, lightCenter.y -3), Point(lightCenter.x,lightCenter.y + 3 ), Scalar(255,255,255), 1);
            line(this->trackLight[led], Point(lightCenter.x-3, lightCenter.y), Point(lightCenter.x + 3,lightCenter.y), Scalar(255,255,255), 1);
           }

           // improve the position
           if(mArea){
            center.x = center.x - (this->windowTrackLightSize/2) + lightCenter.x;
            center.y = center.y - (this->windowTrackLightSize/2) + lightCenter.y;
           }

           if(led == LED_RED) { // only the red LED determine if the user is drawing
               if(mArea != 0){
                   // LEDs ON
                    putText(this->bgr, "ON", Point(center.x + 100, center.y + 100), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,255,0),3);
               } else {
                   putText(this->bgr, "OFF", Point(center.x + 100, center.y + 100), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),3);
               }
           }

           if(DEBUG){

                if(led == LED_RED){
                    imshow("trackLightRed", this->trackLight[LED_RED]);
                }
                else{
                    imshow("trackLightGreen", this->trackLight[LED_GREEN]);
                }
           }

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
