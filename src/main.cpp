#include "tgaimage.h"

constexpr int width = 100;
constexpr int height = 100;

int main(){
	TGAImage image(width, height, TGAImage::RGB);
	image.write_tga_file("img/test.tga");
	return 0;
}

