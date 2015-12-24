#ifndef PAPERDECTECTOR_H
#define PAPERDECTECTOR_H

#include "videocontroller.h"
#include <vector>


class PaperDectector : public VideoController
{
public:
    PaperDectector();

signals:

public slots:

    // VideoController interface
protected:
    virtual void handleFrame(Mat &frame);

private:

/*
 * The color to detect has to ben within range low and hight
 *
 * HUE          [0-179]  represents the color
 * SATURATION   [0-255]  represents the amount to which that respective color is mixed with white
 * VALUE        [0-255]  represents the  amount to which that respective color is mixed with black.
 *
 * SATURATION and VALUE may be vary according to the lighting condition of that environment.
 */
    int lowH;  // low HUE
    int lowS;  // low SATURATION
    int lowV;  // high VALUE

    int highH; // high HUE
    int highS; // high SATURATION
    int highV; // high VALUE

    int lastX;
    int lastY;
};

#endif // PAPERDECTECTOR_H
