#ifndef __CLOUD__
#define __CLOUD__

#include <GL/glut.h>
#include <vector>

// Structure simple pour stocker la position et la taille d'une sphère composant le nuage
struct CloudSphere {
    float x, y, z;
    float radius;
};

class Cloud {
private:
    float x_pos, y_pos, z_pos; // Position centrale du nuage
    std::vector<CloudSphere> spheres; // Liste des sphères qui forment le nuage

public:
    // Constructeur qui génère un nuage aléatoire autour d'une position donnée
    Cloud(float x, float y, float z);

    // Fonction pour dessiner le nuage
    void draw();
};

#endif