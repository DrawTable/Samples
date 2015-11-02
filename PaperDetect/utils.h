#ifndef UTILS
#define UTILS

#include <QDebug>
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <vector>

/*
 *          C1	C2	C3	C4
 *   CV_8U	0	8	16	24
 *   CV_8S	1	9	17	25
 *   CV_16U	2	10	18	26
 *   CV_16S	3	11	19	27
 *   CV_32S	4	12	20	28
 *   CV_32F	5	13	21	29
 *   CV_64F	6	14	22	30
 *
 */

std::string typeToString(int type){

    int numTypes = 7;
    int numSubTypes = 4;

    int CV8U  [] = {0,8,16,24};
    int CV8S  [] = {1,9,17,25};
    int CV16U [] = {2,10,18,26};
    int CV16S [] = {3,11,19,27};
    int CV32S [] = {4,12,20,28};
    int CV32F [] = {5,13,21,29};
    int CV64F [] = {6,14,22,30};

    int* types[] = {CV8U,CV8S,CV16U,CV16S,CV32S,CV32F,CV64F};
    std::string typesChar[] = {"CV_8U","CV_8S","CV_16U","CV_16S","CV_32S","CV_32F","CV_64F"};
    std::string subTypesChar[] = {"C1","C2", "C3", "C4"};

    std::string s = "";

    for(int i=0; i< numTypes; ++i){
        for(int j=0; j < numSubTypes; ++j){
             int value = *(*(types + i) + j);
             if(type == value){
                 s = typesChar[i] + subTypesChar[j];
                 break;
             }
        }
    }

    return s;
}

void showImageInfos(const cv::Mat* img){

    qDebug() << "Size:" << img->rows << "x" << img->cols << "\n"
             << "Type:" << QString::fromStdString(typeToString(img->type())) << "\n"
             << "Depth:" << img->depth() << "\n"
             << "Data pointer:" << img->data << "\n";
}

bool compareContourAreas ( std::vector<cv::Point> contour1, std::vector<cv::Point> contour2 ) {
    double i = fabs( contourArea(cv::Mat(contour1)) );
    double j = fabs( contourArea(cv::Mat(contour2)) );
    return ( i > j );
}

#endif // UTILS

