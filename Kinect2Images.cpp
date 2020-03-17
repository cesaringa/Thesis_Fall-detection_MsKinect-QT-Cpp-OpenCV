#include "Kinect2Images.h"
#include "ui_Kinect2Images.h"

#include <QFileDialog>
#include <QDebug>
#include <QDateTime>

#include <opencv2/viz.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Kinect2Images::Kinect2Images(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Kinect2Images),
    mpThread(NULL),
    mRecord(false)
{
    ui->setupUi(this);

    ui->gv_colorImage->setScene(&mSceneColor);
    ui->gv_depthImage->setScene(&mSceneDepth);

    mpThread = new Kinect2_Thread();
    mpThread->start();

    QObject::connect(mpThread,SIGNAL(newImage()),this,SLOT(getImage()));
    QObject::connect(ui->pb_path,SIGNAL(pressed()),this,SLOT(setPath()));
    QObject::connect(ui->pb_shot,SIGNAL(pressed()),this,SLOT(shot()));
}

Kinect2Images::~Kinect2Images()
{
    //---Exit Thread---
    if(mpThread!=NULL)
    {
        mpThread->exitThread();
        mpThread->deleteLater();
        delete mpThread;
        mpThread = NULL;
    }

    delete ui;
}

void Kinect2Images::getImage()
{
    if(mpThread->isCameraOpen() && mpThread->isRunning())
    {
        //---color image---
        QImage image;
        mpThread->getColorImage(image);

        mSceneColor.clear();
        mSceneColor.addPixmap(QPixmap::fromImage(image));

        ui->gv_colorImage->fitInView(mSceneColor.sceneRect(),Qt::KeepAspectRatio);

        //---depth image---
        float* pDepthImage;
        int width = 0;
        int height = 0;
        mpThread->getDepthImage(pDepthImage,width,height);

        if(pDepthImage==NULL)
            return;

        unsigned char* pDepthGreyImage  = new unsigned char[width*height];
        unsigned char* pDepthColorImage = new unsigned char[width*height*3];
        int size = width*height;

        // plot color depth image or gray scale image
#define PLOTOPT 2 // 0: do not plot depth image, 1: plot color depth image, 2: plot grey depth image
#if PLOTOPT == 1
        for(int i = 0; i < size; i++)
        {
          if(pDepthGreyImage[i] <= 0)
          {
            pDepthColorImage[i] = 0;
            pDepthColorImage[i + size] = 0;
            pDepthColorImage[i + size * 2] = 0;
          }
          else
          {
            pDepthColorImage[i] = (unsigned char)((double)(pDepthGreyImage[i]) / 255.0 * 170.0);
            pDepthColorImage[i + size] = 255;
            pDepthColorImage[i + size * 2] = 128;
          }
        }
//        convert_hsl_2_rgb(pDepthColorImage, width, height, 3);

        QImage depthImage(width,height,QImage::Format_RGB888);
        unsigned char* data = depthImage.bits();
        for(int i=0; i < size; i++)
        {
            data[i*3]   = pDepthColorImage[i];
            data[i*3+1] = pDepthColorImage[i+size];
            data[i*3+2] = pDepthColorImage[i+size*2];
        }
#elif PLOTOPT == 2
        float minVal = 9999999;
        float maxVal = 0;

        for(int i = 0; i < size; i++)
        {
          if(pDepthImage[i] > 0.0)
          {
            if(pDepthImage[i] > maxVal)
            {
              maxVal = pDepthImage[i];
            }
            if(pDepthImage[i] < minVal)
            {
              minVal = pDepthImage[i];
            }
          }
        }
        for(int i = 0; i < size; i++)
        {
          if(pDepthImage[i] > 0.0)
          {
            pDepthGreyImage[i] = (unsigned char)((pDepthImage[i] - minVal) / (maxVal - minVal) * 255.0);
          }
          else
          {
            pDepthGreyImage[i] = 0;
          }
        }

        QImage depthImage(width,height,QImage::Format_Indexed8);
        unsigned char* data = depthImage.bits();
        for(int i=0; i < size; i++)
        {
            data[i]   = pDepthGreyImage[i];
        }

        //---Show image in the application---
        mSceneDepth.clear();
        mSceneDepth.addPixmap(QPixmap::fromImage(depthImage));
        ui->gv_depthImage->fitInView(mSceneDepth.sceneRect(), Qt::KeepAspectRatio);

       ///Drawing a frame for the image
        viz::Viz3d myWindow("MY WINDOW");
        myWindow.showWidget("COORDINATES Widget", viz::WCoordinateSystem());
        myWindow.spin();


    /*    // Point Cloud Buffer
       // cv::viz::Viz3d viewer;
        Mat cloudMat;

        //Create cv::Mat from Coordinate Buffer
         cloudMat = Mat( height, width, CV_CN_MAX);

        // Create Point Cloud
        cv::viz::WCloud cloud( cloudMat, viz::Color::green() );

        // Show Point Cloud
        myWindow.showWidget( "Cloud", cloud );
        myWindow.spinOnce();*/


#endif



        //---save images---
        if(mRecord)
        {
            QString path = ui->le_path->text();

            QString lightsource = ui->lineEdit_lightsource->text();
            QString lightsourceStr;
            if (lightsource.size() > 0)
            {
                lightsourceStr = lightsource + "_";
            }
            else
            {
                lightsourceStr.clear();
            }


            QString label = ui->lineEdit_label->text();
            QString labelStr;
            if (label.size() > 0)
            {
                labelStr = label + "_";
            }
            else
            {
                labelStr.clear();
            }

            QString slot = ui->lineEdit_slot->text();
            QString slotStr;
            if (slot.size() > 0)
            {
                slotStr = slot + "_";
            }
            else
            {
                slotStr.clear();
            }


            quint64 timestamp = QDateTime::currentMSecsSinceEpoch();

            QString basefilename;
            basefilename = "/" + lightsourceStr + labelStr + slotStr + QString::number(timestamp);

            QString filePathDepth = path + basefilename + "_Depth";
            QString filePathRGB = path + basefilename + "_RGB.bmp";
            qDebug() << filePathDepth << filePathRGB;

            //---DepthImage---
            QFile depthFile(filePathDepth);
            depthFile.open( QIODevice::WriteOnly);
            QDataStream out(&depthFile);

            //--- damit nur 32 bit floats geschrieben werden ---
            out.setFloatingPointPrecision(QDataStream::SinglePrecision);

            // write image size
            out << width;
            out << height;

            for(int i = 0; i < size; i++)
            {
                out << pDepthImage[i];
            }
            depthFile.close();

            //---RGBimage---
            image.save(filePathRGB,"BMP",100);

            //ui->lb_status->setText("Last image : " + path + "/DepthImage_" + QString::number(timestamp));
            ui->lb_status->setText("Last image : " + filePathRGB);


            mRecord=false;
        }



        delete[] pDepthColorImage;
        delete[] pDepthGreyImage;
        delete[] pDepthImage;
    }
}

