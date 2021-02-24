#include "BmpLoader.h"
#include "Game.h"
using namespace std;


/* Global Variable */
car dtPlayer;
//car dtEnemies[7];
car dtMidEnemies;
pathEnemy movement;
char title[] = "Battle Cars";
GLuint  _textureSnow, _textureSky;
GLint spin_x = 0;
GLint spin_y = 0;
GLfloat carPositionFromPlayer = -30;
int movingType = 0;
dSpaceID collisionSkybox;
skybox dtSb;
dContactGeom contacts[10];
boundary center;
int level, maxLevel;
bool reachMaxLevel = false;
unsigned int refreshTimeEnemyMovement;

/* Camera Variable */
// Default for third person
bool stylePersonPOV = true; // if true = third person else first person
GLdouble eyeCamZ = 20.0;
GLdouble eyeCamY = 6.0;
GLdouble centerCamZ = 0.0;

GLdouble eyeCam[] = {0.0};
GLdouble centerCam[] = {0.0, 2.0};
GLdouble upCam[] = {0.0, 3.0, 0.0};

// Methods
void Display();
void bulletHitDetection(int timer);
void enemyMovement(int timer);
void playerMovement(int timer);
void Init();
static void nearCallback (void *, dGeomID o1, dGeomID o2);
void moveBullets();
void KeyboardHandler(unsigned char key, int x, int y);
void specialKeyboardCb(int key, int x, int y);
void Reshape(GLsizei width, GLsizei height);
GLuint loadTexture(const char* filename);

