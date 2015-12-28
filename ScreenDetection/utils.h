#ifndef UTILS
#define UTILS

#include "opencv2/imgproc/imgproc.hpp"

namespace Utils{
    bool contour_compare_area(const std::vector<cv::Point> c1, const std::vector<cv::Point> c2){
        return contourArea(c1) > contourArea(c2);
    }

    bool compare_points(const cv::Point& p1, const cv::Point& p2){
        return (p1.x + p1.y) < (p2.x + p2.y);
    }
}

#endif // UTILS

