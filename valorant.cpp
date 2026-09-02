#include <GL/glut.h>
#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>


float bladeAngle = 0.0f;

// BASIC CUBE

void drawCube(float x, float y, float z,
              float width, float height, float depth)
{
    glPushMatrix();

    glTranslatef(x, y, z);
    glScalef(width, height, depth);

    glutSolidCube(1.0f);

    glPopMatrix();
}

// BOX

void drawBox(float x, float z)
{
    glColor3f(0.55f, 0.32f, 0.12f);

    drawCube(
        x, 1.0f, z,
        2.0f, 2.0f, 2.0f);
}

// GENERATOR FOR A SITE

void drawGenerator(float x, float z)
{
    glColor3f(0.45f, 0.45f, 0.45f);

    drawCube(
        x, 1.5f, z,
        3.0f, 3.0f, 2.0f);

    glColor3f(0.25f, 0.25f, 0.25f);

    drawCube(
        x, 3.1f, z,
        3.2f, 0.25f, 2.2f);
    glColor3f(0.15f, 0.15f, 0.15f);

    drawCube(
        x + 0.9f, 4.0f, z,
        0.3f, 1.5f, 0.3f);
}

// LONG BUILDING

void drawLongBuilding(float x, float z)
{
    glColor3f(0.35f, 0.38f, 0.42f);

    // Tallest
    drawCube(
        x, 5.0f, z,
        5.0f, 10.0f, 3.0f);

    // Roof
    glColor3f(0.20f, 0.20f, 0.22f);

    drawCube(
        x, 10.2f, z,
        5.3f, 0.4f, 3.3f);
}

// FAT BUILDING

void drawFatBuilding(float x, float z)
{
    glColor3f(0.42f, 0.42f, 0.45f);

    drawCube(
        x, 3.5f, z,
        8.0f, 7.0f, 4.0f);

    glColor3f(0.20f, 0.20f, 0.22f);

    drawCube(
        x, 7.2f, z,
        8.3f, 0.4f, 4.3f);
}

// SMALL BUILDING

void drawSmallBuilding(float x, float z)
{
    glColor3f(0.50f, 0.40f, 0.32f);

    drawCube(
        x, 1.75f, z,
        2.8f, 3.5f, 2.5f);

    glColor3f(0.20f, 0.20f, 0.22f);

    drawCube(
        x, 3.7f, z,
        3.1f, 0.4f, 2.8f);
}

// PYRAMID ROOF FOR WINDMILL

void drawWindMillPyramid(
    float x,
    float baseY,
    float z)
{
    float halfWidth = 2.4f;
    float halfDepth = 2.0f;
    float topY = baseY + 5.0f;

    // FRONT FACE

    glBegin(GL_TRIANGLES);

    glVertex3f(
        x - halfWidth,
        baseY,
        z + halfDepth);

    glVertex3f(
        x + halfWidth,
        baseY,
        z + halfDepth);

    glVertex3f(
        x,
        topY,
        z);

    // RIGHT FACE

    glVertex3f(
        x + halfWidth,
        baseY,
        z + halfDepth);

    glVertex3f(
        x + halfWidth,
        baseY,
        z - halfDepth);

    glVertex3f(
        x,
        topY,
        z);

    // BACK FACE

    glVertex3f(
        x + halfWidth,
        baseY,
        z - halfDepth);

    glVertex3f(
        x - halfWidth,
        baseY,
        z - halfDepth);

    glVertex3f(
        x,
        topY,
        z);

    // LEFT FACE

    glVertex3f(
        x - halfWidth,
        baseY,
        z - halfDepth);

    glVertex3f(
        x - halfWidth,
        baseY,
        z + halfDepth);

    glVertex3f(
        x,
        topY,
        z);

    glEnd();
}

// LOG

void drawWindMillCylinder(
    float radius,
    float length)
{
    GLUquadric *quad =
        gluNewQuadric();

    glPushMatrix();

    glRotatef(
        -90.0f,
        1.0f,
        0.0f,
        0.0f);

    gluCylinder(
        quad,
        radius,
        radius,
        length,
        32,
        8);

    // Front 
    glPushMatrix();

    glRotatef(
        180.0f,
        1.0f,
        0.0f,
        0.0f);

    gluDisk(
        quad,
        0.0f,
        radius,
        32,
        1);

    glPopMatrix();

    // Back 
    glPushMatrix();

    glTranslatef(
        0.0f,
        0.0f,
        length);

    gluDisk(
        quad,
        0.0f,
        radius,
        32,
        1);

    glPopMatrix();

    glPopMatrix();

    gluDeleteQuadric(quad);
}

