#include <windows.h>
#include <GL/glut.h>

// ============================================================
// MAP ROTATION
// ============================================================

float mapRotationX = 0.0f;
float mapRotationY = 0.0f;
float mapRotationZ = 0.0f;

// ============================================================
// WINDMILL
// ============================================================

float windmillAngle = 0.0f;

// ============================================================
// DRAW CUBE
// ============================================================

void drawCube(float x, float y, float z,
              float width, float height, float depth)
{
    glPushMatrix();

    glTranslatef(x, y, z);
    glScalef(width, height, depth);

    glutSolidCube(1.0f);

    glPopMatrix();
}

// ============================================================
// DRAW FLOOR
// ============================================================

void drawFloor()
{
    glColor3f(0.42f, 0.44f, 0.45f);

    drawCube(
        0.0f,
        -0.20f,
        0.0f,
        22.0f,
        0.40f,
        22.0f
    );
}

// ============================================================
// DRAW WALL
// ============================================================

void drawWall(float x, float y, float z,
              float width, float height, float depth)
{
    glColor3f(0.72f, 0.74f, 0.75f);

    drawCube(x, y, z, width, height, depth);
}

// ============================================================
// DRAW BUILDING
// ============================================================

void drawBuilding(float x, float y, float z,
                  float width, float height, float depth)
{
    glColor3f(0.58f, 0.60f, 0.61f);

    drawCube(
        x, y, z,
        width, height, depth
    );

    // Building top
    glColor3f(0.78f, 0.79f, 0.80f);

    drawCube(
        x,
        y + height / 2.0f + 0.025f,
        z,
        width - 0.12f,
        0.05f,
        depth - 0.12f
    );
}

// ============================================================
// DRAW T1 TUNNEL
// ============================================================

void drawT1()
{
    float wallY = 0.9f;
    float wallH = 1.8f;
    float wallD = 0.35f;

    // Left section
    drawWall(
        -7.0f,
        wallY,
        -1.8f,
        5.0f,
        wallH,
        wallD
    );

    // Right section
    drawWall(
        0.2f,
        wallY,
        -1.8f,
        7.0f,
        wallH,
        wallD
    );

    // Top of tunnel
    drawWall(
        -3.3f,
        1.8f,
        -1.8f,
        12.4f,
        0.25f,
        0.35f
    );

}

// ============================================================
// DRAW T2
// ============================================================

void drawT2()
{
    drawBuilding(
        -2.4f,
        0.8f,
        4.0f,
        1.8f,
        1.6f,
        1.8f
    );

}

// ============================================================
// DRAW WINDMILL
// ============================================================

void drawWindMill()
{
    // Base
    glColor3f(0.58f, 0.60f, 0.61f);

    drawCube(
        -1.8f,
        0.35f,
        -4.0f,
        1.7f,
        0.7f,
        1.8f
    );

    // Tower
    glColor3f(0.55f, 0.57f, 0.58f);

    glPushMatrix();

    glTranslatef(
        -1.8f,
        1.35f,
        -4.0f
    );

    glRotatef(
        -90.0f,
        1.0f,
        0.0f,
        0.0f
    );

    GLUquadric* quadric = gluNewQuadric();

    gluCylinder(
        quadric,
        0.30f,
        0.22f,
        1.5f,
        20,
        10
    );

    gluDeleteQuadric(quadric);

    glPopMatrix();

    // Blades
    glPushMatrix();

    glTranslatef(
        -1.8f,
        2.85f,
        -3.65f
    );

    glRotatef(
        windmillAngle,
        0.0f,
        0.0f,
        1.0f
    );

    glColor3f(
        0.82f,
        0.83f,
        0.84f
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
            0.35f,
            0.0f,
            0.12f,
            0.7f,
            0.08f
        );

        glPopMatrix();
    }

    glPopMatrix();

}

// ============================================================
// OUTER WALLS
// ============================================================

void drawOuterWalls()
{
    float wallY = 0.9f;
    float wallH = 1.8f;
    float wallD = 0.35f;

    // LEFT OUTER WALL
    drawWall(
        -8.5f,
        wallY,
        0.0f,
        wallD,
        wallH,
        15.0f
    );

    // LOWER LEFT WALL
    drawWall(
        -6.5f,
        wallY,
        -7.5f,
        4.0f,
        wallH,
        wallD
    );

    // BOTTOM WALL
    drawWall(
        0.0f,
        wallY,
        -9.0f,
        10.5f,
        wallH,
        wallD
    );

    // BOTTOM RIGHT WALL
    drawWall(
        5.2f,
        wallY,
        -7.0f,
        wallD,
        wallH,
        4.0f
    );

    // RIGHT LOWER WALL
    drawWall(
        6.5f,
        wallY,
        -4.5f,
        wallD,
        wallH,
        5.0f
    );

    // RIGHT MIDDLE WALL
    drawWall(
        7.0f,
        wallY,
        0.5f,
        wallD,
        wallH,
        5.0f
    );

    // RIGHT UPPER WALL
    drawWall(
        6.0f,
        wallY,
        4.0f,
        3.0f,
        wallH,
        wallD
    );

    drawWall(
        4.6f,
        wallY,
        5.6f,
        wallD,
        wallH,
        3.0f
    );

    // TOP RIGHT
    drawWall(
        4.6f,
        wallY,
        7.0f,
        wallD,
        wallH,
        2.5f
    );

    drawWall(
        2.8f,
        wallY,
        8.2f,
        3.5f,
        wallH,
        wallD
    );

    // TOP CENTER
    drawWall(
        0.0f,
        wallY,
        8.8f,
        5.0f,
        wallH,
        wallD
    );

    drawWall(
        -2.5f,
        wallY,
        7.8f,
        wallD,
        wallH,
        2.5f
    );

    // TOP LEFT
    drawWall(
        -4.0f,
        wallY,
        7.0f,
        3.0f,
        wallH,
        wallD
    );

    drawWall(
        -5.5f,
        wallY,
        5.8f,
        wallD,
        wallH,
        3.0f
    );

    drawWall(
        -6.5f,
        wallY,
        4.5f,
        2.0f,
        wallH,
        wallD
    );
}

