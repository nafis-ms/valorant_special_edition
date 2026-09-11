#include <GL/glut.h>
#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>

// ============================================================
// GRAPHICS AND ANIMATION PROJECT
// 3D GAME MAP
//
// SB  = Small Building
// FB  = Fat Building
// WM  = Wind Mill
// T1  = Tunnel 1
// T2  = Tunnel 2
// FB2 = Fat Building 2
//
// Fixed camera
// Keyboard controls ONLY map rotation
// No camera rotation
// No textures
// No lighting
// ============================================================

float windmillAngle = 0.0f;

// ============================================================
// MAP ROTATION
// ============================================================

float mapRotationX = 0.0f;
float mapRotationY = 0.0f;
float mapRotationZ = 0.0f;

// ============================================================
// DRAW SOLID CUBE
// ============================================================

void drawCube(
    float x,
    float y,
    float z,
    float width,
    float height,
    float depth)
{
    glPushMatrix();

    glTranslatef(x, y, z);
    glScalef(width, height, depth);

    glutSolidCube(1.0f);

    glPopMatrix();
}

// ============================================================
// DRAW TOP SURFACE
// ============================================================

void drawTop(
    float x,
    float y,
    float z,
    float width,
    float depth)
{
    glBegin(GL_QUADS);

    glVertex3f(
        x - width / 2,
        y,
        z - depth / 2
    );

    glVertex3f(
        x + width / 2,
        y,
        z - depth / 2
    );

    glVertex3f(
        x + width / 2,
        y,
        z + depth / 2
    );

    glVertex3f(
        x - width / 2,
        y,
        z + depth / 2
    );

    glEnd();
}

// ============================================================
// FLOOR
// ============================================================

void drawFloor()
{
    glColor3f(
        0.22f,
        0.22f,
        0.23f
    );

    glBegin(GL_QUADS);

    glVertex3f(-25.0f, 0.0f, -24.0f);
    glVertex3f(25.0f, 0.0f, -24.0f);
    glVertex3f(25.0f, 0.0f, 24.0f);
    glVertex3f(-25.0f, 0.0f, 24.0f);

    glEnd();
}

// ============================================================
// WALL
// ============================================================

void drawWall(
    float x,
    float z,
    float width,
    float depth,
    float height = 3.0f)
{
    glColor3f(
        0.62f,
        0.63f,
        0.65f
    );

    drawCube(
        x,
        height / 2.0f,
        z,
        width,
        height,
        depth
    );
}

// ============================================================
// WALL WITH SMALLER TOP
// ============================================================

void drawWallDetailed(
    float x,
    float z,
    float width,
    float depth,
    float height = 3.0f)
{
    drawWall(
        x,
        z,
        width,
        depth,
        height
    );

    glColor3f(
        0.72f,
        0.73f,
        0.75f
    );

    drawCube(
        x,
        height + 0.08f,
        z,
        width + 0.10f,
        0.16f,
        depth + 0.10f
    );
}

// ============================================================
// SMALL BUILDING
// ============================================================

void drawSmallBuilding(
    float x,
    float z,
    float width = 3.5f,
    float depth = 3.5f)
{
    glColor3f(
        0.40f,
        0.41f,
        0.43f
    );

    drawCube(
        x,
        2.0f,
        z,
        width,
        4.0f,
        depth
    );

    glColor3f(
        0.72f,
        0.73f,
        0.75f
    );

    drawCube(
        x,
        4.08f,
        z,
        width + 0.18f,
        0.16f,
        depth + 0.18f
    );
}

// ============================================================
// FAT BUILDING
// ============================================================

void drawFatBuilding(
    float x,
    float z,
    float width = 5.0f,
    float depth = 6.0f)
{
    glColor3f(
        0.40f,
        0.41f,
        0.43f
    );

    drawCube(
        x,
        3.0f,
        z,
        width,
        6.0f,
        depth
    );

    glColor3f(
        0.72f,
        0.73f,
        0.75f
    );

    drawCube(
        x,
        6.08f,
        z,
        width + 0.18f,
        0.16f,
        depth + 0.18f
    );
}

// ============================================================
// T2
// ============================================================

void drawT2()
{
    float x = -7.0f;
    float z = 11.2f;

    glColor3f(
        0.39f,
        0.40f,
        0.42f
    );

    drawCube(
        x,
        2.5f,
        z,
        3.6f,
        5.0f,
        3.2f
    );

    glColor3f(
        0.72f,
        0.73f,
        0.75f
    );

    drawCube(
        x,
        5.08f,
        z,
        3.82f,
        0.16f,
        3.42f
    );
}

