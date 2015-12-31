#include "controller.h"
#include <QCameraInfo>
#include <QList>




void testVideo(){

//    VideoCapture cap("/Users/paulnta/Documents/HEIG4/PDG/Led_Tests/experimental.mov");
    VideoCapture cap(0);
    if(!cap.isOpened())
        return;

    waitKey(1);

    LedDetector* ld = LedDetector::getInstance();


    Mat frame;
    for(;;){

        cap >> frame;
        if(frame.empty())
            return;

//        int H = 700;
//        Size newSize(H, cvRound((double)H*frame.rows/frame.cols));
//        resize(frame, frame, newSize);

        ld->setImage(frame);
        ld->debugLedDetection();

        int key = waitKey(1);
        switch (key) {
        case 27:
            return;
            break;
        case (int)'p':
            waitKey(0);
        }
    }

}

void bgs(){
cv::Mat frame;
  cv::Mat back;
  cv::Mat fore;
  cv::VideoCapture cap ("/Users/paulnta/Documents/HEIG4/PDG/Led_Tests/iPhone_light.mov");
  cv::Ptr<BackgroundSubtractorMOG2> bg = createBackgroundSubtractorMOG2();
  bg->setNMixtures(3);
  bg->setDetectShadows(false);
  std::vector < std::vector < cv::Point > >contours;

  cv::namedWindow ("Frame", 0);
  cv::namedWindow ("Background", 0);

    for (;;){

      cap >> frame;
      int H = 700;
      Size newSize(H, cvRound((double)H*frame.rows/frame.cols));
      resize(frame, frame, newSize);
      bg->apply(frame,fore);
      bg->getBackgroundImage(back);
              cv::erode (fore, fore, cv::Mat ());
              cv::dilate (fore, fore, cv::Mat ());
              cv::findContours (fore, contours, CV_RETR_EXTERNAL,
                    CV_CHAIN_APPROX_NONE);
              cv::drawContours (frame, contours, -1, cv::Scalar (0, 0, 255), 2);
      cv::imshow ("Frame", frame);
      cv::imshow ("Background", back);
      if (cv::waitKey (1) >= 0)
         break;
    }
}

int main()
{

//    Mat src = imread("/Users/paulnta/Desktop/Test_LedDetect/5.png");
//    LedDetector* ld = LedDetector::getInstance();
//    ld->setImage(src);
//    ld->debugLedDetection();
//    waitKey(0);

//    bgs();
    testVideo();
    return 0;
}

