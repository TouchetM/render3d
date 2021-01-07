#include "tgaimage.h"

constexpr int width = 100;
constexpr int height = 100;

const TGAColor white = { 255, 255, 255, 255 };

void triangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &image, TGAColor couleur){
	image.set(x0, y0, couleur);
	image.set(x1, y1, couleur);
	image.set(x2, y2, couleur);	
}

int main(){
	TGAImage image(width, height, TGAImage::RGB);
	triangle(5,10,10,50,20,30,image,white);
	image.write_tga_file("img/test.tga");
	return 0;
}