// ============================================================
// T1 TUNNEL
// LONG HORIZONTAL STRUCTURE WITH OPENING
// ============================================================

void drawT1()
{
    float left = -21.0f;
    float right = -2.5f;

    float front = 2.7f;
    float back = 6.0f;

    float height = 4.0f;
    float wallThickness = 0.65f;

    glColor3f(
        0.46f,
        0.47f,
        0.49f
    );

    // --------------------------------------------------------
    // BACK
    // --------------------------------------------------------

    drawCube(
        (left + right) / 2.0f,
        height / 2.0f,
        back,
        right - left,
        height,
        wallThickness
    );

    // --------------------------------------------------------
    // LEFT SIDE
    // --------------------------------------------------------

    drawCube(
        left,
        height / 2.0f,
        (front + back) / 2.0f,
        wallThickness,
        height,
        back - front
    );

    // --------------------------------------------------------
    // RIGHT SIDE
    // --------------------------------------------------------

    drawCube(
        right,
        height / 2.0f,
        (front + back) / 2.0f,
        wallThickness,
        height,
        back - front
    );

    // --------------------------------------------------------
    // FRONT LEFT
    // --------------------------------------------------------

    drawCube(
        -16.7f,
        height / 2.0f,
        front,
        8.6f,
        height,
        wallThickness
    );

    // --------------------------------------------------------
    // FRONT RIGHT
    // --------------------------------------------------------

    drawCube(
        -5.0f,
        height / 2.0f,
        front,
        4.6f,
        height,
        wallThickness
    );

    // --------------------------------------------------------
    // ROOF
    // --------------------------------------------------------

    glColor3f(
        0.55f,
        0.56f,
        0.58f
    );

    drawCube(
        (left + right) / 2.0f,
        height,
        (front + back) / 2.0f,
        right - left + 0.7f,
        0.55f,
        back - front + 0.7f
    );

    // --------------------------------------------------------
    // TUNNEL OPENING
    // --------------------------------------------------------

    glColor3f(
        0.08f,
        0.08f,
        0.09f
    );

    glBegin(GL_QUADS);

    float openingX = -11.5f;
    float openingWidth = 1.35f;
    float openingHeight = 1.35f;

    glVertex3f(
        openingX - openingWidth / 2.0f,
        0.0f,
        front - 0.36f
    );

    glVertex3f(
        openingX + openingWidth / 2.0f,
        0.0f,
        front - 0.36f
    );

    glVertex3f(
        openingX + openingWidth / 2.0f,
        openingHeight,
        front - 0.36f
    );

    glVertex3f(
        openingX - openingWidth / 2.0f,
        openingHeight,
        front - 0.36f
    );

    glEnd();

    // --------------------------------------------------------
    // SEMI-CIRCULAR TOP OF OPENING
    // --------------------------------------------------------

    glBegin(GL_TRIANGLE_FAN);

    glVertex3f(
        openingX,
        openingHeight,
        front - 0.37f
    );

    for (int i = 0; i <= 20; i++)
    {
        float angle =
            3.1415926f -
            (3.1415926f * i / 20.0f);

        float px =
            openingX +
            cos(angle) * openingWidth / 2.0f;

        float py =
            openingHeight +
            sin(angle) * openingWidth / 2.0f;

        glVertex3f(
            px,
            py,
            front - 0.37f
        );
    }

    glEnd();
}

// ============================================================
// STAIRS
// ============================================================

void drawStairs(
    float x,
    float y,
    float z,
    int count,
    bool alongX)
{
    glColor3f(
        0.55f,
        0.56f,
        0.58f
    );

    for (int i = 0; i < count; i++)
    {
        float height =
            0.30f +
            i * 0.30f;

        if (alongX)
        {
            drawCube(
                x + i * 0.55f,
                height / 2.0f,
                z,
                0.60f,
                height,
                3.0f
            );
        }
        else
        {
            drawCube(
                x,
                height / 2.0f,
                z + i * 0.55f,
                3.0f,
                height,
                0.60f
            );
        }
    }
}

// ============================================================
// WINDMILL BASE
// ============================================================

