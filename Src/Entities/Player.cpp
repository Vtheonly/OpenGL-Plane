#define GLUT_DISABLE_ATEXIT_HACK
#define GLUT_BUILDING_LIB
#include "Player.h"
#include <algorithm>
#include <cmath>
#include <GL/glut.h>

// Constructeur : Initialisation des variables au démarrage
Player::Player() {
    x_pos = 0.0f; y_pos = 0.0f; z_pos = 0.0f;
    pitch = 0.0f; yaw = 0.0f; roll = 0.0f;
    speed = 0.1f; nitro = 0.0f;
    slices = 16; stacks = 16; prop_angle = 0.0f;
    // rami texture
    fuselageTexture = 0;
    useFuselageTexture = false;
}

// Fonctions de mouvement
void Player::moveUp(float val) { pitch += val * 2.0f; }
void Player::moveDown(float val) { pitch -= val * 2.0f; }
void Player::turnLeft(float val) { yaw += val * 1.5f;  roll += val * 2.5f; }
void Player::turnRight(float val) { yaw -= val * 1.5f;  roll -= val * 2.5f; }

// Activation du Boost (Nitro)
void Player::hitNitro() {
    nitro += 0.8f;  // Boost temporaire puissant
    if (nitro > 5.0f) nitro = 5.0f;
}

// Manette des gaz (Throttle)
void Player::throttleUp(float delta) {
    speed += delta;
    if (speed > 1.5f) speed = 1.5f; // Vitesse max
}

void Player::throttleDown(float delta) {
    speed -= delta;
    if (speed < 0.05f) speed = 0.05f; // Vitesse min (pour ne pas reculer)
}

// Mise à jour de la logique à chaque image
void Player::update(float globalTimeStep) {
    // Rotation de l'hélice (plus rapide si nitro active)
    prop_angle += 30.0f * globalTimeStep * (1.0f + nitro * 2.0f);

    // VRAI DEPLACEMENT : L'inclinaison (roll) et le tangage (pitch) modifient la position X et Y
    x_pos -= (roll / 30.0f) * globalTimeStep * (speed * 10.0f);
    y_pos += (pitch / 30.0f) * globalTimeStep * (speed * 10.0f);

    // On limite la position pour ne pas sortir infiniment de la carte
    if (x_pos > 25.0f) x_pos = 25.0f;
    if (x_pos < -25.0f) x_pos = -25.0f;
    if (y_pos > 15.0f) y_pos = 15.0f;
    if (y_pos < -10.0f) y_pos = -10.0f;

    // Vibration effect when nitro is active
    if (nitro > 0.5f) {
        x_pos += ((rand() % 100) / 1000.0f - 0.05f) * nitro;
        y_pos += ((rand() % 100) / 1000.0f - 0.05f) * nitro;
    }

    // Amortissement : l'avion revient doucement à sa position stable
    roll *= 0.95f;   // L'inclinaison revient à 0
    pitch *= 0.98f;  // Le nez revient à l'horizontale
    yaw *= 0.95f;
    nitro *= 0.97f;  // Nitro s'épuise (légèrement plus rapide)
    
    // Friction naturelle : La vitesse revient doucement vers une vitesse de croisière (0.1)
    if (speed > 0.15f) speed *= 0.99f; 
    else if (speed < 0.1f) speed += 0.001f;
}
void Player::setDetail(int delta) {
    slices += delta;
    stacks += delta;
    if (slices < 3) slices = 3;
    if (stacks < 3) stacks = 3;
}

//rami
void Player::setFuselageTexture(GLuint tex) {
    fuselageTexture = tex;
    useFuselageTexture = true;
}

// rami
void drawBlinkingSideLight(float x, float y, float z) {
    // Temps écoulé en millisecondes
    int time = glutGet(GLUT_ELAPSED_TIME);

    // Change de couleur toutes les 500 ms
    bool redPhase = (time / 500) % 2 == 0;

    GLfloat emissionRed[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    GLfloat emissionBlue[] = { 0.0f, 0.2f, 1.0f, 1.0f };
    GLfloat noEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    GLfloat* currentEmission = redPhase ? emissionRed : emissionBlue;

    glPushMatrix();

    glDisable(GL_COLOR_MATERIAL);

    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, currentEmission);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, currentEmission);

    glTranslatef(x, y, z);
    glutSolidSphere(0.15, 20, 20);

    // Remettre l'émission à zéro pour ne pas rendre tout l'avion lumineux
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, noEmission);
    glEnable(GL_COLOR_MATERIAL);

    glPopMatrix();
}




