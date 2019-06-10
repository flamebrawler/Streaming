#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <stdio.h>
namespace imgdata{

struct pixel{
    unsigned char R,G,B;
};

class Image{
    private:
        int width;
        int height;
		int bytes;
        
    public:
		std::vector<char> pixels;
        Image(int width, int height,int bytes = 3);
        Image(const char* pixels,int width, int height,int bytes = 3);
        Image(pixel* pixels,int width, int height,int bytes = 3);
		Image(std::vector<char> pixels, int width, int height,int bytes = 3);
        Image(const Image* image);
        ~Image();

        pixel* getPixel(int x, int y);
        pixel* operator()(int x,int y);
		pixel* operator[](int pos);
		void setImage(Image image);
        void setImage(std::vector<char> pixels,int width, int height,int bytes =3);
		void setImage(const char* pixels, int width, int height, int bytes = 3);

        const pixel* getImage() const;
        const pixel* checkPixel(int x, int y) const;
        
        float getRatio() const;
        int getHeight() const;
        int getWidth() const;
		int getBytes() const;
};

}
#endif