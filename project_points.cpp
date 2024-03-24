/*
    Problem Set 4: Calibration and Augmented Reality
    Joseph Nelson Farrell & Harshil Bhojwani  
    Spring 2024
    CS 5330 Northeastern 
    Professor Bruce Maxwell, PhD
    
    This file contains a program that will read in calibration data from a csv and generate augmented
    reality objects over a target. Different key strokes will gererate different shapes.

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
    char lastKey = 0;
    bool save_flag = false;
	std::vector<std::vector<cv::Vec3f>> point_list; // vector of vectors, world coordinates
	std::vector<std::vector<cv::Point2f>> corner_list; // vector of vectors, image coordinates
    std::vector<cv::Point2f> corners_set; // set of corner coordinates
    std::vector<cv::Point2f> image_points; // set of image points
    std::vector<double> rvec;
    std::vector<double> tvec;
    std::vector<double> camera_matrix_vec;
    std::vector<double> distortion_vec;
    std::string file_path = "/Users/nelsonfarrell/Documents/Northeastern/5330/problem_sets/problem_set_4/calibration_data.csv";

    // declare world points
    std::vector<cv::Vec3f> point_set;
    for (int i = 0; i < 6; i++){
        for (int j = 0; j < 9; j++){
            float x = static_cast<float>(j);
            float y = -static_cast<float>(i);
            float z = 0.0f;
            point_set.push_back( cv::Vec3f(x , y, z));
        }
    }

    // read in data
    read_data_from_csv( camera_matrix_vec, distortion_vec, file_path);

    // generate camera matrix
    cv::Mat camera_matrix(3, 3, CV_64FC1, camera_matrix_vec.data());

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

        // check if findChessboard Corners found all the corners
        bool patternfound = cv::findChessboardCorners(gray, pattern_size, corners_set, 
                                                    cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FAST_CHECK);

        // if corners found
        if(patternfound) {
                cv::cornerSubPix(gray, corners_set, cv::Size(11, 11), cv::Size(-1, -1),
                cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1));
                bool solved = cv::solvePnP(point_set, corners_set, camera_matrix, distortion_vec, rvec, tvec);

                std::cout << "Rotation Matrix: " << std::endl;
                for(int i = 0; i < rvec.size(); i++){
                    std::cout << rvec[i] << std::endl;}

                std::cout << "Translation Matrix: " << std::endl;
                for(int i = 0; i < tvec.size(); i++){
                    std::cout << tvec[i] << std::endl;
                    }
                cv::projectPoints( point_set, rvec, tvec, camera_matrix, distortion_vec, image_points);

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////        Draw Shapes           ////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        if( lastKey == 'r'){

            // define corners in world space
            std::vector<cv::Point3f> four_corners_set;
            cv::Point3f c_1(0, 0, 0); 
            cv::Point3f c_2(8, 0, 0); 
            cv::Point3f c_3(0, -5, 0);
            cv::Point3f c_4(8, -5, 0);
            cv::Point3f c_5(0, 0, 3); 
            cv::Point3f c_6(8, 0, 3); 
            cv::Point3f c_7(0, -5, 3);
            cv::Point3f c_8(8, -5, 3);
            cv::Point3f c_9(3, -5, 0);
            cv::Point3f c_10(4, -5, 0);
            cv::Point3f c_11(3, -5, 3);
            cv::Point3f c_12(4, -5, 3);
            cv::Point3f c_13(3, 0, 3);
            cv::Point3f c_14(4, 0, 3);
            cv::Point3f c_15(3, 0, 0);
            cv::Point3f c_16(4, 0, 0);

            four_corners_set.push_back(c_1);
            four_corners_set.push_back(c_2);                
            four_corners_set.push_back(c_3);
            four_corners_set.push_back(c_4);
            four_corners_set.push_back(c_5);
            four_corners_set.push_back(c_6);                
            four_corners_set.push_back(c_7);
            four_corners_set.push_back(c_8);
            four_corners_set.push_back(c_9);
            four_corners_set.push_back(c_10);                
            four_corners_set.push_back(c_11);
            four_corners_set.push_back(c_12);
            four_corners_set.push_back(c_13);
            four_corners_set.push_back(c_14);                
            four_corners_set.push_back(c_15);
            four_corners_set.push_back(c_16);


            // project points to image
            std::vector<cv::Point2f>four_corbers_points;
            cv::projectPoints(four_corners_set, rvec, tvec, camera_matrix, distortion_vec, four_corbers_points);

            // circle the four corneres
            cv::circle(src, four_corbers_points[0], 5, cv::Scalar(242, 242, 7), 2);
            cv::circle(src, four_corbers_points[1], 5, cv::Scalar(242, 242, 7), 2);
            cv::circle(src, four_corbers_points[2], 5, cv::Scalar(242, 242, 7), 2);
            cv::circle(src, four_corbers_points[3], 5, cv::Scalar(242, 242, 7), 2);

            // draw shapes
            cv::line(src, four_corbers_points[0], four_corbers_points[1], cv::Scalar(242, 242, 7), 2);
            cv::line(src, four_corbers_points[0], four_corbers_points[2], cv::Scalar(242, 242, 7), 2);
            cv::line(src, four_corbers_points[2], four_corbers_points[3], cv::Scalar(242, 242, 7), 2);
            cv::line(src, four_corbers_points[3], four_corbers_points[1], cv::Scalar(242, 242, 7), 2);
            cv::line(src, four_corbers_points[0], four_corbers_points[4], cv::Scalar(242, 242, 7), 2);
            cv::line(src, four_corbers_points[1], four_corbers_points[5], cv::Scalar(242, 242, 7), 2);
            cv::line(src, four_corbers_points[2], four_corbers_points[6], cv::Scalar(242, 242, 7), 2);
            cv::line(src, four_corbers_points[3], four_corbers_points[7], cv::Scalar(242, 242, 7), 2);
            cv::line(src, four_corbers_points[4], four_corbers_points[5], cv::Scalar(242, 242, 7), 2);
            cv::line(src, four_corbers_points[4], four_corbers_points[6], cv::Scalar(242, 242, 7), 2);
            cv::line(src, four_corbers_points[6], four_corbers_points[7], cv::Scalar(242, 242, 7), 2);
            cv::line(src, four_corbers_points[7], four_corbers_points[5], cv::Scalar(242, 242, 7), 2);
            cv::line(src, four_corbers_points[8], four_corbers_points[10], cv::Scalar(0, 0, 255), 3);
            cv::line(src, four_corbers_points[9], four_corbers_points[11], cv::Scalar(0, 0, 255), 3);
            cv::line(src, four_corbers_points[10], four_corbers_points[11], cv::Scalar(0, 0, 255), 3);
            cv::line(src, four_corbers_points[10], four_corbers_points[12], cv::Scalar(0, 0, 255), 3);
            cv::line(src, four_corbers_points[11], four_corbers_points[13], cv::Scalar(0, 0, 255), 3);
            cv::line(src, four_corbers_points[12], four_corbers_points[13], cv::Scalar(0, 0, 255), 3);
            cv::line(src, four_corbers_points[12], four_corbers_points[14], cv::Scalar(0, 0, 255), 3);
            cv::line(src, four_corbers_points[8], four_corbers_points[14], cv::Scalar(0, 0, 255), 3);
            cv::line(src, four_corbers_points[13], four_corbers_points[15], cv::Scalar(0, 0, 255), 3);
            cv::line(src, four_corbers_points[9], four_corbers_points[15], cv::Scalar(0, 0, 255), 3);

            // color sides
            std::vector<std::vector<cv::Point>> side_1(1, std::vector<cv::Point>());
            side_1[0] = {four_corbers_points[8], four_corbers_points[10], four_corbers_points[11], four_corbers_points[9]};
            cv::fillPoly(src, side_1, cv::Scalar(242, 242, 7));

            std::vector<std::vector<cv::Point>> side_2(1, std::vector<cv::Point>());
            side_2[0] = {four_corbers_points[8] ,four_corbers_points[14], four_corbers_points[12], four_corbers_points[10]};
            cv::fillPoly(src, side_2, cv::Scalar(242, 242, 7));

            std::vector<std::vector<cv::Point>> side_3(1, std::vector<cv::Point>());
            side_3[0] = {four_corbers_points[9] ,four_corbers_points[15], four_corbers_points[13], four_corbers_points[11]};
            cv::fillPoly(src, side_3, cv::Scalar(242, 242, 7));

            std::vector<std::vector<cv::Point>> side_4(1, std::vector<cv::Point>());
            side_4[0] = {four_corbers_points[14] ,four_corbers_points[15], four_corbers_points[13], four_corbers_points[12]};
            cv::fillPoly(src, side_4, cv::Scalar(242, 242, 7));

            std::vector<std::vector<cv::Point>> top(1, std::vector<cv::Point>());
            top[0] = {four_corbers_points[10] ,four_corbers_points[12], four_corbers_points[13], four_corbers_points[11]};
            cv::fillPoly(src, top, cv::Scalar(0, 0, 255));

            // draw lines
            cv::line(src, four_corbers_points[13], four_corbers_points[15], cv::Scalar(0, 0, 255), 3);
            cv::line(src, four_corbers_points[14], four_corbers_points[15], cv::Scalar(0, 0, 255), 3);
            cv::line(src, four_corbers_points[8], four_corbers_points[10], cv::Scalar(0, 0, 255), 3);
            cv::line(src, four_corbers_points[8], four_corbers_points[10], cv::Scalar(0, 0, 255), 3);
            cv::line(src, four_corbers_points[9], four_corbers_points[11], cv::Scalar(0, 0, 255), 3);
            cv::line(src, four_corbers_points[10], four_corbers_points[11], cv::Scalar(0, 0, 255), 3);
            cv::line(src, four_corbers_points[10], four_corbers_points[12], cv::Scalar(0, 0, 255), 3);
            cv::line(src, four_corbers_points[11], four_corbers_points[13], cv::Scalar(0, 0, 255), 3);
            cv::line(src, four_corbers_points[12], four_corbers_points[13], cv::Scalar(0, 0, 255), 3);
            cv::line(src, four_corbers_points[12], four_corbers_points[14], cv::Scalar(0, 0, 255), 3);
            cv::line(src, four_corbers_points[8], four_corbers_points[14], cv::Scalar(0, 0, 255), 3);
            cv::line(src, four_corbers_points[13], four_corbers_points[15], cv::Scalar(0, 0, 255), 3);
            cv::line(src, four_corbers_points[9], four_corbers_points[15], cv::Scalar(0, 0, 255), 3);
            cv::line(src, four_corbers_points[9], four_corbers_points[8], cv::Scalar(0, 0, 255), 3);
        }
     
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////     Floating Diamond     //////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        if( lastKey == 'd'){ 

            // diamond set 
            std::vector<cv::Point3f> diamond_set;
            cv::Point3f d_1(6, -2, 4); 
            cv::Point3f d_2(5, -3, 5); 
            cv::Point3f d_3(5, -1, 5);
            cv::Point3f d_4(7, -3, 5);
            cv::Point3f d_5(7, -1, 5); 
            cv::Point3f d_6(6, -2, 6);

            diamond_set.push_back(d_1);
            diamond_set.push_back(d_2);
            diamond_set.push_back(d_3);
            diamond_set.push_back(d_4);
            diamond_set.push_back(d_5);
            diamond_set.push_back(d_6);

            // project diamond set to image
            std::vector<cv::Point2f>diamnond_points;
            cv::projectPoints(diamond_set, rvec, tvec, camera_matrix, distortion_vec, diamnond_points);

            // draw diamond
            std::vector<std::vector<cv::Point>> middle(1, std::vector<cv::Point>());
            middle[0] = {diamnond_points[1] , diamnond_points[3], diamnond_points[4], diamnond_points[2]};

            std::vector<std::vector<cv::Point>> tri_1(1, std::vector<cv::Point>());
            tri_1[0] = {diamnond_points[0] , diamnond_points[3], diamnond_points[4]};
            cv::fillPoly(src, tri_1, cv::Scalar(255, 255, 255));

            std::vector<std::vector<cv::Point>> tri_2(1, std::vector<cv::Point>());
            tri_2[0] = {diamnond_points[0] , diamnond_points[3], diamnond_points[1]};
            cv::fillPoly(src, tri_2, cv::Scalar(255, 255, 255));

            std::vector<std::vector<cv::Point>> tri_3(1, std::vector<cv::Point>());
            tri_3[0] = {diamnond_points[0] , diamnond_points[1], diamnond_points[2]};
            cv::fillPoly(src, tri_3, cv::Scalar(255, 255, 255));
        
            std::vector<std::vector<cv::Point>> tri_4(1, std::vector<cv::Point>());
            tri_4[0] = {diamnond_points[0] , diamnond_points[2], diamnond_points[4]};
            cv::fillPoly(src, tri_4, cv::Scalar(255, 255, 255));

            std::vector<std::vector<cv::Point>> tri_5(1, std::vector<cv::Point>());
            tri_5[0] = {diamnond_points[5] , diamnond_points[3], diamnond_points[4]};
            cv::fillPoly(src, tri_5, cv::Scalar(255, 255, 255));

            std::vector<std::vector<cv::Point>> tri_6(1, std::vector<cv::Point>());
            tri_6[0] = {diamnond_points[5] , diamnond_points[3], diamnond_points[1]};
            cv::fillPoly(src, tri_6, cv::Scalar(255, 255, 255));

            std::vector<std::vector<cv::Point>> tri_7(1, std::vector<cv::Point>());
            tri_7[0] = {diamnond_points[5] , diamnond_points[1], diamnond_points[2]};
            cv::fillPoly(src, tri_7, cv::Scalar(255, 255, 255));
        
            std::vector<std::vector<cv::Point>> tri_8(1, std::vector<cv::Point>());
            tri_8[0] = {diamnond_points[5] , diamnond_points[2], diamnond_points[4]};
            cv::fillPoly(src, tri_8, cv::Scalar(255, 255, 255));

            cv::line(src, diamnond_points[0], diamnond_points[1], cv::Scalar(0, 0, 255), 2);
            cv::line(src, diamnond_points[0], diamnond_points[2], cv::Scalar(0, 0, 255), 2);
            cv::line(src, diamnond_points[0], diamnond_points[3], cv::Scalar(0, 0, 255), 2);
            cv::line(src, diamnond_points[0], diamnond_points[4], cv::Scalar(0, 0, 255), 2);

            cv::line(src, diamnond_points[1], diamnond_points[2], cv::Scalar(0, 0, 255), 2);
            cv::line(src, diamnond_points[1], diamnond_points[3], cv::Scalar(0, 0, 255), 2);
            cv::line(src, diamnond_points[3], diamnond_points[4], cv::Scalar(0, 0, 255), 2);
            cv::line(src, diamnond_points[4], diamnond_points[2], cv::Scalar(0, 0, 255), 2);

            cv::line(src, diamnond_points[5], diamnond_points[1], cv::Scalar(0, 0, 255), 2);
            cv::line(src, diamnond_points[5], diamnond_points[2], cv::Scalar(0, 0, 255), 2);
            cv::line(src, diamnond_points[5], diamnond_points[3], cv::Scalar(0, 0, 255), 2);
            cv::line(src, diamnond_points[5], diamnond_points[4], cv::Scalar(0, 0, 255), 2);

        }   
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////  Cover Target: Draw Scene  //////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        if( lastKey == 'c'){

            // cover target set  
            std::vector<cv::Point3f> cover_set;
            cv::Point3f cov_1(-2, 2, 0); 
            cv::Point3f cov_2(10, 2, 0); 
            cv::Point3f cov_3(-2, -10, 0);
            cv::Point3f cov_4(10, -10, 0);

            cover_set.push_back( cov_1 );
            cover_set.push_back( cov_2 );
            cover_set.push_back( cov_3 );
            cover_set.push_back( cov_4 );

            // cover target points
            std::vector<cv::Point2f>cover_points;
            cv::projectPoints(cover_set, rvec, tvec, camera_matrix, distortion_vec, cover_points);

            // cover target
            std::vector<std::vector<cv::Point>> cover_vec(1, std::vector<cv::Point>());
            cover_vec[0] = {cover_points[0], cover_points[2], cover_points[3], cover_points[1]};
            cv::fillPoly(src, cover_vec, cv::Scalar(0, 63, 127));

            // river set
            std::vector<cv::Point3f> river_set;
            cv::Point3f r_1(7, -10, 0); 
            cv::Point3f r_2(7.5, -9.5, 0); 
            cv::Point3f r_3(8, -9.2, 0);
            cv::Point3f r_4(8.2, -9.0, 0);
            cv::Point3f r_5(8.5, -8.4, 0); 
            cv::Point3f r_6(8.9, -8.2, 0); 
            cv::Point3f r_7(9.3, -7.7, 0);
            cv::Point3f r_8(9.5, -7.1, 0);
            cv::Point3f r_9(9.9, -7.0, 0); 
            cv::Point3f r_10(10, -6.5, 0);
            cv::Point3f r_11(10, -10, 0);

            river_set.push_back( r_1 );
            river_set.push_back( r_2 );
            river_set.push_back( r_3 );
            river_set.push_back( r_4 );
            river_set.push_back( r_5 );
            river_set.push_back( r_6 );
            river_set.push_back( r_7 );
            river_set.push_back( r_8 );
            river_set.push_back( r_9 );
            river_set.push_back( r_10 );
            river_set.push_back( r_11 );

            // river points
            std::vector<cv::Point2f>river_points;
            cv::projectPoints(river_set, rvec, tvec, camera_matrix, distortion_vec, river_points);

            // draw river 
            std::vector<std::vector<cv::Point>> river_vec(1, std::vector<cv::Point>());
            river_vec[0] = {river_points[0], river_points[1], river_points[2], river_points[4], river_points[5], river_points[6], river_points[7],
                            river_points[8], river_points[9], river_points[10], river_points[0]};
            cv::fillPoly(src, river_vec, cv::Scalar(255, 0, 0));

            // mountain set
            std::vector<cv::Point3f> mountain_set;
            cv::Point3f m_1_a(-2, 2, 0); 
            cv::Point3f m_1_b(0, 2, 3); 
            cv::Point3f m_1_c(3, 2, 3);
            cv::Point3f m_1_d(5, 2, 4);
            cv::Point3f m_1_e(7, 2, 0);

            cv::Point3f m_1_f(-2, -4, 0); 
            cv::Point3f m_1_g(0, -4, 3); 
            cv::Point3f m_1_h(3, -4, 3);
            cv::Point3f m_1_i(5, -4, 4);
            cv::Point3f m_1_j(7, -4, 0);
            cv::Point3f sun(10, 0, 7);

            mountain_set.push_back( m_1_a );
            mountain_set.push_back( m_1_b );
            mountain_set.push_back( m_1_c );
            mountain_set.push_back( m_1_d );
            mountain_set.push_back( m_1_e );

            mountain_set.push_back( m_1_f );
            mountain_set.push_back( m_1_g );
            mountain_set.push_back( m_1_h );
            mountain_set.push_back( m_1_i );
            mountain_set.push_back( m_1_j );
            mountain_set.push_back( sun );

            // mountain points
            std::vector<cv::Point2f>mountain_points;
            cv::projectPoints(mountain_set, rvec, tvec, camera_matrix, distortion_vec, mountain_points);

            // draw mountain
            std::vector<std::vector<cv::Point>> mountain_vec(1, std::vector<cv::Point>());
            mountain_vec[0] = {mountain_points[0], mountain_points[1], mountain_points[2], mountain_points[3], mountain_points[4], mountain_points[0]};
            cv::fillPoly(src, mountain_vec, cv::Scalar(0, 123, 0));

            std::vector<std::vector<cv::Point>> mountain_vec_3(1, std::vector<cv::Point>());
            mountain_vec_3[0] = {mountain_points[0], mountain_points[1], mountain_points[6], mountain_points[5], mountain_points[0]};
            cv::fillPoly(src, mountain_vec_3, cv::Scalar(0, 180, 0));

            std::vector<std::vector<cv::Point>> mountain_vec_4(1, std::vector<cv::Point>());
            mountain_vec_4[0] = {mountain_points[1], mountain_points[2], mountain_points[7], mountain_points[6], mountain_points[1]};
            cv::fillPoly(src, mountain_vec_4, cv::Scalar(0, 165, 0));

            std::vector<std::vector<cv::Point>> mountain_vec_5(1, std::vector<cv::Point>());
            mountain_vec_5[0] = {mountain_points[2], mountain_points[3], mountain_points[8], mountain_points[7], mountain_points[2]};
            cv::fillPoly(src, mountain_vec_5, cv::Scalar(0, 100, 0));

            std::vector<std::vector<cv::Point>> mountain_vec_6(1, std::vector<cv::Point>());
            mountain_vec_6[0] = {mountain_points[3], mountain_points[4], mountain_points[9], mountain_points[8], mountain_points[3]};
            cv::fillPoly(src, mountain_vec_6, cv::Scalar(0, 230, 0));

            std::vector<std::vector<cv::Point>> mountain_vec_2(1, std::vector<cv::Point>());
            mountain_vec_2[0] = {mountain_points[5], mountain_points[6], mountain_points[7], mountain_points[8], mountain_points[9], mountain_points[5]};
            cv::fillPoly(src, mountain_vec_2, cv::Scalar(0, 90, 0));
            
            // draw sun
            cv::circle(src, mountain_points[10], 80, cv::Scalar(0, 255, 255), -1);


            // tree set
            std::vector<cv::Point3f> trees_set;
            cv::Point3f t_1_a(6, -6, 0); 
            cv::Point3f t_1_b(6, -6, .5);
            cv::Point3f t_2_a(7, -6.3, 0); 
            cv::Point3f t_2_b(7, -6.3, .5); 
            cv::Point3f t_3_a(7.3, -6.5, 0); 
            cv::Point3f t_3_b(7.3, -6.5, .5); 
            cv::Point3f t_4_a(7.5, -6.9, 0); 
            cv::Point3f t_4_b(7.5, -6.9, .5); 
            cv::Point3f t_5_a(8, -6, 0); 
            cv::Point3f t_5_b(8, -6, .5); 
            trees_set.push_back( t_1_a );
            trees_set.push_back( t_1_b );
            trees_set.push_back( t_2_a );
            trees_set.push_back( t_2_b );
            trees_set.push_back( t_3_a );
            trees_set.push_back( t_3_b );
            trees_set.push_back( t_4_a );
            trees_set.push_back( t_4_b );
            trees_set.push_back( t_5_a );
            trees_set.push_back( t_5_b );

            // tree points
            std::vector<cv::Point2f>tree_points;
            cv::projectPoints(trees_set, rvec, tvec, camera_matrix, distortion_vec, tree_points);

            // draw trees
            cv::line(src, tree_points[0], tree_points[1], cv::Scalar(61, 33, 7), 3);
            cv::circle(src, tree_points[1], 7, cv::Scalar(0, 191, 95), -1);

            cv::line(src, tree_points[2], tree_points[3], cv::Scalar(61, 33, 7), 3);
            cv::circle(src, tree_points[3], 7, cv::Scalar(0, 191, 95), -1);

            cv::line(src, tree_points[4], tree_points[5], cv::Scalar(61, 33, 7), 3);
            cv::circle(src, tree_points[5], 7, cv::Scalar(0, 191, 95), -1);

            cv::line(src, tree_points[6], tree_points[7], cv::Scalar(61, 33, 7), 3);
            cv::circle(src, tree_points[7], 7, cv::Scalar(0, 191, 95), -1);

            cv::line(src, tree_points[8], tree_points[9], cv::Scalar(61, 33, 7), 3);
            cv::circle(src, tree_points[9], 7, cv::Scalar(0, 191, 95), -1);
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////           Draw Axes        ///////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        if( lastKey == 'a'){  

            // axes set
            std::vector<cv::Point3f> axis_set;

            // axis world
            cv::Point3f x_axis_end(2, 0, 0); 
            cv::Point3f y_axis_end(0, -2, 0); 
            cv::Point3f z_axis_end(0, 0, 2);

            axis_set.push_back(x_axis_end);
            axis_set.push_back(y_axis_end);
            axis_set.push_back(z_axis_end);

            // axes image points
            std::vector<cv::Point2f> axis_points;
            cv::projectPoints(axis_set, rvec, tvec, camera_matrix, distortion_vec, axis_points);

            // draw axis lines on the image
            cv::Point origin = image_points[0];
            cv::arrowedLine(src, origin, axis_points[0], cv::Scalar(86, 255, 255), 3); 
            cv::arrowedLine(src, origin, axis_points[1], cv::Scalar(0, 255, 0), 3); 
            cv::arrowedLine(src, origin, axis_points[2], cv::Scalar(0, 0, 255), 3);

            // annotate
            cv::putText(src, "Y World", axis_points[0] + cv::Point2f(20, 20), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(86, 255, 255), 2);
            cv::putText(src, "X World", axis_points[1]+ cv::Point2f(20, 20), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);
            cv::putText(src, "Z World", axis_points[2] + cv::Point2f(20, 20), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
        }
        
    }
        
    // display
    cv::imshow("Source Display", src);

    }
}

