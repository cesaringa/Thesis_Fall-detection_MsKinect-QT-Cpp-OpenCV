///////////////////////////////////////////////////////////////////////
// Fall Detection with MS Kinect
// Work in collaboration with SRH Hochschule Heidelberg
// Update May-2019: Work is still in progress
///////////////////////////////////////////////////////////////////////

#include "Kinect2Images.h"
#include <QApplication>

#include <opencv2/viz.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>
using namespace cv;
using namespace std;

static void keyboardCallback( const cv::viz::KeyboardEvent& event, void* cookie );

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Kinect2Images w;
    w.show();
    return a.exec();
}

