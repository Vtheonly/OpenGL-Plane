#ifndef ____Player____
#define ____Player____

#define GLUT_DISABLE_ATEXIT_HACK
#define GLUT_BUILDING_LIB

#include <GL/glut.h>

class Player {
private:
    // Positions
    float x_pos, y_pos, z_pos;

    // Rotations (Aeronautical angles)
    float pitch; // X-axis (Nose up/down)
    float yaw;   // Y-axis (Left/right turn)
    float roll;  // Z-axis (Banking)

    // Speed & Nitro logic
    float speed;
    float nitro;
    int x_press_count;

    // Geometry
    int slices, stacks;
    float prop_angle;

    //texture rami
    GLuint fuselageTexture;
    bool useFuselageTexture;

public:
    Player();
    void moveUp(float val);
    void moveDown(float val);
    void turnLeft(float val);
    void turnRight(float val);
    void hitNitro();
    void throttleUp(float delta);   // NOUVEAU
    void throttleDown(float delta); // NOUVEAU
    void setDetail(int delta);

    void update(float globalTimeStep);
    void draw();

    //texture rami
    void setFuselageTexture(GLuint tex);

    // Getters for Stats
    float getSpeed() { return speed + nitro; }
    float getPitch() { return pitch; }
    float getRoll() { return roll; }
    float getYaw() { return yaw; }  // rami
    float getX() { return x_pos; }
    float getY() { return y_pos; }
    float getZ() { return z_pos; }
    float getNitro() { return nitro; }
};

#endif