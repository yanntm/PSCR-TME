#pragma once

#include "Vec3D.h"

namespace pr {

// A POD that links the camera to the screen point.
struct Ray {
	Vec3D ori; // origine
	Vec3D dest; // destination

	Ray(const Vec3D & ori = Vec3D(0, 0, 0), const Vec3D & dest = Vec3D(0, 0, 0)) : ori(ori), dest(dest) {}

	// returns a unit vector colinear to this Ray
	Vec3D direction() const { return (dest - ori).normalize(); }
};

} /* namespace pr */
