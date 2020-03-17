#ifndef KINECT2_THREAD_H
#define KINECT2_THREAD_H

#include <QThread>
#include <QMutex>
#include <QImage>
#include <QVector3D>
#include "include/Kinect.h"



class Kinect2_Thread : public QThread
{
    Q_OBJECT

public:
    Kinect2_Thread(QObject * parent = 0);
    ~Kinect2_Thread(void);

    void run(void);

    void exitThread(void);
    void getColorImage(QImage& image);
    void getDepthImage(float*& depthImage, int& width, int& height);
    void getBodiesInColorSpace(QVector< QList<QPointF> > &ColorSpacePoints);
    void getCameraSpacePoints(float*& spacePoints, int& width, int& height);
    void getDepthReliableDistance(unsigned short& min, unsigned short& max);
    void getHandState(HandState& leftHandState, HandState& rightHandState);
    void getGeometryOfDepth(int& width, int& height);
    void calculateBodies(bool calculate);
    void calculateSpacePoints(bool calculate);
    bool isCameraOpen(void);

signals:

    void newImage(void);

private:

    QMutex mMutex;
    QImage mImage;
    QVector< QList<CameraSpacePoint> > mJointPoints;
    float* mpDepthImage;
    float* mpSpacePoints;

    IKinectSensor*              mpKinectSensor;
    ICoordinateMapper*          mpCoordinateMapper;
    IMultiSourceFrameReader*    mpMultiSourceFrameReader;
    UINT16*                     mpDepthBuffer;

    RGBQUAD*                    mpColorRGBX;
    RGBQUAD*                    mpDepthRGBX;

    HandState mLeftHandState;
    HandState mRightHandState;

    int mColorWidth;
    int mColorHeight;
    int mDepthWidth;
    int mDepthHeight;
    int mCounter;

    UINT mDepthBufferSize;

    unsigned short mDepthMinReliableDistance;
    unsigned short mDepthMaxReliableDistance;

    bool mNewRGBImage;
    bool mNewDepthImage;
    bool mNewBody;
    bool mCameraIsOpen;
    bool mExit;
    bool mCalculateBodies;
    bool mCalculateSpacePoints;

    HRESULT mHR_Color;
    HRESULT mHR_Depth;

    QPointF BodyToScreen(const CameraSpacePoint& bodyPoint, int width, int height);
};

#endif // KINECT2_THREAD_H
