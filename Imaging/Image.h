#ifndef IMAGE_H
#define IMAGE_H

namespace imgdata{

struct pixel{
    char R,G,B;
};

class Image{
    private:
        int width;
        int height;
        char* pixels;
    public:
        Image(int width, int height);
        Image(char* pixels,int width, int height);
        Image(pixel* pixels,int width, int height);
        Image(const Image* image);
        ~Image();

        pixel* getPixel(int x, int y);
        pixel* operator[](int pos);
        void setImage(char* pixels,int width, int height);

        const pixel* getImage() const;
        const pixel* checkPixel(int x, int y) const;
        
        float getRatio() const;
        int getHeight() const;
        int getWidth() const;

};

}
#endif