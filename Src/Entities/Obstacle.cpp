#define GLUT_DISABLE_ATEXIT_HACK
#define GLUT_BUILDING_LIB

#include "Obstacle.h"
#include <cstdlib> 
#include <GL/glut.h>

Obstacle::Obstacle(float start_z) {
    z_pos = start_z;
    // Génère une position aléatoire au démarrage
    // On agrandit la zone pour forcer à se déplacer
    x_pos = (rand() % 2000 / 100.0f) - 10.0f; // Entre -10 et 10
    y_pos = (rand() % 1000 / 100.0f) - 5.0f;  // Entre -5 et +5
    status = 0;
}

void Obstacle::update(float speed) {
    z_pos += speed;

    // Si le cercle dépasse largement la caméra, on le remet au fond
    if (z_pos > 5.0f) {
        z_pos = -150.0f;
        x_pos = (rand() % 2000 / 100.0f) - 10.0f;
        y_pos = (rand() % 1000 / 100.0f) - 5.0f;
        status = 0; // Remise à zéro du statut
    }
}

void Obstacle::draw() {
    glPushMatrix();

    // On place le cercle à ses coordonnées X, Y et Z
    glTranslatef(x_pos, y_pos, z_pos);

    // Choix de la couleur selon le statut (0: Bleu, 1: Vert (Succès), 2: Rouge (Raté))
    GLfloat diffuse_color[4];
    if (status == 1) {
        diffuse_color[0] = 0.0f; diffuse_color[1] = 1.0f; diffuse_color[2] = 0.0f; diffuse_color[3] = 1.0f;
    } else if (status == 2) {
        diffuse_color[0] = 1.0f; diffuse_color[1] = 0.0f; diffuse_color[2] = 0.0f; diffuse_color[3] = 1.0f;
    } else {
        diffuse_color[0] = 0.0f; diffuse_color[1] = 0.5f; diffuse_color[2] = 1.0f; diffuse_color[3] = 1.0f;
    }
    glColor3f(diffuse_color[0], diffuse_color[1], diffuse_color[2]);

    glPushMatrix();

    GLfloat blanc_spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, blanc_spec);

    glutWireTorus(0.2, 2.0, 15, 30);
    glPopMatrix();
    glutWireTorus(0.2, 2.0, 15, 30);

    glPopMatrix();
}