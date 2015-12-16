
#include <QCoreApplication>
#include <QDebug>
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>

#include "paperdectector.h"
#include "stylusdetect.h"

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{    
//    PaperDectector paperDect("/Users/paulnta/Movies/hand.mov");
//    paperDect.start();

    StylusDetect stylusDetect("/Users/paulnta/Documents/HEIG4/PDG/videos/test1.m4v");
    stylusDetect.start();

//    return 0;
}

