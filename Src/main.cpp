#define _CRT_SECURE_NO_WARNINGS
#define GLUT_DISABLE_ATEXIT_HACK

#include <windows.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include "Entities/Player.h"
#include "Entities/Obstacle.h"
#include "Engine/Camera.h"

// --- STRUCTURE DE LA VILLE ---
struct Building {
    float x, z;
    float width, height, depth;
    float r, g, b; // Couleur
};
std::vector<Building> city; // Liste des gratte-ciels

Player avion;
Camera camera;
GLuint textureFuselage = 0; // rami texture
GLuint textureSky = 0; // texture du ciel
std::vector<Obstacle> listeCercles;
float mouseX = 0, mouseY = 0;
//bool useCamera1 = true;
int cameraMode = 0; // Remplace bool useCamera1
// --- Variables à ajouter en haut de Main.cpp pour les contrôles ---
bool lightOn = true;    // Gère l'activation des lumières
bool textureOn = true;  // Gère l'affichage des textures
bool wireframe = false; // Gère le mode plein ou fil de fer
bool isPaused = false;
bool isNightMode = false; // Par défaut, c'est le jour
//rami
bool headlightOn = true; // Phare avant de l'avion
bool autopilotOn = false; // Gère l'activation de l'autopilote
int score = 0; // Le score du joueur

void drawHUD(std::string text, int x, int y) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);

    glColor3f(1.0f, 1.0f, 0.0f);
    glRasterPos2i(x, y);

    for (char c : text) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);

    //rami
    if (lightOn)
        glEnable(GL_LIGHTING);
    else
        glDisable(GL_LIGHTING);
    //

    glPopMatrix();
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
    glMatrixMode(GL_MODELVIEW);
}

/*
void updateEnvironment() {
    if (isNightMode) {
        // NOIR PROFOND (Nuit)
        glClearColor(0.02f, 0.02f, 0.05f, 1.0f);

        // Lumière plus faible et bleutée (Lune)
        GLfloat light_ambient[] = { 0.1f, 0.1f, 0.2f, 1.0f };
        GLfloat light_diffuse[] = { 0.4f, 0.4f, 0.6f, 1.0f };
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    }
    else {
        // BLEU CIEL (Jour)
        glClearColor(0.5f, 0.7f, 1.0f, 1.0f);

        // Lumière forte et blanche (Soleil)
        GLfloat light_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
        GLfloat light_diffuse[] = { 1.0f, 1.0f, 0.9f, 1.0f };
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    }
}
*/

// rami 
void updateEnvironment() {
    if (isNightMode) {
        // Ciel de nuit
        glClearColor(0.02f, 0.02f, 0.08f, 1.0f);

        // Ambiance bleutée de nuit
        GLfloat nightAmbient[] = { 0.04f, 0.06f, 0.15f, 1.0f };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, nightAmbient);
    }
    else {
        // Ciel de jour
        glClearColor(0.5f, 0.7f, 1.0f, 1.0f);

        // Ambiance claire de jour
        GLfloat dayAmbient[] = { 0.18f, 0.18f, 0.18f, 1.0f };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, dayAmbient);
    }
}





// new func rami
void setupSunLight() {
    // GL_LIGHT0 = Soleil

    if (!lightOn) {
        glDisable(GL_LIGHT0);
        return;
    }

    glEnable(GL_LIGHT0);

    // Lumière directionnelle : w = 0.0f
    // Cela simule un soleil très loin.
    GLfloat sun_direction[] = { -0.4f, 1.0f, 0.6f, 0.0f };

    if (isNightMode) {
        // Mode nuit : lumière faible et bleutée
        GLfloat sun_ambient[] = { 0.05f, 0.05f, 0.12f, 1.0f };
        GLfloat sun_diffuse[] = { 0.25f, 0.25f, 0.45f, 1.0f };
        GLfloat sun_specular[] = { 0.3f, 0.3f, 0.5f, 1.0f };

        glLightfv(GL_LIGHT0, GL_POSITION, sun_direction);
        glLightfv(GL_LIGHT0, GL_AMBIENT, sun_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, sun_specular);
    }
    else {
        // Mode jour : soleil blanc légèrement jaune
        GLfloat sun_ambient[] = { 0.18f, 0.18f, 0.15f, 1.0f };
        GLfloat sun_diffuse[] = { 1.0f, 0.95f, 0.75f, 1.0f };
        GLfloat sun_specular[] = { 1.0f, 1.0f, 0.9f, 1.0f };

        glLightfv(GL_LIGHT0, GL_POSITION, sun_direction);
        glLightfv(GL_LIGHT0, GL_AMBIENT, sun_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, sun_specular);
    }
}


