#include "Game.h"

void drawAssets(){
    glPushMatrix();

        // variabel batang
        GLfloat xCoor1b = -90;
        GLfloat xCoor2b = -88;
        GLfloat yCoor1b = -1.4;
        GLfloat yCoor2b = 6.0;
        GLfloat zCoor1b = -90;
        GLfloat zCoor2b = -88;

        // variabel pohon
        GLfloat xCoorp = -90;
        GLfloat yCoorp = -90;
        GLfloat zCoorp = -90;

        for(int i=0; i < 10; i++){
            glColor3f(0.545, 0.271, 0.075);
            // Alas
            glBegin(GL_QUADS);
                glVertex3f(xCoor1b, yCoor1b, zCoor1b);
                glVertex3f(xCoor1b, yCoor1b, zCoor2b);
                glVertex3f(xCoor2b, yCoor1b, zCoor2b);
                glVertex3f(xCoor2b, yCoor1b, zCoor1b);
            glEnd();

            // bagian kiri
            glBegin(GL_QUADS);
                glVertex3f(xCoor1b, yCoor1b, zCoor1b);
                glVertex3f(xCoor1b, yCoor1b, zCoor2b);
                glVertex3f(xCoor1b, yCoor2b, zCoor2b);
                glVertex3f(xCoor1b, yCoor2b, zCoor1b);
            glEnd();
            // bagian kanan
            glBegin(GL_QUADS);
                glVertex3f(xCoor2b, yCoor1b, zCoor1b);
                glVertex3f(xCoor2b, yCoor1b, zCoor2b);
                glVertex3f(xCoor2b, yCoor2b, zCoor2b);
                glVertex3f(xCoor2b, yCoor2b, zCoor1b);
            glEnd();
            // bagian depan
            glBegin(GL_QUADS);
                glVertex3f(xCoor1b, yCoor1b, zCoor2b);
                glVertex3f(xCoor2b, yCoor1b, zCoor2b);
                glVertex3f(xCoor2b, yCoor2b, zCoor2b);
                glVertex3f(xCoor1b, yCoor2b, zCoor2b);
            glEnd();
            // bagian belakang
            glBegin(GL_QUADS);
                glVertex3f(xCoor1b, yCoor1b, zCoor1b);
                glVertex3f(xCoor2b, yCoor1b, zCoor1b);
                glVertex3f(xCoor2b, yCoor2b, zCoor1b);
                glVertex3f(xCoor1b, yCoor2b, zCoor1b);
            glEnd();

            // Daun
            glColor3f(0.133, 0.545, 0.133);
            // bagian depan
            glBegin(GL_TRIANGLES);
               glVertex3f(xCoor1b-3, 1.0, zCoor2b+2);
               glVertex3f(xCoor1b+1, 9.0, zCoor1b+1);
               glVertex3f(xCoor2b+3, 1.0, zCoor2b+2);
            glEnd();
            // bagian kanan
            glBegin(GL_TRIANGLES);
               glVertex3f(xCoor2b+3, 1.0, zCoor2b+2);
               glVertex3f(xCoor1b+1, 9.0, zCoor1b+1);
               glVertex3f(xCoor2b+3, 1.0, zCoor1b-2);
            glEnd();
            // bagian belakang
            glBegin(GL_TRIANGLES);
               glVertex3f(xCoor2b+3, 1.0, zCoor1b-2);
               glVertex3f(xCoor1b+1, 9.0, zCoor1b+1);
               glVertex3f(xCoor1b-3, 1.0, zCoor1b-2);
            glEnd();
            // bagian kiri
            glBegin(GL_TRIANGLES);
               glVertex3f(xCoor1b-3, 1.0, zCoor1b-2);
               glVertex3f(xCoor1b+1, 9.0, zCoor1b+1);
               glVertex3f(xCoor1b-3, 1.0, zCoor2b+2);
            glEnd();

            xCoor1b += 20;
            xCoor2b += 20;
        }
        glColor3f(1.0, 1.0, 1.0);
    glPopMatrix();
}

void drawWheel(float radius) {
    const float phi = 3.142f;
    float theta;
    int totVertices = 40;
    GLfloat x, y;

    // Isi roda
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_QUAD_STRIP);
        for(int i = 0; i <= totVertices; i++){
            theta = (i*360/totVertices)*phi/180;
            x = radius * cos(theta);
            y = radius * sin(theta);
            glVertex3f(x, y, -0.5);
            glVertex3f(x, y, 0.5);
        }
    glEnd();
    glColor3f(1.0, 1.0, 1.0);
    // Tutup Roda
    for(float j = 0.5; j >= -0.5; j -= 1.0){
        glBegin(GL_TRIANGLE_FAN);
            glVertex3f(0.0, 0.0, j);
            for(int k = 0; k <= totVertices; k++){
                theta = (k*360/totVertices)*phi/180;
                x = radius * cos(theta);
                y = radius * sin(theta);
                glVertex3f(x, y, j);
            }
        glEnd();
    }
}