//WINDMILL BLADE

void drawWindMillBlade()
{
    glBegin(GL_QUADS);

    // FRONT

    glVertex3f(
        -0.35f,
        0.0f,
        0.12f);

    glVertex3f(
        0.35f,
        0.0f,
        0.12f);

    glVertex3f(
        0.55f,
        4.0f,
        0.12f);

    glVertex3f(
        -0.55f,
        4.0f,
        0.12f);

    // BACK

    glVertex3f(
        -0.35f,
        0.0f,
        -0.12f);

    glVertex3f(
        -0.55f,
        4.0f,
        -0.12f);

    glVertex3f(
        0.55f,
        4.0f,
        -0.12f);

    glVertex3f(
        0.35f,
        0.0f,
        -0.12f);

    // LEFT SIDE

    glVertex3f(
        -0.35f,
        0.0f,
        -0.12f);

    glVertex3f(
        -0.35f,
        0.0f,
        0.12f);

    glVertex3f(
        -0.55f,
        4.0f,
        0.12f);

    glVertex3f(
        -0.55f,
        4.0f,
        -0.12f);

    // RIGHT SIDE

    glVertex3f(
        0.35f,
        0.0f,
        0.12f);

    glVertex3f(
        0.35f,
        0.0f,
        -0.12f);

    glVertex3f(
        0.55f,
        4.0f,
        -0.12f);

    glVertex3f(
        0.55f,
        4.0f,
        0.12f);

    // TOP

    glVertex3f(
        -0.55f,
        4.0f,
        0.12f);

    glVertex3f(
        0.55f,
        4.0f,
        0.12f);

    glVertex3f(
        0.55f,
        4.0f,
        -0.12f);

    glVertex3f(
        -0.55f,
        4.0f,
        -0.12f);

    glEnd();
}

// WIND MILL BUILDING

void drawWindMillBuilding(float x, float z)
{

    glColor3f(
        0.35f,
        0.38f,
        0.42f);

    drawCube(
        x,
        7.0f,
        z,
        4.5f,
        14.0f,
        3.5f);

    glColor3f(
        0.20f,
        0.20f,
        0.22f);

    drawCube(
        x,
        0.25f,
        z,
        4.8f,
        0.5f,
        3.8f);

    glColor3f(
        0.20f,
        0.20f,
        0.22f);
    drawWindMillPyramid(
        x,
        14.0f,
        z);


    glColor3f(
        0.55f,
        0.55f,
        0.55f);

    float rotorY = 16.5f;

    glPushMatrix();

    glTranslatef(
        x,
        rotorY,
        z + 1.0f);

    drawWindMillCylinder(
        0.45f,
        2.0f);

    glPopMatrix();

    glPushMatrix();

    glTranslatef(
        x,
        rotorY,
        z + 3.0f);


    glRotatef(
        bladeAngle,
        0.0f,
        0.0f,
        1.0f);

    glColor3f(
        0.70f,
        0.70f,
        0.70f);

    glPushMatrix();

    drawWindMillBlade();

    glPopMatrix();

    glPushMatrix();

    glRotatef(
        90.0f,
        0.0f,
        0.0f,
        1.0f);

    drawWindMillBlade();

    glPopMatrix();

    glPushMatrix();

    glRotatef(
        180.0f,
        0.0f,
        0.0f,
        1.0f);

    drawWindMillBlade();

    glPopMatrix();

    glPushMatrix();

    glRotatef(
        270.0f,
        0.0f,
        0.0f,
        1.0f);

    drawWindMillBlade();

    glPopMatrix();

    glColor3f(
        0.20f,
        0.20f,
        0.22f);

    glutSolidSphere(
        0.65f,
        32,
        32);

    glPopMatrix();
}

// B MAIN TUNNEL

void drawBMainTunnel(float x, float z)
{
    float width = 4.5f;
    float height = 5.0f;

    glColor3f(0.32f, 0.32f, 0.35f);

    // Left wall
    drawCube(
        x - width / 2.0f,
        height / 2.0f,
        z,
        1.0f,
        height,
        4.0f);

    // Right wall
    drawCube(
        x + width / 2.0f,
        height / 2.0f,
        z,
        1.0f,
        height,
        4.0f);

    // Top
    drawCube(
        x,
        height,
        z,
        width + 1.0f,
        1.0f,
        4.0f);
}