int main(int argc, char **argv) {
    string playerName;
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1400, 800);
	glutInitWindowPosition(270, 50);
	glutCreateWindow(title);
	glEnable(GL_DEPTH_TEST);

	glutDisplayFunc(Display);
	glutIdleFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(KeyboardHandler);
	glutSpecialFunc(specialKeyboardCb);
	glutTimerFunc(1,enemyMovement,0);
	glutTimerFunc(1,playerMovement,0);
	glutTimerFunc(1, bulletHitDetection, 0);

	Init();
	glutMainLoop();
	return 0;
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
    gluLookAt(
          eyeCam[0] + dtPlayer.currPost.x,
          eyeCamY + dtPlayer.currPost.y,
          eyeCamZ + dtPlayer.currPost.z,
          centerCam[0] + dtPlayer.currPost.x,
          centerCam[1] + dtPlayer.currPost.y,
          centerCamZ + dtPlayer.currPost.z,
          upCam[0],
          upCam[1],
          upCam[2]);
    glRotatef(spin_x, 0.0, 1.0, 0.0);
	glRotatef(spin_y, 0.0, 0.0, 1.0);

	/* Sky box section */
    // Snow
    glPushMatrix();
    	glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureSnow);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTranslatef(0,0,-6);
        glBegin(GL_QUADS);
            glTexCoord3f(0.0,1.0,1);  glVertex3f(-100,-1.5,100);
            glTexCoord3f(0.0,0.0,1);  glVertex3f(-100,-1.5,-100);
            glTexCoord3f(1.0,0.0,1);  glVertex3f(100,-1.5,-100);
            glTexCoord3f(1.0,1.0,1);  glVertex3f(100,-1.5,100);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // Sky
    glPushMatrix();
    	glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureSky);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTranslatef(0,0,-6);
        glBegin(GL_QUADS); //Belakang
            glTexCoord3f(1.0,1.0,0);  glVertex3f(-100,50,-100);
            glTexCoord3f(0.0,0.0,0);  glVertex3f(-100,-50,-100);
            glTexCoord3f(1.0,0.0,0);  glVertex3f(100,-50,-100);
            glTexCoord3f(0.0,1.0,0);  glVertex3f(100,50,-100);
        glEnd();
        glBegin(GL_QUADS); //Depan
            glTexCoord3f(1.0,1.0,0);  glVertex3f(-100,50,100);
            glTexCoord3f(0.0,0.0,0);  glVertex3f(-100,-50,100);
            glTexCoord3f(1.0,0.0,0);  glVertex3f(100,-50,100);
            glTexCoord3f(0.0,1.0,0);  glVertex3f(100,50,100);
        glEnd();
        glBegin(GL_QUADS); //Kanan
            glTexCoord3f(1.0,1.0,0);  glVertex3f(100,50,-100);
            glTexCoord3f(0.0,0.0,0);  glVertex3f(100,-50,-100);
            glTexCoord3f(1.0,0.0,0);  glVertex3f(100,-50,100);
            glTexCoord3f(0.0,1.0,0);  glVertex3f(100,50,100);
        glEnd();
        glBegin(GL_QUADS); //Kiri
            glTexCoord3f(1.0,1.0,0);  glVertex3f(-100,50,-100);
            glTexCoord3f(0.0,0.0,0);  glVertex3f(-100,-50,-100);
            glTexCoord3f(1.0,0.0,0);  glVertex3f(-100,-50,100);
            glTexCoord3f(0.0,1.0,0);  glVertex3f(-100,50,100);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    drawCenterBorder();
    drawAssets();

    /* Enemy Section */
    if(dtMidEnemies.status){
        glPushMatrix();
            //glTranslatef(0.0, -1.4, -30.0);
            glTranslatef(movement.x, movement.y, movement.z);
            glRotatef(180.0, 0.0, 1.0, 0.0);
            glPushMatrix();
                glScalef(0.7, 1.0, 0.7);
                drawCar(dtMidEnemies);
            glPopMatrix();
            glPushMatrix();
                glRotatef(90.0, 0.0, 1.0, 0.0);
                glScalef(0.5, 0.5, 0.5);
                glTranslatef(-3.0, 0.8, 2.7);
                drawWheel(1.0);
                glTranslatef(0.0, 0.0, -5.4);
                drawWheel(1.0);
                glTranslatef(8.0, 0.0, 0.0);
                drawWheel(1.0);
                glTranslatef(0.0, 0.0, 5.4);
                drawWheel(1.0);
            glPopMatrix();
            /*for(int i=0; i < 7; i++){
                glPushMatrix();
                    glScalef(0.7, 1.0, 0.7);
                    drawCar(dtEnemies[i]);
                glPopMatrix();
                glPushMatrix();
                    glRotatef(90.0, 0.0, 1.0, 0.0);
                    glScalef(0.5, 0.5, 0.5);
                    glTranslatef(-3.0, 0.8, 2.7);
                    drawWheel(1.0);
                    glTranslatef(0.0, 0.0, -5.4);
                    drawWheel(1.0);
                    glTranslatef(8.0, 0.0, 0.0);
                    drawWheel(1.0);
                    glTranslatef(0.0, 0.0, 5.4);
                    drawWheel(1.0);
                glPopMatrix();
                glTranslatef(-20.0, 0.0, 0.0);
            }*/
        glPopMatrix();
    }

    /* Player section */
    glPushMatrix();
        // Move back closer to end of map
        //glTranslatef(0.0, 0.0, 25.0);
        // Car movement
        glTranslatef(dtPlayer.slide, dtPlayer.currPost.y, dtPlayer.speed);

        //glTranslatef(0.0, -1.4, 0.0);
        // Make car smaller
        glPushMatrix();
            glScalef(0.7, 1.0, 0.7);
            drawCar(dtPlayer);
        glPopMatrix();

        // Rotate to Z-axis
        // For better look of wheels
        glRotatef(90.0, 0.0, 1.0, 0.0);
        glScalef(0.5, 0.5, 0.5);
        glTranslatef(-3,1,2.9);

        // Wheel section
        // Rear right wheel
        drawWheel(1.0);

        // Rear left wheel
        glTranslatef(0,0,-5.9);
        drawWheel(1.0);

        // Front left wheel
        glTranslatef(8,0,0);
        drawWheel(1.0);

        // Front right wheel
        glTranslatef(0,0,5.9);
        drawWheel(1.0);
    glPopMatrix();

    // Check if weapon active
    if(dtPlayer.weapon.active){
        drawBullets(dtPlayer);
    }

    glutSwapBuffers();
}

void bulletHitDetection(int timer){
    // Check if bullet hit the enemy
    /*if(dtPlayer.weapon.active && dtMidEnemies.status){
        // Calculate based on position start of bullet (x and z) and current position of enemy
        if((dtPlayer.weapon.startPos.x < dtMidEnemies.currPost.x+1 && dtPlayer.weapon.startPos.x > dtMidEnemies.currPost.x-1) && (dtMidEnemies.currPost.z >= dtPlayer.weapon.dz+dtPlayer.weapon.startPos.z)){
            dtMidEnemies.status = false;
        }
    }*/
    if(dtPlayer.weapon.active){
        if(dCollide(dtPlayer.weapon.geometri, dtMidEnemies.geometri, 1, &contacts[0], sizeof(dContactGeom)) > 0){
            dtMidEnemies.status = false;
        }
    }
    //dSpaceCollide(collisionSkybox,0,&nearCallback);
    glutPostRedisplay();
    glutTimerFunc(100, bulletHitDetection, timer);
}

