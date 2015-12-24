
#include <QCoreApplication>
#include <QDebug>
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>

//#include "paperdectector.h"
//#include "stylusdetect.h"
#include "infrareddetect.h"

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{    
//    PaperDectector paperDect("/Users/paulnta/Documents/HEIG4/PDG/tests/led.mov");
//    paperDect.start();

    InfraredDetect* irDetect = InfraredDetect::getInstance();
//    irDetect->setSource("/Users/paulnta/Downloads/IMG_1907.MOV");
    irDetect->setSource(nullptr);
    irDetect->start();

//    StylusDetect stylusDetect("/Users/paulnta/Documents/HEIG4/PDG/tests/led.mov");
//    stylusDetect.start();

//    return 0;
}

