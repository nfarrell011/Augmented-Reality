/*
    Problem Set 4: Calibration and Augmented Reality
    Joseph Nelson Farrell & Harshil Bhojwani  
    Spring 2024
    CS 5330 Northeastern 
    Professor Bruce Maxwell, PhD
    
    This file contains a program that calibrate a camera using a checkerboard. It will output the
    calibration data to a csv
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
#include "utils.h"

int main(int argc, char *argv[]) {

    // declare variables
    cv::Mat src;
    char filename[256];
    int ncolors = 2;
    char lastKey = 0;
    bool save_flag = false;
    int calibration_frames_tracker = 0; // used to ensure enough frames
	std::vector<std::vector<cv::Vec3f>> point_list; // vector of vectors, world coordinates
	std::vector<std::vector<cv::Point2f>> corner_list; // vector of vectors, image coordinates
    std::vector<cv::Mat> calibration_images; // vector to store the calibration images
    cv::Mat last_good_frame; // used to save the last frame where all corners were found
    std::vector<cv::Point2f> last_good_corners_set; // used to save the last set of corners where all corners where found

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
        char key = cv::waitKey(1) & 0xFF;

        // save the last keystroke
        if (key != -1) {
            if (key == 't'){
                save_flag = true;
            } else { lastKey = key;}
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

        // create grayscale image for cv::cornerSubPix
        cv::Mat gray;
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

        // declare variables
        cv::Size pattern_size(9, 6); // number of corners
        std::vector<cv::Point2f> corners_set; // set of corner coordinates

        // check if findChessboard Corners found all the corners
        bool patternfound = cv::findChessboardCorners(gray, pattern_size, corners_set, 
                                                    cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FAST_CHECK);

        // refine corners
        if(patternfound) {
                cv::cornerSubPix(gray, corners_set, cv::Size(11, 11), cv::Size(-1, -1),
                cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1));
        }

        // draw corners
        cv::drawChessboardCorners(src, pattern_size, cv::Mat(corners_set), patternfound);

        // display results
        int num_corners = corners_set.size();
        std::cout << "Number of corners found: " << num_corners << std::endl; // number of corners found
        if ( num_corners != 0 ){ // make sure there is at least 1
                std::cout << "Coordinates of the First Corner:" << std::endl;
                std::cout << "\t" << corners_set[0] << std::endl;
        }

        // save the last successful frame and corners set      
        if ( patternfound ) {
                last_good_corners_set = corners_set; 
                last_good_frame = src.clone();
                cv::imshow("Last Good Frame", last_good_frame);
        }

        // display source
        cv::imshow("Source Display", src);

        // user option for selecting frame for calibration
        if( key == 's') {

                // declare 3D world point set
                std::vector<cv::Vec3f> point_set;

                // add last_good_corners_set to corners_list
                corner_list.push_back( last_good_corners_set );

                // set world points
                for (int i = 0; i < 6; i++){
                    for (int j = 0; j < 9; j++){
                        float x = static_cast<float>(j);
                        float y = -static_cast<float>(i);
                        float z = 0.0f;
                        point_set.push_back( cv::Vec3f(x , y, z));
                    }
                }

                // print world coordinates    
                for(int i = 0; i < point_set.size(); i++){
                    std::cout << "3D World Points" << point_set[i] << std::endl;
                }

                // print world coordinates    
                for(int i = 0; i < last_good_corners_set.size(); i++){
                    std::cout << "3D World Points" << last_good_corners_set[i] << std::endl;
                }

                // add world coordinates set to points_list
                point_list.push_back( point_set );
                calibration_images.push_back( last_good_frame );
        }

        // user option for calibrating camera
        if( key == 'c') {
                if ( point_list.size() < 5 ){
                        std::cout << "No, no, no...We need at least 5 frames!" << std::endl;
                        std::cout << "We only have: " << point_list.size() << std::endl;
                        cv::waitKey(0);
                        continue;
                } else  {

                        // instantiate camera matrix
                        double val_1 = static_cast<double>(last_good_frame.cols / 2.0);
                        double val_2 = static_cast<double>(last_good_frame.rows / 2.0);
                        double data[9] = {1, 0, val_1,
                                        0, 1, val_2,
                                        0, 0, 1};
                        cv::Mat camera_matrix(3, 3, CV_64FC1, data);

                        // instantiate size param
                        cv::Size image_size(last_good_frame.cols, last_good_frame.rows);

                        // instantiate distortion vector
                        std::vector<double> distortion_coef;

                        // instantiate output vectors
                        std::vector<cv::Mat> rvecs, tvecs;
                        std::vector<double> errors;
                        std::vector<double> std_intrinsics;
                        std::vector<double> std_extrinsics;
                        
                        // print distortion coefficients
                        std::cout << "_________________________________________" << std::endl;
                        std::cout << " " << std::endl;
                        std::cout << "Distortion Coefficients: Pre Calibration" << std::endl;
                        for( int i = 0; i < distortion_coef.size(); i++){
                            std::cout << distortion_coef[i] << ", ";
                        }
                            std::cout << "Empty" << std::endl;
                        std::cout << "_________________________________________" << std::endl;
                        std::cout << " " << std::endl;

                        // print camera_matrix
                        std::cout << "Camera Matrix: Pre Calibration " << std::endl;
                        for( int i = 0; i < camera_matrix.rows; i++){
                            for( int j = 0; j < camera_matrix.cols; j++){
                                std::cout << camera_matrix.at<double>(i, j) << " ";
                            }
                                std::cout << std::endl;
                        }
                        std::cout << "_________________________________________" << std::endl;
                        std::cout << " " << std::endl;
                        
                        // calibrate camera
                        double re_project_error = cv::calibrateCamera( point_list, corner_list, image_size, camera_matrix, distortion_coef, rvecs, tvecs, 
                                                                        std_intrinsics, std_extrinsics, errors);
                        
                        // print distortion coefficients
                        std::cout << "Distortion Coefficients: Post Calibration" << std::endl;
                        for( int i = 0; i < distortion_coef.size(); i++){
                            std::cout << distortion_coef[i] << ", ";
                        }
                            std::cout << std::endl;
                        std::cout << "_________________________________________" << std::endl;
                        std::cout << " " << std::endl;

                        // print camera_matrix
                        std::cout << "Camera Matrix: Post Calibration " << std::endl;
                        for( int i = 0; i < camera_matrix.rows; i++){
                            for( int j = 0; j < camera_matrix.cols; j++){
                                std::cout << camera_matrix.at<double>(i, j) << " ";
                            }
                                std::cout << std::endl;
                        }
                        std::cout << "_________________________________________" << std::endl;
                        std::cout << " " << std::endl;


                        // print re-projection error
                        std::cout << "Reprojection Error = " << re_project_error << std::endl;
                        std::cout << "_________________________________________" << std::endl;
                        std::cout << " " << std::endl;

                        // file path to csv file
                        std::string file_path = "/Users/nelsonfarrell/Documents/Northeastern/5330/problem_sets/problem_set_4/calibration_data.csv";

                        // save camera matrix
                        save_data_to_csv( camera_matrix, distortion_coef, file_path );


                        cv::waitKey(0);
                }
            }

    
        std::cout << "Size Corner List " << corner_list.size() << std::endl;
        std::cout << "Size Point List " << point_list.size() << std::endl;
        std::cout << "Num Cal Frames " << calibration_images.size() << std::endl;

    }
            
    delete capdev;
    return(0);
}