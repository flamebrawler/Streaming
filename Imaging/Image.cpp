#include "Image.h"
#include <iostream>

namespace imgdata {

	Image::Image(int width, int height,int bytes)
	{
		this->pixels.assign(width * height * bytes,0);
		this->width = width;
		this->height = height;
		this->bytes = bytes;
	}

	Image::Image(const char* pixels, int width, int height,int bytes)
	{
		this->pixels.assign(width * height * bytes, 0);
		memcpy(&this->pixels[0], pixels, width * height * bytes);
		this->width = width;
		this->height = height;
		this->bytes = bytes;
	}

	Image::Image(pixel* pixels, int width, int height,int bytes)
	{
		this->pixels.assign(width * height * bytes, 0);
		memcpy(&this->pixels[0], pixels, width * height * bytes);
		this->width = width;
		this->height = height;
		this->bytes = bytes;
	}


	Image::Image(std::vector<char> pixels, int width, int height,int bytes)
	{
		this->pixels = pixels;
		this->width = width;
		this->height = height;
		this->bytes = bytes;
	}



	Image::Image(const Image* image)
	{
		pixels = image->pixels;
		this->width = image->width;
		this->height = image->height;
		bytes = image->bytes;
	}

	Image::~Image()
	{

	}

	const pixel* Image::getImage() const
	{
		return (pixel*) &pixels[0];
	}

	const pixel* Image::checkPixel(int x, int y) const
	{
		if (y * width + x <= width * height && y * width + x >= 0)
			return (pixel*)& pixels[bytes * (y * width + x)];
		std::cerr << "point: " << x << " " << y << " is out of bounds\n maximum dimensions are: " << width << "x" << height << "\n";
		return 0;
	}
	pixel* Image::getPixel(int x, int y)
	{
		if (y * width + x <= width * height && y * width + x >= 0)
			return (pixel*)& pixels[bytes * (y * width + x)];
		std::cerr << "point: " << x << " " << y << " is out of bounds\n maximum dimensions are: " << width << "x" << height << "\n";
		return 0;
	}

	pixel* Image::operator()(int x, int y)
	{
		if (y * width + x <= width * height && y * width + x >= 0)
			return (pixel*)& pixels[bytes * (y * width + x)];
		std::cerr << "point: " << x << " " << y << " is out of bounds\n maximum dimensions are: " << width << "x" << height << "\n";
		return 0;
	}

	pixel* Image::operator[](int pos) {
		if (pos <= width * height && pos >= 0)
			return (pixel*)& pixels[pos * bytes];
		std::cerr << "point: " << pos << " is out of bounds\n maximum pixel is: " << width * height << "\n";
		return 0;
	}

	float Image::getRatio() const {
		return (float)height / (float)width;
	}

	int Image::getWidth() const {
		return width;
	}

	int Image::getHeight() const {
		return height;
	}

	int Image::getBytes() const{
		return bytes;
	}

	void Image::setImage(Image image)
	{
		pixels = image.pixels;
		width = image.width;
		height = image.height;
		bytes = image.bytes;
	}

	void Image::setImage(std::vector<char> pixels, int width, int height,int bits) {
		this->pixels = pixels;
		this->width = width;
		this->height = height;
		this->bytes = bits;
	}


	void Image::setImage(const char* pixels, int width, int height,int bits) {
		this->pixels.assign(width * height * bits, 0);
		memcpy(&this->pixels[0], pixels, width * height * bits);
		this->width = width;
		this->height = height;
		this->bytes = bits;
	}

}