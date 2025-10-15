#pragma once

#include "Vec3D.h"
#include "Color.h"
#include "Ray.h"
#include "util/mtrand.h"
#include <limits>
#include <algorithm>
#include <cmath>

namespace pr {

// une sphere 3D, coloree, ayant un radius et une position pour son centre
class Sphere {
	Vec3D centre;
	double radius;
	Color color;
public:
	Sphere(const Vec3D & centre=Vec3D(0,0,0), double radius=10.0, const Color & c=Colors::white):centre(centre),radius(radius),color(c) {}

	// return distance to closest intersection with the given object
	double intersects (const Ray & ray) const {
		// trouver vecteur unitaire l donnant la direction
		Vec3D l = ray.direction();
		// trouver le vector liant le centre de la sphere a l'obs
		Vec3D oc = ray.ori - centre;
		// Combien le polynome (degre 2) a-t-il de solutions ? a x^2 + b x + c = 0
		double a = 1.0; // longueur de l au carre
		double b = 2.0 * (l & oc); // produit scalaire note & note &
		double c = (oc & oc) - (radius*radius) ; // carre de oc - carre du radius
		double discriminant = b*b - 4.0*a*c;
		if (discriminant < 0) {
			// pas d'inter
			return std::numeric_limits<double>::max();
		} else if (discriminant == 0) {
			// tangeant au cercle
			double t = -b / (2.0 * a);
			return t > 0 ? t : std::numeric_limits<double>::max();
		} else {
			// on veut la distance la plus petite a l'obs = la plus petite racine positive
			double r1 = (-b + sqrt(discriminant)) / (2.0*a);
			double r2 = (-b - sqrt(discriminant)) / (2.0*a);
			double min_t = std::numeric_limits<double>::max();
			if (r1 > 0) min_t = std::min(min_t, r1);
			if (r2 > 0) min_t = std::min(min_t, r2);
			return min_t;
		}

	}
	// normale a la sphere au point d'intersection (sur la sphere)
	Vec3D getNormale(const Vec3D & intersection) const {
		// simplement le vecteur du centre au point d'intersection 
		// normalise donc divise par le radius vu que le point est sur la sphere
		return (intersection - centre) / radius;
	}

	const Color & getColor () const {return color ;}

	static Sphere random() {
		return Sphere(Vec3D(mtrand(-200, 200), mtrand(-200, 200), 300 + mtrand(-100, 200)),
					  mtrand(3, 34), Colors::random());
	}
};

} /* namespace pr */
