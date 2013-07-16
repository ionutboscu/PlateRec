#ifndef PLATE_H
#define PLATE_H

#include <string.h>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cvaux.h>
#include <opencv/cv.h>

using namespace std;
using namespace cv;

class Plate{
    public:
        Plate();
        Plate(Mat img, Rect pos);
        Rect position;
        Mat plateImg;
};

#endif // PLATE_H
