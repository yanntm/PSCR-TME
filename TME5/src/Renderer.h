#pragma once

#include "Scene.h"
#include "Image.h"
#include "Ray.h"

namespace pr {

// Classe pour rendre une scène dans une image
class Renderer {
public:
    // Rend la scène dans l'image
    void render(const Scene& scene, Image& img) {
        // les points de l'ecran, en coordonnées 3D, au sein de la Scene.
        // on tire un rayon de l'observateur vers chacun de ces points
        const Scene::screen_t& screen = scene.getScreenPoints();

        // pour chaque pixel, calculer sa couleur
        for (int x = 0; x < scene.getWidth(); x++) {
            for (int y = 0; y < scene.getHeight(); y++) {
                // le point de l'ecran par lequel passe ce rayon
                auto& screenPoint = screen[y][x];
                // le rayon a inspecter
                Ray ray(scene.getCameraPos(), screenPoint);

                int targetSphere = scene.findClosestInter(ray);

                if (targetSphere == -1) {
                    // keep background color
                    continue;
                } else {
                    const Sphere& obj = scene.getObject(targetSphere);
                    // pixel prend la couleur de l'objet
                    Color finalcolor = scene.computeColor(obj, ray);
                    // mettre a jour la couleur du pixel dans l'image finale.
                    img.pixel(x, y) = finalcolor;
                }
            }
        }
    }
};

} // namespace pr