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
	int x;
	int y;
	int z;
};

struct Vecteur{
	double x;
	double y;
	double z;
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
		if(steep){
			image.set(y,x,couleur);
		}
		else{
			image.set(x,y,couleur);
		}
		error2 += derror2;
		if(error2 > dx){
			y += (y1>y0?1:-1);
			error2 -= dx*2;
		}
	}

}

Vecteur cross(const Vecteur &v1, const Vecteur &v2){
	Vecteur v;
	v.x = v1.y*v2.z - v1.z*v2.y;
	v.y = v1.z*v2.x - v1.x*v2.z;
	v.z = v1.x*v2.y - v1.y*v2.x;
	return v;
}


bool appartient(int x0, int y0, int x1, int y1, int x2, int y2, int x, int y){
	Vecteur v1,v2,u;
	
	v1.x = x2 - x0;
	v1.y = x1 - x0;
	v1.z = x0 - x;
	v2.x = y2 - y0;
	v2.y = y1 - y0;
	v2.z = y0 - y;
	
	u = cross(v1,v2);
	if(std::abs(u.z)<1)	return false;
	if( (1.f-(u.x+u.y)/u.z <0) || (u.y/u.z <0) || (u.x/u.z <0))	return false;
	return true;
}

int max(int a, int b, int c){
	if(a>b){
		if(a>c)	return a;
		return c;
	}
	return b;
}

int min(int a, int b, int c){
	if(a<b){
		if(a<c)	return a;
		return c;
	}
	return b;
}

void fill_triangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &image, TGAColor couleur){
	for(int i = min(x0,x1,x2)-50; i <= max(x0,x1,x2)+50 ; i++){
		for(int j = min(y0,y1,y2)-50; j <= max(y0,y1,y2)+50; j++){
			if(appartient(x0,y0,x1,y1,x2,y2,i,j)){
				image.set(i,j,couleur);
			}
		}
	}
}

void draw_triangle(int x0, int y0, int z0, int x1, int y1, int z1, int x2, int y2, int z2, TGAImage &image){
	Vecteur v1,v2;
	v1.x = x1 - x0;	v2.x = x2 - x0;
	v1.y = y1 - y0;	v2.y = y2 - y0;
	v1.z = z1 - z0;	v2.z = z2 - z0;
	
	Vecteur n = cross(v1,v2);
	float longueur = sqrt((n.x*n.x)+(n.y*n.y)+(n.z*n.z));
	n.x = n.x/longueur;
	n.y = n.y/longueur;
	n.z = n.z/longueur;
	
	Vecteur lum;
	lum.x = lum.y = 0;
	lum.z = 1;
	
	float intensity = n.x*lum.x + n.y*lum.y + n.z*lum.z;	
	
	if(intensity > 0)	fill_triangle(x0,y0,x1,y1,x2,y2,image,TGAColor(intensity*255,intensity*255,intensity*255,255));

}


void lecture(string name,TGAImage &image){
        string line, mot;
        int x,y,z,pt1,pt2,pt3;
        
        //Lecture fichier
        vector<Point> points(0);
        ifstream flux2(name);
        Point point;
        
        if(flux2){
           while(flux2){
         	flux2 >> mot;
           	if(mot == "v"){
           		//On récupère le x
           		flux2 >> mot;
           		x = (stof(mot)+1)*300+100;
           		
           		//On récupère le y
           		flux2 >> mot;
           		y = (stof(mot)+1)*300+100;
           		
           		//On récupère le z
           		flux2 >> mot;
           		z = (stof(mot)+1)*300+100;
           		
           		//On enregistre le point
           		point.x = x;
           		point.y = y;
           		point.z = z;
           		points.push_back(point);
           		
           	}
           	if(mot == "f"){
           		
           		//Premier point
           		flux2 >> mot;
           		istringstream iss1(mot);
           		getline(iss1,mot,'/');
			pt1 = stoi(mot) -1;
			
			//Deuxième point
			flux2 >> mot;
			istringstream iss2(mot);
           		getline(iss2,mot,'/');
			pt2 = stoi(mot) -1;
			
			//Troisième point
			flux2 >> mot;
			istringstream iss3(mot);
           		getline(iss3,mot,'/');
			pt3 = stoi(mot) -1;
			
			//On dessine le triangle
			draw_triangle(points[pt1].x, points[pt1].y, points[pt1].z, points[pt2].x,points[pt2].y, points[pt2].z, points[pt3].x, points[pt3].y, points[pt3].z, image);
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





