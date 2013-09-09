#include "dialog.h"
#include "ui_dialog.h"
#include "unistd.h"

#include <QtCore>
#include<iostream>
#include <string.h>
#include <vector>

using namespace std;

static bool DEBUG = false;
static int iteration = 0;
int thresh = 100;
int max_thresh = 255;

Plate::Plate() {
}

Plate::Plate(Mat img, Rect pos){
    plateImg = img;
    position = pos;
}

CharSegment::CharSegment() {
}

CharSegment::CharSegment(Mat i, Rect p){
    img=i;
    pos=p;
}

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{

    ui->setupUi(this);
    capwebcam.open(0);

    if (capwebcam.isOpened() == false) {
        cout << "erorr - cannt load camera image!" << endl;
    }

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(processFrameAndUpdate()));
    timer->start(20);
}

Mat Dialog::toGray(Mat input)
{
    /* Convert image to gray */
    Mat img_gray;
    cvtColor(input, img_gray, CV_BGR2GRAY);
    QImage qImageGray((uchar*)img_gray.data, img_gray.cols, img_gray.rows, img_gray.step, QImage::Format_Indexed8);
    ui->label_2->setPixmap(QPixmap::fromImage(qImageGray));
    return img_gray;
}

Mat Dialog::toBlur(Mat input)
{
    blur(input, input, Size(5,5));
    QImage qImageBlur((uchar*)input.data, input.cols, input.rows, input.step, QImage::Format_Indexed8);
    ui->label_3->setPixmap(QPixmap::fromImage(qImageBlur));
    return input;
}

Mat Dialog::toSobel(Mat input)
{
    /* Finde vertical lines. Car plates have high density of vertical lines */
    Mat img_sobel;
    Sobel(input, img_sobel, CV_8U, 1, 0, 3, 1, 0, BORDER_DEFAULT);
    QImage qImageBlur((uchar*)img_sobel.data, img_sobel.cols, img_sobel.rows, img_sobel.step, QImage::Format_Indexed8);
    ui->label_4->setPixmap(QPixmap::fromImage(qImageBlur));
    return img_sobel;
}

Mat Dialog::toThreshold(Mat input)
{
    /* Threshold image */
    Mat img_threshold;
    threshold(input, img_threshold, 0, 255, CV_THRESH_OTSU+CV_THRESH_BINARY);
    QImage qImageTreshold((uchar*)img_threshold.data, img_threshold.cols, img_threshold.rows, img_threshold.step, QImage::Format_Indexed8);
    ui->label_10->setPixmap(QPixmap::fromImage(qImageTreshold));
    return img_threshold;
}

Mat Dialog::toMorphologyEx(Mat input)
{
    /* Morphplogic operation close */
    Mat element = getStructuringElement(MORPH_RECT, Size(17, 3) );
    morphologyEx(input, input, CV_MOP_CLOSE, element);
    return input;
}

bool Dialog::verifySizes(RotatedRect mr){

    /* car plate size: 52x11 aspect 4,7272
     * Set a min and max area. All other patchs are discarded
     * Get only patchs that match to a respect ratio.
     */
    float error = 0.3;
    float aspect = 4.7272;
    int min = 5 * aspect * 5; // minimum area
    int max = 125 * aspect * 125; // maximum area
    float rmin = aspect - aspect * error;
    float rmax = aspect + aspect * error;

    int area = mr.size.height * mr.size.width;
    float r = (float)mr.size.width / (float)mr.size.height;
    if (r < 1) {
        r = (float)mr.size.height / (float)mr.size.width;
    }
    if (( area < min || area > max ) || ( r < rmin || r > rmax )) {
        return false;
    }
    else {
        return true;
    }
}

