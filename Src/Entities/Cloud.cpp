#define GLUT_DISABLE_ATEXIT_HACK // LA LIGNE MAGIQUE POUR VISUAL STUDIO
#include <GL/glut.h> // OBLIGATOIRE EN PREMIER
#include <cstdlib> 
#include "Cloud.h"

Cloud::Cloud(float x, float y, float z) {
    x_pos = x;
    y_pos = y;
    z_pos = z;

    // Génère entre 5 et 10 sphères pour former un nuage
    int numSpheres = 5 + (rand() % 6);

    for (int i = 0; i < numSpheres; i++) {
        CloudSphere s;
        // Positionne chaque sphère aléatoirement autour du centre du nuage
        // (Décalage entre -1.5 et +1.5 sur X et Z, -0.5 et +0.5 sur Y)
        s.x = ((rand() % 300) / 100.0f) - 1.5f;
        s.y = ((rand() % 100) / 100.0f) - 0.5f;
        s.z = ((rand() % 300) / 100.0f) - 1.5f;

        // Taille aléatoire de la sphère (entre 0.8 et 1.8)
        s.radius = 0.8f + ((rand() % 100) / 100.0f);

        spheres.push_back(s);
    }
}

void Cloud::draw() {
    glPushMatrix();
    glTranslatef(x_pos, y_pos, z_pos);

    glDisable(GL_LIGHTING); // ON COUPE LA LUMIERE pour tester
    glColor3f(1.0f, 1.0f, 1.0f); // BLANC PUR

    for (const auto& s : spheres) {
        glPushMatrix();
        glTranslatef(s.x, s.y, s.z);
        glutSolidSphere(s.radius, 10, 10); // Sphères simples
        glPopMatrix();
    }

    glEnable(GL_LIGHTING); // ON RE-ACTIVE
    glPopMatrix();
}