void drawBullets(car dtPlayer){
    glPushMatrix();
        glColor3f(0.0, 0.0, 0.0);
        glTranslatef(dtPlayer.weapon.startPos.x, dtPlayer.weapon.startPos.y, dtPlayer.weapon.startPos.z+dtPlayer.weapon.dz);
        //glTranslatef(1.0, -1.3, -1);
        // Top side
        glBegin(GL_QUADS);
            glVertex3f(-0.85, 1.25, -7.5);
            glVertex3f(-1.05, 1.25, -7.5);
            glVertex3f(-1.05, 1.25, -8.2);
            glVertex3f(-0.85, 1.25, -8.2);
        glEnd();
        // Bottom side
        glBegin(GL_QUADS);
            glVertex3f(-0.85, 1.05, -7.5);
            glVertex3f(-1.05, 1.05, -7.5);
            glVertex3f(-1.05, 1.05, -8.2);
            glVertex3f(-0.85, 1.05, -8.2);
        glEnd();
        // Left side
        glBegin(GL_QUADS);
            glVertex3f(-1.05, 1.25, -7.5);
            glVertex3f(-1.05, 1.05, -7.5);
            glVertex3f(-1.05, 1.05, -8.2);
            glVertex3f(-1.05, 1.25, -8.2);
        glEnd();
        // Right side
        glBegin(GL_QUADS);
            glVertex3f(-0.85, 1.25, -7.5);
            glVertex3f(-0.85, 1.05, -7.5);
            glVertex3f(-0.85, 1.05, -8.2);
            glVertex3f(-0.85, 1.25, -8.2);
        glEnd();
        // Rear side
        glBegin(GL_QUADS);
            glVertex3f(-0.85, 1.25, -7.5);
            glVertex3f(-1.05, 1.25, -7.5);
            glVertex3f(-1.05, 1.05, -7.5);
            glVertex3f(-0.85, 1.05, -7.5);
        glEnd();
        // Front side
        glBegin(GL_QUADS);
            glVertex3f(-0.85, 1.25, -8.2);
            glVertex3f(-1.05, 1.25, -8.2);
            glVertex3f(-1.05, 1.05, -8.2);
            glVertex3f(-0.85, 1.05, -8.2);
        glEnd();
        glColor3f(1.0, 1.0, 1.0);
    glPopMatrix();
}

void drawCenterBorder(){
    glPushMatrix();
        glTranslatef(0.0, -1.45, 0.0);
        glColor3f(0.0, 0.0, 0.0);
        float x = -100;
        while(x < 100){
            glBegin(GL_QUADS);
                glVertex3f(x, 0.0, -6);
                glVertex3f(x+5, 0.0, -6);
                glVertex3f(x+5, 0.0, -4);
                glVertex3f(x, 0.0, -4);
            glEnd();
            x += 10;
        }
        glColor3f(1.0, 1.0, 1.0);
    glPopMatrix();
}

