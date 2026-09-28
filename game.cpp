#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#ifndef GL_GENERATE_MIPMAP
#define GL_GENERATE_MIPMAP 0x8191
#endif
#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

// TEXTURES

GLuint wallTexture;
GLuint floorTexture;
GLuint buildTexture;
GLuint build1Texture;
GLuint windmillTowerTexture;
GLuint windmillBladeTexture;
GLuint skyDayTexture;
GLuint skyNightTexture;
GLuint sunTexture;
GLuint moonTexture;

const int TEX_SIZE = 512;

// MENU

enum GameState
{
    STATE_MENU,
    STATE_PLAYING,
    STATE_ADMIN
};

GameState gameState = STATE_MENU;

int menuSelection = 0; 


float menuStartX1, menuStartX2, menuStartY1, menuStartY2;
float menuAdminX1, menuAdminX2, menuAdminY1, menuAdminY2;
float menuExitX1, menuExitX2, menuExitY1, menuExitY2;

const float ADMIN_FLY_SPEED = 0.12f;

void resetGameToMenu();

const float MAP_SCALE = 2.0f;

// FPS CAMERA

float cameraX = -2.0f;
float cameraY = 0.8f;
float cameraZ = 15.5f;

float cameraYaw = 0.0f;
float cameraPitch = 0.0f;

float moveSpeed = 0.15f;
float strafeSpeed = 0.08f;
float mouseSensitivity = 0.06f;

bool firstMouse = true;
bool ignoreMouse = false;

// JUMP

bool isJumping = false;

float verticalVelocity = 0.0f;

const float GROUND_Y = 0.8f;

const float GRAVITY = -0.02f;

const float JUMP_STRENGTH = 0.22f;

// ENEMY SYSTEM

struct Enemy
{
    float x;
    float y;
    float z;
    bool alive;
};

const int ENEMY_COUNT = 5;

Enemy enemies[ENEMY_COUNT] =
    {
        {-14.0f, 0.75f, 2.0f, true},
        {-6.0f, 0.75f, 12.0f, true},
        {8.0f, 0.75f, 12.0f, true},
        {8.0f, 0.75f, -12.0f, true},
        {-4.0f, 0.75f, -4.0f, true}};

int enemiesAlive = ENEMY_COUNT;

bool victory = false;

DWORD victoryStartTime = 0;

const DWORD VICTORY_DISPLAY_MS = 5000;

// DAY / NIGHT MODE

bool isNightMode = false;

GLfloat dayAmbient[] = {0.25f, 0.25f, 0.25f, 1.0f};
GLfloat dayDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat daySky[] = {0.25f, 0.25f, 0.25f, 1.0f};

GLfloat nightAmbient[] = {0.05f, 0.05f, 0.09f, 1.0f};
GLfloat nightDiffuse[] = {0.22f, 0.25f, 0.42f, 1.0f};
GLfloat nightSky[] = {0.015f, 0.015f, 0.05f, 1.0f};

void applyLightingMode()
{
    if (isNightMode)
    {
        glClearColor(
            nightSky[0],
            nightSky[1],
            nightSky[2],
            nightSky[3]);

        glLightfv(
            GL_LIGHT0,
            GL_DIFFUSE,
            nightDiffuse);

        glLightfv(
            GL_LIGHT0,
            GL_AMBIENT,
            nightAmbient);
    }
    else
    {
        glClearColor(
            daySky[0],
            daySky[1],
            daySky[2],
            daySky[3]);

        glLightfv(
            GL_LIGHT0,
            GL_DIFFUSE,
            dayDiffuse);

        glLightfv(
            GL_LIGHT0,
            GL_AMBIENT,
            dayAmbient);
    }
}

// SHOOTING

DWORD lastShotTime = 0;

const DWORD SHOT_COOLDOWN = 250;

// PLAYER COLLISION

float playerRadius = 0.15f;

struct WallCollision
{
    float minX;
    float maxX;
    float minZ;
    float maxZ;
    float topY;
};

WallCollision walls[100];

int wallCount = 0;

// COLLISION

void addCollisionWall(
    float x,
    float z,
    float width,
    float depth,
    float topY = 2.8f) 
{
    if (wallCount >= 100)
        return;

    x *= MAP_SCALE;
    z *= MAP_SCALE;

    width *= MAP_SCALE;
    depth *= MAP_SCALE;

    walls[wallCount].minX =
        x - width / 2.0f;

    walls[wallCount].maxX =
        x + width / 2.0f;

    walls[wallCount].minZ =
        z - depth / 2.0f;

    walls[wallCount].maxZ =
        z + depth / 2.0f;
   
    walls[wallCount].topY = topY;

    wallCount++;
}

bool checkCollision(
    float x,
    float z,
    float y)
{
    const float xMin = x - playerRadius;
    const float xMax = x + playerRadius;
    const float zMin = z - playerRadius;
    const float zMax = z + playerRadius;

    for (int i = 0; i < wallCount; i++)
    {
        const WallCollision &w = walls[i];

        if (xMax > w.minX && xMin < w.maxX &&
            zMax > w.minZ && zMin < w.maxZ &&
            y < w.topY)
        {
            return true;
        }
    }

    return false;
}

void setupCollisions()
{
    wallCount = 0;

    float wallD = 0.35f;

    // OUTER WALLS
    addCollisionWall(
        -8.5f,
        -2.8f,
        wallD,
        9.5f);

    // LOWER LEFT WALL
    addCollisionWall(
        -6.3f,
        -7.5f,
        4.7f,
        wallD);

    // BOTTOM WALL
    addCollisionWall(
        0.7f,
        -9.0f,
        9.3f,
        wallD);

    // BOTTOM RIGHT WALL
    addCollisionWall(
        5.2f,
        -8.0f,
        wallD,
        2.0f);

    // SMALL BOTTOM LEFT VERTICAL WALL
    addCollisionWall(
        -4.1f,
        -8.3f,
        wallD,
        1.8f);

    // RIGHT LOWER WALL
    addCollisionWall(
        6.5f,
        -1.5f,
        wallD,
        11.0f);

    // RIGHT UPPER WALL
    addCollisionWall(
        5.9f,
        -7.0f,
        1.5f,
        wallD);

    // RIGHT CENTER WALL
    addCollisionWall(
        5.5f,
        4.0f,
        2.0f,
        wallD);

    // TOP RIGHT
    addCollisionWall(
        4.5f,
        6.4f,
        wallD,
        5.1f);

    // TOP CENTER
    addCollisionWall(
        1.0f,
        8.8f,
        7.0f,
        wallD);

    // TOP CENTER-LEFT
    addCollisionWall(
        -2.5f,
        7.8f,
        wallD,
        2.5f);

    // TOP LEFT
    addCollisionWall(
        -3.8f,
        7.0f,
        2.8f,
        1.0f);

    // INTERNAL WALLS

    // LEFT UPPER VERTICAL ROOM
    addCollisionWall(
        -5.0f,
        4.5f,
        0.35f,
        5.4f);

    // LEFT UPPER HORIZONTAL
    addCollisionWall(
        -6.7f,
        1.8f,
        3.5f,
        0.35f);

    // T1 LEFT WALL
    addCollisionWall(
        -6.3f,
        -1.8f,
        4.5f,
        0.35f);

    // T1 RIGHT WALL
    addCollisionWall(
        0.2f,
        -1.8f,
        7.0f,
        0.35f);

    // CENTRAL VERTICAL WALL
    addCollisionWall(
        0.0f,
        -2.0f,
        0.35f,
        6.0f);

    // RIGHT INTERNAL WALLS
    addCollisionWall(
        4.2f,
        1.5f,
        0.35f,
        2.9f);

    addCollisionWall(
        5.0f,
        3.0f,
        2.0f,
        0.35f);

    // LEFT SB
    addCollisionWall(
        -6.2f,
        -4.0f,
        1.7f,
        1.7f,
        4.6f);

    // T2
    addCollisionWall(
        -2.4f,
        4.0f,
        1.8f,
        1.8f,
        4.1f);

    // TOP FB
    addCollisionWall(
        0.0f,
        3.0f,
        2.0f,
        2.4f,
        6.0f);

    // FB2
    addCollisionWall(
        2.8f,
        4.3f,
        1.1f,
        2.1f,
        5.3f);

    // MIDDLE RIGHT FB
    addCollisionWall(
        2.8f,
        -1.8f,
        1.8f,
        2.5f,
        3.8f);

    // BOTTOM FB
    addCollisionWall(
        1.0f,
        -6.8f,
        3.8f,
        1.5f,
        3.6f);

    // BOTTOM SB 
    addCollisionWall(
        3.7f,
        -6.8f,
        1.4f,
        1.5f,
        6.1f);

    // WINDMILL TOWER 
    addCollisionWall(
        -1.8f,
        -4.0f,
        0.70f,
        0.70f,
        10.5f);
}

// PLAYER MOVEMENT