void enemyMovement(int timer){
    /* Enemy movement */
    // Validate value of movement and leveling
    if(movement.countMove > 4){
        movement.countMove = 1;
        if(!reachMaxLevel){
            cout << "Level : " << level << endl;
            level++;
            refreshTimeEnemyMovement -= 40;
        }
    }
    if(refreshTimeEnemyMovement <= 500){
        refreshTimeEnemyMovement = 500;
        maxLevel = level;
        reachMaxLevel = true;
        cout << "Level Max" << endl;
    }
    // Animation of movement
    switch(movement.countMove){
        case 1 : if(movement.x < -20){
                    movement.x += (rand() % 10) + 5;
                 } else if(movement.x < -10){
                    movement.x += (rand() % 8) + 3;
                 } else {
                    movement.x += (rand() % 2) - 4;
                 }
                 dtMidEnemies.currPost.x = movement.x;
                 break;
        case 2 : if(movement.z > -20){
                    movement.z += (rand() % 2) - 9;
                 } else if(movement.z > -10){
                    movement.z += (rand() % 2) - 5;
                 } else{
                    movement.z += (rand() % 5);
                 }
                 dtMidEnemies.currPost.z = movement.z;
                 break;
        case 3 : if(movement.x > 20){
                    movement.x += (rand() % 2) - 9;
                 } else if(movement.x > 10) {
                    movement.x += (rand() % 2) - 5;
                 } else {
                    movement.x += (rand() % 8);
                 }
                 dtMidEnemies.currPost.x = movement.x;
                 break;
        case 4 : if(movement.z > -60){
                    movement.z += (rand() % 8);
                 } else if(movement.z < -40){
                    movement.z += (rand() % 6);
                 } else {
                    movement.z += (rand() % 2) - 5;
                 }
                 dtMidEnemies.currPost.z = movement.z;
                 break;
    }
    // Set current post after movement changes for every enemy
    /*for(int i=0; i < 7; i++){
        carDistance = i == 0 ? 0.0 : -20.0;
        dtEnemies[i].currPost.x = movement.x + carDistance;
        dtEnemies[i].currPost.z = movement.z + carPositionFromPlayer;
        cout << "Posisi enemy " << i << " ( x: " << dtEnemies[i].currPost.x << " - z: " << dtEnemies[i].currPost.z << " )" << endl;
    }*/
    /*if(movement.countMove > 0){
        cout << "Posisi enemy ( x: " << dtMidEnemies.currPost.x << " - z: " << dtMidEnemies.currPost.z << " )" << endl;
    }*/
    dGeomSetPosition(dtMidEnemies.geometri, dtMidEnemies.currPost.x, dtMidEnemies.currPost.y, dtMidEnemies.currPost.z);
    movement.countMove++;
    glutPostRedisplay();
    glutTimerFunc(refreshTimeEnemyMovement, enemyMovement, timer);
}

