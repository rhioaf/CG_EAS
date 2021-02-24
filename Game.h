#ifndef Game_H
#define Game_H

#include <iostream>
#include <string>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <math.h>
#include <stdlib.h>
#include <ode/ode.h>

struct vec{
    GLfloat x,y,z = 0.0;
};

struct carColor{
    GLfloat r,g,b;
};

struct pathEnemy{
    GLfloat x,y,z;
    int countMove = 0;
};

struct bullet{
    GLfloat dz = 0;
    int status = 0;
    bool active = false;
    vec startPos;
    vec lastPost;
    dSpaceID space;
    dGeomID geometri;
};

struct car{
    GLfloat speed = 0.0;
    GLfloat acceleration = 0.0;
    bool status = true;
    bullet weapon;
    GLfloat slide = 0.0;
    carColor color;
    vec currPost;
    dSpaceID space;
    dGeomID geometri;
};

struct skyboxSide{
    vec side;
    dSpaceID space;
    dGeomID geometri;
};

struct skybox{
    skyboxSide depan;
    skyboxSide belakang;
    skyboxSide kanan;
    skyboxSide kiri;
};

struct boundary{
    vec border;
    dSpaceID space;
    dGeomID geometri;
};



// Method
void drawAssets();
void drawWheel(float radius);
void drawBullets(car dtPlayer);
void drawCenterBorder();
void drawCar(car dtPlayer);

#endif // Game_H