void updateCamera()
{
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
    {
        PostQuitMessage(0);
        return;
    }

    if (gameState != STATE_PLAYING &&
        gameState != STATE_ADMIN)
    {
        return;
    }

    if (gameState == STATE_ADMIN)
    {
        
        if (GetAsyncKeyState('Q') & 0x8000)
        {
            resetGameToMenu();
            return;
        }
    }

    if (gameState == STATE_PLAYING && victory)
        return;

    float yawRad =
        cameraYaw *
        3.14159265f /
        180.0f;

    float forwardX =
        sinf(yawRad);

    float forwardZ =
        -cosf(yawRad);

    float rightX =
        cosf(yawRad);

    float rightZ =
        sinf(yawRad);

    float newX = cameraX;
    float newZ = cameraZ;

    // FORWARD
    if (GetAsyncKeyState('W') & 0x8000)
    {
        newX +=
            forwardX * moveSpeed;

        newZ +=
            forwardZ * moveSpeed;
    }

    // BACKWARD
    if (GetAsyncKeyState('S') & 0x8000)
    {
        newX -=
            forwardX * moveSpeed;

        newZ -=
            forwardZ * moveSpeed;
    }

    // LEFT
    if (GetAsyncKeyState('A') & 0x8000)
    {
        newX -=
            rightX * strafeSpeed;

        newZ -=
            rightZ * strafeSpeed;
    }

    // RIGHT
    if (GetAsyncKeyState('D') & 0x8000)
    {
        newX +=
            rightX * strafeSpeed;

        newZ +=
            rightZ * strafeSpeed;
    }

    // Check X movement
    if (!checkCollision(newX, cameraZ, cameraY))
    {
        cameraX = newX;
    }

    // Check Z movement
    if (!checkCollision(cameraX, newZ, cameraY))
    {
        cameraZ = newZ;
    }
    
    // ADMIN

    if (gameState == STATE_ADMIN)
    {

        if (GetAsyncKeyState('H') & 0x8000)
        {
            cameraY += ADMIN_FLY_SPEED;
        }

        if (GetAsyncKeyState('L') & 0x8000)
        {
            cameraY -= ADMIN_FLY_SPEED;
        }

        if (cameraY < 0.3f)
            cameraY = 0.3f;

        if (cameraY > 25.0f)
            cameraY = 25.0f;

        return;
    }

    // JUMP

    if (GetAsyncKeyState(VK_SPACE) & 0x8000)
    {
        if (!isJumping)
        {
            isJumping = true;

            verticalVelocity = JUMP_STRENGTH;
        }
    }

    if (isJumping)
    {
        verticalVelocity += GRAVITY;

        cameraY += verticalVelocity;

        if (cameraY <= GROUND_Y)
        {
            cameraY = GROUND_Y;

            verticalVelocity = 0.0f;

            isJumping = false;
        }
    }
}

// MOUSE LOOK
void centerMouse()
{
    int windowX =
        glutGet(
            GLUT_WINDOW_X);

    int windowY =
        glutGet(
            GLUT_WINDOW_Y);

    int windowWidth =
        glutGet(
            GLUT_WINDOW_WIDTH);

    int windowHeight =
        glutGet(
            GLUT_WINDOW_HEIGHT);

    POINT center;

    center.x =
        windowX +
        windowWidth / 2;

    center.y =
        windowY +
        windowHeight / 2;

    ignoreMouse = true;

    SetCursorPos(
        center.x,
        center.y);
}

// MOUSE MOTION
void mouseMotion(
    int x,
    int y)
{
    if (gameState != STATE_PLAYING &&
        gameState != STATE_ADMIN)
    {
        return;
    }

    if (ignoreMouse)
    {
        ignoreMouse = false;
        return;
    }

    int centerX =
        glutGet(
            GLUT_WINDOW_WIDTH) /
        2;

    int centerY =
        glutGet(
            GLUT_WINDOW_HEIGHT) /
        2;

    if (firstMouse)
    {
        firstMouse = false;

        centerMouse();

        return;
    }

    float deltaX =
        (float)(x - centerX);

    float deltaY =
        (float)(y - centerY);

    cameraYaw +=
        deltaX *
        mouseSensitivity;

    cameraPitch -=
        deltaY *
        mouseSensitivity;

    if (cameraPitch > 89.0f)
        cameraPitch = 89.0f;

    if (cameraPitch < -89.0f)
        cameraPitch = -89.0f;

    centerMouse();
}


float windmillAngle = 0.0f;

bool windmillPaused = false;

void drawEnemy(
    float x,
    float y,
    float z)
{
    glDisable(
        GL_TEXTURE_2D);

    glPushMatrix();

    glTranslatef(
        x,
        y,
        z);

    glColor3f(
        0.85f,
        0.05f,
        0.05f);

    glPushMatrix();

    glScalef(
        0.65f,
        1.0f,
        0.65f);

    glutSolidCube(
        1.0f);

    glPopMatrix();

    glColor3f(
        0.95f,
        0.70f,
        0.55f);

    glPushMatrix();

    glTranslatef(
        0.0f,
        0.75f,
        0.0f);

    glutSolidSphere(
        0.32f,
        20,
        20);

    glPopMatrix();

    glPopMatrix();

    glEnable(
        GL_TEXTURE_2D);
}

// PLACING ALL ENEMIES
void drawEnemies()
{
    if (gameState == STATE_ADMIN)
        return;

    if (victory)
        return;

    for (int i = 0;
         i < ENEMY_COUNT;
         i++)
    {
        if (enemies[i].alive)
        {
            drawEnemy(
                enemies[i].x,
                enemies[i].y,
                enemies[i].z);
        }
    }
}

// ENEMY COLLISION

bool rayHitsEnemy(
    float originX,
    float originY,
    float originZ,

    float directionX,
    float directionY,
    float directionZ,

    Enemy &enemy)
{
    const float radius = 0.85f;

    float ocX =
        originX -
        enemy.x;

    float ocY =
        originY -
        enemy.y;

    float ocZ =
        originZ -
        enemy.z;

    float b =
        2.0f *
        (ocX * directionX +
         ocY * directionY +
         ocZ * directionZ);

    float c =
        ocX * ocX +
        ocY * ocY +
        ocZ * ocZ -
        radius * radius;

    float discriminant =
        b * b -
        4.0f * c;

    if (discriminant < 0.0f)
        return false;

    float sqrtDiscriminant =
        sqrtf(
            discriminant);

    float t1 =
        (-b -
         sqrtDiscriminant) /
        2.0f;

    float t2 =
        (-b +
         sqrtDiscriminant) /
        2.0f;

    if (t1 >= 0.0f)
        return true;

    if (t2 >= 0.0f)
        return true;

    return false;
}

// SHOOT
void shoot()
{
    if (gameState != STATE_PLAYING)
        return;

    if (victory)
        return;

    DWORD currentTime =
        GetTickCount();

    if (currentTime -
            lastShotTime <
        SHOT_COOLDOWN)
    {
        return;
    }

    lastShotTime =
        currentTime;

    float yawRad =
        cameraYaw *
        3.14159265f /
        180.0f;

    float pitchRad =
        cameraPitch *
        3.14159265f /
        180.0f;

    float directionX =
        sinf(yawRad) *
        cosf(pitchRad);

    float directionY =
        sinf(pitchRad);

    float directionZ =
        -cosf(yawRad) *
        cosf(pitchRad);

    int hitEnemy = -1;

    float closestDistance =
        100000.0f;

    for (int i = 0;
         i < ENEMY_COUNT;
         i++)
    {
        if (!enemies[i].alive)
            continue;

        if (rayHitsEnemy(
                cameraX,
                cameraY,
                cameraZ,

                directionX,
                directionY,
                directionZ,

                enemies[i]))
        {
            float dx =
                enemies[i].x -
                cameraX;

            float dy =
                enemies[i].y -
                cameraY;

            float dz =
                enemies[i].z -
                cameraZ;

            float distance =
                sqrtf(
                    dx * dx +
                    dy * dy +
                    dz * dz);

            if (distance <
                closestDistance)
            {
                closestDistance =
                    distance;

                hitEnemy = i;
            }
        }
    }

    // ENEMY HIT

    if (hitEnemy != -1)
    {
        enemies[hitEnemy].alive =
            false;

        enemiesAlive--;

        printf(
            "Enemy %d eliminated!\n",
            hitEnemy + 1);

        printf(
            "Enemies remaining: %d / %d\n",
            enemiesAlive,
            ENEMY_COUNT);

        // VICTORY
        if (enemiesAlive <= 0)
        {
            enemiesAlive = 0;

            victory = true;

            victoryStartTime =
                GetTickCount();

            printf(
                "\n"
                "                VICTORY!\n"
                "\n");
        }
    }
    else
    {
        printf(
            "SHOT - No enemy hit.\n");
    }
}

// START GAME

void startGame()
{
    gameState = STATE_PLAYING;

    firstMouse = true;

    glutSetCursor(
        GLUT_CURSOR_NONE);

    centerMouse();
}


void startAdminMode()
{
    gameState = STATE_ADMIN;

    cameraX = -2.0f;
    cameraY = 0.8f;
    cameraZ = 15.5f;

    cameraYaw = 0.0f;
    cameraPitch = 0.0f;

    verticalVelocity = 0.0f;

    isJumping = false;

    firstMouse = true;

    glutSetCursor(
        GLUT_CURSOR_NONE);

    centerMouse();
}

void resetGameToMenu()
{
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        enemies[i].alive = true;
    }

    enemiesAlive = ENEMY_COUNT;

    victory = false;

    cameraX = -2.0f;
    cameraY = 0.8f;
    cameraZ = 15.5f;

    cameraYaw = 0.0f;
    cameraPitch = 0.0f;

    verticalVelocity = 0.0f;

    isJumping = false;

    gameState = STATE_MENU;

    menuSelection = 0;

    firstMouse = true;

    glutSetCursor(
        GLUT_CURSOR_LEFT_ARROW);
}

