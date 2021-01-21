#include "tgaimage.h"
#include <math.h>
#include <iostream>
#include <list>
#include <sstream>

constexpr int width = 800;
constexpr int height = 800;

using namespace std;

const TGAColor white = { 255, 255, 255, 255 };
const TGAColor red = { 255, 0, 0, 255};

struct Point{
	int num;
	int x;
	int y;
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
        string line, mot;
        int cpt = -1;
        int cpt2 = -1;
        int pt = -1;
        int pt2 = -1;
        int pt3 = -1;
        int taille = 0;
        int x,y;
        
        //Lecture fichier
        vector<Point> points(0);
        ifstream flux2(name);
        Point point;
        
        if(flux2){
           while(getline(flux2,line)){
           	 istringstream iss(line);
	         while(getline(iss,mot,' ')){
	       	if(mot == "v")	cpt = 0;	//On détecte un point
	       	else{
		       	if(cpt == 0){			//On récupère le x
		       		x = (stof(mot)+1)*300+100;
		       		cpt = 1;
		       	}
		       	else{
			       	if(cpt == 1){			//On récupère le y
			       		y = (stof(mot)+1)*300+100;
			       		cpt = -1;
				       	point.x = x;
				      		point.y = y;
				      		points.push_back(point);
				      		image.set(x,y,white);
				      		taille++;
			       	}
		       	}
	       	}
			 if(mot == "f") cpt2 = 0;			//On détecte un vecteur
			 else{
			 	istringstream iss2(mot);
			 	if(cpt2 == 0){	    			//On récupère le premier point
			 		getline(iss2,mot,'/');
			 		pt = stoi(mot) -1;
			 		cpt2 = 1;
			 	}
			 	else{
			 		if(cpt2 == 1){			//On récupère le second point
			 			getline(iss2,mot,'/');
			 			pt2 = stoi(mot) -1;
			 			cpt2 = 2;
			 		}
			 		else{
			 		      if(cpt2 == 2){
			 		        getline(iss2,mot,'/');
			 			pt3 = stoi(mot) -1;
			 			triangle(points[pt].x, points[pt].y, points[pt2].x, 
			 				 points[pt2].y, points[pt3].x, points[pt3].y, image, white);
			 			cpt2 = -1;
			 		      }
			 		}
			 	}
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
	lecture("/home/maeva/Bureau/Projets/M1 S2/3D/render3d/ressources/african_head/african_head.obj",image);
	image.write_tga_file("img/test.tga");
	return 0;
}