// B MARKET TUNNEL

void drawBMarketTunnel(float x, float z)
{
    float width = 3.5f;
    float height = 4.0f;

    glColor3f(0.38f, 0.38f, 0.40f);

    // Left wall
    drawCube(
        x - width / 2.0f,
        height / 2.0f,
        z,
        0.9f,
        height,
        3.5f);

    // Right wall
    drawCube(
        x + width / 2.0f,
        height / 2.0f,
        z,
        0.9f,
        height,
        3.5f);

    // Top
    drawCube(
        x,
        height,
        z,
        width + 0.9f,
        0.9f,
        3.5f);
}

// WALL

void drawWall(float x, float z,
              float width,
              float height,
              float depth)
{
    glColor3f(0.30f, 0.30f, 0.32f);

    drawCube(
        x,
        height / 2.0f,
        z,
        width,
        height,
        depth);
}

// STAIR

void drawStair(float x, float z)
{
    glColor3f(0.45f, 0.45f, 0.47f);

    int steps = 8;

    for (int i = 0; i < steps; i++)
    {
        float height = (i + 1) * 0.3f;

        drawCube(
            x,
            height / 2.0f,
            z + i * 0.45f,
            3.0f,
            height,
            0.5f);
    }
}

// FLOOR

void drawFloor()
{
    glColor3f(0.18f, 0.18f, 0.18f);

    glBegin(GL_QUADS);

    glVertex3f(
        -40.0f,
        0.0f,
        -12.0f);

    glVertex3f(
        40.0f,
        0.0f,
        -12.0f);

    glVertex3f(
        40.0f,
        0.0f,
        12.0f);

    glVertex3f(
        -40.0f,
        0.0f,
        12.0f);

    glEnd();
}

// DISPLAY

void display()
{
    glClear(
        GL_COLOR_BUFFER_BIT |
        GL_DEPTH_BUFFER_BIT);

    glMatrixMode(
        GL_MODELVIEW);

    glLoadIdentity();

    gluLookAt(
        0.0f,
        17.0f,
        32.0f,

        0.0f,
        3.5f,
        0.0f,

        0.0f,
        1.0f,
        0.0f);

    drawFloor();
  

    drawBox(
        -27.0f,
        0.0f);

    drawGenerator(
        -21.0f,
        0.0f);

    drawLongBuilding(
        -12.0f,
        0.0f);

    drawFatBuilding(
        -3.0f,
        0.0f);

    drawSmallBuilding(
        4.5f,
        0.0f);

    drawWindMillBuilding(
        11.0f,
        0.0f);

    drawBMainTunnel(
        17.0f,
        0.0f);

    drawBMarketTunnel(
        23.0f,
        0.0f);

    drawStair(
        28.0f,
        0.0f);

    drawWall(
        0.0f,
        -7.0f,
        60.0f,
        3.0f,
        0.5f);

    glutSwapBuffers();
}

// ANIMATION

void update(int value)
{
    // Windmill rotation speed
    bladeAngle += 2.0f;

    if (bladeAngle >= 360.0f)
    {
        bladeAngle -= 360.0f;
    }

    glutPostRedisplay();

    glutTimerFunc(
        16,
        update,
        0);
}

// WINDOW RESIZE

void reshape(
    int width,
    int height)
{
    if (height == 0)
        height = 1;

    float ratio =
        (float)width /
        (float)height;

    glViewport(
        0,
        0,
        width,
        height);

    glMatrixMode(
        GL_PROJECTION);

    glLoadIdentity();

    gluPerspective(
        60.0f,
        ratio,
        0.1f,
        200.0f);

    glMatrixMode(
        GL_MODELVIEW);
}

// MAIN

int main(
    int argc,
    char **argv)
{
    glutInit(
        &argc,
        argv);

    glutInitDisplayMode(
        GLUT_DOUBLE |
        GLUT_RGB |
        GLUT_DEPTH);

    glutInitWindowSize(
        1280,
        720);

    glutCreateWindow(
        "FPS Game - Temporary Structures");

    glEnable(
        GL_DEPTH_TEST);

    glClearColor(
        0.08f,
        0.08f,
        0.12f,
        1.0f);

    glutDisplayFunc(
        display);

    glutReshapeFunc(
        reshape);

    //windmill animation
    glutTimerFunc(
        0,
        update,
        0);

    glutMainLoop();

    return 0;
}
