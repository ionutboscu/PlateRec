#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <baseapi.h>
#include <allheaders.h>

#include "plate.h"

using namespace std;
using namespace cv;

namespace Ui {
class Dialog;
}

typedef struct
{
    std::string charValue;
    int         pos;
}
CharInfo;
class CharSegment{
public:
    CharSegment();
    CharSegment(Mat i, Rect p);
    Mat img;
    Rect pos;
};

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_btn_clicked();
    Mat toGray(Mat mat);
    Mat toBlur(Mat mat);
    Mat toSobel(Mat mat);
    Mat toThreshold(Mat mat);
    Mat toMorphologyEx(Mat mat);
    vector<RotatedRect> drawBlueContours(Mat mat, Mat threshold, Mat &result);
    vector<Plate> getPlate(Mat mat, Mat result, vector<RotatedRect> rects);
    vector<CharSegment> segmentPlate(Plate plate);
    string saveCharsFromPlate(Plate *input);
    Mat preprocessChar(Mat in);
    bool verifySizes(RotatedRect mr);
    bool verifyCharSizes(Mat r);
    Mat histeq(Mat mat);
    int classify(Mat f);

public slots:
    void processFrameAndUpdate();

private:
    Ui::Dialog *ui;
    cv::VideoCapture capwebcam;

    cv::Mat matOrig;
    cv::Mat matProc;

    QImage qImageOrig;
    QImage qImageProc;

    std::vector<cv::Vec3f> vecCircles;
    std::vector<cv::Vec3f>::iterator itrCircles;
    std::string m_currentPlate;
    std::string m_previousPlate;

    tesseract::TessBaseAPI m_OCR;

    QTimer *timer;
};

#endif // DIALOG_H
