#ifndef LEDDETECTOR
#define LEDDETECTOR

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

class LedDetector {

    Mat orig, img, hsv, thresholded;
    int lowH = 169, lowS = 21, lowV = 219, highH = 179, highS = 255, highV = 255;

    bool isSelecting = false;
    Rect selection;
    Point origin;
    Mat img_selection;

public:

    /////////////////////////// static ////////////////////////////////

    static LedDetector* instance;
    static LedDetector* getInstance(){
        if(instance == nullptr)
            instance = new LedDetector();
        return instance;
    }

    static void onTrackBarChanged(int, void*){
        LedDetector::getInstance()->processThreshold();
    }


    static void onMouse(int event, int x, int y, int, void*){
        LedDetector::getInstance()->selectRoi(event, Point(x,y));
    }

    static void onSelectionMouse(int event, int, int, int, void*){
        if(event == EVENT_LBUTTONDBLCLK)
            LedDetector::getInstance()->calcHistograms(LedDetector::getInstance()->getImgSelection());
    }

    /////////////////////////// public ////////////////////////////////

    void setImage(Mat& img){
        this->img = img;
        this->img.copyTo(orig);
        cvtColor(this->img, hsv, CV_BGR2HSV);
    }

    void debugLedDetection(){
        namedWindow("source", 0);
        namedWindow("threshold", 0);
        namedWindow("masked", 0);
        setMouseCallback("source", onMouse);
        setMouseCallback("threshold", onMouse);

        createTrackbar("lowH", "threshold", &lowH, 179, onTrackBarChanged);
        createTrackbar("lowS", "threshold", &lowS, 255, onTrackBarChanged);
        createTrackbar("lowV", "threshold", &lowV, 255, onTrackBarChanged);
        createTrackbar("highH", "threshold", &highH, 179, onTrackBarChanged);
        createTrackbar("highS", "threshold", &highS, 255, onTrackBarChanged);
        createTrackbar("highV", "threshold", &highV, 255, onTrackBarChanged);
        processThreshold();
    }