// MOUSE CLICK

void mouseClick(
    int button,
    int state,
    int x,
    int y)
{
    if (button != GLUT_LEFT_BUTTON ||
        state != GLUT_DOWN)
    {
        return;
    }

    if (gameState == STATE_MENU)
    {
        float clickX = (float)x;

        float clickY =
            (float)(glutGet(
                        GLUT_WINDOW_HEIGHT) -
                    y);

        if (clickX >= menuStartX1 &&
            clickX <= menuStartX2 &&
            clickY >= menuStartY1 &&
            clickY <= menuStartY2)
        {
            startGame();
        }
        else if (
            clickX >= menuAdminX1 &&
            clickX <= menuAdminX2 &&
            clickY >= menuAdminY1 &&
            clickY <= menuAdminY2)
        {
            startAdminMode();
        }
        else if (
            clickX >= menuExitX1 &&
            clickX <= menuExitX2 &&
            clickY >= menuExitY1 &&
            clickY <= menuExitY2)
        {
            exit(0);
        }

        return;
    }

    shoot();
}

// KEYBOARD

void keyboard(
    unsigned char key,
    int ,
    int )
{
    if (gameState == STATE_MENU)
    {
        if (key == 13 || key == ' ')
        {
            if (menuSelection == 0)
            {
                startGame();
            }
            else if (menuSelection == 1)
            {
                startAdminMode();
            }
            else
            {
                exit(0);
            }
        }

        return;
    }

    if (key == 'n' || key == 'N')
    {
        isNightMode = !isNightMode;

        applyLightingMode();

        printf(
            "Switched to %s mode\n",
            isNightMode ? "NIGHT" : "DAY");
    }

    if (key == 'z' || key == 'Z')
    {
        windmillPaused = !windmillPaused;

        printf(
            "Windmill blades %s\n",
            windmillPaused ? "PAUSED" : "RESUMED");
    }
}

// SPECIAL KEYS 

void specialKeys(
    int key,
    int ,
    int )
{
    if (gameState != STATE_MENU)
        return;

    if (key == GLUT_KEY_UP)
    {
        menuSelection--;

        if (menuSelection < 0)
            menuSelection = 2;
    }
    else if (key == GLUT_KEY_DOWN)
    {
        menuSelection++;

        if (menuSelection > 2)
            menuSelection = 0;
    }
}

// SCREEN TEXT

void drawScreenText(
    float x,
    float y,
    const char *text)
{
    glRasterPos2f(
        x,
        y);

    for (const char *c = text;
         *c != '\0';
         c++)
    {
        glutBitmapCharacter(
            GLUT_BITMAP_HELVETICA_18,
            *c);
    }
}

// MAIN MENU

void drawMenuScreen()
{
    float screenWidth =
        (float)glutGet(
            GLUT_WINDOW_WIDTH);

    float screenHeight =
        (float)glutGet(
            GLUT_WINDOW_HEIGHT);

    glMatrixMode(
        GL_PROJECTION);

    glPushMatrix();

    glLoadIdentity();

    glOrtho(
        0.0,
        screenWidth,

        0.0,
        screenHeight,

        -1.0,
        1.0);

    glMatrixMode(
        GL_MODELVIEW);

    glPushMatrix();

    glLoadIdentity();

    glDisable(
        GL_DEPTH_TEST);

    glDisable(
        GL_TEXTURE_2D);

    glDisable(
        GL_LIGHTING);

    // BACKGROUND MENU

    glBegin(
        GL_QUADS);

    glColor3f(
        0.0f,
        0.0f,
        0.0f);

    glVertex2f(
        0.0f,
        0.0f);

    glVertex2f(
        screenWidth,
        0.0f);

    glVertex2f(
        screenWidth,
        screenHeight);

    glVertex2f(
        0.0f,
        screenHeight);

    glEnd();

    // TITLE

    const char *titleText =
        "3D MAP SHOOTER";

    int titleWidth =
        glutBitmapLength(
            GLUT_BITMAP_TIMES_ROMAN_24,
            (const unsigned char *)titleText);

    glColor3f(
        1.0f,
        1.0f,
        1.0f);

    glRasterPos2f(
        screenWidth / 2.0f -
            titleWidth / 2.0f,

        screenHeight * 0.68f);

    for (const char *c = titleText;
         *c != '\0';
         c++)
    {
        glutBitmapCharacter(
            GLUT_BITMAP_TIMES_ROMAN_24,
            *c);
    }

    // START

    const char *startText =
        "START";

    int startWidth =
        glutBitmapLength(
            GLUT_BITMAP_TIMES_ROMAN_24,
            (const unsigned char *)startText);

    float startRasterX =
        screenWidth / 2.0f -
        startWidth / 2.0f;

    float startRasterY =
        screenHeight * 0.56f;

    menuStartX1 = startRasterX - 25.0f;
    menuStartX2 = startRasterX + startWidth + 25.0f;
    menuStartY1 = startRasterY - 10.0f;
    menuStartY2 = startRasterY + 28.0f;

    if (menuSelection == 0)
    {
        glColor3f(
            1.0f,
            1.0f,
            1.0f);
    }
    else
    {
        glColor3f(
            0.45f,
            0.45f,
            0.45f);
    }

    glRasterPos2f(
        startRasterX,
        startRasterY);

    for (const char *c = startText;
         *c != '\0';
         c++)
    {
        glutBitmapCharacter(
            GLUT_BITMAP_TIMES_ROMAN_24,
            *c);
    }

    // ADMIN MODE

    const char *adminText =
        "ADMIN MODE";

    int adminWidth =
        glutBitmapLength(
            GLUT_BITMAP_TIMES_ROMAN_24,
            (const unsigned char *)adminText);

    float adminRasterX =
        screenWidth / 2.0f -
        adminWidth / 2.0f;

    float adminRasterY =
        screenHeight * 0.56f -
        60.0f;

    menuAdminX1 = adminRasterX - 25.0f;
    menuAdminX2 = adminRasterX + adminWidth + 25.0f;
    menuAdminY1 = adminRasterY - 10.0f;
    menuAdminY2 = adminRasterY + 28.0f;

    if (menuSelection == 1)
    {
        glColor3f(
            1.0f,
            1.0f,
            1.0f);
    }
    else
    {
        glColor3f(
            0.45f,
            0.45f,
            0.45f);
    }

    glRasterPos2f(
        adminRasterX,
        adminRasterY);

    for (const char *c = adminText;
         *c != '\0';
         c++)
    {
        glutBitmapCharacter(
            GLUT_BITMAP_TIMES_ROMAN_24,
            *c);
    }

    // EXIT

    const char *exitText =
        "EXIT";

    int exitWidth =
        glutBitmapLength(
            GLUT_BITMAP_TIMES_ROMAN_24,
            (const unsigned char *)exitText);

    float exitRasterX =
        screenWidth / 2.0f -
        exitWidth / 2.0f;

    float exitRasterY =
        screenHeight * 0.56f -
        120.0f;

    menuExitX1 = exitRasterX - 25.0f;
    menuExitX2 = exitRasterX + exitWidth + 25.0f;
    menuExitY1 = exitRasterY - 10.0f;
    menuExitY2 = exitRasterY + 28.0f;

    if (menuSelection == 2)
    {
        glColor3f(
            1.0f,
            1.0f,
            1.0f);
    }
    else
    {
        glColor3f(
            0.45f,
            0.45f,
            0.45f);
    }

    glRasterPos2f(
        exitRasterX,
        exitRasterY);

    for (const char *c = exitText;
         *c != '\0';
         c++)
    {
        glutBitmapCharacter(
            GLUT_BITMAP_TIMES_ROMAN_24,
            *c);
    }

    const char *hintText =
        " ";

    int hintWidth =
        glutBitmapLength(
            GLUT_BITMAP_HELVETICA_18,
            (const unsigned char *)hintText);

    glColor3f(
        0.75f,
        0.75f,
        0.75f);

    glRasterPos2f(
        screenWidth / 2.0f -
            hintWidth / 2.0f,

        screenHeight * 0.22f);

    for (const char *c = hintText;
         *c != '\0';
         c++)
    {
        glutBitmapCharacter(
            GLUT_BITMAP_HELVETICA_18,
            *c);
    }

    glEnable(
        GL_LIGHTING);

    glEnable(
        GL_TEXTURE_2D);

    glEnable(
        GL_DEPTH_TEST);

    glPopMatrix();

    glMatrixMode(
        GL_PROJECTION);

    glPopMatrix();

    glMatrixMode(
        GL_MODELVIEW);
}

// GAME UI