// ============================================================
// INTERNAL WALLS
// ============================================================

void drawInternalWalls()
{
    float wallY = 0.9f;
    float wallH = 1.8f;
    float wallD = 0.35f;

    // LEFT UPPER ROOM
    drawWall(
        -5.0f,
        wallY,
        3.0f,
        0.35f,
        wallH,
        3.0f
    );

    drawWall(
        -6.7f,
        wallY,
        1.8f,
        3.5f,
        wallH,
        0.35f
    );

    // T1
    drawT1();

    // CENTRAL VERTICAL WALL
    drawWall(
        0.0f,
        wallY,
        -2.0f,
        0.35f,
        wallH,
        6.0f
    );

    // CENTRAL LOWER CONNECTION
    drawWall(
        0.0f,
        wallY,
        -6.5f,
        0.35f,
        wallH,
        2.0f
    );

    // RIGHT INTERNAL WALLS
    drawWall(
        4.0f,
        wallY,
        0.0f,
        0.35f,
        wallH,
        1.7f
    );

    drawWall(
        5.0f,
        wallY,
        0.0f,
        2.0f,
        wallH,
        0.35f
    );

    drawWall(
        4.0f,
        wallY,
        -2.0f,
        0.35f,
        wallH,
        1.5f
    );

    drawWall(
        5.0f,
        wallY,
        -2.7f,
        2.0f,
        wallH,
        0.35f
    );
}

// ============================================================
// BUILDINGS
// ============================================================

void drawBuildings()
{
    // LEFT SB
    drawBuilding(
        -6.2f,
        0.8f,
        -4.0f,
        1.7f,
        1.6f,
        1.7f
    );


    // T2
    drawT2();

    // TOP FB
    drawBuilding(
        0.0f,
        1.0f,
        3.0f,
        2.0f,
        2.0f,
        2.4f
    );


    // FB2
    drawBuilding(
        2.8f,
        0.9f,
        4.3f,
        1.1f,
        1.8f,
        2.1f
    );


    // MIDDLE RIGHT FB
    drawBuilding(
        2.8f,
        0.9f,
        -1.8f,
        1.8f,
        1.8f,
        2.5f
    );


    // BOTTOM FB
    drawBuilding(
        1.0f,
        0.8f,
        -6.8f,
        3.8f,
        1.6f,
        1.5f
    );


    // BOTTOM SB
    drawBuilding(
        3.7f,
        0.8f,
        -6.8f,
        1.4f,
        1.6f,
        1.5f
    );

}

// ============================================================
// DRAW MAP
// ============================================================

void drawMap()
{
    // Floor first
    drawFloor();

    // Map structures
    drawOuterWalls();
    drawInternalWalls();
    drawBuildings();
    drawWindMill();
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

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Fixed camera
    gluLookAt(
        16.0f,
        22.0f,
        19.0f,

        0.0f,
        0.0f,
        0.0f,

        0.0f,
        1.0f,
        0.0f
    );

    // Rotate complete map including floor
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

    drawMap();

    glPopMatrix();

    glutSwapBuffers();
}

// ============================================================
// KEYBOARD
// ============================================================

void keyboard(
    unsigned char key,
    int x,
    int y
)
{
    switch (key)
    {
        // X-axis rotation
        case 'w':
        case 'W':
            mapRotationX -= 5.0f;
            break;

        case 's':
        case 'S':
            mapRotationX += 5.0f;
            break;

        // Y-axis rotation
        case 'a':
        case 'A':
            mapRotationY -= 5.0f;
            break;

        case 'd':
        case 'D':
            mapRotationY += 5.0f;
            break;

        // Z-axis rotation
        case 'q':
        case 'Q':
            mapRotationZ -= 5.0f;
            break;

        case 'e':
        case 'E':
            mapRotationZ += 5.0f;
            break;

        // Reset
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

void update(int value)
{
    windmillAngle += 2.0f;

    if (windmillAngle >= 360.0f)
        windmillAngle -= 360.0f;

    glutPostRedisplay();

    glutTimerFunc(
        30,
        update,
        0
    );
}

// ============================================================
// RESIZE
// ============================================================

void resize(
    int width,
    int height
)
{
    if (height == 0)
        height = 1;

    glViewport(
        0,
        0,
        width,
        height
    );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect =
        (float)width /
        (float)height;

    gluPerspective(
        45.0f,
        aspect,
        1.0f,
        100.0f
    );

    glMatrixMode(GL_MODELVIEW);
}

// ============================================================
// INITIALIZE
// ============================================================

void init()
{
    glClearColor(
        0.25f,
        0.25f,
        0.25f,
        1.0f
    );

    glEnable(GL_DEPTH_TEST);

    glDisable(GL_LIGHTING);

    glShadeModel(GL_FLAT);

    glEnable(GL_COLOR_MATERIAL);
}

// ============================================================
// MAIN
// ============================================================

int main(
    int argc,
    char** argv
)
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
        1200,
        800
    );

    glutCreateWindow(
        "Graphics and Animation - 3D Map"
    );

    init();

    glutDisplayFunc(display);

    glutReshapeFunc(resize);

    glutKeyboardFunc(keyboard);

    glutTimerFunc(
        30,
        update,
        0
    );

    glutMainLoop();

    return 0;
}
