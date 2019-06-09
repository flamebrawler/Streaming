#ifndef RECORDER_H
#define RECORDER_H

#include<opencv2/opencv.hpp>
#include <vector>
#include "Image.h"

class videoLoader {
private:
	cv::VideoCapture* vid;
public:
	videoLoader();
	imgdata::Image getFrame();
	videoLoader(std::string filename);
};

#endif