void drawGameUI()
{
    glMatrixMode(
        GL_PROJECTION);

    glPushMatrix();

    glLoadIdentity();

    glOrtho(
        0.0,
        glutGet(
            GLUT_WINDOW_WIDTH),

        0.0,
        glutGet(
            GLUT_WINDOW_HEIGHT),

        -1.0,
        1.0);

    glMatrixMode(
        GL_MODELVIEW);

    glPushMatrix();

    glLoadIdentity();

    glDisable(
        GL_DEPTH_TEST);

    glDisable(
        GL_TEXTURE_2D);

    glDisable(
        GL_LIGHTING);

    // ENEMY COUNTER

    glColor3f(
        1.0f,
        1.0f,
        1.0f);

    if (gameState == STATE_ADMIN)
    {
        glColor3f(
            1.0f,
            0.9f,
            0.3f);

        drawScreenText(
            30.0f,
            glutGet(
                GLUT_WINDOW_HEIGHT) -
                40.0f,

            "ADMIN MODE");

        glColor3f(
            0.9f,
            0.9f,
            0.9f);

        drawScreenText(
            30.0f,
            glutGet(
                GLUT_WINDOW_HEIGHT) -
                95.0f,

            "H: Fly Up   L: Fly Down   Q: Return to Menu");
    }
    else
    {
        char enemyText[100];

        sprintf(
            enemyText,
            "Enemy : %d / %d",
            enemiesAlive,
            ENEMY_COUNT);

        drawScreenText(
            30.0f,
            glutGet(
                GLUT_WINDOW_HEIGHT) -
                40.0f,

            enemyText);
    }

    // CROSSHAIR

    if (gameState != STATE_ADMIN)
    {
        float centerX =
            glutGet(
                GLUT_WINDOW_WIDTH) /
            2.0f;

        float centerY =
            glutGet(
                GLUT_WINDOW_HEIGHT) /
            2.0f;

        glColor3f(
            1.0f,
            1.0f,
            1.0f);

        glLineWidth(
            2.0f);

        glBegin(
            GL_LINES);

        // Horizontal
        glVertex2f(
            centerX - 10.0f,
            centerY);

        glVertex2f(
            centerX + 10.0f,
            centerY);

        // Vertical
        glVertex2f(
            centerX,
            centerY - 10.0f);

        glVertex2f(
            centerX,
            centerY + 10.0f);

        glEnd();

        glLineWidth(
            1.0f);
    }

    // VICTORY SCREEN

    if (victory)
    {
        float screenWidth =
            (float)glutGet(
                GLUT_WINDOW_WIDTH);

        float screenHeight =
            (float)glutGet(
                GLUT_WINDOW_HEIGHT);

        glColor3f(
            0.1f,
            1.0f,
            0.2f);

        const char *victoryText =
            "VICTORY!";

        glRasterPos2f(
            screenWidth / 2.0f -
                75.0f,

            screenHeight / 2.0f +
                30.0f);

        for (const char *c =
                 victoryText;
             *c != '\0';
             c++)
        {
            glutBitmapCharacter(
                GLUT_BITMAP_TIMES_ROMAN_24,
                *c);
        }

        glColor3f(
            1.0f,
            1.0f,
            1.0f);

        const char *subText =
            "You Have Killed Everyone";

        glRasterPos2f(
            screenWidth / 2.0f -
                120.0f,

            screenHeight / 2.0f -
                10.0f);

        for (const char *c =
                 subText;
             *c != '\0';
             c++)
        {
            glutBitmapCharacter(
                GLUT_BITMAP_HELVETICA_18,
                *c);
        }

        // COUNTDOWN TO MENU

        DWORD elapsed =
            GetTickCount() -
            victoryStartTime;

        DWORD remainingMs =
            (elapsed < VICTORY_DISPLAY_MS)
                ? (VICTORY_DISPLAY_MS - elapsed)
                : 0;

        int remainingSeconds =
            (int)(remainingMs / 1000) + 1;

        if (remainingSeconds > 5)
            remainingSeconds = 5;

        char countdownText[64];

        sprintf(
            countdownText,
            "Returning to menu in %d...",
            remainingSeconds);

        glColor3f(
            0.7f,
            0.7f,
            0.7f);

        glRasterPos2f(
            screenWidth / 2.0f -
                110.0f,

            screenHeight / 2.0f -
                45.0f);

        for (const char *c =
                 countdownText;
             *c != '\0';
             c++)
        {
            glutBitmapCharacter(
                GLUT_BITMAP_HELVETICA_18,
                *c);
        }
    }

    glEnable(
        GL_LIGHTING);

    glEnable(
        GL_TEXTURE_2D);

    glEnable(
        GL_DEPTH_TEST);

    glPopMatrix();

    glMatrixMode(
        GL_PROJECTION);

    glPopMatrix();

    glMatrixMode(
        GL_MODELVIEW);
}

float pseudoRandom(int seed)
{
    unsigned int n = (unsigned int)seed;

    n = (n << 13) ^ n;

    unsigned int nn = (n * (n * n * 15731u + 789221u) +
                        1376312589u) &
                       0x7fffffffu;

    return 1.0f -
           ((float)nn / 1073741824.0f);
}

float smoothstepf(float edge0, float edge1, float x)
{
    float t = (x - edge0) / (edge1 - edge0);

    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    return t * t * (3.0f - 2.0f * t);
}

float pixelGrain(int x, int y, int seedOffset)
{
    return pseudoRandom(
        x * 131 +
        y * 977 +
        seedOffset);
}


GLuint uploadProceduralTexture(
    unsigned char *pixels,
    int size,
    bool useLighting)
{
    GLuint texID;

    glGenTextures(
        1,
        &texID);

    glBindTexture(
        GL_TEXTURE_2D,
        texID);

    glPixelStorei(
        GL_UNPACK_ALIGNMENT,
        1);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR_MIPMAP_LINEAR);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_REPEAT);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_REPEAT);

    glTexEnvi(
        GL_TEXTURE_ENV,
        GL_TEXTURE_ENV_MODE,
        useLighting ? GL_MODULATE : GL_REPLACE);


    glTexParameteri(
        GL_TEXTURE_2D,
        GL_GENERATE_MIPMAP,
        GL_TRUE);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        size,
        size,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        pixels);

    glBindTexture(
        GL_TEXTURE_2D,
        0);

    return texID;
}


GLuint uploadProceduralTextureRGBA(
    unsigned char *pixels,
    int size)
{
    GLuint texID;

    glGenTextures(
        1,
        &texID);

    glBindTexture(
        GL_TEXTURE_2D,
        texID);

    glPixelStorei(
        GL_UNPACK_ALIGNMENT,
        1);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR_MIPMAP_LINEAR);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR);


    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_CLAMP_TO_EDGE);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_CLAMP_TO_EDGE);

    glTexEnvi(
        GL_TEXTURE_ENV,
        GL_TEXTURE_ENV_MODE,
        GL_MODULATE);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_GENERATE_MIPMAP,
        GL_TRUE);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        size,
        size,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        pixels);

    glBindTexture(
        GL_TEXTURE_2D,
        0);

    return texID;
}


unsigned char *generateBrickTexture(int size)
{
    unsigned char *data =
        new unsigned char[size * size * 3];

    int brickRows = 8;
    int brickCols = 4;

    int brickH = size / brickRows;
    int brickW = size / brickCols;

    int mortar = size / 85;
    if (mortar < 2) mortar = 2;


    const int JITTER_COLS = brickCols + 2;
    float brickJitter[8][JITTER_COLS];

    for (int row = 0; row < brickRows; row++)
    {
        for (int col = 0; col < JITTER_COLS; col++)
        {
            int brickID = col + row * 13;
            brickJitter[row][col] = pseudoRandom(brickID) * 16.0f;
        }
    }

    for (int y = 0; y < size; y++)
    {
        int row = y / brickH;

        int rowOffset =
            (row % 2 == 0) ? 0 : brickW / 2;

        int ys = y % brickH;

        for (int x = 0; x < size; x++)
        {
            int idx = (y * size + x) * 3;

            int xs = (x + rowOffset) % brickW;

            bool isMortarX = xs < mortar;
            bool isMortarY = ys < mortar;
            bool isMortar = isMortarX || isMortarY;

            float grain =
                pixelGrain(x, y, 4001) * 6.0f;

            if (isMortar)
            {

                int pos = isMortarY ? ys : xs;

                float bevel = 0.0f;

                if (pos == 0)
                    bevel = 22.0f;
                else if (pos == mortar - 1)
                    bevel = -20.0f;

                float shade = 182.0f + bevel + grain;

                if (shade < 0) shade = 0;
                if (shade > 255) shade = 255;

                data[idx] = (unsigned char)shade;
                data[idx + 1] = (unsigned char)(shade - 5);
                data[idx + 2] = (unsigned char)(shade - 15);
            }
            else
            {
                int brickCol = (x + rowOffset) / brickW;

                float jitter = brickJitter[row][brickCol];

 
                float faceShade =
                    ((float)(brickH - ys) / (float)brickH) *
                    10.0f;

                float r = 148.0f + jitter + faceShade + grain;
                float g = 66.0f + jitter * 0.5f + faceShade * 0.4f + grain * 0.5f;
                float b = 53.0f + jitter * 0.35f + faceShade * 0.3f + grain * 0.35f;

                if (r < 0) r = 0;
                if (r > 255) r = 255;
                if (g < 0) g = 0;
                if (g > 255) g = 255;
                if (b < 0) b = 0;
                if (b > 255) b = 255;

                data[idx] = (unsigned char)r;
                data[idx + 1] = (unsigned char)g;
                data[idx + 2] = (unsigned char)b;
            }
        }
    }

    return data;
}


