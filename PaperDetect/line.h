#ifndef LINE
#define LINE

#include <opencv2/opencv.hpp>
#include <QString>

using namespace std;

class Line{
public:
    Line(cv::Point a, cv::Point b): a(a), b(b){}

    cv::Point a;
    cv::Point b;

    double dy(){ return std::abs(a.y - b.y); }

    double dx(){ return std::abs(a.x - b.x); }

    void draw(cv::Mat &frame){ cv::line(frame, a, b, cv::Scalar(0,255,255),2); }

    double length(){ return sqrt(pow(dx(), 2) + std::pow(dy(), 2)); }

    double m(){ return dy()/dx() ; }

    void debug(){ qDebug("a(%d, %d) b(%d, %d) lenght: %f, dx: %f, dy: %f, m: %f",
                            a.x, a.y, b.x, b.y,     length(), dx(), dy(), m()); }

    Line& operator = (Line &other){
        other.a = a;
        other.b = b;
        return other;
    }

    Line& translateY(double delta){
        a.y += delta;
        b.y += delta;
    }

    Line& translateX(double delta){
        a.x += delta;
        b.x += delta;
    }

    void increaseLenght(double l){
        // calculate x
        double current_l = length();
        double alpha = (l+current_l)/ current_l;
        double pente = double(a.y - b.y)/double(a.x - b.x);
        b.x = a.x + (pente > 0 ? (-dx() * alpha): (dx()* alpha));
        b.y = a.y + (pente > 0 ? (-dy() * alpha): (dy() * alpha));
    }
};

#endif // LINE

