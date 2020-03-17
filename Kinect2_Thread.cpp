#include "Kinect2_Thread.h"
#include <stdio.h>
#include <QDebug>
#include <QString>

Kinect2_Thread::Kinect2_Thread(QObject *parent) :
    QThread(parent),
    mpDepthImage(NULL),
    mpSpacePoints(NULL),
    mpKinectSensor(NULL),
    mpCoordinateMapper(NULL),
    mpMultiSourceFrameReader(NULL),
    mpDepthBuffer(NULL),
    mpColorRGBX(NULL),
    mpDepthRGBX(NULL),
    mLeftHandState(HandState_Unknown),
    mRightHandState(HandState_Unknown),
    mColorWidth(1920),
    mColorHeight(1080),
    mDepthWidth(512),
    mDepthHeight(484),
    mCounter(0),
    mDepthBufferSize(0),
    mDepthMinReliableDistance(0),
    mDepthMaxReliableDistance(0),
    mNewRGBImage(false),
    mNewDepthImage(false),
    mNewBody(false),
    mCameraIsOpen(false),
    mExit(false),
    mCalculateBodies(false),
    mCalculateSpacePoints(false)
{
    HRESULT hr = GetDefaultKinectSensor(&mpKinectSensor);

    if(SUCCEEDED(hr) && mpKinectSensor)
    {
        hr = mpKinectSensor->Open();

        if (SUCCEEDED(hr))
        {
            hr = mpKinectSensor->OpenMultiSourceFrameReader(
                 FrameSourceTypes::FrameSourceTypes_Depth |
                 FrameSourceTypes::FrameSourceTypes_Color |
                 FrameSourceTypes::FrameSourceTypes_Body,
                 &mpMultiSourceFrameReader);
        }

        if (SUCCEEDED(hr))
        {
            hr = mpKinectSensor->get_CoordinateMapper(&mpCoordinateMapper);
        }

        if(SUCCEEDED(hr))
        {
            mCameraIsOpen = true;
            qDebug() << "Camera is Open...";
        }
    }

    // create heap storage for color pixel data in RGBX format
    mpColorRGBX = new RGBQUAD[mColorWidth * mColorHeight];
    // create heap storage for depth pixel data in RGBX format
    mpDepthRGBX = new RGBQUAD[mDepthWidth * mDepthHeight];
}

Kinect2_Thread::~Kinect2_Thread()
{
    if(mpDepthImage)
    {
        delete [] mpDepthImage;
        mpDepthImage = NULL;
    }

    if(mpSpacePoints)
    {
        delete[] mpSpacePoints;
        mpSpacePoints = NULL;
    }

    if (mpColorRGBX)
    {
        delete [] mpColorRGBX;
        mpColorRGBX = NULL;
    }

    if (mpDepthRGBX)
    {
        delete [] mpDepthRGBX;
        mpDepthRGBX = NULL;
    }

    if(mpMultiSourceFrameReader)
    {
        mpMultiSourceFrameReader->Release();
        mpMultiSourceFrameReader = NULL;
    }

    if(mpCoordinateMapper)
    {
        mpCoordinateMapper->Release();
        mpCoordinateMapper = NULL;
    }

    if(mpKinectSensor)
    {
        mpKinectSensor->Close();
        mpKinectSensor->Release();
        mpKinectSensor = NULL;
    }
}