unsigned char *generateFloorTexture(int size)
{
    unsigned char *data =
        new unsigned char[size * size * 3];

    int tileSize = size / 6;
    int grout = size / 170;
    if (grout < 2) grout = 2;

    int tilesPerAxis = size / tileSize + 2;
    float *tileJitter = new float[tilesPerAxis * tilesPerAxis];

    for (int ty = 0; ty < tilesPerAxis; ty++)
    {
        for (int tx = 0; tx < tilesPerAxis; tx++)
        {
            int tileID = tx * 31 + ty * 17;
            tileJitter[ty * tilesPerAxis + tx] =
                pseudoRandom(tileID) * 10.0f;
        }
    }

    for (int y = 0; y < size; y++)
    {
        int ys = y % tileSize;

        int tileRow = y / tileSize;

        for (int x = 0; x < size; x++)
        {
            int idx = (y * size + x) * 3;

            int xs = x % tileSize;

            bool isGroutX = xs < grout;
            bool isGroutY = ys < grout;
            bool isGrout = isGroutX || isGroutY;

            int tileCol = x / tileSize;

            float jitter =
                tileJitter[tileRow * tilesPerAxis + tileCol];


            float speckle =
                pixelGrain(x, y, 8123) * 9.0f;

            if (isGrout)
            {
                int pos = isGroutY ? ys : xs;

                float bevel = (pos == 0) ? -14.0f : 4.0f;

                float shade = 92.0f + bevel;

                data[idx] = (unsigned char)shade;
                data[idx + 1] = (unsigned char)shade;
                data[idx + 2] = (unsigned char)(shade - 3);
            }
            else
            {

                float edgeBevel = 0.0f;

                int distFromEdgeX =
                    (xs < tileSize / 2)
                        ? xs
                        : tileSize - xs;

                int distFromEdgeY =
                    (ys < tileSize / 2)
                        ? ys
                        : tileSize - ys;

                int distFromEdge =
                    (distFromEdgeX < distFromEdgeY)
                        ? distFromEdgeX
                        : distFromEdgeY;

                if (distFromEdge < grout + 2)
                    edgeBevel = -6.0f;

                float base =
                    145.0f + jitter + speckle + edgeBevel;

                if (base < 0) base = 0;
                if (base > 255) base = 255;

                data[idx] = (unsigned char)base;
                data[idx + 1] = (unsigned char)base;
                data[idx + 2] = (unsigned char)(base - 6);
            }
        }
    }

    delete[] tileJitter;

    return data;
}

unsigned char *generateGlassTexture(int size)
{
    unsigned char *data =
        new unsigned char[size * size * 3];

    int cols = 8;
    int rows = 12;

    int cellW = size / cols;
    int cellH = size / rows;

    int frame = size / 128;
    if (frame < 2) frame = 2;

    bool *litTable = new bool[cols * rows];

    for (int cellRow = 0; cellRow < rows; cellRow++)
    {
        for (int cellCol = 0; cellCol < cols; cellCol++)
        {
            int cellID = cellCol * 7 + cellRow * 19;
            litTable[cellRow * cols + cellCol] =
                pseudoRandom(cellID) > 0.35f;
        }
    }

    for (int y = 0; y < size; y++)
    {
        int ys = y % cellH;

        int cellRow = y / cellH;

        for (int x = 0; x < size; x++)
        {
            int idx = (y * size + x) * 3;

            int xs = x % cellW;

            bool isFrameX = xs < frame;
            bool isFrameY = ys < frame;
            bool isFrame = isFrameX || isFrameY;

            if (isFrame)
            {
                int pos = isFrameY ? ys : xs;

                float bevel = (pos == 0) ? 18.0f : -10.0f;

                float shade = 40.0f + bevel;
                if (shade < 0) shade = 0;

                data[idx] = (unsigned char)shade;
                data[idx + 1] = (unsigned char)shade;
                data[idx + 2] = (unsigned char)(shade + 6);
            }
            else
            {
                int cellCol = x / cellW;

                bool litWindow = litTable[cellRow * cols + cellCol];

                float paneFrac =
                    (float)ys / (float)cellH;

                float grain =
                    pixelGrain(x, y, 5231) * 5.0f;

                if (litWindow)
                {
                    float r = 232.0f - paneFrac * 25.0f + grain;
                    float g = 216.0f - paneFrac * 30.0f + grain;
                    float b = 150.0f - paneFrac * 20.0f + grain;

                    if (r < 0) r = 0;
                    if (r > 255) r = 255;
                    if (g < 0) g = 0;
                    if (g > 255) g = 255;
                    if (b < 0) b = 0;
                    if (b > 255) b = 255;

                    data[idx] = (unsigned char)r;
                    data[idx + 1] = (unsigned char)g;
                    data[idx + 2] = (unsigned char)b;
                }
                else
                {
                    float r = 60.0f + (1.0f - paneFrac) * 35.0f + grain;
                    float g = 88.0f + (1.0f - paneFrac) * 32.0f + grain;
                    float b = 108.0f + (1.0f - paneFrac) * 28.0f + grain;

                    if (r < 0) r = 0;
                    if (r > 255) r = 255;
                    if (g < 0) g = 0;
                    if (g > 255) g = 255;
                    if (b < 0) b = 0;
                    if (b > 255) b = 255;

                    data[idx] = (unsigned char)r;
                    data[idx + 1] = (unsigned char)g;
                    data[idx + 2] = (unsigned char)b;
                }
            }
        }
    }

    delete[] litTable;

    return data;
}

// SHORT BUILDINGS
unsigned char *generatePanelTexture(int size)
{
    unsigned char *data =
        new unsigned char[size * size * 3];

    int cols = 4;
    int rows = 6;

    int cellW = size / cols;
    int cellH = size / rows;

    int frame = size / 85;
    if (frame < 3) frame = 3;

    float *cellJitter = new float[cols * rows];

    for (int cellRow = 0; cellRow < rows; cellRow++)
    {
        for (int cellCol = 0; cellCol < cols; cellCol++)
        {
            int cellID = cellCol * 5 + cellRow * 23;
            cellJitter[cellRow * cols + cellCol] =
                pseudoRandom(cellID) * 16.0f;
        }
    }

    int streakBuckets = size / 3 + 2;
    float *streakTable = new float[streakBuckets];

    for (int b = 0; b < streakBuckets; b++)
    {
        streakTable[b] = pseudoRandom(b + 9001) * 14.0f;
    }

    for (int y = 0; y < size; y++)
    {
        int ys = y % cellH;

        int cellRow = y / cellH;

        for (int x = 0; x < size; x++)
        {
            int idx = (y * size + x) * 3;

            int xs = x % cellW;

            bool isFrameX = xs < frame;
            bool isFrameY = ys < frame;
            bool isFrame = isFrameX || isFrameY;

            int cellCol = x / cellW;

            float jitter = cellJitter[cellRow * cols + cellCol];


            float streak = streakTable[x / 3];

            float grain =
                pixelGrain(x, y, 7331) * 6.0f;

            float base = 195.0f + jitter + streak + grain;

            if (base < 0) base = 0;
            if (base > 255) base = 255;

            if (isFrame)
            {
                int pos = isFrameY ? ys : xs;

                float bevel = (pos == 0) ? 14.0f : -16.0f;

                float shade = base - 42.0f + bevel;
                if (shade < 0) shade = 0;

                data[idx] = (unsigned char)shade;
                data[idx + 1] = (unsigned char)(shade + 2);
                data[idx + 2] = (unsigned char)(shade + 5);
            }
            else
            {
                data[idx] = (unsigned char)base;
                data[idx + 1] = (unsigned char)(base - 3);
                data[idx + 2] = (unsigned char)(base - 9);
            }
        }
    }

    delete[] cellJitter;
    delete[] streakTable;

    return data;
}

// WINDMILL TOWER
unsigned char *generateMetalTexture(int size)
{
    unsigned char *data =
        new unsigned char[size * size * 3];

    int bandSpacing = size / 5;
    int bandWidth = size / 22;
    int stripeW = size / 36;

    for (int y = 0; y < size; y++)
    {
        int bandPos = y % bandSpacing;

        bool inBand = bandPos < bandWidth;

        for (int x = 0; x < size; x++)
        {
            int idx = (y * size + x) * 3;

            float grain =
                pixelGrain(x, y, 6111) * 2.0f;

            if (inBand)
            {
                int stripe = ((x + y) / stripeW) % 2;

                if (stripe == 0)
                {
                    data[idx] = (unsigned char)(225.0f + grain);
                    data[idx + 1] = (unsigned char)(40.0f + grain * 0.3f);
                    data[idx + 2] = (unsigned char)(36.0f + grain * 0.3f);
                }
                else
                {
                    data[idx] = (unsigned char)(238.0f + grain);
                    data[idx + 1] = (unsigned char)(238.0f + grain);
                    data[idx + 2] = (unsigned char)(238.0f + grain);
                }
            }
            else
            {
                float base = 190.0f + grain;

                if (base < 0) base = 0;
                if (base > 255) base = 255;

                data[idx] = (unsigned char)base;
                data[idx + 1] = (unsigned char)base;
                data[idx + 2] = (unsigned char)(base + 8.0f);
            }
        }
    }

    return data;
}

// WINDMILL BLADES
unsigned char *generateBladeTexture(int size)
{
    unsigned char *data =
        new unsigned char[size * size * 3];

    for (int y = 0; y < size; y++)
    {
        float lengthFrac = (float)y / (float)size;

        bool isTip = lengthFrac > 0.85f;

        int stripe = (y / (size / 20)) % 2;

        for (int x = 0; x < size; x++)
        {
            int idx = (y * size + x) * 3;

            float grain =
                pixelGrain(x, y, 7411) * 2.0f;

            if (isTip)
            {
                if (stripe == 0)
                {
                    data[idx] = (unsigned char)(230.0f + grain);
                    data[idx + 1] = (unsigned char)(30.0f + grain * 0.3f);
                    data[idx + 2] = (unsigned char)(28.0f + grain * 0.3f);
                }
                else
                {
                    float shade = 240.0f + grain;
                    if (shade < 0) shade = 0;
                    if (shade > 255) shade = 255;

                    data[idx] = (unsigned char)shade;
                    data[idx + 1] = (unsigned char)shade;
                    data[idx + 2] = (unsigned char)shade;
                }
            }
            else
            {
                float base = 232.0f + grain;

                if (base < 0) base = 0;
                if (base > 255) base = 255;

                data[idx] = (unsigned char)base;
                data[idx + 1] = (unsigned char)(base + 1);
                data[idx + 2] = (unsigned char)(base + 3);
            }
        }
    }

    return data;
}

