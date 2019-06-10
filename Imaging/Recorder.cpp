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
	//std::cout << frame.total() << "\n";
	//std::cout << result.size()<<" "<<(int)(frame.ptr<imgdata::pixel>(639,1239)->R) <<"\n";
	//std::cout << (int)result[2400000] << "\n";
	imgdata::Image im(result, frame.size().width, frame.size().height);
	result.clear();
	return im;
}