void drawCar(car dtPlayer){
    glTranslatef(1.0, 0.0, -1);
    glColor3f(dtPlayer.color.r, dtPlayer.color.g, dtPlayer.color.b);
    // Right Side
    glBegin(GL_QUAD_STRIP);
        glVertex3f(1.0, 0.5, -6.0);
        glVertex3f(1.0, 1.0, -4.3);
        glVertex3f(1.0, 0.5, -3.3);
        glVertex3f(1.0, 1.9, -3.0);

        glVertex3f(1.0, 0.5, -2.0);
        glVertex3f(1.0, 1.9, -1.7);
        glVertex3f(1.0, 0.5, -0.5);
        glVertex3f(1.0, 1.9, 2.0);

        glVertex3f(1.0, 0.5, 2.0);
        glVertex3f(1.0, 1.9, 3.0);
        glVertex3f(1.0, 0.5, 3.0);
        glVertex3f(1.0, 1.9, 3.5);

        glVertex3f(1.0, 0.5, 4.0);
        glVertex3f(1.0, 1.9, 4.5);
        glVertex3f(1.0, 0.5, 5.0);
        glVertex3f(1.0, 1.9, 5.5);
    glEnd();

    // Left side
    glTranslatef(-4.0, 0.0, 0.0);
    glBegin(GL_QUAD_STRIP);
        glVertex3f(1.0, 0.5, -6.0);
        glVertex3f(1.0, 1.0, -4.3);
        glVertex3f(1.0, 0.5, -3.3);
        glVertex3f(1.0, 1.9, -3.0);

        glVertex3f(1.0, 0.5, -2.0);
        glVertex3f(1.0, 1.9, -1.7);
        glVertex3f(1.0, 0.5, -0.5);
        glVertex3f(1.0, 1.9, 2.0);

        glVertex3f(1.0, 0.5, 2.0);
        glVertex3f(1.0, 1.9, 3.0);
        glVertex3f(1.0, 0.5, 3.0);
        glVertex3f(1.0, 1.9, 3.5);

        glVertex3f(1.0, 0.5, 4.0);
        glVertex3f(1.0, 1.9, 4.5);
        glVertex3f(1.0, 0.5, 5.0);
        glVertex3f(1.0, 1.9, 5.5);
    glEnd();

    // Reset position
    glTranslatef(4.0, 0.0, 0.0);

    // Back cover
    glBegin(GL_QUADS);
        glVertex3f(1.0, 1.9, 5.5);
        glVertex3f(-3.0, 1.9, 5.5);
        glVertex3f(-3.0, 0.5, 5.0);
        glVertex3f(1.0, 0.5, 5.0);
    glEnd();

    // Back seat
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_QUADS);
        glVertex3f(-3.0, 1.9, 5.5);
        glVertex3f(-3.0, 2.4, 3.0);
        glVertex3f(1.0, 2.4, 3.0);
        glVertex3f(1.0, 1.9, 5.5);
    glEnd();
    glColor3f(dtPlayer.color.r, dtPlayer.color.g, dtPlayer.color.b);
    glBegin(GL_QUADS);
        glVertex3f(-3.0, 2.4, 3.0);
        glVertex3f(-3.0, 1.9, 3.0);
        glVertex3f(1.0, 1.9, 3.0);
        glVertex3f(1.0, 2.4, 3.0);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex3f(1.0, 2.4, 3.0);
        glVertex3f(1.0, 1.9, 5.5);
        glVertex3f(1.0, 1.9, 3.0);
    glEnd();
    glTranslatef(-4.0, 0.0, 0.0);
    glBegin(GL_TRIANGLES);
        glVertex3f(1.0, 2.4, 3.0);
        glVertex3f(1.0, 1.9, 5.5);
        glVertex3f(1.0, 1.9, 3.0);
    glEnd();
    glTranslatef(4.0, 0.0, 0.0);

    // Bottom front cover
    glBegin(GL_QUADS);
        glVertex3f(1.0, 1.9, -3.0);
        glVertex3f(-3.0, 1.9, -3.0);
        glVertex3f(-3.0, 0.5, -4.0);
        glVertex3f(1.0, 0.5, -4.0);
    glEnd();
    glBegin(GL_QUADS);
        glVertex3f(1.0, 1.9, -3.0);
        glVertex3f(-3.0, 1.9, -3.0);
        glVertex3f(-3.0, 1.9, -1.5);
        glVertex3f(1.0, 1.9, -1.5);
    glEnd();

    // Front glass
    glColor3f(0.941, 1.0, 1.0);
    glBegin(GL_QUADS);
        glVertex3f(1.0, 1.9, -1.5);
        glVertex3f(-3.0, 1.9, -1.5);
        glVertex3f(-3.0, 3.0, -0.5);
        glVertex3f(1.0, 3.0, -0.5);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    // Weapon
    // Bottom side
    glBegin(GL_QUADS);
        glVertex3f(-0.9, 1.1, -3.5);
        glVertex3f(-1.1, 1.1, -3.5);
        glVertex3f(-1.1, 1.1, -7.5);
        glVertex3f(-0.9, 1.1, -7.5);
    glEnd();
    // Top side
    glBegin(GL_QUADS);
        glVertex3f(-0.9, 1.3, -3.4);
        glVertex3f(-1.1, 1.3, -3.4);
        glVertex3f(-1.1, 1.3, -7.5);
        glVertex3f(-0.9, 1.3, -7.5);
    glEnd();
    // Right side
    glBegin(GL_QUADS);
        glVertex3f(-0.9, 1.3, -3.4);
        glVertex3f(-0.9, 1.1, -3.5);
        glVertex3f(-0.9, 1.1, -7.5);
        glVertex3f(-0.9, 1.3, -7.5);
    glEnd();
    // Left side
    glBegin(GL_QUADS);
        glVertex3f(-1.1, 1.3, -3.4);
        glVertex3f(-1.1, 1.1, -3.5);
        glVertex3f(-1.1, 1.1, -7.5);
        glVertex3f(-1.1, 1.3, -7.5);
    glEnd();

    glBegin(GL_QUADS);
        glVertex3f(-3.0, 0.5, 5.0);
        glVertex3f(-3.0, 0.5, -4.0);
        glVertex3f(1.0, 0.5, -4.0);
        glVertex3f(1.0, 0.5, 5.0);
    glEnd();
    glColor3f(1.0, 1.0, 1.0);
}
