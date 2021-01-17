#include "tgaimage.h"
#include <math.h>
#include <iostream>
#include <list>
#include <sstream>

constexpr int width = 100;
constexpr int height = 100;

using namespace std;

const TGAColor white = { 255, 255, 255, 255 };
const TGAColor red = { 255, 0, 0, 255};

typedef struct Point Point;
struct Point{
	int num;
	float x;
	float y;
};

typedef struct Element Element;
struct Element{
	Point point;
	Element *suivant;
};

typedef struct List List;
struct List{
	Element *premier;
};

void ligne(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor couleur){
	bool steep = false;
	if(std::abs(x0-x1) < std::abs(y0-y1)){
		std::swap(x0,y0);
		std::swap(x1,y1);
		steep = true;
	}
	if(x0 > x1){
		std::swap(x0,x1);
		std::swap(y0,y1);
	}
	
	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror2 = std::abs(dy)*2;
	int error2 = 0;
	int y = y0;
	for(int x=x0; x<=x1; x++){
		if(steep)	image.set(y,x,couleur);
		else		image.set(x,y,couleur);
		error2 += derror2;
		if(error2 > dx){
			y += (y1>y0?1:-1);
			error2 -= dx*2;
		}
	}

}


void triangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &image, TGAColor couleur){
	ligne(x0,y0,x1,y1,image,couleur);
	ligne(x1,y1,x2,y2,image,couleur);
	ligne(x0,y0,x2,y2,image,couleur);
}

void lecture(string name,TGAImage &image){
        List liste;
        string ligne, mot;
        int cpt = -1;
        float x,y,z;
        
        //Lecture fichier
        ifstream flux(name);
        if(flux){
           while(getline(flux,ligne)){
           	 istringstream iss(ligne);
	         while(getline(iss,mot,' ')){
	         	cout << mot << endl;
	       	//if(mot == "v")	cpt = 0;
	       	if(cpt == 0)	cpt = 1;
	       	if(cpt == 1){
	       		x = stof(mot);
	       		cout << x << endl;
	       		cpt = 2;
	       	}
	       	if(cpt == 2){
	       		y = stof(mot);
	       		cout << y << endl;
	       		cpt = 3;
	       	}
	       	if(cpt == 3){
	       		z = stof(mot);
	       		cout << z << endl;
	       		cpt = -1;
	       		cout << "Ajout point:" << x << y << z << endl;
	       	}
	         }
           }
        }
        else{
           cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << endl;
        }
}
    

int main(){
	TGAImage image(width, height, TGAImage::RGB);
	image.write_tga_file("img/test.tga");
	lecture("/home/maeva/Bureau/M1/S2/3D/render3d/ressources/african_head/african_head.obj",image);
	return 0;
}





