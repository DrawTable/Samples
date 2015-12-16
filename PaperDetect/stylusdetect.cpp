#include "stylusdetect.h"
#include "line.h"

void StylusDetect::handleFrame(cv::Mat &frame)
{
    this->frame = frame;

    // original BGR image
    bgr = frame.clone();

    cvtColor(frame, frame, CV_BGR2HSV);

    // original HSV image
    orig = frame.clone();

    // HSV images for led tracking
    trackGreen = frame.clone();
    trackRed = frame.clone();

    processLedTracking();

    imshow(this->windowName, bgr);
}


void StylusDetect::processLedTracking()
{
   /* Filter HSV image to isolate leds color
    * This value are importante
    */
    int i1 = getTickCount();
    inRange(trackGreen, lowHSV[LED_GREEN], highHSV[LED_GREEN], trackGreen);
    inRange(trackRed, lowHSV[LED_RED], highHSV[LED_RED], trackRed);
    int i2 = getTickCount();

    /* Optimize threshold to improve object detection
     */
    int o1 = getTickCount();
    optimizeThreshold(this->trackGreen);
//    optimizeThreshold(this->trackRed);
    int o2 = getTickCount();

    /* Leds are now isolated, we can find their positions
     */
    int p1 = getTickCount();
    Point positionLedGreen = findObjectPosition(trackGreen, LED_GREEN, true);
    Point positionLedRed = findObjectPosition(trackRed, LED_RED, true);
    int p2 = getTickCount();

    circle(this->bgr, positionLedGreen, 20, Scalar(0,255,0), 5);
    circle(this->bgr, positionLedRed, 20, Scalar(0,0,255), 5);
    calculatePenPos(positionLedRed, positionLedGreen);

    if(DEBUG){
        imshow("trackGreen", trackGreen);
        imshow("trackRed", trackRed);
    }

    if(PROFILING){
        double f = getTickFrequency();

        double iTime = (i2- i1) / f;
        double oTime = (o2- o1) / f;
        double pTime = (p2- p1) / f;
        double time = (p2 - i1) / f;

        qDebug("Tracking:\t %f ms", time*1000);
        qDebug("inRange: \t%f ms", iTime*1000);
        qDebug("optimze: \t%f ms", oTime*1000);
        qDebug("position: \t%f ms", pTime*1000);
        qDebug("\n");
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

    /* calculate estimated center */
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

    if(DEBUG){
        drawCross(imgThresholded, center, Scalar(255,255,255), 20);
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

           if(DEBUG){
               if(led == LED_RED){
                   imshow("trackLightRedHSV", this->trackLight[LED_RED]);
               }
               else{
                   imshow("trackLightGreenHSV", this->trackLight[LED_GREEN]);
               }
           }

           // detect light contour
           Canny(this->trackLight[led], this->trackLight[led], 100, 255);

           // calculate the exact position of the light
           Moments m = moments(this->trackLight[led]);
           double mArea = m.m00;
           Point lightCenter(double(m.m10/mArea), double(m.m01/mArea));


           if(DEBUG){ // here we can see, the new exact position by drawing a cross
               drawCross(this->trackLight[led], Point(windowTrackLightSize/2, windowTrackLightSize/2), Scalar(255,255,255), 1);
               if(lightCenter.x > 0 && lightCenter.y > 0)
                drawCross(this->trackLight[led], lightCenter, Scalar(255,255,255), 3);
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

       }
    }

    return center;
}


void StylusDetect::calculatePenPos(const Point &red, const Point &green){
    Line a(green,red);
     a.draw(this->bgr);

    if(estimatePenPos){
        a.increaseLenght(a.length()*0.6);
        Line b = a;
        b.translateY(0);
        b.translateX(-25);

        circle(this->bgr, b.b, 10, Scalar(0,255,255), 2);

        int x2 = red.x + a.dx();

        b.draw(this->bgr);
    }
}

void StylusDetect::drawCross(Mat &frame, Point center, Scalar color, int size){
    line(frame, Point(center.x, center.y -size), Point(center.x,center.y + size ), color, 1);
    line(frame, Point(center.x-size, center.y), Point(center.x + size,center.y), color, 1);
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