void Kinect2_Thread::run()
{
    //---no camera available---
    if(!mCameraIsOpen)
        return;

    if (!mpMultiSourceFrameReader)
        return;

    IMultiSourceFrame* pMultiSourceFrame = NULL;
    IColorFrame* pColorFrame = NULL;
    IDepthFrame* pDepthFrame = NULL;
    IBodyFrame* pBodyFrame   = NULL;
    RGBQUAD* pColorBuffer    = NULL;
    UINT nBufferSize = 0;

    while(!mExit)
    {
        HRESULT hr = mpMultiSourceFrameReader->AcquireLatestFrame(&pMultiSourceFrame);

        //---COLOR---
        if(SUCCEEDED(hr))
        {
            IColorFrameReference* pColorFrameReference = NULL;
            IFrameDescription* pFrameDescription = NULL;
            ColorImageFormat imageFormat = ColorImageFormat_None;
            int width = 0;
            int height = 0;
            nBufferSize = 0;

            mHR_Color = pMultiSourceFrame->get_ColorFrameReference(&pColorFrameReference);

            if (SUCCEEDED(mHR_Color))
            {
                mHR_Color = pColorFrameReference->AcquireFrame(&pColorFrame);
            }

            if (SUCCEEDED(mHR_Color))
            {
                mHR_Color = pColorFrame->get_FrameDescription(&pFrameDescription);
                pFrameDescription->get_Width(&width);
                pFrameDescription->get_Height(&height);
            }

            if (SUCCEEDED(mHR_Color))
            {
                mHR_Color = pColorFrame->get_RawColorImageFormat(&imageFormat);

                if (imageFormat == ColorImageFormat_Rgba)
                {
                    mHR_Color = pColorFrame->AccessRawUnderlyingBuffer(&nBufferSize, reinterpret_cast<BYTE**>(&pColorBuffer));
                    if (SUCCEEDED(mHR_Color))
                    {
                        mMutex.lock();
                        mImage = QImage(reinterpret_cast<BYTE*>(pColorBuffer),width,height,QImage::Format_RGBA8888);
                        mMutex.unlock();
                    }
                }
                else if (mpColorRGBX)
                {
                    pColorBuffer = mpColorRGBX;
                    nBufferSize = mColorWidth * mColorHeight * sizeof(RGBQUAD);
                    mHR_Color = pColorFrame->CopyConvertedFrameDataToArray(nBufferSize, reinterpret_cast<BYTE*>(pColorBuffer), ColorImageFormat_Rgba);

                    if (SUCCEEDED(mHR_Color))
                    {
                        // Make sure we've received valid data
                        if (pColorBuffer && (width == mColorWidth) && (height == mColorHeight))
                        {
                            mMutex.lock();
                            mImage = QImage(reinterpret_cast<BYTE*>(pColorBuffer),mColorWidth,mColorHeight,QImage::Format_RGBA8888).copy();
                            mMutex.unlock();
                        }
                    }
                }
                else
                {
                    mHR_Color = E_FAIL;
                }
            }

            //---free buffer---
            if(pColorFrameReference!=NULL)
            {
                pColorFrameReference->Release();
                pColorFrameReference = NULL;
            }
            if(pFrameDescription!=NULL)
            {
                pFrameDescription->Release();
                pFrameDescription = NULL;
            }
            if(pColorFrame!=NULL)
            {
                pColorFrame->Release();
                pColorFrame=NULL;
            }
        }

        //---DEPTH---
        if(SUCCEEDED(hr))
        {
            IDepthFrameReference* pDepthFrameReference = NULL;
            IFrameDescription* pFrameDescription = NULL;
            mDepthBufferSize = 0;
            mDepthMinReliableDistance = 0;
            mDepthMaxReliableDistance = 0;

            mHR_Depth = pMultiSourceFrame->get_DepthFrameReference(&pDepthFrameReference);
            if (SUCCEEDED(mHR_Depth))
            {
                mHR_Depth = pDepthFrameReference->AcquireFrame(&pDepthFrame);
            }

            if (SUCCEEDED(mHR_Depth))
            {
                mMutex.lock();
                mHR_Depth = pDepthFrame->get_FrameDescription(&pFrameDescription);
                pFrameDescription->get_Width(&mDepthWidth);
                pFrameDescription->get_Height(&mDepthHeight);
                pDepthFrame->get_DepthMinReliableDistance(&mDepthMinReliableDistance);
                pDepthFrame->get_DepthMaxReliableDistance(&mDepthMaxReliableDistance);
                mHR_Depth = pDepthFrame->AccessUnderlyingBuffer(&mDepthBufferSize, &mpDepthBuffer);

                if(SUCCEEDED(mHR_Depth))
                {
                    if(mpDepthImage)
                        delete[] mpDepthImage;
                    mpDepthImage = new float[mDepthBufferSize];
                    if(mpDepthBuffer)
                    {
                        for(UINT i=0; i<mDepthBufferSize; i++)
                        {
                            //---convert in meter---
                            //---depth values are in millimeter---
                            mpDepthImage[i] = (float)(mpDepthBuffer[i] & 0xffff)*0.001;
                        }
                    }
                }
                mMutex.unlock();
            }

            //---free buffer---
            if(pDepthFrameReference!=NULL)
            {
                pDepthFrameReference->Release();
                pDepthFrameReference = NULL;
            }
            if(pFrameDescription!=NULL)
            {
                pFrameDescription->Release();
                pFrameDescription = NULL;
            }
            if(pDepthFrame!=NULL)
            {
                pDepthFrame->Release();
                pDepthFrame = NULL;
            }

            if(SUCCEEDED(mHR_Color) && SUCCEEDED(mHR_Depth))
            {
                emit newImage();
            }
        }

        //---Bodies---
        if(mCalculateBodies && SUCCEEDED(hr))
        {
            IBodyFrameReference* pBodyFrameReference = NULL;
            HRESULT hr_body  = pMultiSourceFrame->get_BodyFrameReference(&pBodyFrameReference);

            if (SUCCEEDED(hr_body))
            {
                hr_body = pBodyFrameReference->AcquireFrame(&pBodyFrame);
            }

            if(SUCCEEDED(hr_body))
            {
                IBody* ppBodies[BODY_COUNT] = {0};
                hr_body = pBodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);

                if (SUCCEEDED(hr_body))
                {
                    mMutex.lock();
                    mJointPoints.clear();
                    for (int i = 0; i < BODY_COUNT; ++i)
                    {
                        IBody* pBody = ppBodies[i];
                        if (pBody)
                        {
                            BOOLEAN bTracked = false;
                            hr_body = pBody->get_IsTracked(&bTracked);

                            if (SUCCEEDED(hr_body) && bTracked)
                            {
                                Joint joints[JointType_Count];
                                QList<CameraSpacePoint> jointPoints;

                                mLeftHandState = HandState_Unknown;
                                mRightHandState = HandState_Unknown;

                                pBody->get_HandLeftState(&mLeftHandState);
                                pBody->get_HandRightState(&mRightHandState);

                                hr_body = pBody->GetJoints(_countof(joints), joints);
                                if (SUCCEEDED(hr_body))
                                {
                                    for (int j = 0; j < _countof(joints); ++j)
                                    {
                                        jointPoints.append(joints[j].Position);
                                    }
                                    mJointPoints.append(jointPoints);
                                }
                            }
                        }
                    }
                    mMutex.unlock();
                }

                for (int i = 0; i < _countof(ppBodies); ++i)
                {
                    if(ppBodies[i])
                    {
                        ppBodies[i]->Release();
                        ppBodies[i] = NULL;
                    }
                }

                if(pBodyFrame)
                {
                    pBodyFrame->Release();
                    pBodyFrame = NULL;
                }
            }

            //---free buffer---
            if(pBodyFrameReference!=NULL)
            {
                pBodyFrameReference->Release();
                pBodyFrameReference = NULL;
            }
        }

        //---SpacePoints---
        if(mCalculateSpacePoints && SUCCEEDED(mHR_Color) && SUCCEEDED(mHR_Depth) && mDepthBufferSize!=0 && mpDepthBuffer)
        {
            mMutex.lock();
            int width = mImage.width();
            int height = mImage.height();
            int size = width*height;

            if(mpSpacePoints)
            {
                delete[] mpSpacePoints;
                mpSpacePoints = NULL;
            }
            mpSpacePoints = new float[size*3];
            memset(mpSpacePoints,0,size*3*sizeof(float));
            CameraSpacePoint* CameraSpacePoints = new CameraSpacePoint[size];

            HRESULT hr_Space = mpCoordinateMapper->MapColorFrameToCameraSpace(mDepthBufferSize,mpDepthBuffer,/*size*/mDepthBufferSize,CameraSpacePoints);

            if(SUCCEEDED(hr_Space))
            {
                for(int i=0; i<size; i++)
                {
                    mpSpacePoints[i*3]   = CameraSpacePoints[i].X;
                    mpSpacePoints[i*3+1] = CameraSpacePoints[i].Y;
                    mpSpacePoints[i*3+2] = CameraSpacePoints[i].Z;
                }
            }

            delete[] CameraSpacePoints;
            mMutex.unlock();
        }

        //---free buffer---
        if(pMultiSourceFrame!=NULL)
        {
           pMultiSourceFrame->Release();
           pMultiSourceFrame = NULL;
        }
    }
}