// new func rami
void setupPlaneHeadlight() {
    // GL_LIGHT1 = Phare avant de l'avion

    if (!lightOn || !headlightOn) {
        glDisable(GL_LIGHT1);
        return;
    }

    glEnable(GL_LIGHT1);

    // Couleurs du phare : blanc légèrement jaune
    GLfloat head_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat head_diffuse[] = { 1.0f, 0.95f, 0.75f, 1.0f };
    GLfloat head_specular[] = { 1.0f, 1.0f, 0.9f, 1.0f };

    glLightfv(GL_LIGHT1, GL_AMBIENT, head_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, head_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, head_specular);

    // On place le phare dans le repère de l'avion
    glPushMatrix();

    glTranslatef(avion.getX(), avion.getY(), 0.0f);
    glRotatef(avion.getYaw(), 0.0f, 1.0f, 0.0f);
    glRotatef(avion.getPitch(), 1.0f, 0.0f, 0.0f);
    glRotatef(avion.getRoll(), 0.0f, 0.0f, 1.0f);

    // Position locale du phare : devant le nez de l'avion
    GLfloat head_position[] = { 0.0f, 0.0f, -2.8f, 1.0f };

    // Direction locale du phare : vers l'avant de l'avion
    GLfloat head_direction[] = { 0.0f, 0.0f, -1.0f };

    glLightfv(GL_LIGHT1, GL_POSITION, head_position);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, head_direction);

    glPopMatrix();

    // Paramètres du projecteur
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25.0f);     // largeur du faisceau
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 20.0f);   // concentration du faisceau

    // Atténuation avec la distance
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.005f);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0005f);
}


// oldd init
//void init() {
//    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
//    glEnable(GL_COLOR_MATERIAL);
//    GLfloat pos[] = { 10, 10, 10, 1 };
//    glLightfv(GL_LIGHT0, GL_POSITION, pos);
//}

//rami tout la fonc init
//void init() {
//    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
//
//    glEnable(GL_DEPTH_TEST);
//
//    // Lumières
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
//
//    glEnable(GL_NORMALIZE);
//
//    // Permet à glColor3f de continuer à influencer la couleur des objets
//    glEnable(GL_COLOR_MATERIAL);
//    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
//
//    GLfloat pos[] = { 10, 10, 10, 1 };
//    glLightfv(GL_LIGHT0, GL_POSITION, pos);
//}


// rami texture
GLuint loadBMP(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        std::cout << "Erreur : impossible d'ouvrir la texture " << filename << std::endl;
        return 0;
    }

    unsigned char header[54];
    if (fread(header, 1, 54, file) != 54) {
        std::cout << "Erreur : fichier BMP invalide." << std::endl;
        fclose(file);
        return 0;
    }

    if (header[0] != 'B' || header[1] != 'M') {
        std::cout << "Erreur : le fichier n'est pas un BMP." << std::endl;
        fclose(file);
        return 0;
    }

    unsigned int dataPos = *(int*)&(header[0x0A]);
    unsigned int imageSize = *(int*)&(header[0x22]);
    unsigned int width = *(int*)&(header[0x12]);
    unsigned int height = *(int*)&(header[0x16]);

    if (imageSize == 0) imageSize = width * height * 3;
    if (dataPos == 0) dataPos = 54;

    unsigned char* data = new unsigned char[imageSize];

    fseek(file, dataPos, SEEK_SET);
    fread(data, 1, imageSize, file);
    fclose(file);

    // BMP stocke les couleurs en BGR, OpenGL attend RGB
    for (unsigned int i = 0; i < imageSize; i += 3) {
        unsigned char temp = data[i];
        data[i] = data[i + 2];
        data[i + 2] = temp;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        width,
        height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        data
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    delete[] data;

    std::cout << "Texture chargee : " << filename << std::endl;
    return textureID;
}

