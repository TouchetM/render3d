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
	int num_texture;
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

void fill_triangle(Point pt1, Point pt2, Point pt3, TGAImage &image, TGAColor couleur){
	for(int i = min(pt1.x,pt2.x,pt3.x)-50; i <= max(pt1.x,pt2.x,pt3.x)+50 ; i++){
		for(int j = min(pt1.y,pt2.y,pt3.y)-50; j <= max(pt1.y,pt2.y,pt3.y)+50; j++){
			if(appartient(pt1.x,pt1.y,pt2.x,pt2.y,pt3.x,pt3.y,i,j)){
				image.set(i,j,couleur);
			}
		}
	}
}

void draw_triangle(Point pt1, Point pt2, Point pt3, vector<Point> coord_textures, TGAImage &image){
	
	//Lumière
	Vecteur v1,v2;
	v1.x = pt2.x - pt1.x;	v2.x = pt3.x - pt1.x;
	v1.y = pt2.y - pt1.y;	v2.y = pt3.y - pt1.y;
	v1.z = pt2.z - pt1.z;	v2.z = pt3.z - pt1.z;
	
	Vecteur n = cross(v1,v2);
	float longueur = sqrt((n.x*n.x)+(n.y*n.y)+(n.z*n.z));
	n.x = n.x/longueur;
	n.y = n.y/longueur;
	n.z = n.z/longueur;
	
	Vecteur lum;
	lum.x = lum.y = 0;
	lum.z = 1;
	
	float intensity = n.x*lum.x + n.y*lum.y + n.z*lum.z;	
	
	//Textures
	//TGAImage textures(width, height, TGAImage::RGB);
	//textures.read_tga_file("/home/maeva/Bureau/Projets/M1 S2/3D/render3d/ressources/african_head/african_head_diffuse.tga");
	//textures.get(coord_textures[pt1.num_texture].x, coord_textures[pt1.num_texture].y);
	
	//Dessin triangle
	if(intensity > 0)	fill_triangle(pt1,pt2,pt3,image,TGAColor(intensity*255,intensity*255,intensity*255,255));

}


void lecture(string name, TGAImage &image){
        string line, mot;
        int x,y,z,pt1,pt2,pt3,num;
        double x2,y2,z2;
        
        //Lecture fichier
        vector<Point> points(0);
        vector<Point> coord_textures(0);
        ifstream flux2(name);
        Point point, texture;
        
        if(flux2){
           while(flux2){
         	flux2 >> mot;
           	if(mot == "v"){	//Points
           	
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
           	if(mot == "vt"){	//Textures
           	
           		//On récupère le x
           		flux2 >> mot;
           		x2 = stof(mot);
           	
           		//On récupère le y
           		flux2 >> mot;
           		y2 = stof(mot);
           		
           		//On récupère le z
           		flux2 >> mot;
           		z2 = stof(mot);
           		
           		//On enregistre les coordonnées de la texture
           		texture.x = x2;
           		texture.y = y2;
           		texture.z = z2;
           		coord_textures.push_back(texture);
           	}
           	if(mot == "f"){	//Triangles
           		
           		//Premier point
           		flux2 >> mot;
           		istringstream iss1(mot);
           		getline(iss1,mot,'/');
			pt1 = stoi(mot) -1;
			
			//Texture point 1
			getline(iss1,mot,'/');
			num = stoi(mot) - 1;
			points[pt1].num_texture = num;
			
			//Deuxième point
			flux2 >> mot;
			istringstream iss2(mot);
           		getline(iss2,mot,'/');
			pt2 = stoi(mot) -1;
			
			//Texture point 2
			getline(iss2,mot,'/');
			num = stoi(mot) - 1;
			points[pt2].num_texture = num;
			
			//Troisième point
			flux2 >> mot;
			istringstream iss3(mot);
           		getline(iss3,mot,'/');
			pt3 = stoi(mot) -1;
			
			//Texture point 3
			getline(iss3,mot,'/');
			num = stoi(mot) - 1;
			points[pt3].num_texture = num;
			
			//On dessine le triangle
			draw_triangle(points[pt1],points[pt2],points[pt3], coord_textures, image);
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





