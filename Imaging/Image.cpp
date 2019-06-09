#include "Image.h"
#include <iostream>

namespace imgdata {

	Image::Image(int width, int height,int bits)
	{
		this->pixels.assign(width * height * bits,0);
		this->width = width;
		this->height = height;
		this->bits = bits;
	}

	Image::Image(char* pixels, int width, int height,int bits)
	{
		this->pixels.assign(width * height * bits, 0);
		memcpy(&this->pixels[0], pixels, width * height * bits);
		this->width = width;
		this->height = height;
		this->bits = bits;
	}

	Image::Image(pixel* pixels, int width, int height,int bits)
	{
		this->pixels.assign(width * height * bits, 0);
		memcpy(&this->pixels[0], pixels, width * height * bits);
		this->width = width;
		this->height = height;
		this->bits = bits;
	}


	Image::Image(std::vector<char> pixels, int width, int height,int bits)
	{
		this->pixels = pixels;
		this->width = width;
		this->height = height;
		this->bits = bits;
	}



	Image::Image(const Image* image)
	{
		pixels = image->pixels;
		this->width = image->width;
		this->height = image->height;
		bits = image->bits;
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
			return (pixel*)& pixels[bits * (y * width + x)];
		std::cerr << "point: " << x << " " << y << " is out of bounds\n maximum dimensions are: " << width << "x" << height << "\n";
		return 0;
	}
	pixel* Image::getPixel(int x, int y)
	{
		if (y * width + x <= width * height && y * width + x >= 0)
			return (pixel*)& pixels[bits * (y * width + x)];
		std::cerr << "point: " << x << " " << y << " is out of bounds\n maximum dimensions are: " << width << "x" << height << "\n";
		return 0;
	}

	pixel* Image::operator()(int x, int y)
	{
		if (y * width + x <= width * height && y * width + x >= 0)
			return (pixel*)& pixels[bits * (y * width + x)];
		std::cerr << "point: " << x << " " << y << " is out of bounds\n maximum dimensions are: " << width << "x" << height << "\n";
		return 0;
	}

	pixel* Image::operator[](int pos) {
		if (pos <= width * height && pos >= 0)
			return (pixel*)& pixels[pos * bits];
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

	int Image::getBits() const{
		return bits;
	}

	void Image::setImage(Image image)
	{
		pixels = image.pixels;
		width = image.width;
		height = image.height;
		bits = image.bits;
	}

	void Image::setImage(std::vector<char> pixels, int width, int height,int bits) {
		this->pixels = pixels;
		this->width = width;
		this->height = height;
		this->bits = bits;
	}


	void Image::setImage(const char* pixels, int width, int height,int bits) {
		this->pixels.assign(0, width * height * bits);
		memcpy(&this->pixels[0], pixels, width * height * bits);
		this->width = width;
		this->height = height;
		this->bits = bits;
	}

}