#ifndef FILES_IO_H
#define	FILES_IO_H

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <CImg.h>

using namespace std;

cimg_library::CImg<double> get_psf(void);

cimg_library::CImg<double> get_image(vector<string> files, int i);

bool has_ending (std::string const &fullString, std::string const &ending);

bool get_dir (string dir, vector<string> &files);

bool init_images(vector<string> files);

#endif	/* FILES_IO_H */

