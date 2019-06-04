#include "Image.h"
#include <iostream>

namespace imgdata {

	Image::Image(int width, int height)
	{
		this->pixels = new char[width * height * 3];
		for (int i = 0; i < width * height * 3; i++)
			pixels[i] = 0;
		this->width = width;
		this->height = height;
	}

	Image::Image(char* pixels, int width, int height)
	{
		this->pixels = pixels;
		this->width = width;
		this->height = height;
	}

	Image::Image(pixel* pixels, int width, int height)
	{
		this->pixels = (char*)pixels;
		this->width = width;
		this->height = height;
	}

	Image::Image(const Image* image)
	{
		for (int i = 0; i < image->width * image->height; i++)
		{
			pixels[i] = image->pixels[i];
		}
		this->width = image->width;
		this->height = image->height;
		pixels = 0;
	}

	Image::~Image()
	{
		//if (pixels)
		//	delete pixels;
	}

	const pixel* Image::getImage() const
	{
		return (pixel*)pixels;
	}

	const pixel* Image::checkPixel(int x, int y) const
	{
		if (y * width + x <= width * height && y * width + x >= 0)
			return (pixel*)& pixels[3 * (y * width + x)];
		std::cerr << "point: " << x << " " << y << " is out of bounds\n maximum dimensions are: " << width << "x" << height << "\n";
		return 0;
	}
	pixel* Image::getPixel(int x, int y)
	{
		if (y * width + x <= width * height && y * width + x >= 0)
			return (pixel*)& pixels[3 * (y * width + x)];
		std::cerr << "point: " << x << " " << y << " is out of bounds\n maximum dimensions are: " << width << "x" << height << "\n";
		return 0;
	}

	pixel* Image::operator[](int pos) {
		if (pos <= width * height && pos >= 0)
			return (pixel*)& pixels[pos * 3];
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

	void Image::setImage(char* pixels, int width, int height) {
		//if(this->pixels)
		  //  delete this->pixels;
		this->pixels = pixels;
		this->width = width;
		this->height = height;
	}

}