void drawWindMill()
{
    float x = -1.0f;
    float z = -5.0f;

    // --------------------------------------------------------
    // BASE
    // --------------------------------------------------------

    glColor3f(
        0.40f,
        0.41f,
        0.43f
    );

    drawCube(
        x,
        2.0f,
        z,
        4.0f,
        4.0f,
        4.0f
    );

    glColor3f(
        0.72f,
        0.73f,
        0.75f
    );

    drawCube(
        x,
        4.08f,
        z,
        4.20f,
        0.16f,
        4.20f
    );

    // --------------------------------------------------------
    // CYLINDER
    // --------------------------------------------------------

    glColor3f(
        0.50f,
        0.51f,
        0.53f
    );

    GLUquadric* quad =
        gluNewQuadric();

    glPushMatrix();

    glTranslatef(
        x,
        4.0f,
        z
    );

    gluCylinder(
        quad,
        0.65f,
        0.45f,
        3.0f,
        20,
        10
    );

    glPopMatrix();

    // --------------------------------------------------------
    // WINDMILL ROTOR
    // --------------------------------------------------------

    glPushMatrix();

    glTranslatef(
        x,
        7.0f,
        z
    );

    glRotatef(
        windmillAngle,
        0.0f,
        0.0f,
        1.0f
    );

    glColor3f(
        0.70f,
        0.71f,
        0.73f
    );

    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();

        glRotatef(
            i * 90.0f,
            0.0f,
            0.0f,
            1.0f
        );

        drawCube(
            0.0f,
            1.25f,
            0.0f,
            0.38f,
            2.5f,
            0.15f
        );

        glPopMatrix();
    }

    glColor3f(
        0.25f,
        0.26f,
        0.28f
    );

    glutSolidSphere(
        0.40f,
        20,
        20
    );

    glPopMatrix();

    gluDeleteQuadric(
        quad
    );
}

// ============================================================
// COMPLETE MAP
// ============================================================