vector<RotatedRect> Dialog::drawBlueContours(Mat input, Mat threshold, Mat &result)
{

    Mat canny_output, tmp;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    threshold.copyTo(tmp);
    /* Detect edges using canny */
    Canny(input, canny_output, thresh, thresh*2, 3);
    /* Find contours */
    findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

    /* Draw contours */
    Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3 );
    for (size_t i = 0; i < contours.size(); i++ ) {
         drawContours(drawing, contours, (int)i, cv::Scalar(255, 0, 0), 2, 8, hierarchy, 0, Point() );
       }

    QImage qImageMorphologyEx((uchar*)drawing.data, drawing.cols, drawing.rows, drawing.step, QImage::Format_RGB888);
    ui->label_12->setPixmap(QPixmap::fromImage(qImageMorphologyEx));

    /* Start to iterate to each contour founded */
    vector<vector<Point> >::iterator itc = contours.begin();
    vector<RotatedRect> rects;

    /* Remove patch that are no inside limits of aspect ratio and area. */
    while (itc != contours.end()) {
        /* Create bounding rect of object */
        RotatedRect mr = minAreaRect(Mat(*itc));

        if (!verifySizes(mr)) {
            itc = contours.erase(itc);
        }
        else {
            ++itc;
            rects.push_back(mr);
        }
    }

    input.copyTo(tmp);
    for (size_t i = 0; i< contours.size(); i++ ) {
        drawContours(tmp, contours, (int)i, cv::Scalar(255, 0, 0), 2, 8, hierarchy, 0, Point() );
    }
    QImage qImageBlueContours((uchar*)tmp.data, tmp.cols, tmp.rows, tmp.step, QImage::Format_RGB888);
    ui->label_14->setPixmap(QPixmap::fromImage(qImageBlueContours));
    tmp.copyTo(result);
    return rects;
}

Mat Dialog::histeq(Mat in)
{
    Mat out(in.size(), in.type());

    if (in.channels() == 3) {
        Mat hsv;
        vector<Mat> hsvSplit;
        cvtColor(in, hsv, CV_BGR2HSV);
        split(hsv, hsvSplit);
        equalizeHist(hsvSplit[2], hsvSplit[2]);
        merge(hsvSplit, hsv);
        cvtColor(hsv, out, CV_HSV2BGR);
    }
    else if (in.channels() == 1) {
        equalizeHist(in, out);
    }

    return out;

}

vector<Plate> Dialog::getPlate(Mat input, Mat result, vector<RotatedRect> rects)
{
    vector<Plate> output;

    for (uint i = 0; i < rects.size(); i++) {

        /*For better rect cropping for each posible box
          Make floodfill algorithm because the plate has white background
          And then we can retrieve more clearly the contour box */
        circle(result, rects[i].center, 3, Scalar(0, 255, 0), -1);

        /* get the min size between width and height */
        float minSize = (rects[i].size.width < rects[i].size.height) ? rects[i].size.width : rects[i].size.height;
        minSize = minSize - minSize * 0.5;

        /* initialize rand and get 5 points around center for floodfill algorithm */
        srand (time(NULL));

        /* Initialize floodfill parameters and variables */
        Mat mask;
        mask.create(input.rows + 2, input.cols + 2, CV_8UC1);
        mask = Scalar::all(0);
        int loDiff = 30;
        int upDiff = 30;
        int connectivity = 4;
        int newMaskVal = 255;
        int NumSeeds = 10;
        Rect ccomp;
        int flags = connectivity + (newMaskVal << 8 ) + CV_FLOODFILL_FIXED_RANGE + CV_FLOODFILL_MASK_ONLY;
        for (int j = 0; j < NumSeeds; j++) {
            Point seed;
            seed.x = rects[i].center.x + rand()%(int)minSize - (minSize/2);
            seed.y = rects[i].center.y + rand()%(int)minSize - (minSize/2);
            circle(result, seed, 1, Scalar(0, 255, 255), -1);
            floodFill(input, mask, seed, Scalar(255, 0, 0), &ccomp, Scalar(loDiff, loDiff, loDiff), Scalar(upDiff, upDiff, upDiff), flags);
        }

        /*Check new floodfill mask match for a correct patch.
        Get all points detected for get Minimal rotated Rect */
        vector<Point> pointsInterest;
        Mat_<uchar>::iterator itMask= mask.begin<uchar>();
        Mat_<uchar>::iterator end= mask.end<uchar>();
        for ( ; itMask!=end; ++itMask) {
            if (*itMask == 255) {
                pointsInterest.push_back(itMask.pos());
            }
        }
        RotatedRect minRect = minAreaRect(pointsInterest);
        if (verifySizes(minRect)) {
            /* rotated rectangle drawing */
            Point2f rect_points[4]; minRect.points( rect_points );
            for (int j = 0; j < 4; j++ ) {
                line( result, rect_points[j], rect_points[(j+1)%4], Scalar(255, 0, 0), 1, 8 );
            }
            /* Get rotation matrix */
            float r = (float)minRect.size.width / (float)minRect.size.height;
            float angle = minRect.angle;
            if (r < 1) {
                angle = 90 + angle;
            }
            Mat rotmat = getRotationMatrix2D(minRect.center, angle, 1);
            /* Create and rotate image */
            Mat img_rotated;
            warpAffine(input, img_rotated, rotmat, input.size(), CV_INTER_CUBIC);
            /* Crop image */
            Size rect_size = minRect.size;
            if (r < 1) {
                swap(rect_size.width, rect_size.height);
            }
            Mat img_crop;
            getRectSubPix(img_rotated, rect_size, minRect.center, img_crop);
            Mat resultResized;
            resultResized.create(33, 144, CV_8UC3);
            cv::resize(img_crop, resultResized, resultResized.size(), 0, 0, INTER_CUBIC);

            QImage qImagePlate((uchar*)resultResized.data, resultResized.cols, resultResized.rows, resultResized.step, QImage::Format_RGB888);
            ui->label_16->setPixmap(QPixmap::fromImage(qImagePlate));

            /* Equalize croped image */
            Mat grayResult;
            cvtColor(resultResized, grayResult, CV_BGR2GRAY);
            QImage qImagePlateGray((uchar*)grayResult.data, grayResult.cols, grayResult.rows, grayResult.step, QImage::Format_Indexed8);
            ui->label_18->setPixmap(QPixmap::fromImage(qImagePlateGray));

            blur(grayResult, grayResult, Size(3,3));
            QImage qImagePlateBlur((uchar*)grayResult.data, grayResult.cols, grayResult.rows, grayResult.step, QImage::Format_Indexed8);
            ui->label_20->setPixmap(QPixmap::fromImage(qImagePlateBlur));

            grayResult = histeq(grayResult);
            if (DEBUG) {
                QTime time = QTime::currentTime();
                QString timeString = time.toString();

                stringstream ss(stringstream::in | stringstream::out);
                ss << "tmp/" <<timeString.toStdString() << "_" << i << ".jpg";
                imwrite(ss.str(), grayResult);
            }

            QImage qImagePlateHisteq((uchar*)grayResult.data, grayResult.cols, grayResult.rows, grayResult.step, QImage::Format_Indexed8);
            ui->label_22->setPixmap(QPixmap::fromImage(qImagePlateHisteq));

            output.push_back(Plate(grayResult, minRect.boundingRect()));
        }
    }
    QImage qImagePlateContours((uchar*)result.data, result.cols, result.rows, result.step, QImage::Format_RGB888);
    ui->label_24->setPixmap(QPixmap::fromImage(qImagePlateContours));

    return output;
}

