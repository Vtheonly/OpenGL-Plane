#ifndef ____Obstacle____
#define ____Obstacle____

class Obstacle {
private:
    float x_pos; // Position horizontale
    float y_pos; // Hauteur
    float z_pos; // Profondeur
    int status;  // 0 = En approche, 1 = TraversÃ© avec succÃ¨s, 2 = RatÃ©

public:
    Obstacle(float start_z);
    void update(float speed);
    void draw();

    float getX() const { return x_pos; }
    float getY() const { return y_pos; }
    float getZ() const { return z_pos; }
    int getStatus() const { return status; }
    void setStatus(int s) { status = s; }
};
#endif