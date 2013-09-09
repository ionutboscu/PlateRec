#-------------------------------------------------
#
# Project created by QtCreator 2013-07-06T17:36:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = zz
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp

HEADERS  += dialog.h

FORMS    += dialog.ui

INCLUDEPATH += /usr/local/include/opencv2 \
              /usr/include/leptonica \
              /usr/local/include/tesseract

LIBS += -L/usr/local/lib \
    -llept -ltesseract \
    -lopencv_ml \
    -lopencv_objdetect \
    -lopencv_stitching \
    -lopencv_imgproc \
    -lopencv_flann \
    -lopencv_videostab \
    -lopencv_nonfree \
    -lopencv_imgproc \
    -lopencv_gpu \
    -lopencv_contrib \
    -lopencv_videostab \
    -lopencv_highgui \
    -lopencv_video \
    -lopencv_stitching \
    -lopencv_legacy \
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_contrib \
    -lopencv_photo \
    -lopencv_imgproc \
    -lopencv_contrib \
    -lopencv_nonfree \
    -lopencv_ts \
    -lopencv_gpu \
    -lopencv_video \
    -lopencv_features2d \
    -lopencv_stitching \
    -lopencv_videostab \
    -lopencv_nonfree \
    -lopencv_objdetect \
    -lopencv_highgui \
    -lopencv_objdetect \
    -lopencv_core \
    -lopencv_flann \
    -lopencv_legacy \
    -lopencv_legacy \
    -lopencv_ml