void Kinect2_Thread::exitThread()
{
    mMutex.lock();
    mExit = true;
    mMutex.unlock();
}

void Kinect2_Thread::getColorImage(QImage &image)
{
    mMutex.lock();
    image = mImage.copy().mirrored(true, false);
    mMutex.unlock();
}

void Kinect2_Thread::getDepthImage(float*& depthImage, int &width, int &height)
{
    mMutex.lock();
    width = mDepthWidth;
    height = mDepthHeight;
    int size = width*height;

    if(mpDepthImage && size!=0)
    {
        depthImage = new float[size];
        // not valid: memcpy(depthImage,mpDepthImage,size*sizeof(float));
        // horizontally flip depth image
        for (int y=0; y<height; y++)
        {
            for (int x=0; x<width; x++)
            {
                depthImage[x+y*width] = mpDepthImage[(width-x)+y*width];
            }
        }
    }
    else
        depthImage = NULL;
    mMutex.unlock();
}

void Kinect2_Thread::getGeometryOfDepth(int &width, int &height)
{
    mMutex.lock();
    width = mDepthWidth;
    height = mDepthHeight;
    mMutex.unlock();
}

void Kinect2_Thread::getCameraSpacePoints(float *&spacePoints, int &width, int &height)
{
    mMutex.lock();
    width = mImage.width();
    height = mImage.height();
    int size = width*height;

    if(mpSpacePoints && size!=0)
    {
        spacePoints = new float[size*3];
        memcpy(spacePoints,mpSpacePoints,3*size*sizeof(float));
    }
    else
        spacePoints = NULL;
    mMutex.unlock();
}