void Player::draw() {
    glPushMatrix();

    // 1. Réflexion diffuse : La couleur de base sous la lumière (Blanc pur pour le metal)
    GLfloat mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    // 2. Réflexion spéculaire : La tache de brillance (blanche et intense)
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    // 3. Brillance (Shininess)
    GLfloat mat_shininess[] = { 80.0f };

    // On applique ces propriétés au matériau de l'objet
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // 1. Position et Rotation globale de l'avion
    glTranslatef(x_pos, y_pos, 0.0f);
    glRotatef(yaw, 0, 1, 0);   // Direction (Lacet)
    glRotatef(pitch, 1, 0, 0); // Inclinaison nez (Tangage)
    glRotatef(roll, 0, 0, 1);  // Inclinaison ailes (Roulis)

    GLUquadric* quad = gluNewQuadric();

    // rami texture
    gluQuadricTexture(quad, GL_TRUE);
    gluQuadricNormals(quad, GLU_SMOOTH);

    // --- APPLICATION DE LA TEXTURE METAL SUR TOUT L'AVION ---
    if (useFuselageTexture && fuselageTexture != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, fuselageTexture);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        // Génération automatique des coordonnées UV pour les cubes, cones, etc.
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        
        // Planes pour eviter un étirement bizarre
        GLfloat s_plane[] = { 2.0f, 0.0f, 0.0f, 0.0f };
        GLfloat t_plane[] = { 0.0f, 0.0f, 2.0f, 0.0f };
        glTexGenfv(GL_S, GL_OBJECT_PLANE, s_plane);
        glTexGenfv(GL_T, GL_OBJECT_PLANE, t_plane);
    }

    // On force la couleur à BLANC pour que la texture métal ne soit pas teinte
    glColor3f(1.0f, 1.0f, 1.0f);

    // 2. FUSELAGE
    glPushMatrix();
    glTranslatef(0, 0, -1.5f);
    gluCylinder(quad, 0.4, 0.4, 3.0, slices, stacks);
    gluDisk(quad, 0, 0.4, slices, 1);
    glTranslatef(0, 0, 3.0f);
    gluDisk(quad, 0, 0.4, slices, 1);
    glPopMatrix();

    // 3. NEZ (Cône orange)
    glPushMatrix();
    glTranslatef(0, 0, -1.5f);
    glRotatef(180, 0, 1, 0);
    glutSolidCone(0.4, 1.0, slices, stacks);
    glPopMatrix();

    // 4. HÉLICE (En rotation)
    glPushMatrix();
    glTranslatef(0, 0, -2.55f);
    glRotatef(prop_angle, 0, 0, 1);
    glutSolidSphere(0.15, 10, 10); // Moyeu de l'hélice
    glScalef(0.1f, 1.4f, 0.02f);    // Forme des pales
    glutSolidCube(1.0);
    glPopMatrix();

    // 5. AILES PRINCIPALES (Vertes)
    glPushMatrix();
    glTranslatef(0, 0, -0.2f);
    glScalef(5.0f, 0.08f, 1.3f);
    glutSolidCube(1.0);
    glPopMatrix();

    // 6. MOTEURS
    float ex[] = { -1.6f, -0.8f, 0.8f, 1.6f };
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(ex[i], -0.25f, -0.8f);
        gluCylinder(quad, 0.18, 0.18, 1.0, 10, 10);
        gluDisk(quad, 0, 0.18, 10, 1);
        glTranslatef(0, 0, 1.0f);
        gluDisk(quad, 0, 0.18, 10, 1);
        glPopMatrix();
    }
  
    // 8. ASSEMBLAGE DE LA QUEUE (L'arrière)
    // DÉRIVE VERTICALE 
    glPushMatrix();
    glTranslatef(0.0f, 0.6f, 1.2f);      
    glRotatef(-25.0f, 1.0f, 0.0f, 0.0f); 
    glScalef(0.1f, 1.0f, 0.7f);          
    glutSolidCube(1.0);
    glPopMatrix();

    // STABILISATEURS HORIZONTAUX
    glPushMatrix();
    glTranslatef(-0.8f, 0.0f, 1.3f);      
    glRotatef(20.0f, 0.0f, 1.0f, 0.0f);   
    glScalef(1.2f, 0.06f, 0.6f);
    glutSolidCube(1.0);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.8f, 0.0f, 1.3f);       
    glRotatef(-20.0f, 0.0f, 1.0f, 0.0f);  
    glScalef(1.2f, 0.06f, 0.6f);
    glutSolidCube(1.0);
    glPopMatrix();

    // --- DESACTIVATION DE LA TEXTURE POUR LES EFFETS LUMINEUX ---
    if (useFuselageTexture && fuselageTexture != 0) {
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);
    }

    // 7.! EFFET DE FEU (Quand la Nitro est active)
    if (nitro > 0.1f) {
        float fireIntensity = nitro / 3.0f;  // Normalisation entre 0 et 1
        for (int i = 0; i < 4; i++) {
            glPushMatrix();
            glTranslatef(ex[i], -0.25f, 0.2f);  // Derrière chaque moteur

            // Flamme extérieure (orange-rouge)
            glColor3f(1.0f, 0.3f + fireIntensity * 0.4f, 0.0f);
            glScalef(0.15f + fireIntensity * 0.1f, 0.15f + fireIntensity * 0.1f, 0.5f + fireIntensity * 0.8f);
            glutSolidSphere(1.0, 8, 8);

            // Flamme intérieure (cœur jaune-blanc)
            glColor3f(1.0f, 0.9f, 0.3f);
            glScalef(0.5f, 0.5f, 0.6f);
            glutSolidSphere(1.0, 6, 6);

            glPopMatrix();
        }
    }

    // rami
    // Feux latéraux clignotants rouge/bleu
    drawBlinkingSideLight(-2.5f, 0.0f, 0.45f); // côté gauche
    drawBlinkingSideLight(2.5f, 0.0f, 0.45f); // côté droit
    //

    // Nettoyage de la mémoire et fin du dessin
    gluDeleteQuadric(quad);
    glPopMatrix();
}