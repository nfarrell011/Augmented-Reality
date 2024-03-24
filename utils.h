/*
    Problem Set 4: Calibration and Augmented Reality
    Joseph Nelson Farrell & Harshil Bhojwani  
    Spring 2024
    CS 5330 Northeastern 
    Professor Bruce Maxwell, PhD
    
    This is teh header file for utils.cpp, containing helper functions for calibrate.cpp and project_points.cpp
*/
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <opencv2/opencv.hpp>

#ifndef UTILS_H
#define UTILS_H

void save_data_to_csv(cv::Mat& mat, std::vector<double> &dist_vec, std::string& filename);

void read_data_from_csv(std::vector<double>& camera_matrix, std::vector<double>& dist_vec, const std::string& filename);

#endif