// NOUVEAU: Initialisation de la ville procédurale
void initCity() {
    for (int i = 0; i < 200; i++) {
        Building b;
        b.x = (rand() % 400) - 200.0f; // Étalé entre -200 et 200 en largeur
        b.z = (rand() % 400) - 350.0f; // Étalé entre -350 et 50 en profondeur
        b.width = 4.0f + (rand() % 6);
        b.depth = 4.0f + (rand() % 6);
        b.height = 15.0f + (rand() % 20); // Hauteur max 35. Base à -50 => Top à -15 (Avion bloqué à -10)
        
        // Couleurs aléatoires (Nuances de gris, bleu gris, verre)
        b.r = 0.2f + (rand() % 20) / 100.0f;
        b.g = 0.2f + (rand() % 20) / 100.0f;
        b.b = 0.3f + (rand() % 30) / 100.0f;
        
        city.push_back(b);
    }
}

void init() {
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    // Activation générale des lumières
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);  // sun                rami
    glEnable(GL_LIGHT1); // Phare avant         rami


    // Important quand on utilise glScalef()
    glEnable(GL_NORMALIZE);

    // Permet à glColor3f de continuer à donner la couleur des objets
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // Matériau de base avec un peu de brillance
    GLfloat mat_specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat mat_shininess[] = { 50.0f };

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

    setupSunLight();

    // On génère la ville
    initCity();

    // rami
    // Texture du fuselage avec les chemins absolus (et chemin relatif de secours)
    textureFuselage = loadBMP("/home/mersel/Documents/Projects/ProjetAvion (4)/ProjetAvion/Src/Resources/metal.bmp");
    if (textureFuselage == 0) {
        textureFuselage = loadBMP("Src/Resources/metal.bmp");
        if (textureFuselage == 0) {
            textureFuselage = loadBMP("../Src/Resources/metal.bmp");
            if (textureFuselage == 0) {
                textureFuselage = loadBMP("../../Src/Resources/metal.bmp");
            }
        }
    }
    std::cout << "ID texture fuselage = " << textureFuselage << std::endl;
    avion.setFuselageTexture(textureFuselage);

    // Texture du ciel avec les chemins absolus (et chemin relatif de secours)
    textureSky = loadBMP("/home/mersel/Documents/Projects/ProjetAvion (4)/ProjetAvion/Src/Resources/sky.bmp");
    if (textureSky == 0) {
        textureSky = loadBMP("Src/Resources/sky.bmp");
        if (textureSky == 0) {
            textureSky = loadBMP("../Src/Resources/sky.bmp");
            if (textureSky == 0) {
                textureSky = loadBMP("../../Src/Resources/sky.bmp");
            }
        }
    }
    std::cout << "ID texture sky = " << textureSky << std::endl;
    //

}

// rami
// sphere englobe la scene pour que je mets une image dans la face interieur de sphere
void drawSkySphere() {
    if (isNightMode)
        return;

    if (textureSky == 0)
        return;

    glPushMatrix();

    // Le ciel ne doit pas être influencé par les lumières
    glDisable(GL_LIGHTING);

    // Le ciel ne doit pas cacher les objets
    glDepthMask(GL_FALSE);

    // Au cas où le mode wireframe est activé, on force le ciel en plein
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureSky);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glColor3f(1.0f, 1.0f, 1.0f);

    // On place la sphère autour de l'avion
    glTranslatef(avion.getX(), avion.getY(), 0.0f);

    // Grande sphère de ciel
    GLUquadric* sky = gluNewQuadric();
    gluQuadricTexture(sky, GL_TRUE);
    gluQuadricNormals(sky, GLU_NONE);
    gluQuadricOrientation(sky, GLU_INSIDE); // Important: texturer l'interieur de la sphere

    // Rotation pour mieux orienter la texture
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

    gluSphere(sky, 200.0, 40, 40); // Agrandie pour ne pas clipper la vue

    gluDeleteQuadric(sky);

    glDisable(GL_TEXTURE_2D);

    // Remettre l'état normal
    glDepthMask(GL_TRUE);

    if (lightOn)
        glEnable(GL_LIGHTING);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glPopMatrix();
}