bool Dialog::verifyCharSizes(Mat r)
{
    /* Char sizes 45x77 */
    float aspect = 45.0f/77.0f;
    float charAspect = (float)r.cols/(float)r.rows;
    float error = 0.35;
    float minHeight = 15;
    float maxHeight = 28;

    float minAspect = 0.2;
    float maxAspect = aspect + aspect * error;
    float area = countNonZero(r);
    float bbArea = r.cols * r.rows;
    float percPixels = area/bbArea;

    if (DEBUG) {
        cout << "Aspect: "<< aspect << " ["<< minAspect << "," << maxAspect << "] "  << "Area "<< percPixels <<" Char aspect " << charAspect  << " Height char "<< r.rows << "\n";
    }
    if( percPixels < 0.8 && charAspect > minAspect && charAspect < maxAspect && r.rows >= minHeight && r.rows < maxHeight) {
        return true;
    }
    else {
        return false;
    }
}

vector<CharSegment> Dialog::segmentPlate(Plate plate)
{
    Mat input = plate.plateImg;
    vector<CharSegment> output;

    /* Threshold input image */
    Mat img_threshold;
    threshold(input, img_threshold, 60, 255, CV_THRESH_BINARY_INV);

    Mat img_contours;
    img_threshold.copyTo(img_contours);

    /* Find contours of possibles characters */
    vector< vector< Point> > contours;
    findContours(img_contours,
                 contours, // a vector of contours
                 CV_RETR_EXTERNAL, // retrieve the external contours
                 CV_CHAIN_APPROX_NONE); // all pixels of each contours

    /* Draw blue contours on a white image */
    cv::Mat result;
    img_threshold.copyTo(result);
    cvtColor(result, result, CV_GRAY2RGB);

    if (DEBUG) {
        QTime time = QTime::currentTime();
        QString timeString = time.toString();

        stringstream ss(stringstream::in | stringstream::out);
        ss << "tmp/" <<timeString.toStdString() << "_" << ".jpg";
        imwrite(ss.str(), result);
    }
    cv::drawContours(result,contours,
                     -1, // draw all contours
                     cv::Scalar(255,0,0), // in blue
                     1); // with a thickness of 1

    /* Start to iterate to each contour founded */
    vector<vector<Point> >::iterator itc = contours.begin();

    /* Remove patch that are no inside limits of aspect ratio and area. */
    while (itc != contours.end()) {

        /* Create bounding rect of object */
        Rect mr = boundingRect(Mat(*itc));
        rectangle(result, mr, Scalar(0,255,0));

        /* Crop image */
        Mat auxRoi(img_threshold, mr);
        if (verifyCharSizes(auxRoi)){
            auxRoi=preprocessChar(auxRoi);
            output.push_back(CharSegment(auxRoi, mr));
            rectangle(result, mr, Scalar(0,125,255));
        }
        ++itc;
    }
    return output;
}