void playerMovement(int timer){
    /* Player Movement */
    // Checking if bullet still movable
    if(dtPlayer.weapon.active){
        moveBullets();
    }

    // Car movement configuration
    dtPlayer.speed += dtPlayer.acceleration;

    // Set current position of car
    dtPlayer.currPost.z = dtPlayer.speed;
    // Update geometri
    dGeomSetPosition(dtPlayer.geometri, dtPlayer.currPost.x, dtPlayer.currPost.y, dtPlayer.currPost.z+2);

    // Check the last movement car (forward || backward)
    if(movingType == 1){
        // Last movement car is forward
        // Validate value of acceleration
        dtPlayer.acceleration = dtPlayer.acceleration > 1.0 ? 0.9 : dtPlayer.acceleration < -0.4 ? -0.4 : dtPlayer.acceleration;
         if(dtPlayer.acceleration < 0){
            dtPlayer.acceleration += 0.02;
        } else {
            if(dtPlayer.acceleration <= 0){
                dtPlayer.speed += 0;
            }
        }
        /*if(dtPlayer.acceleration == 0){
            dtPlayer.speed += 0;
        } else {
            if(dtPlayer.acceleration <= 0){
                dtPlayer.acceleration += 0.02;
            } else {
                dtPlayer.acceleration -= 0.02;
            }
        }*/
    } else if(movingType == 2){
        // Last movement car is backward
        // Validate value of acceleration
        dtPlayer.acceleration = dtPlayer.acceleration > 1.0 ? 0.9 : dtPlayer.acceleration < -0.4 ? -0.4 : dtPlayer.acceleration;
        if(dtPlayer.acceleration > 0){
            dtPlayer.acceleration -= 0.02;
        } else {
            if(dtPlayer.acceleration <= 0){
                dtPlayer.speed += 0;
            }
        }
                /*if(dtPlayer.acceleration == 0){
            dtPlayer.speed += 0;
        } else {
            if(dtPlayer.acceleration <= 0){
                dtPlayer.acceleration += 0.02;
            } else {
                dtPlayer.acceleration -= 0.02;
            }
        }*/
    } else {
        dtPlayer.acceleration += 0.0;
    }
    //cout << "Posisi mobil player z : " << dtPlayer.currPost.z << ", speed mobil : " << dtPlayer.speed << endl;
    glutPostRedisplay();
    glutTimerFunc(1000/60, playerMovement, timer);
}