    void processThreshold(){
        Mat im;
        hsv.copyTo(im);
        inRange(im, Scalar(lowH, lowS, lowV), Scalar(highH,highS,highV), thresholded);

        erode(thresholded, thresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        dilate(thresholded, thresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        dilate(thresholded, thresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        erode(thresholded, thresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

        if(selection.area() > 0)
            calcHistograms(img_selection);
        showImages();
    }

    void showImages(){
        imshow("source", img);
        imshow("threshold", thresholded);

        Mat masked;
        orig.copyTo(masked, thresholded);
        imshow("masked", masked);
    }

    void selectRoi(int event, Point p){

        if(isSelecting){
            selection.x = MIN(p.x, origin.x);
            selection.y = MIN(p.y, origin.y);
            selection.width = abs(p.x - origin.x);
            selection.height = abs(p.y - origin.y);

            orig.copyTo(img);
            rectangle(img, selection, Scalar(0,255,0),1);
            imshow("source", img);
        }

        switch (event) {
        case EVENT_LBUTTONDOWN:
            origin = p;
            selection = Rect(p,p);
            isSelecting = true;
            break;
        case EVENT_LBUTTONUP:
            isSelecting = false;
            if(selection.area() > 0){
                handleSelection();
            } else {
                destroyWindow("selection");
                destroyWindow("h_hist");
                destroyWindow("s_hist");
                destroyWindow("v_hist");
            }
            break;
        }
    }

    void handleSelection(){
        namedWindow("selection", 0);            // show window selection
        setMouseCallback("selection", onSelectionMouse);
        img_selection = orig(selection);         // get the region of interset
        imshow("selection", img_selection);
    }

    void calcHistograms(Mat hsv_img){
        // split hsv channels
        vector<Mat> hsv_planes;
        split(hsv_img, hsv_planes);
        int bins[] = {179,255,255};
        float range256[] = {0,256};
        float range179[] = {0,179};
        const float *h_range = {range179}, *s_range = {range256}, *v_range = {range256};

        MatND h_hist, s_hist, v_hist;
        bool uniform = true, accumulate = false;

        calcHist(&hsv_planes[0], 1, 0, Mat(), h_hist, 1, &bins[0], &h_range, uniform, accumulate);
        calcHist(&hsv_planes[1], 1, 0, Mat(), s_hist, 1, &bins[1], &s_range, uniform, accumulate);
        calcHist(&hsv_planes[2], 1, 0, Mat(), v_hist, 1, &bins[2], &v_range, uniform, accumulate);

        namedWindow("h_hist", 0);
        namedWindow("s_hist", 0);
        namedWindow("v_hist", 0);
        int W = 512; int H = 400;
        int h_bin_w = cvRound( (double) W/bins[0]);
        int s_bin_w = cvRound( (double) W/bins[1]);
        int v_bin_w = cvRound( (double) W/bins[2]);

        cv::normalize(h_hist, h_hist, 0, H, NORM_MINMAX, -1, Mat());
        cv::normalize(s_hist, s_hist, 0, H, NORM_MINMAX, -1, Mat());
        cv::normalize(v_hist, v_hist, 0, H, NORM_MINMAX, -1, Mat());

        Mat img_h_hist = Mat::zeros(H, W, CV_8UC3),
            img_s_hist = Mat::zeros(H, W, CV_8UC3),
            img_v_hist = Mat::zeros(H, W, CV_8UC3);

        for(int i=1; i < bins[0]; ++i){
            Scalar color(255,0,0);
            rectangle( img_h_hist,
                       Point( i*h_bin_w, H ),
                       Point( (i+1)*h_bin_w, H - cvRound( h_hist.at<float>(i)*H/255.0)),
                       color, -1 );
        }

        for(int i=1; i < bins[0]; ++i){
            Scalar color(0,255,0);
            rectangle( img_s_hist,
                       Point( i*h_bin_w, H ),
                       Point( (i+1)*s_bin_w, H - cvRound( s_hist.at<float>(i)*H/255.0)),
                       color, -1 );
        }

        for(int i=1; i < bins[0]; ++i){
            Scalar color(0,0,255);
            rectangle( img_v_hist,
                       Point( i*v_bin_w, H ),
                       Point( (i+1)*v_bin_w, H - cvRound( v_hist.at<float>(i)*H/255.0)),
                       color, -1 );
        }


        line(img_h_hist, Point( W * lowH / 179.0, 0), Point(W * lowH / 179.0, H-1), Scalar(255,255,0), 1);
        line(img_h_hist, Point( W * highH / 179.0, 0), Point(W * highH / 179.0, H-1), Scalar(255,255,0), 1);

        line(img_s_hist, Point( W * lowS / 255.0, 0), Point(W * lowS / 255.0, H-1), Scalar(255,255,0), 1);
        line(img_s_hist, Point( W * highS / 255.0, 0), Point(W * highS / 255.0, H-1), Scalar(255,255,0), 1);

        line(img_v_hist, Point( W * lowV / 255.0, 0), Point(W * lowV / 255.0, H-1), Scalar(255,255,0), 1);
        line(img_v_hist, Point( W * highV / 255.0, 0), Point(W * highV / 255.0, H-1), Scalar(255,255,0), 1);

        /**
         * 179 -> 169
         * 512 -> x
         *
         * 200 -> 100
         * 512 -> x    W * val / maxVal
         */
        imshow("h_hist", img_h_hist);
        imshow("s_hist", img_s_hist);
        imshow("v_hist", img_v_hist);
    }

    ///////////////////////// Getter&Setter ////////////////////////////

    Mat getImgSelection(){ return img_selection; }

private:

    /////////////////////////// private ////////////////////////////////
    LedDetector(){

    }

};

#endif // LEDDETECTOR

