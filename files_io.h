#ifndef FILES_IO_H
#define	FILES_IO_H

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include "CImg.h"
#include <sstream>

using namespace std;

/**
 * Saves the image to dir ./Output with name output-#.bmp
 * @param output the image to save
 * @param i the # in the image name
 */
void save_image(cimg_library::CImg<double> output, int i);

/**
 * Loads the psf file psf.bmp from the same dir as the program
 * @return the psf
 */
cimg_library::CImg<double> get_psf(void);

/**
 * Loads the ith image from ./Images
 * @param files list of all the image files paths
 * @param i the image in the list to load
 * @return the ith image
 */
cimg_library::CImg<double> get_image(vector<string> files, int i);

/**
 * Checks if a string has the specifed ending
 * @param fullString the full string
 * @param ending the desired end
 * @return true if it has the ending, false otherwise
 */
bool has_ending (std::string const &fullString, std::string const &ending);

/**
 * Gets the list of image files in a directory
 * @param dir the directory
 * @param files the list of image file paths
 * @return true if sucessful, false otherwise
 */
bool get_dir (string dir, vector<string> *files);

/**
 * Scans Images folder to get a list of the images
 * @param files the list of the images in the folder
 * @return true if files present, false otherwise
 */
bool init_images(vector<string> *files);

#endif	/* FILES_IO_H */

