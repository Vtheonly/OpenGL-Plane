#ifndef __CAMERA__
#define __CAMERA__

#define GLUT_DISABLE_ATEXIT_HACK
#define GLUT_BUILDING_LIB

#include <GL/glut.h>

class Camera {
private:
    float eyeX, eyeY, eyeZ;
    float centerX, centerY, centerZ;

public:
    Camera();
    // On utilise un entier 'mode' au lieu d'un booléen
    void update(int mode, float mouseX, float mouseY, float playerX, float playerY);
    void apply();
};

#endif