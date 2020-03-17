///////////////////////////////////////////////////////////////////////
// Point Cloud buffer (Viz3D) to be used in order to show images in 3D.
// This is a demo to show how VIZ works
///////////////////////////////////////////////////////////////////////

#include <opencv2/viz.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>

static void keyboardCallback( const cv::viz::KeyboardEvent& event, void* cookie );
static void viz3d_dem();

using namespace cv;
using namespace std;

// Point Cloud Buffer
    cv::viz::Viz3d viewer;
    cv::Mat cloudMat;

void keyboardCallback( const cv::viz::KeyboardEvent& event, void* cookie )
{
    // Exit Viewer when Pressed ESC key
}

void viz3d_dem()
{
    ///Drawing a frame for the image
   viz::Viz3d viewer("3D Image");                              // Creating a 3D window "Viewer"
    viewer.showWidget("Coordinates", viz::WCoordinateSystem());    // Showing the created window at the system coordinates

    /// Drawing a cube
    viz::WCube cube_widget(Point3f(0.5,0.5,0.0), Point3f(0.0,0.0,-0.5), false, viz::Color::blue()); //
    cube_widget.setRenderingProperty(viz::LINE_WIDTH, 2.0);
    viewer.showWidget("Cube Widget", cube_widget);

    Mat rot_vec = Mat::zeros(1,3,CV_32F);
       float translation_phase = 0.0, translation = 0.0;
       while(!viewer.wasStopped())
       {
           /* Rotation using rodrigues */
           rot_vec.at<float>(0,0) += (float)CV_PI * 0.01f;
           rot_vec.at<float>(0,1) += (float)CV_PI * 0.01f;
           rot_vec.at<float>(0,2) += (float)CV_PI * 0.01f;
           translation_phase += (float)CV_PI * 0.01f;
           translation = sin(translation_phase);
           Mat rot_mat;
           Rodrigues(rot_vec, rot_mat);
           Affine3f pose(rot_mat, Vec3f(translation, translation, translation));
           viewer.setWidgetPose("Cube Widget", pose);
           viewer.spinOnce(1, true);
       }
}
