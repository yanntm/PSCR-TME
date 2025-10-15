#pragma once

#include "Scene.h"
#include "Vec3D.h"
#include "Sphere.h"
#include "Color.h"
#include "util/mtrand.h"

namespace pr {

// construit une scene aleatoire avec spheres et lumieres
Scene buildRandomScene(int width, int height, int num_spheres = 250) {
	Scene scene(width, height);
	// Nombre de spheres (rend le probleme plus dur)
	for (int i = 0; i < num_spheres; i++) {
		scene.add(Sphere::random());
	}
	// quelques spheres de plus pour ajouter du gout a la scene
	scene.add(Sphere(Vec3D(50, 50, 40), 15.0, Colors::red));
	scene.add(Sphere(Vec3D(100, 20, 50), 55.0, Colors::blue));

	// lumieres
	scene.addLight(Vec3D(50, 50, -50));
	scene.addLight(Vec3D(50, 50, 120));
	scene.addLight(Vec3D(200, 0, 120));

	return scene;
}

} // namespace pr