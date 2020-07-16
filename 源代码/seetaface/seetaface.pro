#-------------------------------------------------
#
# Project created by QtCreator 2020-03-16T14:40:38
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = seetaface
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    videocapturethread.cpp \
    qdlglogin.cpp \
    g.cpp

HEADERS += \
        mainwindow.h \
    videocapturethread.h \
    qdlglogin.h \
    g.h

FORMS += \
        mainwindow.ui \
    qdlglogin.ui

#windows adm64:

#INCLUDEPATH += C:/thirdparty/opencv4.2/build/include \
#               C:/study/SF3.0/sf3.0_windows/sf3.0_windows/include


#CONFIG(debug, debug|release) {
#LIBS += -LC:/thirdparty/opencv4.2/build/x64/vc14/lib -lopencv_world420d \
#        -LC:/study/SF3.0/sf3.0_windows/sf3.0_windows/lib/x64 -lSeetaFaceDetector600d -lSeetaFaceLandmarker600d \
#        -lSeetaFaceAntiSpoofingX600d -lSeetaFaceTracking600d -lSeetaFaceRecognizer610d \
#        -lSeetaQualityAssessor300d -lSeetaPoseEstimation600d

#} else {
#LIBS += -LC:/thirdparty/opencv4.2/build/x64/vc14/lib -lopencv_world420 \
#        -LC:/study/SF3.0/sf3.0_windows/sf3.0_windows/lib/x64 -lSeetaFaceDetector600 -lSeetaFaceLandmarker600 \
#        -lSeetaFaceAntiSpoofingX600 -lSeetaFaceTracking600 -lSeetaFaceRecognizer610 \
#        -lSeetaQualityAssessor300 -lSeetaPoseEstimation600
#}

#linux:
INCLUDEPATH += /home/chen/opencv/include/opencv \
               /home/chen/SF3/include/seeta/  \
                /home/chen/SF3/include/

LIBS += -L/home/chen/opencv/build/lib -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs \
        -L/home/chen/SF3/lib64 -lSeetaFaceDetector600 -lSeetaFaceLandmarker600 \
        -lSeetaFaceAntiSpoofingX600 -lSeetaFaceTracking600 -lSeetaFaceRecognizer610 \
        -lSeetaQualityAssessor300 -lSeetaPoseEstimation600 -lSeetaAuthorize -ltennis

RESOURCES += \
    face_resource.qrc