unsigned char *generateSkyTexture(int size, bool night)
{
    unsigned char *data =
        new unsigned char[size * size * 3];

    for (int y = 0; y < size; y++)
    {
        float v = (float)y / (float)(size - 1);

        float r, g, b;

        if (night)
        {

            r = 4.0f + (1.0f - v) * 10.0f;
            g = 5.0f + (1.0f - v) * 14.0f;
            b = 18.0f + (1.0f - v) * 28.0f;
        }
        else
        {

            r = 70.0f + (1.0f - v) * 110.0f;
            g = 130.0f + (1.0f - v) * 100.0f;
            b = 220.0f + (1.0f - v) * 30.0f;

            if (b > 255.0f) b = 255.0f;
        }

        for (int x = 0; x < size; x++)
        {
            int idx = (y * size + x) * 3;

            float fr = r;
            float fg = g;
            float fb = b;

            if (!night)
            {
                float cloudBand =
                    sinf((x * 0.021f) + v * 3.0f) *
                        0.5f +
                    sinf((x * 0.009f) - v * 5.0f) *
                        0.5f;

                float cloudMask =
                    cloudBand - (0.55f - v * 0.3f);

                if (cloudMask > 0.0f && v > 0.2f && v < 0.85f)
                {
                    float strength = cloudMask * 90.0f;

                    if (strength > 55.0f) strength = 55.0f;

                    fr += strength;
                    fg += strength;
                    fb += strength * 0.85f;
                }
            }

            if (fr < 0) fr = 0;
            if (fr > 255) fr = 255;
            if (fg < 0) fg = 0;
            if (fg > 255) fg = 255;
            if (fb < 0) fb = 0;
            if (fb > 255) fb = 255;

            data[idx] = (unsigned char)fr;
            data[idx + 1] = (unsigned char)fg;
            data[idx + 2] = (unsigned char)fb;
        }
    }

    return data;
}

// SUN / MOON
unsigned char *generateSunMoonTexture(int size, bool isSun)
{
    unsigned char *data =
        new unsigned char[size * size * 4];

    float center = size / 2.0f;

    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
        {
            int idx = (y * size + x) * 4;

            float dx = (x - center) / center;
            float dy = (y - center) / center;

            float dist = sqrtf(dx * dx + dy * dy);

            float r, g, b, a;

            if (isSun)
            {
                float disc =
                    1.0f - smoothstepf(0.28f, 0.34f, dist);

                float glow =
                    1.0f - smoothstepf(0.34f, 1.0f, dist);

                r = 255.0f;
                g = 245.0f - (1.0f - disc) * 25.0f;
                b = 190.0f - (1.0f - disc) * 70.0f;

                a = (disc * 255.0f) + (glow * 140.0f);
            }
            else
            {
                float disc =
                    1.0f - smoothstepf(0.26f, 0.32f, dist);

                float glow =
                    1.0f - smoothstepf(0.32f, 0.95f, dist);

                float crater =
                    pseudoRandom(x * 37 + y * 53 + 4471) > 0.86f
                        ? -18.0f
                        : 0.0f;

                r = 222.0f + crater;
                g = 228.0f + crater;
                b = 240.0f + crater;

                a = (disc * 235.0f) + (glow * 70.0f);
            }

            if (a > 255.0f) a = 255.0f;
            if (a < 0.0f) a = 0.0f;

            if (r < 0) r = 0;
            if (r > 255) r = 255;
            if (g < 0) g = 0;
            if (g > 255) g = 255;
            if (b < 0) b = 0;
            if (b > 255) b = 255;

            data[idx] = (unsigned char)r;
            data[idx + 1] = (unsigned char)g;
            data[idx + 2] = (unsigned char)b;
            data[idx + 3] = (unsigned char)a;
        }
    }

    return data;
}

void loadwallTexture()
{
    unsigned char *pixels =
        generateBrickTexture(TEX_SIZE);

    wallTexture =
        uploadProceduralTexture(
            pixels,
            TEX_SIZE,
            true);

    delete[] pixels;

    printf(
        "Wall texture (brick) generated: %d x %d\n",
        TEX_SIZE,
        TEX_SIZE);
}

void loadFloorTexture()
{
    unsigned char *pixels =
        generateFloorTexture(TEX_SIZE);

    floorTexture =
        uploadProceduralTexture(
            pixels,
            TEX_SIZE,
            true);

    delete[] pixels;

    printf(
        "Floor texture (tile) generated: %d x %d\n",
        TEX_SIZE,
        TEX_SIZE);
}

void loadBuildTexture()
{
    unsigned char *pixels =
        generateGlassTexture(TEX_SIZE);

    buildTexture =
        uploadProceduralTexture(
            pixels,
            TEX_SIZE,
            true);

    delete[] pixels;

    printf(
        "Building texture (glass) generated: %d x %d\n",
        TEX_SIZE,
        TEX_SIZE);
}

void loadBuild1Texture()
{
    unsigned char *pixels =
        generatePanelTexture(TEX_SIZE);

    build1Texture =
        uploadProceduralTexture(
            pixels,
            TEX_SIZE,
            true);

    delete[] pixels;

    printf(
        "Building1 texture (panel) generated: %d x %d\n",
        TEX_SIZE,
        TEX_SIZE);
}

void loadWindmillTowerTexture()
{
    unsigned char *pixels =
        generateMetalTexture(TEX_SIZE);

    windmillTowerTexture =
        uploadProceduralTexture(
            pixels,
            TEX_SIZE,
            true);

    delete[] pixels;

    printf(
        "Windmill tower texture (metal) generated: %d x %d\n",
        TEX_SIZE,
        TEX_SIZE);
}

void loadWindmillBladeTexture()
{
    unsigned char *pixels =
        generateBladeTexture(TEX_SIZE);

    windmillBladeTexture =
        uploadProceduralTexture(
            pixels,
            TEX_SIZE,
            true);

    delete[] pixels;

    printf(
        "Windmill blade texture (painted metal) generated: %d x %d\n",
        TEX_SIZE,
        TEX_SIZE);
}

void loadSkyTextures()
{
    const int SKY_TEX_SIZE = 256;

    unsigned char *dayPixels =
        generateSkyTexture(SKY_TEX_SIZE, false);

    skyDayTexture =
        uploadProceduralTexture(
            dayPixels,
            SKY_TEX_SIZE,
            false);

    delete[] dayPixels;

    unsigned char *nightPixels =
        generateSkyTexture(SKY_TEX_SIZE, true);

    skyNightTexture =
        uploadProceduralTexture(
            nightPixels,
            SKY_TEX_SIZE,
            false);

    delete[] nightPixels;

    printf(
        "Sky textures (day + night) generated: %d x %d\n",
        SKY_TEX_SIZE,
        SKY_TEX_SIZE);
}

void loadSunMoonTextures()
{
    const int GLOW_TEX_SIZE = 256;

    unsigned char *sunPixels =
        generateSunMoonTexture(GLOW_TEX_SIZE, true);

    sunTexture =
        uploadProceduralTextureRGBA(
            sunPixels,
            GLOW_TEX_SIZE);

    delete[] sunPixels;

    unsigned char *moonPixels =
        generateSunMoonTexture(GLOW_TEX_SIZE, false);

    moonTexture =
        uploadProceduralTextureRGBA(
            moonPixels,
            GLOW_TEX_SIZE);

    delete[] moonPixels;

    printf(
        "Sun / moon glow sprites generated: %d x %d\n",
        GLOW_TEX_SIZE,
        GLOW_TEX_SIZE);
}

// DRAW CUBE

void drawCube(
    float x,
    float y,
    float z,
    float width,
    float height,
    float depth)
{
    glPushMatrix();

    glTranslatef(
        x,
        y,
        z);

    glScalef(
        width,
        height,
        depth);

    glutSolidCube(
        1.0f);

    glPopMatrix();
}

// DRAW TEXTURED CUBE

