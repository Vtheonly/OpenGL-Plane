#include "Camera.h"

Camera::Camera() {
    eyeX = 0; eyeY = 3.0f; eyeZ = 10.0f;
    centerX = 0; centerY = 0; centerZ = -5.0f;
}

void Camera::update(int mode, float mouseX, float mouseY, float playerX, float playerY) {
    if (mode == 0) {
        // VUE 1 : Souris (Suit l'avion par derrière)
        eyeX = playerX + (mouseX / 100.0f);
        eyeY = playerY + 2.0f - (mouseY / 100.0f);
        eyeZ = 8.0f;
        centerX = playerX; centerY = playerY; centerZ = -5.0f;
    }
    else if (mode == 1) {
        // VUE 2 : Côté (Suit l'avion de côté)
        eyeX = playerX + 12.0f; eyeY = playerY + 2.0f; eyeZ = 2.0f;
        centerX = playerX; centerY = playerY; centerZ = -5.0f;
    }
    else if (mode == 2) {
        // VUE 3 : Poursuite (Derrière l'avion proche)
        eyeX = playerX; eyeY = playerY + 1.2f; eyeZ = 6.0f;
        centerX = playerX; centerY = playerY; centerZ = -50.0f;
    }
    else {
        // VUE 4 : FRONTALE (L'avion vient vers nous, mais on le suit)
        eyeX = playerX;
        eyeY = playerY + 1.0f;
        eyeZ = -15.0f;
        
        centerX = playerX;
        centerY = playerY;
        centerZ = 100.0f;
    }
}

void Camera::apply() {
    gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0, 1, 0);
}