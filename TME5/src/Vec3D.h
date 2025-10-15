#pragma once

#include <cmath>

namespace pr {

// un Vecteur 3D, ou un Point 3D selon l'usage qui en est fait.
// on l'appelle Vec3D car il propose les operations vectorielles usuelles
class Vec3D {
private:
	const double x;
	const double y;
	const double z;

public:
	Vec3D(double x=0,double y=0,double z=0):x(x),y(y),z(z) {}

	// somme de vecteurs
	Vec3D operator+ (const Vec3D & o) const { return Vec3D(x+o.x, y+o.y, z+o.z); }
	// difference utile pour calculer un vecteur a partir de deux points
	Vec3D operator- (const Vec3D & o) const { return Vec3D(x-o.x, y-o.y, z-o.z); }
	// produit par un scalaire, rallonger ou reduire
	Vec3D operator* (double d) const { return Vec3D(x*d, y*d, z*d); }
	// produit vectoriel (cross product)
	Vec3D operator* (const Vec3D & r) const { return Vec3D(y*r.z - z*r.y, z*r.x - x*r.z, x*r.y - y*r.x); }
	// division par un scalaire (utile pour normaliser)
	Vec3D operator/ (double d) const { return Vec3D(x/d, y/d, z/d); }

	// produit scalaire (dot product) donne l'angle d'incidence entre deux vecteurs
	double operator&(const Vec3D & r) const { return x*r.x + y*r.y + z*r.z ; }

	double length() const { return std::sqrt(x*x + y*y + z*z); } // norme/longueur du vecteur
	Vec3D normalize() const { // vector de longueur 1
		double l = length();
		if (l == 0) return *this;
		return (*this) / l;
	}
};

// produit par un scalaire (rallonger/reduire) mais scalaire a gauche 3 * v
inline Vec3D operator* (double d, const Vec3D & v) { return v * d; }

} /* namespace pr */