void drawTexturedCube(
    float x,
    float y,
    float z,
    float width,
    float height,
    float depth,
    GLuint texture)
{
    float x1 =
        -width / 2.0f;

    float x2 =
        width / 2.0f;

    float y1 =
        -height / 2.0f;

    float y2 =
        height / 2.0f;

    float z1 =
        -depth / 2.0f;

    float z2 =
        depth / 2.0f;

    glPushMatrix();

    glTranslatef(
        x,
        y,
        z);

    glEnable(
        GL_TEXTURE_2D);

    glBindTexture(
        GL_TEXTURE_2D,
        texture);

    glColor3f(
        1.0f,
        1.0f,
        1.0f);

    glBegin(
        GL_QUADS);

    // FRONT
    glTexCoord2f(
        0.0f,
        0.0f);

    glVertex3f(
        x1,
        y1,
        z2);

    glTexCoord2f(
        1.0f,
        0.0f);

    glVertex3f(
        x2,
        y1,
        z2);

    glTexCoord2f(
        1.0f,
        1.0f);

    glVertex3f(
        x2,
        y2,
        z2);

    glTexCoord2f(
        0.0f,
        1.0f);

    glVertex3f(
        x1,
        y2,
        z2);

    // BACK
    glTexCoord2f(
        0.0f,
        0.0f);

    glVertex3f(
        x2,
        y1,
        z1);

    glTexCoord2f(
        1.0f,
        0.0f);

    glVertex3f(
        x1,
        y1,
        z1);

    glTexCoord2f(
        1.0f,
        1.0f);

    glVertex3f(
        x1,
        y2,
        z1);

    glTexCoord2f(
        0.0f,
        1.0f);

    glVertex3f(
        x2,
        y2,
        z1);

    // LEFT
    glTexCoord2f(
        0.0f,
        0.0f);

    glVertex3f(
        x1,
        y1,
        z1);

    glTexCoord2f(
        1.0f,
        0.0f);

    glVertex3f(
        x1,
        y1,
        z2);

    glTexCoord2f(
        1.0f,
        1.0f);

    glVertex3f(
        x1,
        y2,
        z2);

    glTexCoord2f(
        0.0f,
        1.0f);

    glVertex3f(
        x1,
        y2,
        z1);

    // RIGHT
    glTexCoord2f(
        0.0f,
        0.0f);

    glVertex3f(
        x2,
        y1,
        z2);

    glTexCoord2f(
        1.0f,
        0.0f);

    glVertex3f(
        x2,
        y1,
        z1);

    glTexCoord2f(
        1.0f,
        1.0f);

    glVertex3f(
        x2,
        y2,
        z1);

    glTexCoord2f(
        0.0f,
        1.0f);

    glVertex3f(
        x2,
        y2,
        z2);

    // TOP
    glTexCoord2f(
        0.0f,
        0.0f);

    glVertex3f(
        x1,
        y2,
        z2);

    glTexCoord2f(
        1.0f,
        0.0f);

    glVertex3f(
        x2,
        y2,
        z2);

    glTexCoord2f(
        1.0f,
        1.0f);

    glVertex3f(
        x2,
        y2,
        z1);

    glTexCoord2f(
        0.0f,
        1.0f);

    glVertex3f(
        x1,
        y2,
        z1);

    // BOTTOM
    glTexCoord2f(
        0.0f,
        0.0f);

    glVertex3f(
        x1,
        y1,
        z1);

    glTexCoord2f(
        1.0f,
        0.0f);

    glVertex3f(
        x2,
        y1,
        z1);

    glTexCoord2f(
        1.0f,
        1.0f);

    glVertex3f(
        x2,
        y1,
        z2);

    glTexCoord2f(
        0.0f,
        1.0f);

    glVertex3f(
        x1,
        y1,
        z2);

    glEnd();

    glBindTexture(
        GL_TEXTURE_2D,
        0);

    glPopMatrix();
}

// FLOOR

void drawFloor()
{
    glEnable(
        GL_TEXTURE_2D);

    glBindTexture(
        GL_TEXTURE_2D,
        floorTexture);

    glColor3f(
        1.0f,
        1.0f,
        1.0f);

    glBegin(
        GL_QUADS);

    glTexCoord2f(
        0.0f,
        0.0f);

    glVertex3f(
        -11.0f,
        0.0f,
        -11.0f);

    glTexCoord2f(
        11.0f,
        0.0f);

    glVertex3f(
        11.0f,
        0.0f,
        -11.0f);

    glTexCoord2f(
        11.0f,
        11.0f);

    glVertex3f(
        11.0f,
        0.0f,
        11.0f);

    glTexCoord2f(
        0.0f,
        11.0f);

    glVertex3f(
        -11.0f,
        0.0f,
        11.0f);

    glEnd();

    glBindTexture(
        GL_TEXTURE_2D,
        0);
}

// DRAW WALL

void drawWall(
    float x,
    float y,
    float z,
    float width,
    float height,
    float depth)
{
    glColor3f(
        1.0f,
        1.0f,
        1.0f);

    drawTexturedCube(
        x,
        y,
        z,
        width,
        height,
        depth,
        wallTexture);
}

// DRAW BUILDING

void drawBuilding(
    float x,
    float y,
    float z,
    float width,
    float height,
    float depth)
{
    GLuint selectedTexture =
        (height <= 7.0f)
            ? build1Texture
            : buildTexture;

    // Main building
    drawTexturedCube(
        x,
        y,
        z,
        width,
        height,
        depth,
        selectedTexture);

    // Rooftop trim
    drawTexturedCube(
        x,
        y +
            height / 2.0f +
            0.025f,
        z,

        width - 0.12f,
        0.05f,
        depth - 0.12f,

        selectedTexture);
}

// T1 TUNNEL

void drawT1()
{
    float wallY = 0.9f;
    float wallH = 3.8f;
    float wallD = 0.35f;

    // Left section
    drawWall(
        -6.3f,
        wallY,
        -1.8f,

        4.5f,
        wallH,
        wallD);

    // Right section
    drawWall(
        0.2f,
        wallY,
        -1.8f,

        7.0f,
        wallH,
        wallD);

    // Top
    drawWall(
        -2.6f,
        2.55f,
        -1.8f,

        3.4f,
        0.5f,
        0.35f);
}

// T2

void drawT2()
{
    drawBuilding(
        -2.4f,
        0.8f,
        4.0f,

        1.8f,
        6.6f,
        1.8f);
}

// WINDMILL

void drawWindMill()
{
    glEnable(
        GL_TEXTURE_2D);

    // TOWER

    glColor3f(
        1.0f,
        1.0f,
        1.0f);

    const float windmillTowerHeight =
        10.5f;

    const float windmillTowerBaseY =
        0.0f;

    const float windmillTopY =
        windmillTowerBaseY +
        windmillTowerHeight;

    glPushMatrix();

    glTranslatef(
        -1.8f,
        windmillTowerBaseY,
        -4.0f);

    glRotatef(
        -90.0f,
        1.0f,
        0.0f,
        0.0f);

    glBindTexture(
        GL_TEXTURE_2D,
        windmillTowerTexture);

    GLUquadric *quadric =
        gluNewQuadric();

    gluQuadricTexture(
        quadric,
        GL_TRUE);

    gluCylinder(
        quadric,

        0.45f,
        0.22f,

        windmillTowerHeight,

        20,
        10);

    gluDeleteQuadric(
        quadric);

    glBindTexture(
        GL_TEXTURE_2D,
        0);

    glPopMatrix();

    // BLADE

    glPushMatrix();

    glTranslatef(
        -1.8f,
        windmillTopY,
        -3.65f);

    glRotatef(
        windmillAngle,
        0.0f,
        0.0f,
        1.0f);

    glColor3f(
        1.0f,
        1.0f,
        1.0f);

    for (int i = 0;
         i < 4;
         i++)
    {
        glPushMatrix();

        glRotatef(
            i * 90.0f,
            0.0f,
            0.0f,
            1.0f);

        drawTexturedCube(
            0.0f,
            0.75f,
            0.0f,

            0.18f,
            1.5f,
            0.12f,

            windmillBladeTexture);

        glPopMatrix();
    }

    glPopMatrix();

    glEnable(
        GL_TEXTURE_2D);
}

// OUTER WALLS

void drawOuterWalls()
{
    float wallY = 0.9f;
    float wallH = 3.8f;
    float wallD = 0.35f;

    // LEFT OUTER WALL
    drawWall(
        -8.5f,
        wallY,
        -2.8f,

        wallD,
        wallH,
        9.5f);

    // LOWER LEFT WALL
    drawWall(
        -6.3f,
        wallY,
        -7.5f,

        4.7f,
        wallH,
        wallD);

    // BOTTOM WALL
    drawWall(
        0.7f,
        wallY,
        -9.0f,

        9.3f,
        wallH,
        wallD);

    // BOTTOM RIGHT WALL
    drawWall(
        5.2f,
        wallY,
        -8.0f,

        wallD,
        wallH,
        2.0f);

    // SMALL BOTTOM LEFT VERTICAL WALL
    drawWall(
        -4.1f,
        wallY,
        -8.3f,

        wallD,
        wallH,
        1.8f);

    // RIGHT LOWER WALL
    drawWall(
        6.5f,
        wallY,
        -1.5f,

        wallD,
        wallH,
        11.0f);

    // RIGHT UPPER WALL
    drawWall(
        5.9f,
        wallY,
        -7.0f,

        1.5f,
        wallH,
        wallD);

    // RIGHT CENTER WALL
    drawWall(
        5.5f,
        wallY,
        4.0f,

        2.0f,
        wallH,
        wallD);

    // TOP RIGHT
    drawWall(
        4.5f,
        wallY,
        6.4f,

        wallD,
        wallH,
        5.1f);

    // TOP CENTER
    drawWall(
        1.0f,
        wallY,
        8.8f,

        7.0f,
        wallH,
        wallD);

    // TOP CENTER-LEFT
    drawWall(
        -2.5f,
        wallY,
        7.8f,

        wallD,
        wallH,
        2.5f);

    // TOP LEFT
    drawWall(
        -3.8f,
        wallY,
        7.0f,

        2.8f,
        wallH,
        1.0f);
}

// INTERNAL WALLS