void drawMap()
{
    // ========================================================
    // OUTER WALL - LEFT
    // ========================================================

    drawWallDetailed(
        -21.5f,
        0.0f,
        0.75f,
        33.0f
    );

    // ========================================================
    // OUTER WALL - TOP LEFT
    // ========================================================

    drawWallDetailed(
        -17.0f,
        16.5f,
        9.0f,
        0.75f
    );

    // ========================================================
    // TOP LEFT RAISED SECTION
    // ========================================================

    drawWallDetailed(
        -12.0f,
        18.8f,
        0.75f,
        5.0f
    );

    // ========================================================
    // TOP CENTER WALL
    // ========================================================

    drawWallDetailed(
        1.0f,
        20.0f,
        12.5f,
        0.75f
    );

    // ========================================================
    // TOP CENTER LEFT VERTICAL
    // ========================================================

    drawWallDetailed(
        -5.0f,
        18.5f,
        0.75f,
        4.0f
    );

    // ========================================================
    // TOP CENTER RIGHT VERTICAL
    // ========================================================

    drawWallDetailed(
        7.0f,
        18.5f,
        0.75f,
        4.0f
    );

    // ========================================================
    // TOP RIGHT RAISED WALL
    // ========================================================

    drawWallDetailed(
        11.0f,
        18.0f,
        0.75f,
        6.0f
    );

    // ========================================================
    // UPPER RIGHT OUTER WALL
    // ========================================================

    drawWallDetailed(
        17.0f,
        15.5f,
        0.75f,
        8.0f
    );

    // ========================================================
    // RIGHT UPPER HORIZONTAL
    // ========================================================

    drawWallDetailed(
        19.5f,
        12.0f,
        5.5f,
        0.75f
    );

    // ========================================================
    // RIGHT OUTER WALL
    // ========================================================

    drawWallDetailed(
        22.0f,
        3.0f,
        0.75f,
        18.0f
    );

    // ========================================================
    // RIGHT MIDDLE OUTER SECTION
    // ========================================================

    drawWallDetailed(
        20.5f,
        -5.0f,
        4.0f,
        0.75f
    );

    // ========================================================
    // RIGHT LOWER OUTER WALL
    // ========================================================

    drawWallDetailed(
        18.0f,
        -11.0f,
        0.75f,
        12.0f
    );

    // ========================================================
    // RIGHT BOTTOM VERTICAL
    // ========================================================

    drawWallDetailed(
        17.0f,
        -17.0f,
        0.75f,
        8.0f
    );

    // ========================================================
    // BOTTOM OUTER WALL
    // ========================================================

    drawWallDetailed(
        0.0f,
        -20.0f,
        34.0f,
        0.75f
    );

    // ========================================================
    // BOTTOM LEFT VERTICAL
    // ========================================================

    drawWallDetailed(
        -17.0f,
        -17.0f,
        0.75f,
        6.0f
    );

    // ========================================================
    // LOWER LEFT HORIZONTAL
    // ========================================================

    drawWallDetailed(
        -19.0f,
        -12.0f,
        5.0f,
        0.75f
    );

    // ========================================================
    // TOP LEFT INTERNAL WALL
    // ========================================================

    drawWallDetailed(
        -12.0f,
        11.0f,
        0.75f,
        8.0f
    );

    // ========================================================
    // WALL ABOVE T1
    // ========================================================

    drawWallDetailed(
        -17.0f,
        8.0f,
        9.0f,
        0.75f
    );

    // ========================================================
    // T1
    // ========================================================

    drawT1();

    // ========================================================
    // CENTRAL VERTICAL WALL
    // ========================================================

    drawWallDetailed(
        -4.0f,
        -2.0f,
        0.65f,
        15.0f
    );

    // ========================================================
    // TOP INTERNAL HORIZONTAL
    // ========================================================

    drawWallDetailed(
        0.5f,
        14.0f,
        15.0f,
        0.65f
    );

    // ========================================================
    // RIGHT INTERNAL WALL
    // ========================================================

    drawWallDetailed(
        15.0f,
        5.0f,
        4.0f,
        0.65f
    );

    // ========================================================
    // SMALL RIGHT WALL
    // ========================================================

    drawWallDetailed(
        14.0f,
        2.0f,
        0.65f,
        4.0f
    );

    // ========================================================
    // LOWER RIGHT INTERNAL WALL
    // ========================================================

    drawWallDetailed(
        15.0f,
        -3.0f,
        4.0f,
        0.65f
    );

    // ========================================================
    // LOWER RIGHT SHORT WALL
    // ========================================================

    drawWallDetailed(
        14.0f,
        -1.0f,
        3.0f,
        0.65f
    );

    // ========================================================
    // LOWER CENTER WALL
    // ========================================================

    drawWallDetailed(
        -4.0f,
        -10.0f,
        0.65f,
        5.0f
    );

    // ========================================================
    // T2
    // ========================================================

    drawT2();

    // ========================================================
    // TOP FB
    // ========================================================

    drawFatBuilding(
        0.0f,
        8.0f,
        4.5f,
        5.5f
    );

    // ========================================================
    // FB2
    // ========================================================

    drawFatBuilding(
        8.0f,
        10.0f,
        3.0f,
        4.5f
    );

    // ========================================================
    // MIDDLE RIGHT FB
    // ========================================================

    drawFatBuilding(
        9.0f,
        -3.0f,
        4.5f,
        6.0f
    );

    // ========================================================
    // LEFT SB
    // ========================================================

    drawSmallBuilding(
        -17.0f,
        -7.0f,
        3.5f,
        3.5f
    );

    // ========================================================
    // WINDMILL
    // ========================================================

    drawWindMill();

    // ========================================================
    // BOTTOM FB
    // ========================================================

    drawFatBuilding(
        0.5f,
        -13.0f,
        8.0f,
        3.5f
    );

    // ========================================================
    // BOTTOM SB
    // ========================================================

    drawSmallBuilding(
        7.0f,
        -13.0f,
        3.0f,
        3.5f
    );

    // ========================================================
    // TOP LEFT STAIRS
    // ========================================================

    drawStairs(
        -8.0f,
        0.0f,
        15.8f,
        7,
        true
    );

    // ========================================================
    // TOP RIGHT STAIRS
    // ========================================================

    drawStairs(
        10.5f,
        0.0f,
        15.8f,
        7,
        true
    );

    // ========================================================
    // BOTTOM LEFT STAIRS
    // ========================================================

    drawStairs(
        -8.5f,
        0.0f,
        -16.0f,
        8,
        true
    );

    // ========================================================
    // SMALL WALL BESIDE BOTTOM STAIRS
    // ========================================================

    drawWallDetailed(
        -6.0f,
        -12.5f,
        0.65f,
        5.0f
    );

    // ========================================================
    // SMALL WALL BESIDE BOTTOM SB
    // ========================================================

    drawWallDetailed(
        11.0f,
        -12.5f,
        0.65f,
        7.0f
    );
}

// ============================================================
// DISPLAY
// ============================================================

void display()
{
    glClear(
        GL_COLOR_BUFFER_BIT |
        GL_DEPTH_BUFFER_BIT
    );

    glMatrixMode(
        GL_MODELVIEW
    );

    glLoadIdentity();

    // ========================================================
    // FIXED CAMERA
    // ========================================================

    gluLookAt(
        0.0f,
        34.0f,
        38.0f,

        0.0f,
        0.0f,
        0.0f,

        0.0f,
        1.0f,
        0.0f
    );

    // ========================================================
    // MAP ROTATION
    // ONLY THE MAP IS ROTATED
    // CAMERA REMAINS FIXED
    // ========================================================

    glPushMatrix();

    glRotatef(
        mapRotationX,
        1.0f,
        0.0f,
        0.0f
    );

    glRotatef(
        mapRotationY,
        0.0f,
        1.0f,
        0.0f
    );

    glRotatef(
        mapRotationZ,
        0.0f,
        0.0f,
        1.0f
    );

    drawFloor();

    drawMap();

    glPopMatrix();

    glutSwapBuffers();
}

