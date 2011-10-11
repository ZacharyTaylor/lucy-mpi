#include "files_io.h"

/**
 * Saves the image to dir ./Output with name output-#.bmp
 * @param output the image to save
 * @param i the # in the image name
 */
void save_image(cimg_library::CImg<double> output, string name){
    string outname = "Output/";
    stringstream ss;
    ss << name;
    outname.append(ss.str());
    output.save(outname.c_str());
}

/**
 * Loads the psf file psf.bmp from the same dir as the program
 * @return the psf
 */
cimg_library::CImg<double> get_psf(void){
    //load image
    cimg_library::CImg<double> temp_psf_img("psf.bmp");

    //creates a greyscale image
    cimg_library::CImg<double> psf_img(3,3,1,1,0);

    //converts psf to greyscale if it isnt already
    cimg_forXY(temp_psf_img,x,y) {
        psf_img(x,y,0,0) = temp_psf_img(x,y,0,0);
    }
    
    return psf_img;
}

/**
 * Loads the ith image from ./Images
 * @param files list of all the image files paths
 * @param i the image in the list to load
 * @return the ith image
 */
cimg_library::CImg<double> get_image(vector<string> files, int i){
    string path = "Images/";
    path.append(files[i]);

    //load image
    cimg_library::CImg<double> temp_img(path.c_str());

    return temp_img;
}

/**
 * Checks if a string has the specifed ending
 * @param fullString the full string
 * @param ending the desired end
 * @return true if it has the ending, false otherwise
 */
bool has_ending (std::string const &fullString, std::string const &ending){
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

/**
 * Gets the list of image files in a directory
 * @param dir the directory
 * @param files the list of image file paths
 * @return true if sucessful, false otherwise
 */
bool get_dir (string dir, vector<string> *files){
DIR *dp;
struct dirent *dirp;
if((dp = opendir(dir.c_str())) == NULL) {
    return false;
}

while ((dirp = readdir(dp)) != NULL) {
    if(has_ending(string(dirp->d_name), ".bmp")){
        files->push_back(string(dirp->d_name));
    }
}
closedir(dp);
return true;
}

/**
 * Scans Images folder to get a list of the images
 * @param files the list of the images in the folder
 * @return true if files present, false otherwise
 */
bool init_images(vector<string> *files){
    string dir = string("./Images");
    return get_dir(dir,files);
}