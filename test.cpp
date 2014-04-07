/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 2012 Michal Uricar
 * Copyright (C) 2012 Michal Uricar
 */

#include <opencv/cv.h>
#include <opencv/cvaux.h>
#include <opencv/highgui.h>

#include <cstring>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <dirent.h>

#include "flandmark_detector.h"

// global variable
int front_face_num = 1;
FILE *fp;

IplImage* getCameraFrame(CvCapture* &camera, const char *filename = 0, int camid=0, int width=320, int height=240)
{
    IplImage *frame = 0;
    int w, h;

    // If the camera hasn't been initialized, then open it.
    if (!camera)
    {
        if (!filename)
        {
            printf("Acessing the camera ...\n");
            camera = cvCaptureFromCAM(camid);
            // Try to set the camera resolution.
            cvSetCaptureProperty(camera, CV_CAP_PROP_FRAME_WIDTH, width);
            cvSetCaptureProperty(camera, CV_CAP_PROP_FRAME_HEIGHT, height);
        } else {
            printf("Acessing the video sequence ...\n");
            camera = cvCaptureFromAVI(filename);
        }

        if (!camera)
        {
            printf("Couldn't access the camera.\n");
            exit(1);
        }

        // Get the first frame, to make sure the camera is initialized.
        frame = cvQueryFrame( camera );
        //frame = cvRetrieveFrame(camera);

        if (frame)
        {
            w = frame->width;
            h = frame->height;
            printf("Got the camera at %dx%d resolution.\n", w, h);
        }
        //sleep(10);
    }

    // Wait until the next camera frame is ready, then grab it.
    frame = cvQueryFrame( camera );
    //frame = cvRetrieveFrame(camera);
    if (!frame)
    {
        printf("Couldn't grab a camera frame.\n");
        return frame;
    }
    return frame;
}


bool judgeFrontFace(double *landmarks){

  // pre-requisite
  if(int(landmarks[0]) == 0){
    return false;
  }

  // For eyes and mouth
  if (abs(landmarks[3] - landmarks[11]) > 2.5 || abs(landmarks[5] - landmarks[13]) > 2.5 ||\
      abs(landmarks[3] - landmarks[5]) > 2.5  || abs(landmarks[7] - landmarks[9]) > 2.5 ||\
      abs(landmarks[6] - landmarks[10] - landmarks[12] + landmarks[8]) > 4.5)
    return false;
  // for nose and center
  if (abs(landmarks[0] - landmarks[14]) > 2.5 || abs(landmarks[1] - landmarks[15]) > 3.5)
    return false;

  return true;
}


void detectFaceInImage(IplImage *orig, IplImage* input, CvHaarClassifierCascade* cascade, FLANDMARK_Model *model, int *bbox, double *landmarks, int frame_num)
{
    // Smallest face size.
    CvSize minFeatureSize = cvSize(150, 150);
    int flags =  CV_HAAR_DO_CANNY_PRUNING;
    // How detailed should the search be.
    float search_scale_factor = 1.1f;
    CvMemStorage* storage;
    CvSeq* rects;
    int nFaces;
    char front_img_file_name[32];
    IplImage* regular_face = cvCreateImage(cvSize(50, 50), 8, 3);

    storage = cvCreateMemStorage(0);
    cvClearMemStorage(storage);

    // Detect all the faces in the greyscale image.
    rects = cvHaarDetectObjects(input, cascade, storage, search_scale_factor, 2, flags, minFeatureSize);
    nFaces = rects->total;

    double t = (double)cvGetTickCount();
    for (int iface = 0; iface < (rects ? nFaces : 0); ++iface)
    {
        CvRect *r = (CvRect*)cvGetSeqElem(rects, iface);

        bbox[0] = r->x;
        bbox[1] = r->y;
        bbox[2] = r->x + r->width;
        bbox[3] = r->y + r->height;

        //if (r->width < 150 || r->height < 150) {
        //  continue;
        //}

        flandmark_detect(input, bbox, model, landmarks);

        // display landmarks
        cvRectangle(orig, cvPoint(bbox[0], bbox[1]), cvPoint(bbox[2], bbox[3]), CV_RGB(255,0,0) );
        cvRectangle(orig, cvPoint(model->bb[0], model->bb[1]), cvPoint(model->bb[2], model->bb[3]), CV_RGB(0,0,255) );
        cvCircle(orig, cvPoint((int)landmarks[0], (int)landmarks[1]), 3, CV_RGB(0, 0,255), CV_FILLED);
        for (int i = 2; i < 2*model->data.options.M; i += 2)
        {
            cvCircle(orig, cvPoint(int(landmarks[i]), int(landmarks[i+1])), 3, CV_RGB(255,0,0), CV_FILLED);

        }
        if( judgeFrontFace(landmarks)){
          sprintf(front_img_file_name, "FrontFace//%d_%d_%d.jpg", frame_num, iface + 1, front_face_num);
          // DEBUG!
          fp = fopen("FrontFace//records.txt", "at");
          fprintf(fp, "FrontFace//%d_%d_%d.jpg is captured, and the following is the points:\n", frame_num, iface + 1, front_face_num);
          //for (int i_debug = 0; i_debug < 15; i_debug += 2) {
          //  fprintf(fp, "%f, %f\n", landmarks[i_debug], landmarks[i_debug+1]);
          //}
          fprintf(fp, "\tface center:%f, %f\n", landmarks[0], landmarks[1]);
          fprintf(fp, "\tleft L-eye :%f, %f\n", landmarks[10], landmarks[11]);
          fprintf(fp, "\tright L-eye:%f, %f\n", landmarks[2], landmarks[3]);
          fprintf(fp, "\tleft R-eye :%f, %f\n", landmarks[4], landmarks[5]);
          fprintf(fp, "\tright R-eye:%f, %f\n", landmarks[12], landmarks[13]);
          fprintf(fp, "\tnose       :%f, %f\n", landmarks[14], landmarks[15]);
          fprintf(fp, "\tleft mouth :%f, %f\n", landmarks[6], landmarks[7]);
          fprintf(fp, "\tright mouth:%f, %f\n", landmarks[8], landmarks[9]);
          fprintf(fp, "\n");
          fclose(fp);
          // DEBUG ends.
          //front_img_file_name = "FrontFace//" + ::itoa(front_face_num) + ".jpg" ;
          cvSetImageROI(orig, *r); // cvRect(r->x-5, r->y-60, r->width+20, r->height+85));
          cvResize(orig, regular_face);
          cvSaveImage(front_img_file_name, orig);
          cvResetImageROI(orig);
          front_face_num++;
        }
    }

    t = (double)cvGetTickCount() - t;
    int ms = cvRound( t / ((double)cvGetTickFrequency() * 1000.0) );

    if (nFaces > 0)
    {
        //printf("Faces detected: %d; Detection of facial landmark on all faces took %d ms\n", nFaces, ms);
    } else {
        //printf("NO Face\n");
    }

    cvReleaseMemStorage(&storage);
}