void Kinect2_Thread::getBodiesInColorSpace(QVector<QList<QPointF> > &ColorSpacePoints)
{
    if(mCalculateBodies==false)
        return;

    mMutex.lock();
    for(int i=0; i<mJointPoints.size(); i++)
    {
        QList<QPointF> ListOfColorSpacePoints;
        for(int j=0;j<mJointPoints[i].size();j++)
        {
            QPointF MyColorSpacePoint;
            ColorSpacePoint colorPoint = {0};
            mpCoordinateMapper->MapCameraPointToColorSpace(mJointPoints[i][j], &colorPoint);
            MyColorSpacePoint.setX(colorPoint.X);
            MyColorSpacePoint.setY(colorPoint.Y);
            ListOfColorSpacePoints.append(MyColorSpacePoint);
        }
        ColorSpacePoints.append(ListOfColorSpacePoints);
    }
    mMutex.unlock();
}

void Kinect2_Thread::getHandState(HandState &leftHandState, HandState &rightHandState)
{
    mMutex.lock();
    leftHandState = mLeftHandState;
    rightHandState = mRightHandState;
    mMutex.unlock();
}

void Kinect2_Thread::calculateBodies(bool calculate)
{
    mMutex.lock();
    mCalculateBodies = calculate;
    mMutex.unlock();
}

void Kinect2_Thread::calculateSpacePoints(bool calculate)
{
    mMutex.lock();
    mCalculateSpacePoints = calculate;
    mMutex.unlock();
}

void Kinect2_Thread::getDepthReliableDistance(unsigned short &min, unsigned short &max)
{
    mMutex.lock();
    min = mDepthMinReliableDistance;
    max = mDepthMaxReliableDistance;
    mMutex.unlock();
}

bool Kinect2_Thread::isCameraOpen()
{
    return mCameraIsOpen;
}
