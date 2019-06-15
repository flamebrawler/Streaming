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
	
	result.assign(frame.data, frame.data + 3*frame.total());	
	imgdata::Image im(result, frame.size().width, frame.size().height);
	result.clear();
	return im;
}