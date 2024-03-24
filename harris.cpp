/*
    Problem Set 4: Calibration and Augmented Reality
    Joseph Nelson Farrell & Harshil Bhojwani  
    Spring 2024
    CS 5330 Northeastern 
    Professor Bruce Maxwell, PhD
    
    This file contains a program that will find the Harris corners in a image.
*/
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>

int main(int argc, char *argv[]) {

    // declare viables
    cv::Mat src;
    char filename[256];
    char lastKey = 0;

    // error checking
    if(argc < 1) {
        printf("usage: %s <image filename>",  argv[0]);
        return(-1);
    }

    cv::VideoCapture *capdev;

    // open the video device
    capdev = new cv::VideoCapture(2);
    if ( !capdev->isOpened()) {
        printf("Unable to open video device\n");
        return(-1);
    }

    // get image properties
    cv::Size refS( (int) capdev->get(cv::CAP_PROP_FRAME_WIDTH),
                (int) capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
    
    // print results
    printf("Expected size: %d %d\n", refS.width, refS.height);

    for(;;) {

            // this will get a new src from the camera, treat as a stream
            *capdev >> src;
            if( src.empty() ) {
                printf( "src is empty!!\n");
                break;
            }

            // check for keystrokes
            char key = cv::waitKey(2) & 0xFF;

            // save the last keystroke
            if (key != -1) {
                    { lastKey = key; }
            }

            // save the last keystroke
            if( lastKey == 'q') {
                    break;
            } 

            // read the file
            if( src.data == NULL ) {
                    printf("error: unable to read filename %s\n", filename);
                    return(-2);
            }

            // create grayscale image for cv::cornerHarris
            cv::Mat gray;
            cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

            // find Harris corners
            cv::Mat dst;
            int blockSize = 2;
            int apertureSize = 3;
            double k = 0.04;
            double threshold = 0.50;  
            cv::cornerHarris(gray, dst, blockSize, apertureSize, k);

            // normalize dst
            cv::normalize(dst, dst, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat());

            // convert dst to uchar
            dst.convertTo(dst, CV_8U);

            // threshold dst
            cv::Mat dst_thresh;
            cv::threshold(dst, dst_thresh, threshold * 255, 255, cv::THRESH_BINARY);

            // generate corners list
            std::vector<std::vector<std::pair<cv::Point, uchar>>> corners_vec;
            std::vector<cv::Point> corners;
            for (int i = 0; i < dst_thresh.rows; i++) {
                for (int j = 0; j < dst_thresh.cols; j++) {
                    if (dst_thresh.at<uchar>(i, j) > 0) {
                        uchar value = dst_thresh.at<uchar>(i, j);
                        corners_vec.push_back({{cv::Point(j, i), value}});
                    }
                }
            }

            // sort by the value at each point
            std::sort(corners_vec.begin(), corners_vec.end(), [](const std::vector<std::pair<cv::Point, uchar>>& a, const std::vector<std::pair<cv::Point, uchar>>& b) {
                return a[0].second > b[0].second;});

            // this can be used to add only a certian number of harris corners, this logic was not completed, 
            // so currently all points are projected.
            for(int i = 0; i < corners_vec.size(); i++){
                corners.push_back(corners_vec[i][0].first);
            }

            // draw circles on src
            for (int i = 0; i < corners.size(); i++){
                cv::circle(src, corners[i], 5, cv::Scalar(0, 0, 255), 2);
            }

            // display
            cv::imshow("Original Image", src);
            //cv::imshow("Harris Corners", dst_thresh);

    }
            
    delete capdev;
    return(0);
}