int main( int argc, char** argv )
{
    char flandmark_window[] = "flandmark_example2";
    double t;
    int ms, frame_num = 0;

    bool video = false, savevideo = false;

    CvVideoWriter *writer = 0;
    int vidfps, frameW, frameH, fourcc, nframes = 0;
    //int fourcc = CV_FOURCC('D', 'I', 'V', 'X');

    // Directory saving front faces
    DIR *pDir;
    pDir = opendir("FrontFace");
    if (pDir != NULL)
      system("rm -r FrontFace");
    system("mkdir FrontFace");

    CvCapture* camera = 0;	// The camera device.
    IplImage *frame = 0;

    int width=320, height=240, camid;
    camid = 0;
    frame = getCameraFrame(camera, 0, camid, width, height);
    vidfps = 10;
    frameW = 640;
    frameH = 480;
    fourcc = CV_FOURCC('D', 'I', 'V', 'X');

    cvNamedWindow(flandmark_window, 0);

    // Haar Cascade file, used for Face Detection.
    char faceCascadeFilename [] = "haarcascade_frontalface_alt.xml";
    // Load the HaarCascade classifier for face detection.
    CvHaarClassifierCascade* faceCascade;
    faceCascade = (CvHaarClassifierCascade*)cvLoad(faceCascadeFilename, 0, 0, 0);
    if( !faceCascade )
    {
        printf("Couldnt load Face detector '%s'\n", faceCascadeFilename);
        exit(1);
    }

    // ------------- begin flandmark load model
    t = (double)cvGetTickCount();
    FLANDMARK_Model * model = flandmark_init("flandmark_model.dat");
    if (model == 0)
    {
        printf("Structure model was not created. Corrupted file flandmark_model.dat?\n");
        exit(1);
    }
    t = (double)cvGetTickCount() - t;
    ms = cvRound( t / ((double)cvGetTickFrequency() * 1000.0) );
    printf("Structure model loaded in %d ms.\n", ms);
    // ------------- end flandmark load model

    int *bbox = (int*)malloc(4*sizeof(int));
    double *landmarks = (double*)malloc(2*model->data.options.M*sizeof(double));
    IplImage *frame_bw = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 1);

    char fps[50];
    CvFont font;
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 1, CV_AA);

    int frameid = 0;
    bool flag = true;

    while ( cvWaitKey(20) != 27 )
    {
        if (frame_num % 5 == 0) {
          frame = getCameraFrame(camera);
          cvConvertImage(frame, frame_bw);
          detectFaceInImage(frame, frame_bw, faceCascade, model, bbox, landmarks, frame_num);
          cvShowImage(flandmark_window, frame);
        } else {
          frame = getCameraFrame(camera);
          cvShowImage(flandmark_window, frame);
        }
    }

    // Free the camera.
    free(landmarks);
    free(bbox);
    cvReleaseCapture(&camera);
    cvReleaseHaarClassifierCascade(&faceCascade);
    cvDestroyWindow(flandmark_window);
    flandmark_free(model);
}