void Kinect2Images::convert_hsl_2_rgb( unsigned char* data, int x, int y, int p ) {

  if ( p != 3 ) return;

  double R = 0.0;
  double G = 0.0;
  double B = 0.0;

  int size = x*y;

  unsigned char* h = &data[0];
  unsigned char* s = &data[size];
  unsigned char* l = &data[2*size];

  for ( int i = 0; i < size; i++ ) {

    double var_H = (double) h[i] / 255.0;
    double var_S = (double) s[i] / 255.0;
    double var_L = (double) l[i] / 255.0;

    if ( var_S == 0.0 ) {
      R = var_L * 255.0;
      G = var_L * 255.0;
      B = var_L * 255.0;
    } else {
      double var1, var2;
      if ( var_L < 0.5 ) {
        var2 = var_L * ( 1.0 + var_S );
      } else {
        var2 = ( var_L + var_S ) - ( var_S * var_L );
      }

      var1 = 2.0 * var_L - var2;

      R = 255.0 * Hue_2_RGB ( var1, var2, var_H + ( 1.0 / 3.0 ) );
      G = 255.0 * Hue_2_RGB ( var1, var2, var_H );
      B = 255.0 * Hue_2_RGB ( var1, var2, var_H - ( 1.0 / 3.0 ) );

    }

    h[i] = R;
    s[i] = G;
    l[i] = B;
  }
}

double Kinect2Images::Hue_2_RGB( double v1, double v2, double vH ) {

  if ( vH < 0.0 ) vH += 1.0;
  if ( vH > 1.0 ) vH -= 1.0;
  if ( ( 6.0 * vH ) < 1.0 ) return ( v1 + ( v2 - v1 ) * 6.0 * vH );
  if ( ( 2.0 * vH ) < 1.0 ) return ( v2 );
  if ( ( 3.0 * vH ) < 2.0 ) return ( v1 + ( v2 - v1 ) * ( ( 2.0 / 3.0 ) - vH ) * 6.0 );
  return ( v1 );
}

void Kinect2Images::setPath()
{
    QString Dir =  QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                   "/home",
                   QFileDialog::ShowDirsOnly
                   | QFileDialog::DontResolveSymlinks);

    QDir MyDir(Dir);
    if(!MyDir.exists())
    {
      return;
    }

    ui->le_path->setText(Dir);
}

void Kinect2Images::shot()
{
    mRecord = true;
}