// NOUVEAU: Fonction pour dessiner la ville
void drawCity() {
    // 1. Le sol (Pour cacher le vide sous les bâtiments)
    glPushMatrix();
    glTranslatef(0.0f, -50.0f, -100.0f);
    glScalef(800.0f, 1.0f, 800.0f);
    glColor3f(0.1f, 0.1f, 0.1f); // Sol sombre
    if (lightOn) {
        GLfloat mat_ambient[] = { 0.05f, 0.05f, 0.05f, 1.0f };
        GLfloat mat_diffuse[] = { 0.1f, 0.1f, 0.1f, 1.0f };
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    }
    glutSolidCube(1.0f);
    glPopMatrix();

    // 2. Les gratte-ciels
    for (const auto& b : city) {
        glPushMatrix();
        
        // La base du cube est au centre. Si on veut que la base repose sur Y=-50, 
        // on place le centre à -50 + hauteur/2
        glTranslatef(b.x, -50.0f + (b.height / 2.0f), b.z);
        
        glColor3f(b.r, b.g, b.b);
        if (lightOn) {
            GLfloat mat_diffuse[] = { b.r, b.g, b.b, 1.0f };
            GLfloat mat_specular[] = { 0.3f, 0.3f, 0.3f, 1.0f }; // Légère réflexion (verre)
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
            glMaterialf(GL_FRONT, GL_SHININESS, 10.0f);
        }
        
        glScalef(b.width, b.height, b.depth);
        glutSolidCube(1.0f);
        
        glPopMatrix();
    }
}