void Init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	gluPerspective(60.0, 1400/800, 200/16, 200*16);
	glMatrixMode(GL_MODELVIEW);

	// Load texture
	_textureSky = loadTexture("texture/sky-3.bmp");
	_textureSnow = loadTexture("texture/snow.bmp");

	// Set player car color
	dtPlayer.color.r = 0.098;
	dtPlayer.color.g = 0.098;
	dtPlayer.color.b = 0.439;

	// Set player position
	dtPlayer.currPost.x = 0.0;
	dtPlayer.currPost.y = -1.4;
	dtPlayer.currPost.z = 25;
	dtPlayer.speed = 25;

	// Set initiate bullet position
	dtPlayer.weapon.startPos.x = 300;
	dtPlayer.weapon.startPos.y = 300;
	dtPlayer.weapon.startPos.z = 300;

	// Set enemy car color
	/*for(int i=0; i < 7; i++){
        dtEnemies[i].color.r = 1.0;
        dtEnemies[i].color.g = 0.0;
        dtEnemies[i].color.b = 0.0;
	}*/
	dtMidEnemies.color.r = 1.0;
	dtMidEnemies.color.g = 0.0;
	dtMidEnemies.color.b = 0.0;

	// Set current position of enemy
	dtMidEnemies.currPost.x = movement.x;
    dtMidEnemies.currPost.z = movement.z + carPositionFromPlayer;

	// Assign value for enemy movement
	movement.x = 0.0;
	movement.y = -1.4;
	movement.z = -30;

	// Collision configuration
	dInitODE2(0);
	collisionSkybox = dHashSpaceCreate(0);

	/* Player section */
	dtPlayer.geometri = dCreateBox(0, 2.8, 2.5, 10.5);
	dGeomSetPosition(dtPlayer.geometri, dtPlayer.currPost.x, dtPlayer.currPost.y, dtPlayer.currPost.z+2);
	dtPlayer.space =  dSimpleSpaceCreate(collisionSkybox);
	dSpaceSetCleanup(dtPlayer.space, 0);
	dSpaceAdd(dtPlayer.space, dtPlayer.geometri);

	/* Enemy section */
	dtMidEnemies.geometri = dCreateBox(0, 2.8, 2.5, 10.5);
	dGeomSetPosition(dtMidEnemies.geometri, dtMidEnemies.currPost.x, dtMidEnemies.currPost.y, dtMidEnemies.currPost.z);
	dtMidEnemies.space = dSimpleSpaceCreate(collisionSkybox);
	dSpaceSetCleanup(dtMidEnemies.space, 0);
	dSpaceAdd(dtMidEnemies.space, dtMidEnemies.geometri);

	/* Bullet section */
	dtPlayer.weapon.geometri = dCreateBox(0, 0.2, 0.2, 0.7);
	dGeomSetPosition(dtPlayer.weapon.geometri, dtPlayer.weapon.startPos.x, dtPlayer.weapon.startPos.y, dtPlayer.weapon.startPos.z+dtPlayer.weapon.dz);
	dtPlayer.weapon.space = dSimpleSpaceCreate(collisionSkybox);
	dSpaceSetCleanup(dtPlayer.weapon.space, 0);
	dSpaceAdd(dtPlayer.weapon.space, dtPlayer.weapon.geometri);

	/* Skybox section */
	// Depan
	dtSb.depan.side.x = 0;
	dtSb.depan.side.y = -1.5;
	dtSb.depan.side.z = 100;
	dtSb.depan.geometri = dCreateBox(0, 199, 1, 1);
	dGeomSetPosition(dtSb.depan.geometri, dtSb.depan.side.x, dtSb.depan.side.y, dtSb.depan.side.z);
	dtSb.depan.space = dSimpleSpaceCreate(collisionSkybox);
	dSpaceSetCleanup(dtSb.depan.space, 0);
	dSpaceAdd(dtSb.depan.space, dtSb.depan.geometri);

	// Belakang
	dtSb.belakang.side.x = 0;
	dtSb.belakang.side.y = -1.5;
	dtSb.belakang.side.z = -100;
	dtSb.belakang.geometri = dCreateBox(0, 199, 1, 1);
	dGeomSetPosition(dtSb.belakang.geometri, dtSb.belakang.side.x, dtSb.belakang.side.y, dtSb.belakang.side.z);
	dtSb.belakang.space = dSimpleSpaceCreate(collisionSkybox);
	dSpaceSetCleanup(dtSb.belakang.space, 0);
	dSpaceAdd(dtSb.belakang.space, dtSb.belakang.geometri);

	// Kanan
	dtSb.kanan.side.x = 100;
	dtSb.kanan.side.y = -1.5;
	dtSb.kanan.side.z = 0;
	dtSb.kanan.geometri = dCreateBox(0, 1, 1, 199);
	dGeomSetPosition(dtSb.kanan.geometri, dtSb.kanan.side.x, dtSb.kanan.side.y, dtSb.kanan.side.z);
	dtSb.kanan.space = dSimpleSpaceCreate(collisionSkybox);
	dSpaceSetCleanup(dtSb.kanan.space, 0);
	dSpaceAdd(dtSb.kanan.space, dtSb.kanan.geometri);

	// Kiri
    dtSb.kiri.side.x = -100;
    dtSb.kiri.side.y = -1.5;
    dtSb.kiri.side.z = 0;
    dtSb.kiri.geometri = dCreateBox(0, 1, 1, 199);
    dGeomSetPosition(dtSb.kiri.geometri, dtSb.kiri.side.x, dtSb.kiri.side.y, dtSb.kiri.side.z);
    dtSb.kiri.space = dSimpleSpaceCreate(collisionSkybox);
	dSpaceSetCleanup(dtSb.kiri.space, 0);
	dSpaceAdd(dtSb.kiri.space, dtSb.kiri.geometri);

	/* Center boundary */
	center.geometri = dCreateBox(0, 198, 1, 1);
	dGeomSetPosition(center.geometri, 0, -1.5, 0);
	center.space = dSimpleSpaceCreate(collisionSkybox);
	dSpaceSetCleanup(center.space, 0);
	dSpaceAdd(center.space, center.geometri);

	/* Level settings */
	level = 1;
	maxLevel = 0;
	refreshTimeEnemyMovement = 1000;
}

void moveBullets(){
    dtPlayer.weapon.dz -= 0.5;
    dtPlayer.weapon.status -= 1;
    dtPlayer.weapon.lastPost.z += dtPlayer.weapon.dz;
    dGeomSetPosition(dtPlayer.weapon.geometri, dtPlayer.weapon.startPos.x, dtPlayer.weapon.startPos.y, dtPlayer.weapon.startPos.z+dtPlayer.weapon.dz);
    if(dtPlayer.weapon.status <= 0){
        dtPlayer.weapon.active = false;
        dtPlayer.weapon.startPos.x = 300;
        dtPlayer.weapon.startPos.y = 300;
        dtPlayer.weapon.startPos.z = 300;
        cout << "Posisi terakhir bullet x: " << dtPlayer.weapon.lastPost.x << " - z: " << dtPlayer.weapon.lastPost.z << endl;
        dtPlayer.weapon.dz = 0.0;
    }
}

