
#include <QCoreApplication>
#include <QDebug>
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>

#include "paperdectector.h"

using namespace std;
using namespace cv;


int main(int argc, char *argv[])
{    
    PaperDectector paperDect;
    paperDect.start();

    return 0;
}