void drawInternalWalls()
{
    float wallY = 0.9f;
    float wallH = 3.8f;


    // LEFT UPPER VERTICAL
    drawWall(
        -5.0f,
        wallY,
        4.5f,

        0.35f,
        wallH,
        5.4f);

    // LEFT UPPER HORIZONTAL
    drawWall(
        -6.7f,
        wallY,
        1.8f,

        3.5f,
        wallH,
        0.35f);

    // T1
    drawT1();

    // CENTRAL VERTICAL WALL
    drawWall(
        0.0f,
        wallY,
        -2.0f,

        0.35f,
        wallH,
        6.0f);

    // RIGHT INTERNAL WALL
    drawWall(
        4.2f,
        wallY,
        1.5f,

        0.35f,
        wallH,
        2.9f);

    // RIGHT INTERNAL WALL
    drawWall(
        5.0f,
        wallY,
        3.0f,

        2.0f,
        wallH,
        0.35f);
}

// BUILDINGS

void drawBuildings()
{
    // LEFT SB
    drawBuilding(
        -6.2f,
        0.8f,
        -4.0f,

        1.7f,
        7.6f,
        1.7f);

    // T2
    drawT2();

    // TOP FB
    drawBuilding(
        0.0f,
        1.0f,
        3.0f,

        2.0f,
        10.0f,
        2.4f);

    // FB2
    drawBuilding(
        2.8f,
        0.9f,
        4.3f,

        1.1f,
        8.8f,
        2.1f);

    // MIDDLE RIGHT FB
    drawBuilding(
        2.8f,
        0.9f,
        -1.8f,

        1.8f,
        5.8f,
        2.5f);

    // BOTTOM FB
    drawBuilding(
        1.0f,
        0.8f,
        -6.8f,

        3.8f,
        5.6f,
        1.5f);

    // BOTTOM SB
    drawBuilding(
        3.7f,
        0.8f,
        -6.8f,

        1.4f,
        10.6f,
        1.5f);
}

// DRAW MAP

void drawMap()
{
    glPushMatrix();

    glScalef(
        MAP_SCALE,
        1.0f,
        MAP_SCALE);

    drawFloor();

    drawOuterWalls();

    drawInternalWalls();

    drawBuildings();

    drawWindMill();

    glPopMatrix();
}

// SKY

void drawSky()
{
    glPushMatrix();

    glTranslatef(
        cameraX,
        cameraY,
        cameraZ);

    glDisable(
        GL_LIGHTING);

    glDepthMask(
        GL_FALSE);

    glEnable(
        GL_TEXTURE_2D);

    glBindTexture(
        GL_TEXTURE_2D,
        isNightMode ? skyNightTexture : skyDayTexture);

    glColor3f(
        1.0f,
        1.0f,
        1.0f);
    glRotatef(
        -90.0f,
        1.0f,
        0.0f,
        0.0f);

    GLUquadric *skyQuadric =
        gluNewQuadric();

    gluQuadricTexture(
        skyQuadric,
        GL_TRUE);


    gluQuadricOrientation(
        skyQuadric,
        GLU_INSIDE);

    gluSphere(
        skyQuadric,
        70.0f,
        24,
        16);

    gluDeleteQuadric(
        skyQuadric);

    glBindTexture(
        GL_TEXTURE_2D,
        0);

    glDepthMask(
        GL_TRUE);

    glEnable(
        GL_LIGHTING);

    glPopMatrix();
}


void drawSunOrMoon()
{

    float dirX = 0.35f;
    float dirY = 0.62f;
    float dirZ = -0.70f;

    float len =
        sqrtf(
            dirX * dirX +
            dirY * dirY +
            dirZ * dirZ);

    dirX /= len;
    dirY /= len;
    dirZ /= len;

    const float distance = 60.0f;
    const float spriteSize = isNightMode ? 4.5f : 6.0f;

    float centerX = cameraX + dirX * distance;
    float centerY = cameraY + dirY * distance;
    float centerZ = cameraZ + dirZ * distance;

    float yawRad =
        cameraYaw *
        3.14159265f /
        180.0f;

    float rightX = cosf(yawRad);
    float rightZ = sinf(yawRad);

    float upX = 0.0f;
    float upY = 1.0f;
    float upZ = 0.0f;

    glDisable(
        GL_LIGHTING);

    glDisable(
        GL_DEPTH_TEST);

    glDepthMask(
        GL_FALSE);

    glEnable(
        GL_BLEND);

    glBlendFunc(
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA);

    glEnable(
        GL_TEXTURE_2D);

    glBindTexture(
        GL_TEXTURE_2D,
        isNightMode ? moonTexture : sunTexture);

    glColor4f(
        1.0f,
        1.0f,
        1.0f,
        1.0f);

    glBegin(
        GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(
        centerX - rightX * spriteSize,
        centerY - upY * spriteSize,
        centerZ - rightZ * spriteSize);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(
        centerX + rightX * spriteSize,
        centerY - upY * spriteSize,
        centerZ + rightZ * spriteSize);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(
        centerX + rightX * spriteSize,
        centerY + upY * spriteSize,
        centerZ + rightZ * spriteSize);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(
        centerX - rightX * spriteSize,
        centerY + upY * spriteSize,
        centerZ - rightZ * spriteSize);

    glEnd();

    glBindTexture(
        GL_TEXTURE_2D,
        0);

    glDisable(
        GL_BLEND);

    glDepthMask(
        GL_TRUE);

    glEnable(
        GL_DEPTH_TEST);

    glEnable(
        GL_LIGHTING);

    (void)upX;
    (void)upZ;
}

// DISPLAY

void display()
{
    glClear(
        GL_COLOR_BUFFER_BIT |
        GL_DEPTH_BUFFER_BIT);

    if (gameState == STATE_MENU)
    {
        drawMenuScreen();

        glutSwapBuffers();

        return;
    }

    glMatrixMode(
        GL_MODELVIEW);

    glLoadIdentity();

    // POV

    float yawRad =
        cameraYaw *
        3.14159265f /
        180.0f;

    float pitchRad =
        cameraPitch *
        3.14159265f /
        180.0f;

    float lookX =
        sinf(yawRad) *
        cosf(pitchRad);

    float lookY =
        sinf(pitchRad);

    float lookZ =
        -cosf(yawRad) *
        cosf(pitchRad);

    gluLookAt(
        cameraX,
        cameraY,
        cameraZ,

        cameraX + lookX,
        cameraY + lookY,
        cameraZ + lookZ,

        0.0f,
        1.0f,
        0.0f);

    // LIGHT POSITION

    GLfloat lightPosition[] =
        {
            0.0f,
            10.0f,
            0.0f,
            1.0f};

    glLightfv(
        GL_LIGHT0,
        GL_POSITION,
        lightPosition);

    drawSky();

    drawSunOrMoon();

    drawMap();

    drawEnemies();

    drawGameUI();

    glutSwapBuffers();
}

// WINDMILL

void update(
    int )
{
    updateCamera();

    if (!windmillPaused)
    {
        windmillAngle +=
            2.0f;

        if (windmillAngle >=
            360.0f)
        {
            windmillAngle -=
                360.0f;
        }
    }


    if (gameState == STATE_PLAYING && victory)
    {
        DWORD elapsed =
            GetTickCount() -
            victoryStartTime;

        if (elapsed >= VICTORY_DISPLAY_MS)
        {
            resetGameToMenu();
        }
    }

    glutPostRedisplay();

    glutTimerFunc(
        30,
        update,
        0);
}

// RESIZE

void resize(
    int width,
    int height)
{
    if (height == 0)
        height = 1;

    glViewport(
        0,
        0,
        width,
        height);

    glMatrixMode(
        GL_PROJECTION);

    glLoadIdentity();

    float aspect =
        (float)width /
        (float)height;

    gluPerspective(
        45.0f,
        aspect,
        1.0f,
        100.0f);

    glMatrixMode(
        GL_MODELVIEW);
}

// INITIALIZE

void init()
{
    glEnable(
        GL_DEPTH_TEST);

    glEnable(
        GL_LIGHTING);

    glEnable(
        GL_LIGHT0);

    GLfloat lightPosition[] =
        {
            0.0f,
            10.0f,
            0.0f,
            1.0f};

    glLightfv(
        GL_LIGHT0,
        GL_POSITION,
        lightPosition);
    applyLightingMode();


    glEnable(
        GL_COLOR_MATERIAL);

    glColorMaterial(
        GL_FRONT_AND_BACK,
        GL_AMBIENT_AND_DIFFUSE);

    glShadeModel(
        GL_SMOOTH);

    // TEXTURES

    glEnable(
        GL_TEXTURE_2D);

    loadwallTexture();

    loadFloorTexture();

    loadBuildTexture();

    loadBuild1Texture();

    loadWindmillTowerTexture();

    loadWindmillBladeTexture();

    loadSkyTextures();

    loadSunMoonTextures();
}

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
        1920,
        1080);

    glutCreateWindow(
        "Graphics and Animation - 3D Map");

    init();

    setupCollisions();


    glutSetCursor(
        GLUT_CURSOR_LEFT_ARROW);

    glutPassiveMotionFunc(
        mouseMotion);


    glutMouseFunc(
        mouseClick);

    glutKeyboardFunc(
        keyboard);

    glutSpecialFunc(
        specialKeys);

    glutDisplayFunc(
        display);

    glutReshapeFunc(
        resize);

    glutTimerFunc(
        30,
        update,
        0);

    glutMainLoop();

    return 0;
}