// NOUVEAU: Fonction pour dessiner la minimap Radar (en haut à droite)
void drawMinimap() {
    // Basculer en 2D
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    
    // Paramètres de la minimap
    float mapX = 630.0f, mapY = 430.0f; // Position
    float mapW = 150.0f, mapH = 150.0f; // Taille
    
    // 1. Fond du radar (gris transparent)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.2f, 0.0f, 0.6f);
    glBegin(GL_QUADS);
        glVertex2f(mapX, mapY);
        glVertex2f(mapX + mapW, mapY);
        glVertex2f(mapX + mapW, mapY + mapH);
        glVertex2f(mapX, mapY + mapH);
    glEnd();
    
    // 2. Le joueur (Triangle jaune en bas au centre)
    float pX = mapX + mapW / 2.0f;
    float pY = mapY + 20.0f;
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(pX, pY + 8);
        glVertex2f(pX - 6, pY - 6);
        glVertex2f(pX + 6, pY - 6);
    glEnd();
    
    // 3. Les Obstacles
    for (const auto& o : listeCercles) {
        // Transformation Z (profondeur) vers l'axe Y du radar
        float relativeZ = o.getZ() - avion.getZ(); 
        if (relativeZ < -150.0f) relativeZ = -150.0f;
        if (relativeZ > 0.0f) relativeZ = 0.0f;
        float dotY = pY + (-relativeZ) * (mapH / 150.0f);
        
        // Transformation X (gauche/droite) vers l'axe X du radar
        float relativeX = o.getX() - avion.getX();
        float dotX = pX + relativeX * (mapW / 40.0f); // 40 = largeur max du terrain (-20 à 20)
        
        // Transformation Y (Hauteur) pour le radar
        float relativeY = o.getY() - avion.getY();
        
        // Dessiner le point si dans le radar
        if (dotX >= mapX && dotX <= mapX + mapW && dotY >= mapY && dotY <= mapY + mapH) {
            
            // Choix de la couleur
            if (o.getStatus() == 1) glColor3f(0.5f, 0.5f, 0.5f); // Gris si passé
            else if (o.getStatus() == 2) glColor3f(1.0f, 0.0f, 0.0f); // Rouge si raté
            else glColor3f(0.0f, 1.0f, 0.0f); // Vert vif si en approche
            
            // Choix de la forme en fonction de la hauteur (relativeY)
            if (relativeY > 1.5f) {
                // Triangle pointant vers le HAUT (L'anneau est au-dessus)
                glBegin(GL_TRIANGLES);
                glVertex2f(dotX, dotY + 5);
                glVertex2f(dotX - 4, dotY - 3);
                glVertex2f(dotX + 4, dotY - 3);
                glEnd();
            } else if (relativeY < -1.5f) {
                // Triangle pointant vers le BAS (L'anneau est en-dessous)
                glBegin(GL_TRIANGLES);
                glVertex2f(dotX, dotY - 5);
                glVertex2f(dotX - 4, dotY + 3);
                glVertex2f(dotX + 4, dotY + 3);
                glEnd();
            } else {
                // Carré (L'anneau est à la même hauteur)
                glBegin(GL_QUADS);
                glVertex2f(dotX - 3, dotY - 3);  glVertex2f(dotX + 3, dotY - 3);
                glVertex2f(dotX + 3, dotY + 3);  glVertex2f(dotX - 3, dotY + 3);
                glEnd();
            }
        }
    }
    
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    if (lightOn) glEnable(GL_LIGHTING);
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

/*
void display() {
    updateEnvironment(); // Garder l'ambiance à jour
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // ramiii
    if (lightOn)
        glEnable(GL_LIGHTING);
    else
        glDisable(GL_LIGHTING);
    //

    // On met à jour et on applique la caméra
    camera.update(cameraMode, mouseX, mouseY, avion.getX(), avion.getY());
    camera.apply();

    avion.draw();
    for (auto& o : listeCercles) o.draw();

    drawHUD("SPEED: " + std::to_string((int)(avion.getSpeed() * 100)), 20, 560);
    drawHUD("PITCH: " + std::to_string((int)avion.getPitch()), 20, 530);
    drawHUD("ROLL:  " + std::to_string((int)avion.getRoll()), 20, 500);
    drawHUD("PRESS 'X' FOR NITRO!", 600, 20);

    glutSwapBuffers();
}
*/

// rami
void display() {
    updateEnvironment(); // Garder l'ambiance à jour

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Dynamic FOV based on Nitro
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float fov = 45.0f + (avion.getNitro() * 10.0f); // Increase FOV with nitro
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    gluPerspective(fov, (float)w / h, 0.1, 1000.0);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    // Caméra
    camera.update(cameraMode, mouseX, mouseY, avion.getX(), avion.getY());
    camera.apply();


    // Ciel texturé autour de la scène
    drawSkySphere();  // rami

    // Soleil après la caméra : lumière dans la scène
    if (lightOn)
        glEnable(GL_LIGHTING);
    else
        glDisable(GL_LIGHTING);

    setupSunLight();
    setupPlaneHeadlight();

    // Dessiner la ville en contrebas
    drawCity();

    avion.draw();

    for (auto& o : listeCercles)
        o.draw();

    drawHUD("SPEED: " + std::to_string((int)(avion.getSpeed() * 100)), 20, 560);
    drawHUD("PITCH: " + std::to_string((int)avion.getPitch()), 20, 530);
    drawHUD("ROLL:  " + std::to_string((int)avion.getRoll()), 20, 500);
    drawHUD("SCORE: " + std::to_string(score), 20, 470);
    
    // Affichage de l'état de l'autopilote
    if (autopilotOn) {
        glColor3f(0.0f, 1.0f, 0.0f); // Vert si activé
        drawHUD("AUTOPILOT: ON", 20, 440);
    } else {
        glColor3f(1.0f, 0.0f, 0.0f); // Rouge si désactivé
        drawHUD("AUTOPILOT: OFF", 20, 440);
    }

    drawHUD("PRESS 'X' FOR NITRO!", 600, 20);
    drawHUD("PRESS 'A' FOR AUTOPILOT", 600, 50);
    drawHUD("W / S : THROTTLE UP/DOWN", 600, 80);

    drawMinimap(); // Affiche la minimap !

    glutSwapBuffers();
}


void timer(int v) {
    if (!isPaused) {
        // --- SYSTEME AUTOPILOTE (PATH FINDER) ---
        if (autopilotOn) {
            float closestZ = -9999.0f;
            Obstacle* target = nullptr;
            
            // Trouver l'anneau actif (status == 0) le plus proche devant l'avion
            for (auto& o : listeCercles) {
                if (o.getStatus() == 0 && o.getZ() < 0.0f) { // Toujours devant nous
                    if (o.getZ() > closestZ) {
                        closestZ = o.getZ();
                        target = &o;
                    }
                }
            }
            
            // Si on a trouvé une cible, on dirige l'avion vers elle
            if (target != nullptr) {
                float dx = target->getX() - avion.getX();
                float dy = target->getY() - avion.getY();
                float autoStep = 1.0f; // Sensibilité de l'IA
                
                // Correction horizontale (Yaw / Roll)
                if (dx > 0.5f) avion.turnRight(autoStep);
                else if (dx < -0.5f) avion.turnLeft(autoStep);
                
                // Correction verticale (Pitch)
                if (dy > 0.5f) avion.moveUp(autoStep);
                else if (dy < -0.5f) avion.moveDown(autoStep);
            }
        }

        avion.update(1.0f);
        
        // --- MISE À JOUR DE LA VILLE INFINIE ---
        for (auto& b : city) {
            b.z += avion.getSpeed() * 3.0f; // Les bâtiments défilent à la même vitesse
            if (b.z > 20.0f) { // S'ils dépassent la caméra, on les replace au fond
                b.z -= 400.0f; 
                b.x = (rand() % 400) - 200.0f; // Nouvelle position X aléatoire
                b.height = 15.0f + (rand() % 20); // Nouvelle hauteur
            }
        }
        
        for (auto& o : listeCercles) {
            // Vitesse des anneaux calquée sur celle de l'avion
            o.update(avion.getSpeed() * 3.0f); 
            
            // --- DETECTION DE COLLISION ---
            // Si l'anneau arrive au niveau de l'avion (Z autour de 0)
            if (o.getStatus() == 0 && o.getZ() > -1.0f && o.getZ() < 1.0f) {
                // Distance en 2D (axe X et axe Y)
                float dx = avion.getX() - o.getX();
                float dy = avion.getY() - o.getY();
                float dist = std::sqrt(dx*dx + dy*dy);
                
                if (dist <= 3.0f) { // Rayon de l'anneau (Tolérance)
                    score += 10;
                    o.setStatus(1); // HIT (Succès)
                } else {
                    o.setStatus(2); // MISS (Raté)
                }
            }
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

// rami 
void idle() {
    glutPostRedisplay();
}
//

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        // 1. TA TOUCHE NITRO
    case 'x':
    case 'X':
        avion.hitNitro();
        break;

    case 'w':
    case 'W':
        avion.throttleUp(0.05f);
        break;

    case 's':
    case 'S':
        avion.throttleDown(0.05f);
        break;

        // 2. SWITCH CAMÉRA
    case 'c':
    case 'C':
        cameraMode = (cameraMode + 1) % 4; // Maintenant on a 4 modes (0, 1, 2, 3)
        break;

        // ACTIVER/DESACTIVER L'AUTOPILOTE
    case 'a':
    case 'A':
        autopilotOn = !autopilotOn;
        break;

        // 3. ACTIVER/DÉSACTIVER LUMIÈRE 
    case 'l':
    case 'L':
        lightOn = !lightOn;
        if (lightOn) glEnable(GL_LIGHTING);
        else glDisable(GL_LIGHTING);
        break;

        // 5. MODE FIL DE FER vs PLEIN 
    case 'f':
    case 'F':
        wireframe = !wireframe;
        if (wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Mode fil de fer
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Mode plein
        break;

        // 6. GESTION DE L'ANIMATION (Espace pour Pause)
    case ' ':
        isPaused = !isPaused;
        break;
    case '+':
        avion.setDetail(2);
        break;
    case '-':
        avion.setDetail(-2);
        break;

    case 'r':
    case 'R':
        // On peut imaginer une fonction reset() dans ta classe Player
        // avion.reset(); 
        cameraMode = 0; // Remet la caméra par défaut
        std::cout << "Simulation réinitialisée !" << std::endl;
        break;
    case 'n':
    case 'N':
        isNightMode = !isNightMode;
        updateEnvironment(); // On applique le changement immédiatement
        break;

    // rami ajouté touche p pour phare
    case 'p':
    case 'P':
        headlightOn = !headlightOn;
        if (headlightOn)
            std::cout << "Phare avion active" << std::endl;
        else
            std::cout << "Phare avion desactive" << std::endl;
        break;

        // 7. QUITTER LE JEU
    case 27: // Touche Echap
        exit(0);
        break;
    }

    // On force le redessin de la scène après un changement de touche
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    float step = 1.5f;
    if (key == GLUT_KEY_UP)    avion.moveUp(step);
    if (key == GLUT_KEY_DOWN)  avion.moveDown(step);
    if (key == GLUT_KEY_LEFT)  avion.turnLeft(step);
    if (key == GLUT_KEY_RIGHT) avion.turnRight(step);
}

void passiveMouse(int x, int y) {
    mouseX = x - 400; 
    mouseY = y - 300;
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / h, 0.1, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char** argv) {

/* #if defined(WIN32) || defined(WIN64)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_crtBreakAlloc = 126;
#endif
*/

    // On ajoute plus de cercles éparpillés !
    listeCercles.push_back(Obstacle(-40));
    listeCercles.push_back(Obstacle(-80));
    listeCercles.push_back(Obstacle(-120));
    listeCercles.push_back(Obstacle(-160));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Advanced Flight Sim");
    init();
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutPassiveMotionFunc(passiveMouse);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}