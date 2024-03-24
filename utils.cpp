/*
    Problem Set 4: Calibration and Augmented Reality
    Joseph Nelson Farrell & Harshil Bhojwani  
    Spring 2024
    CS 5330 Northeastern 
    Professor Bruce Maxwell, PhD
    
    This file contains a library of helper functions for calibrate.cpp and project_points.cpp
*/

#include <cstdio>
#include "opencv2/opencv.hpp"
#include <fstream>
#include <sstream>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////     read_data_to_csv        /////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void save_data_to_csv(cv::Mat& mat, std::vector<double> &dist_vec, std::string& filename){
    /*
        Funtion: save_data_to_csv

        Parameters:
            mat: (cv::Mat) containing the calibration matrix
            dist_vec: (std::vec) containing the distortion coefficients

        Returns:
            None

        This function will read matrix data and vector data into a csv.
    */

    // open the file
    std::ofstream file(filename);

    // check if the file is open
    if (!file.is_open()) {
        std::cerr << "Could not open csv file: " << filename << std::endl;
        return;
    }

    // mat data to csv
    for(int i = 0; i < mat.rows; i++){
        for(int j = 0; j < mat.cols; j++){
            file << mat.at<double>(i, j);
            if (i != mat.rows - 1 || j != mat.cols - 1) {
                file << ","; 
            }
        }
    }
    file << std::endl;

    // vec data to csv
    for(int i = 0; i < dist_vec.size(); i++){
        file << dist_vec[i];
        if (i != dist_vec.size() - 1){
            file << ",";
        }
    }

    file.close();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////     read_data_from_csv        ///////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void read_data_from_csv(std::vector<double>& camera_matrix, std::vector<double>& dist_vec, const std::string& filename) {
    /*
        Funtion: read_data_from_csv

        Parameters:
            camera_matrix: (cv::Mat) this will store the calibration matrix data from the csv
            dist_vec: (std::vec) this will store the distortion coefficients
            filename: (str) this is the file name of the csv containing the calibration matrix and distortion coefficients

        Returns:
            None

        This function will read matrix data and vector data from a csv and populate the parameters.
    */
    
    // open the file
    std::ifstream file(filename);

    // check if file is open
    if (!file.is_open()) {
        std::cerr << "Could not open csv file: " << filename << std::endl;
        return;
    }

    // read in camera matric data
    std::string line;
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        size_t index = 0;
        while (std::getline(iss, token, ',')) {
            double value = std::stod( token );
            camera_matrix.push_back( value );
            index++;
        }
    }

    // read in distortion vector data
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        while (std::getline(iss, token, ',')) {
            double value = std::stod( token );
            dist_vec.push_back( value );
        }
    }

    file.close();
}

