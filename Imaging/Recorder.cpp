#include "Recorder.h"

videoLoader::videoLoader() {
	vid = new cv::VideoCapture(0);
}

videoLoader::videoLoader(std::string filename) {
	vid = new cv::VideoCapture(filename);
}

imgdata::Image videoLoader::getFrame() {
	cv::Mat frame;
	*vid >> frame;
	std::vector<char> result;
	if (frame.isContinuous()) 
		result.assign(frame.data, frame.data + frame.total());	
	else 
		for (int i = 0; i < frame.rows; ++i) 
			result.insert(result.end(), frame.ptr<char>(i), frame.ptr<char>(i) + frame.cols);
	
	imgdata::Image im(result, frame.size().width, frame.size().height,1);
	return im;
}