#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "plate.h"

using namespace std;
using namespace cv;

namespace Ui {
class Dialog;
}

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
    bool verifySizes(RotatedRect mr);
    Mat histeq(Mat mat);

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

    QTimer *timer;
};

#endif // DIALOG_H
