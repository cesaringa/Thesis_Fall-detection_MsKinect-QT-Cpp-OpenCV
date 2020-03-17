#ifndef RATIONALONESHOT_H
#define RATIONALONESHOT_H

#include <QWidget>
#include <QGraphicsScene>
#include <QTimer>

#include "Kinect2_Thread.h"


namespace Ui {
class Kinect2Images;
}

class Kinect2Images : public QWidget
{
    Q_OBJECT

public:
    explicit Kinect2Images(QWidget *parent = 0);
    ~Kinect2Images();

private:
    Ui::Kinect2Images *ui;

    Kinect2_Thread* mpThread;

    QGraphicsScene mSceneColor;
    QGraphicsScene mSceneDepth;

    QTimer mTimer;

    QString mPath;


    bool mRecord;

    void convert_hsl_2_rgb( unsigned char* data, int x, int y, int p );
    double Hue_2_RGB( double v1, double v2, double vH );

private slots:

    void getImage(void);
    void shot(void);
    void setPath(void);
};

#endif // RATIONALONESHOT_H