Mat Dialog::preprocessChar(Mat in)
{
    /* Remap image */
    int h = in.rows;
    int w = in.cols;
    Mat transformMat = Mat::eye(2,3,CV_32F);
    int m = max(w,h);
    transformMat.at<float>(0,2)=m/2 - w/2;
    transformMat.at<float>(1,2)=m/2 - h/2;

    Mat warpImage(m,m, in.type());
    warpAffine(in, warpImage, transformMat, warpImage.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar(0) );

    Mat out;
    cv::resize(warpImage, out, Size(20, 20));

    return out;
}

string Dialog::saveCharsFromPlate(Plate *input)
{
    /* Segment chars of plate */
    vector<CharSegment> segments = segmentPlate(*input);

    if (segments.size() != 7) {
        cout << "segments size is:" << segments.size();
        return "Cannot recognize the plate";
    }

    tesseract::TessBaseAPI *myOCR = new tesseract::TessBaseAPI();
    if (myOCR->Init(NULL, "eng")) {
        return "Could not initialize tesseract.";
    }

    std::vector<CharInfo> chars;
    std::string output = "";
    for (unsigned int i = 0; i < segments.size(); i++) {
        /* Preprocess each char for all images have same sizes */
        Mat ch = preprocessChar(segments[i].img);
        QTime time = QTime::currentTime();
        QString timeString = time.toString();

        stringstream ss(stringstream::in | stringstream::out);
        ss << "tmpChars/" << timeString.toStdString() << "_" << i << ".jpg";
        imwrite(ss.str(),ch);

        Pix *pix = pixRead(ss.str().c_str());
        myOCR->SetImage(pix);

        CharInfo charInfo;
        std::string val = myOCR->GetUTF8Text();
        charInfo.charValue = val.substr(0, 1);
        charInfo.pos = segments[i].pos.x;
        chars.push_back(charInfo);
        pixDestroy(&pix);

    }
    myOCR->Clear();
    myOCR->End();

    while (chars.size() > 0) {
        int minPos = 100000;
        unsigned int i, pos = 0;
        for ( i = 0; i < chars.size(); i++) {
            if (chars[i].pos < minPos) {
                minPos = chars[i].pos;
                pos = i;
            }
        }
        output.append(chars[pos].charValue);
        chars.erase(chars.begin() + pos);
    }
    if (output.length() == 7) {
        std::string pos4 = output.substr(3,1);
        if (pos4.compare("A") == 0 || pos4.compare("l") == 0 ) {
            output.replace(3,1,"4");
        }
    }
    return output;
}

void Dialog::processFrameAndUpdate()
{
    capwebcam.read(matOrig);
    if (matOrig.empty() == true) return;

    cv::cvtColor(matOrig, matOrig, CV_BGR2RGB);
    QImage qImageOrig((uchar*)matOrig.data, matOrig.cols, matOrig.rows, matOrig.step, QImage::Format_RGB888);
    ui->label->setPixmap(QPixmap::fromImage(qImageOrig));

    Mat tmp = toGray(matOrig);
    tmp = toBlur(tmp);
    tmp = toSobel(tmp);
    tmp = toThreshold(tmp);
    tmp = toMorphologyEx(tmp);
    Mat result;
    vector<RotatedRect> rects = drawBlueContours(matOrig, tmp, result);
    vector<Plate> plates = getPlate(matOrig, result, rects);

    if (iteration++ == 1) {
        iteration = 0;
        for (unsigned int i = 0; i < plates.size(); i++) {
            Plate plate = plates[i];
            std::string number = saveCharsFromPlate(&plate);
            cout << "================================================\n";
            cout << "License plate number: "<< number << "\n";
            cout << "================================================\n";
            m_previousPlate = m_currentPlate;
            m_currentPlate = number;
        }
        if (m_currentPlate.length() == 7 && m_currentPlate == m_previousPlate) {
            ui->label_25->setText(QString(m_currentPlate.c_str()));
        }
    }
}

Dialog::~Dialog()
{
    delete ui;
}
