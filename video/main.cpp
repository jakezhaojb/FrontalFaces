#include "opencv2/opencv.hpp"

using namespace cv;

int main()
{
  VideoCapture cap;
  cap.open("/home/junbo/UndergradThesis/FrontalFace/video/ipart_head_video/img%04d.png"); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
    {std::cout << "NO" << std::endl;
        return -1;}

    namedWindow("edges",1);
    int i=0;
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        imshow("edges", frame);
        ++i;
        if (waitKey(3) == 0) {
          break;
        }
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