static void nearCallback (void *, dGeomID o1, dGeomID o2)
{
    int i,n;
    const int N = 4;
    dContact contact[N];
    n = dCollide (o1,o2,N,&contact[0].geom,sizeof(dContact));
    if (n > 0) {
        for (i=0; i<n; i++) {
            printf("contact (%f, %f) => pos: (%f, %f, %f), depth: %f, side1: %d, side2: %d, normal: (%f, %f, %f)\n",
              dGeomGetPosition(contact[i].geom.g1)[0],
              dGeomGetPosition(contact[i].geom.g2)[0],
              contact[i].geom.pos[0],
              contact[i].geom.pos[1],
              contact[i].geom.pos[2],
              contact[i].geom.depth,
              contact[i].geom.side1,
              contact[i].geom.side2,
              contact[i].geom.normal[0],
              contact[i].geom.normal[1],
              contact[i].geom.normal[2]
            );
        }
    }
}

void KeyboardHandler(unsigned char key, int x, int y) {
	switch (key) {
    case 'A':
	case 'a':
		dtPlayer.slide -= 0.5;
		break;
    case 'D':
	case 'd':
        dtPlayer.slide += 0.5;
	    break;
    case 'W':
	case 'w':
	    /*if(dtPlayer.speed > -18){
            dtPlayer.acceleration -= 0.05;
	    } else {
            dtPlayer.acceleration += 0.0;
	    }*/
	    if(dCollide(dtPlayer.geometri, center.geometri, 1, &contacts[1], sizeof(dContactGeom)) > 0){
            dtPlayer.acceleration += 0.0;
	    } else {
            movingType = 1;
            dtPlayer.acceleration -= 0.05;
	    }
	    //movingType = 1;
        //dtPlayer.acceleration -= 0.05;
	    break;
    case 'S':
	case 's':
	    /*if(dtPlayer.speed < 45){
            dtPlayer.acceleration += 0.05;
	    } else {
            dtPlayer.acceleration -= 0.00;
	    }*/
	    movingType = 2;
        dtPlayer.acceleration += 0.05;
	    break;
	case 'o':
		exit(0);
		break;
    case 'E':
    case 'e':
        if(!dtPlayer.weapon.active){
            // Assign start position of bullet
            dtPlayer.weapon.startPos.x = dtPlayer.currPost.x + 1;
            dtPlayer.weapon.startPos.y = -1.3;
            dtPlayer.weapon.startPos.z = dtPlayer.currPost.z - 0.5;
            dtPlayer.weapon.lastPost = dtPlayer.weapon.startPos;
            // Change state of the car (shooting action)
            dtPlayer.weapon.active = true;
            dtPlayer.weapon.status = 100;
            cout << "Start position of bullet, x : " << dtPlayer.weapon.startPos.x << " dan z : " << dtPlayer.weapon.startPos.z << endl;
        }
        break;
    case 'Q':
    case 'q':
            if(stylePersonPOV){
               eyeCamZ =  -4.0;
               eyeCamY = 0.9;
               centerCamZ = -40.0;
               stylePersonPOV = false;
            } else{
                eyeCamZ = 20.0;
                eyeCamY = 6.0;
                centerCamZ = 0.0;
                stylePersonPOV = true;
            }
        break;
	default:
		break;
	}
	dtPlayer.currPost.x = dtPlayer.slide;
	dGeomSetPosition(dtPlayer.geometri, dtPlayer.currPost.x, dtPlayer.currPost.y, dtPlayer.currPost.z+2);
	glutPostRedisplay();
}

void specialKeyboardCb(int key, int x, int y) {
    switch(key){
        case GLUT_KEY_UP:
        {
            spin_y += 1;
            spin_y = spin_y % 360;
            glutPostRedisplay();
            break;
        }
        case GLUT_KEY_DOWN:
        {
            spin_y -= 1;
            spin_y = spin_y % 360;
            glutPostRedisplay();
            break;
        }
        case GLUT_KEY_LEFT:
        {
            spin_x += 1;
            spin_x = spin_x % 360;
            glutPostRedisplay();
            break;
        }
        case GLUT_KEY_RIGHT:
        {
            spin_x -= 1;
            spin_x = spin_x % 360;
            glutPostRedisplay();
            break;
        }
    }
}

void Reshape(GLsizei width, GLsizei height) {
	const float ar = (float) width / (float) height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 300.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

GLuint loadTexture(const char* filename) {
	BmpLoader bl(filename);
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_RGB,
				 bl.iWidth, bl.iHeight,
				 0,
				 GL_RGB,
				 GL_UNSIGNED_BYTE,
				 bl.textureData);
	return textureId;
}
