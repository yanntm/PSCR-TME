#pragma once

#include "Vec3D.h"
#include "Sphere.h"
#include <vector>

namespace pr {

// une scene basique, la camera est fixe
// porte un ensemble d'objets, des spheres a ce stade
class Scene {
	// les objets
	std::vector<Sphere> objects;
	// la camera 
	Vec3D camposition;
	// son angle de vue, vecteur unitaire dans la direction de la vue
	Vec3D viewDir;
	// suppose fixe actuellement

	// largeur de l'ecran en pixels : controle la resolution
	// pas la largeur du champ
	int width;
	int height;
	// les lumieres
	std::vector<Vec3D> lights;
public :
	// les points d'un ecran 3D 
	using screen_t = std::vector<std::vector<Vec3D>>;
private :
	// l'ecran, calcule a partir de la position et l'angle
	std::vector<std::vector<Vec3D> > screen;
public:
	// une scene, on donne seulement la resolution
	Scene(int width=800, int height=600)
		// on positionne en dur les positions et champ de vue
		:camposition(0,0,-1000),viewDir(0,0,1),width(width),height(height),screen() {
		// distance from camera to screen
		double distance = 1000.0;
		Vec3D screenCenter = camposition + distance * viewDir;
		// up vector
		Vec3D up0(0, 1, 0);
		// right vector
		Vec3D right = (viewDir * up0).normalize();
		// re-orthogonalize up
		Vec3D up = (right * viewDir).normalize();

		double W = 400;
		double H = (W * height) / width;
		double pixelSizeX = W / width;
		double pixelSizeY = H / height;

		// compute screen points
		screen.resize(height);
		for (int y = 0; y < height; y++) {
			screen[y].reserve(width);
			for (int x = 0; x < width; x++) {
				Vec3D offset = (x - width / 2.0) * pixelSizeX * right + (y - height / 2.0) * pixelSizeY * up;
				Vec3D pixelPos = screenCenter + offset;
				screen[y].emplace_back(pixelPos);
			}
		}
	}
	// ajoute un objet a la scene
	void add (const Sphere & s) {
		objects.push_back(s);
	}
	// ajoute une lumiere a la scene
	void addLight (const Vec3D & l) {
		lights.push_back(l);
	}

	// les points de l'ecran par lesquels passent les rayons
	const screen_t & getScreenPoints() const { return screen; }
	// resolution en pixels
	int getHeight() const { return height ;}
	int getWidth() const { return width ;}
	// la camera (const)
	const Vec3D & getCameraPos() const { return camposition ; }

	// return the index of the closest object in the scene that intersects "ray"
	// or -1 if the ray does not intersect any object.
	int findClosestInter(const Ray & ray) const {
		auto minz = std::numeric_limits<double>::max();
		int targetSphere = -1;
		int index = 0;
		for (const auto & obj : objects) {
			// rend la distance de l'objet a la camera
			auto zinter = obj.intersects(ray);
			// si intersection plus proche  ?
			if (zinter < minz) {
				minz = zinter;
				targetSphere = index;
			}
			index++;
		}
		return targetSphere;
	}

	// get an object by index
	const Sphere& getObject(int index) const { return objects[index]; }

	// Calcule l'angle d'incidence du rayon à la sphere, cumule l'éclairage des lumières
	// En déduit la couleur d'un pixel de l'écran.
	Color computeColor(const Sphere & obj, const Ray & ray) const {
		Color finalcolor = obj.getColor();

		// calcul du rayon et de sa normale a la sphere
		// on prend le vecteur de la camera vers le point de l'ecran (dest - origine)
		// on le normalise a la longueur 1, on multiplie par la distance à l'intersection
		Vec3D rayInter = (ray.dest - ray.ori).normalize() * obj.intersects(ray);
		// le point d'intersection
		Vec3D intersection = rayInter + camposition;
		// la normale a la sphere au point d'intersection donne l'angle pour la lumiere
		Vec3D normal = obj.getNormale(intersection);
		// le niveau d'eclairage total contribue par les lumieres 0 sombre 1 total lumiere
		double dt = 0;
		// modifier par l'eclairage la couleur
		for (const auto & light : lights) {
			// le vecteur de la lumiere au point d'intersection
			Vec3D tolight = (light - intersection);
			// si on est du bon cote de la sphere, i.e. le rayon n'intersecte pas avant de l'autre cote
			if (obj.intersects(Ray(light,intersection)) >= tolight.length() - 1e-4 ) {   //  epsilon 1e-4 for double issues
				dt += tolight.normalize() & normal ; // l'angle (scalaire) donne la puissance de la lumiere reflechie
			}
		}
		// eclairage total
		finalcolor = finalcolor * dt + finalcolor * 0.2; // *0.2 = lumiere speculaire ambiante

		return finalcolor;
	}
};

} /* namespace pr */