// ============================================================
// KEYBOARD
// MAP ROTATION ONLY
// ============================================================

void keyboard(
    unsigned char key,
    int x,
    int y)
{
    switch (key)
    {
        // ----------------------------------------------------
        // ROTATE LEFT
        // ----------------------------------------------------

        case 'a':
        case 'A':
            mapRotationY -= 5.0f;
            break;

        // ----------------------------------------------------
        // ROTATE RIGHT
        // ----------------------------------------------------

        case 'd':
        case 'D':
            mapRotationY += 5.0f;
            break;

        // ----------------------------------------------------
        // ROTATE UP
        // ----------------------------------------------------

        case 'w':
        case 'W':
            mapRotationX -= 5.0f;
            break;

        // ----------------------------------------------------
        // ROTATE DOWN
        // ----------------------------------------------------

        case 's':
        case 'S':
            mapRotationX += 5.0f;
            break;

        // ----------------------------------------------------
        // ROTATE CLOCKWISE
        // ----------------------------------------------------

        case 'q':
        case 'Q':
            mapRotationZ += 5.0f;
            break;

        // ----------------------------------------------------
        // ROTATE COUNTER-CLOCKWISE
        // ----------------------------------------------------

        case 'e':
        case 'E':
            mapRotationZ -= 5.0f;
            break;

        // ----------------------------------------------------
        // RESET ROTATION
        // ----------------------------------------------------

        case 'r':
        case 'R':
            mapRotationX = 0.0f;
            mapRotationY = 0.0f;
            mapRotationZ = 0.0f;
            break;
    }

    glutPostRedisplay();
}

// ============================================================
// WINDMILL ANIMATION
// ============================================================

void update(
    int value)
{
    windmillAngle += 2.0f;

    if (windmillAngle >= 360.0f)
    {
        windmillAngle -= 360.0f;
    }

    glutPostRedisplay();

    glutTimerFunc(
        16,
        update,
        0
    );
}

// ============================================================
// WINDOW RESIZE
// ============================================================

void reshape(
    int width,
    int height)
{
    if (height == 0)
    {
        height = 1;
    }

    float aspect =
        (float)width /
        (float)height;

    glViewport(
        0,
        0,
        width,
        height
    );

    glMatrixMode(
        GL_PROJECTION
    );

    glLoadIdentity();

    gluPerspective(
        55.0f,
        aspect,
        0.1f,
        150.0f
    );

    glMatrixMode(
        GL_MODELVIEW
    );
}

// ============================================================
// MAIN
// ============================================================

int main(
    int argc,
    char** argv)
{
    glutInit(
        &argc,
        argv
    );

    glutInitDisplayMode(
        GLUT_DOUBLE |
        GLUT_RGB |
        GLUT_DEPTH
    );

    glutInitWindowSize(
        1280,
        720
    );

    glutInitWindowPosition(
        100,
        50
    );

    glutCreateWindow(
        "Graphics and Animation - Game Map"
    );

    // --------------------------------------------------------
    // DEPTH
    // --------------------------------------------------------

    glEnable(
        GL_DEPTH_TEST
    );

    // --------------------------------------------------------
    // NO LIGHTING
    // --------------------------------------------------------

    glDisable(
        GL_LIGHTING
    );

    // --------------------------------------------------------
    // FLAT SHADING
    // --------------------------------------------------------

    glShadeModel(
        GL_FLAT
    );

    // --------------------------------------------------------
    // BACKGROUND
    // --------------------------------------------------------

    glClearColor(
        0.13f,
        0.13f,
        0.15f,
        1.0f
    );

    // --------------------------------------------------------
    // CALLBACKS
    // --------------------------------------------------------

    glutDisplayFunc(
        display
    );

    glutReshapeFunc(
        reshape
    );

    // --------------------------------------------------------
    // KEYBOARD FUNCTION
    // --------------------------------------------------------

    glutKeyboardFunc(
        keyboard
    );

    // --------------------------------------------------------
    // WINDMILL ANIMATION
    // --------------------------------------------------------

    glutTimerFunc(
        0,
        update,
        0
    );


    glutMainLoop();

    return 0;
}
