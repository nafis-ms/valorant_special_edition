#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "image/stb_image.h"

// ============================================================
// TEXTURES
// ============================================================

GLuint wallTexture;
GLuint floorTexture;
GLuint buildTexture;
GLuint build1Texture;


// ============================================================
// MAP SCALE
// ============================================================

const float MAP_SCALE = 2.0f;


// ============================================================
// FPS CAMERA
// ============================================================

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


// ============================================================
// PLAYER COLLISION
// ============================================================

float playerRadius = 0.15f;

struct WallCollision
{
    float minX;
    float maxX;
    float minZ;
    float maxZ;
};

WallCollision walls[100];
int wallCount = 0;


// ============================================================
// ADD COLLISION BOX
// ============================================================

void addCollisionWall(
    float x,
    float z,
    float width,
    float depth
)
{
    if (wallCount >= 100)
        return;

    // Apply the same 2x scale used by the map
    x *= MAP_SCALE;
    z *= MAP_SCALE;

    width *= MAP_SCALE;
    depth *= MAP_SCALE;

walls[wallCount].minX =
    x - width / 2.0f - 0.5f;

walls[wallCount].maxX =
    x + width / 2.0f + 0.5f;

walls[wallCount].minZ =
    z - depth / 2.0f - 0.5f;

walls[wallCount].maxZ =
    z + depth / 2.0f + 0.5f;

    wallCount++;
}


// ============================================================
// CHECK COLLISION
// ============================================================

bool checkCollision(
    float x,
    float z
)
{
    for (int i = 0; i < wallCount; i++)
    {
        if (x + playerRadius > walls[i].minX &&
            x - playerRadius < walls[i].maxX &&
            z + playerRadius > walls[i].minZ &&
            z - playerRadius < walls[i].maxZ)
        {
            return true;
        }
    }

    return false;
}


// ============================================================
// SETUP ALL COLLISIONS
// ============================================================

void setupCollisions()
{
    wallCount = 0;

    float wallD = 0.35f;

    // ========================================================
    // OUTER WALLS
    // ========================================================

    // LEFT OUTER WALL
    addCollisionWall(
        -8.5f,
        -2.8f,
        wallD,
        9.5f
    );

    // LOWER LEFT WALL
    addCollisionWall(
        -6.3f,
        -7.5f,
        4.7f,
        wallD
    );

    // BOTTOM WALL
    addCollisionWall(
        0.7f,
        -9.0f,
        9.3f,
        wallD
    );

    // BOTTOM RIGHT WALL
    addCollisionWall(
        5.2f,
        -8.0f,
        wallD,
        2.0f
    );

    // SMALL BOTTOM LEFT VERTICAL WALL
    addCollisionWall(
        -4.1f,
        -8.3f,
        wallD,
        1.8f
    );

    // RIGHT LOWER WALL
    addCollisionWall(
        6.5f,
        -1.5f,
        wallD,
        11.0f
    );

    // RIGHT UPPER WALL
    addCollisionWall(
        5.9f,
        -7.0f,
        1.5f,
        wallD
    );

    // RIGHT CENTER WALL
    addCollisionWall(
        5.5f,
        4.0f,
        2.0f,
        wallD
    );

    // TOP RIGHT
    addCollisionWall(
        4.5f,
        6.4f,
        wallD,
        5.1f
    );

    // TOP CENTER
    addCollisionWall(
        1.0f,
        8.8f,
        7.0f,
        wallD
    );

    // TOP CENTER-LEFT
    addCollisionWall(
        -2.5f,
        7.8f,
        wallD,
        2.5f
    );

    // TOP LEFT
    addCollisionWall(
        -3.8f,
        7.0f,
        2.8f,
        1.0f
    );


    // ========================================================
    // INTERNAL WALLS
    // ========================================================

    // LEFT UPPER ROOM - VERTICAL
    addCollisionWall(
        -5.0f,
        4.5f,
        0.35f,
        5.4f
    );

    // LEFT UPPER ROOM - HORIZONTAL
    addCollisionWall(
        -6.7f,
        1.8f,
        3.5f,
        0.35f
    );


    // ========================================================
    // T1 TUNNEL
    // ========================================================

    // T1 LEFT WALL
    addCollisionWall(
        -6.3f,
        -1.8f,
        4.5f,
        0.35f
    );

    // T1 RIGHT WALL
    addCollisionWall(
        0.2f,
        -1.8f,
        7.0f,
        0.35f
    );


    // ========================================================
    // CENTRAL VERTICAL WALL
    // ========================================================

    addCollisionWall(
        0.0f,
        -2.0f,
        0.35f,
        6.0f
    );


    // ========================================================
    // RIGHT INTERNAL WALLS
    // ========================================================

    addCollisionWall(
        4.2f,
        1.5f,
        0.35f,
        2.9f
    );

    addCollisionWall(
        5.0f,
        3.0f,
        2.0f,
        0.35f
    );


    // ========================================================
    // BUILDINGS
    // ========================================================

    // LEFT SB
    addCollisionWall(
        -6.2f,
        -4.0f,
        1.7f,
        1.7f
    );

    // T2
    addCollisionWall(
        -2.4f,
        4.0f,
        1.8f,
        1.8f
    );

    // TOP FB
    addCollisionWall(
        0.0f,
        3.0f,
        2.0f,
        2.4f
    );

    // FB2
    addCollisionWall(
        2.8f,
        4.3f,
        1.1f,
        2.1f
    );

    // MIDDLE RIGHT FB
    addCollisionWall(
        2.8f,
        -1.8f,
        1.8f,
        2.5f
    );

    // BOTTOM FB
    addCollisionWall(
        1.0f,
        -6.8f,
        3.8f,
        1.5f
    );

    // BOTTOM SB
    addCollisionWall(
        3.7f,
        -6.8f,
        1.4f,
        1.5f
    );


    // ========================================================
    // WINDMILL BASE
    // ========================================================

    // WINDMILL BASE COLLISION
    addCollisionWall(
        -1.8f,
        -4.0f,
        1.7f,
        1.8f
    );

    // WINDMILL TOWER COLLISION
    // The collision system uses X/Z only, so this box
    // protects the full height of the tower.
    addCollisionWall(
        -1.8f,
        -4.0f,
        0.70f,
        0.70f
    );
}


// ============================================================
// FPS CAMERA MOVEMENT
// ============================================================

void updateCamera()
{
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
    {
        PostQuitMessage(0);
        return;
    }

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


    // ========================================================
    // FORWARD
    // ========================================================

    if (GetAsyncKeyState('W') & 0x8000)
    {
        newX +=
            forwardX * moveSpeed;

        newZ +=
            forwardZ * moveSpeed;
    }


    // ========================================================
    // BACKWARD
    // ========================================================

    if (GetAsyncKeyState('S') & 0x8000)
    {
        newX -=
            forwardX * moveSpeed;

        newZ -=
            forwardZ * moveSpeed;
    }


    // ========================================================
    // LEFT
    // ========================================================

    if (GetAsyncKeyState('A') & 0x8000)
    {
        newX -=
            rightX * strafeSpeed;

        newZ -=
            rightZ * strafeSpeed;
    }


    // ========================================================
    // RIGHT
    // ========================================================

    if (GetAsyncKeyState('D') & 0x8000)
    {
        newX +=
            rightX * strafeSpeed;

        newZ +=
            rightZ * strafeSpeed;
    }


    // ========================================================
    // COLLISION + WALL SLIDING
    // ========================================================

    // Check X movement
    if (!checkCollision(newX, cameraZ))
    {
        cameraX = newX;
    }

    // Check Z movement
    if (!checkCollision(cameraX, newZ))
    {
        cameraZ = newZ;
    }
}


// ============================================================
// MOUSE LOOK
// ============================================================

void centerMouse()
{
    int windowX =
        glutGet(GLUT_WINDOW_X);

    int windowY =
        glutGet(GLUT_WINDOW_Y);

    int windowWidth =
        glutGet(GLUT_WINDOW_WIDTH);

    int windowHeight =
        glutGet(GLUT_WINDOW_HEIGHT);

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
        center.y
    );
}


void mouseMotion(
    int x,
    int y
)
{
    if (ignoreMouse)
    {
        ignoreMouse = false;
        return;
    }

    int centerX =
        glutGet(GLUT_WINDOW_WIDTH) / 2;

    int centerY =
        glutGet(GLUT_WINDOW_HEIGHT) / 2;

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


// ============================================================
// WINDMILL
// ============================================================

float windmillAngle = 0.0f;


// ============================================================
// LOAD WALL TEXTURE
// ============================================================

void loadwallTexture()
{
    int width;
    int height;
    int channels;

    unsigned char* image =
        stbi_load(
            "image/wall.jpg",
            &width,
            &height,
            &channels,
            0
        );

    if (!image)
    {
        printf(
            "FAILED TO LOAD: image/wall.jpg\n"
        );

        printf(
            "Reason: %s\n",
            stbi_failure_reason()
        );

        return;
    }

    GLenum format;

    if (channels == 4)
        format = GL_RGBA;
    else if (channels == 3)
        format = GL_RGB;
    else if (channels == 1)
        format = GL_LUMINANCE;
    else
    {
        printf(
            "Unsupported image format!\n"
        );

        stbi_image_free(image);

        return;
    }

    glGenTextures(
        1,
        &wallTexture
    );

    glBindTexture(
        GL_TEXTURE_2D,
        wallTexture
    );

    glPixelStorei(
        GL_UNPACK_ALIGNMENT,
        1
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_REPEAT
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_REPEAT
    );

    glTexEnvi(
        GL_TEXTURE_ENV,
        GL_TEXTURE_ENV_MODE,
        GL_REPLACE
    );

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        format,
        width,
        height,
        0,
        format,
        GL_UNSIGNED_BYTE,
        image
    );

    glBindTexture(
        GL_TEXTURE_2D,
        0
    );

    stbi_image_free(image);

    printf(
        "wall texture loaded successfully!\n"
    );

    printf(
        "Size: %d x %d | Channels: %d\n",
        width,
        height,
        channels
    );
}


// ============================================================
// LOAD FLOOR TEXTURE
// ============================================================

void loadFloorTexture()
{
    int width;
    int height;
    int channels;

    unsigned char* image =
        stbi_load(
            "image/floor.jpg",
            &width,
            &height,
            &channels,
            0
        );

    if (!image)
    {
        printf(
            "FAILED TO LOAD: image/floor.jpg\n"
        );

        printf(
            "Reason: %s\n",
            stbi_failure_reason()
        );

        return;
    }

    GLenum format;

    if (channels == 4)
        format = GL_RGBA;
    else if (channels == 3)
        format = GL_RGB;
    else if (channels == 1)
        format = GL_LUMINANCE;
    else
    {
        printf(
            "Unsupported floor image format!\n"
        );

        stbi_image_free(image);

        return;
    }

    glGenTextures(
        1,
        &floorTexture
    );

    glBindTexture(
        GL_TEXTURE_2D,
        floorTexture
    );

    glPixelStorei(
        GL_UNPACK_ALIGNMENT,
        1
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_REPEAT
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_REPEAT
    );

    glTexEnvi(
        GL_TEXTURE_ENV,
        GL_TEXTURE_ENV_MODE,
        GL_REPLACE
    );

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        format,
        width,
        height,
        0,
        format,
        GL_UNSIGNED_BYTE,
        image
    );

    glBindTexture(
        GL_TEXTURE_2D,
        0
    );

    stbi_image_free(image);

    printf(
        "Floor texture loaded successfully!\n"
    );

    printf(
        "Floor size: %d x %d | Channels: %d\n",
        width,
        height,
        channels
    );
}


// ============================================================
// LOAD BUILDING TEXTURE
// ============================================================

void loadBuildTexture()
{
    int width;
    int height;
    int channels;

    unsigned char* image =
        stbi_load(
            "image/build.jpg",
            &width,
            &height,
            &channels,
            0
        );

    if (!image)
    {
        printf(
            "FAILED TO LOAD: image/build.jpg\n"
        );

        printf(
            "Reason: %s\n",
            stbi_failure_reason()
        );

        return;
    }

    GLenum format;

    if (channels == 4)
        format = GL_RGBA;
    else if (channels == 3)
        format = GL_RGB;
    else if (channels == 1)
        format = GL_LUMINANCE;
    else
    {
        printf(
            "Unsupported building image format!\n"
        );

        stbi_image_free(image);
        return;
    }

    glGenTextures(
        1,
        &buildTexture
    );

    glBindTexture(
        GL_TEXTURE_2D,
        buildTexture
    );

    glPixelStorei(
        GL_UNPACK_ALIGNMENT,
        1
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_REPEAT
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_REPEAT
    );

    glTexEnvi(
        GL_TEXTURE_ENV,
        GL_TEXTURE_ENV_MODE,
        GL_REPLACE
    );

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        format,
        width,
        height,
        0,
        format,
        GL_UNSIGNED_BYTE,
        image
    );

    glBindTexture(
        GL_TEXTURE_2D,
        0
    );

    stbi_image_free(image);

    printf(
        "Building texture loaded successfully!\n"
    );

    printf(
        "Building size: %d x %d | Channels: %d\n",
        width,
        height,
        channels
    );
}


// ============================================================
// DRAW CUBE
// ============================================================

void drawCube(
    float x,
    float y,
    float z,
    float width,
    float height,
    float depth
)
{
    glPushMatrix();

    glTranslatef(
        x,
        y,
        z
    );

    glScalef(
        width,
        height,
        depth
    );

    glutSolidCube(1.0f);

    glPopMatrix();
}


// ============================================================
// DRAW TEXTURED CUBE
// ============================================================

void drawTexturedCube(
    float x,
    float y,
    float z,
    float width,
    float height,
    float depth,
    GLuint texture
)
{
    float x1 = -width / 2.0f;
    float x2 =  width / 2.0f;

    float y1 = -height / 2.0f;
    float y2 =  height / 2.0f;

    float z1 = -depth / 2.0f;
    float z2 =  depth / 2.0f;

    glPushMatrix();

    glTranslatef(
        x,
        y,
        z
    );

    glEnable(GL_TEXTURE_2D);

    glBindTexture(
        GL_TEXTURE_2D,
        texture
    );

    glColor3f(
        1.0f,
        1.0f,
        1.0f
    );

    glBegin(GL_QUADS);

    // FRONT
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x1, y1, z2);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(x2, y1, z2);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(x2, y2, z2);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(x1, y2, z2);

    // BACK
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x2, y1, z1);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(x1, y1, z1);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(x1, y2, z1);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(x2, y2, z1);

    // LEFT
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x1, y1, z1);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(x1, y1, z2);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(x1, y2, z2);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(x1, y2, z1);

    // RIGHT
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x2, y1, z2);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(x2, y1, z1);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(x2, y2, z1);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(x2, y2, z2);

    // TOP
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x1, y2, z2);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(x2, y2, z2);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(x2, y2, z1);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(x1, y2, z1);

    // BOTTOM
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x1, y1, z1);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(x2, y1, z1);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(x2, y1, z2);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(x1, y1, z2);

    glEnd();

    glBindTexture(
        GL_TEXTURE_2D,
        0
    );

    glPopMatrix();
}


// ============================================================
// DRAW FLOOR
// ============================================================

void drawFloor()
{
    glEnable(GL_TEXTURE_2D);

    glBindTexture(
        GL_TEXTURE_2D,
        floorTexture
    );

    glColor3f(
        1.0f,
        1.0f,
        1.0f
    );

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(
        -11.0f,
        0.0f,
        -11.0f
    );

    glTexCoord2f(11.0f, 0.0f);
    glVertex3f(
        11.0f,
        0.0f,
        -11.0f
    );

    glTexCoord2f(11.0f, 11.0f);
    glVertex3f(
        11.0f,
        0.0f,
        11.0f
    );

    glTexCoord2f(0.0f, 11.0f);
    glVertex3f(
        -11.0f,
        0.0f,
        11.0f
    );

    glEnd();

    glBindTexture(
        GL_TEXTURE_2D,
        0
    );
}


// ============================================================
// LOAD BUILDING1 TEXTURE (HEIGHT <= 7)
// ============================================================

void loadBuild1Texture()
{
    int width;
    int height;
    int channels;

    unsigned char* image =
        stbi_load(
            "image/build1.jpg",
            &width,
            &height,
            &channels,
            0
        );

    if (!image)
    {
        printf(
            "FAILED TO LOAD: image/build1.jpg\n"
        );

        printf(
            "Reason: %s\n",
            stbi_failure_reason()
        );

        return;
    }

    GLenum format;

    if (channels == 4)
        format = GL_RGBA;
    else if (channels == 3)
        format = GL_RGB;
    else if (channels == 1)
        format = GL_LUMINANCE;
    else
    {
        printf(
            "Unsupported building1 image format!\n"
        );

        stbi_image_free(image);
        return;
    }

    glGenTextures(
        1,
        &build1Texture
    );

    glBindTexture(
        GL_TEXTURE_2D,
        build1Texture
    );

    glPixelStorei(
        GL_UNPACK_ALIGNMENT,
        1
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_REPEAT
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_REPEAT
    );

    glTexEnvi(
        GL_TEXTURE_ENV,
        GL_TEXTURE_ENV_MODE,
        GL_REPLACE
    );

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        format,
        width,
        height,
        0,
        format,
        GL_UNSIGNED_BYTE,
        image
    );

    glBindTexture(
        GL_TEXTURE_2D,
        0
    );

    stbi_image_free(image);

    printf(
        "Building1 texture loaded successfully!\n"
    );

    printf(
        "Building size: %d x %d | Channels: %d\n",
        width,
        height,
        channels
    );
}



// ============================================================
// DRAW WALL
// ============================================================

void drawWall(
    float x,
    float y,
    float z,
    float width,
    float height,
    float depth
)
{
    glColor3f(
        1.0f,
        1.0f,
        1.0f
    );

    drawTexturedCube(
        x,
        y,
        z,
        width,
        height,
        depth,
        wallTexture
    );
}


// ============================================================
// DRAW BUILDING
// ============================================================

void drawBuilding(
    float x,
    float y,
    float z,
    float width,
    float height,
    float depth
)
{
    // Buildings with height <= 7 use build1.jpg.
    // Taller buildings use build.jpg.
    GLuint selectedTexture =
        (height <= 7.0f) ? build1Texture : buildTexture;

    // Main building body.
    drawTexturedCube(
        x,
        y,
        z,
        width,
        height,
        depth,
        selectedTexture
    );

    // Small rooftop trim uses the same texture as the building.
    drawTexturedCube(
        x,
        y + height / 2.0f + 0.025f,
        z,
        width - 0.12f,
        0.05f,
        depth - 0.12f,
        selectedTexture
    );
}


// ============================================================
// DRAW T1 TUNNEL
// ============================================================

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
        -2.6f,
        2.55f,
        -1.8f,
        3.4f,
        0.5f,
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
        6.6f,
        1.8f
    );
}


// ============================================================
// DRAW WINDMILL
// ============================================================

void drawWindMill()
{
    // ========================================================
    // BASE
    // ========================================================

    glDisable(GL_TEXTURE_2D);


    // ========================================================
    // TOWER - TALLEST STRUCTURE
    // ========================================================

    glColor3f(
        0.55f,
        0.57f,
        0.58f
    );

    const float windmillTowerHeight = 10.5f;
    const float windmillTowerBaseY = 0.0f;
    const float windmillTopY = windmillTowerBaseY + windmillTowerHeight;

    glPushMatrix();

    glTranslatef(
        -1.8f,
        windmillTowerBaseY,
        -4.0f
    );

    glRotatef(
        -90.0f,
        1.0f,
        0.0f,
        0.0f
    );

    GLUquadric* quadric =
        gluNewQuadric();

    gluCylinder(
        quadric,
        0.45f,
        0.22f,
        windmillTowerHeight,
        20,
        10
    );

    gluDeleteQuadric(
        quadric
    );

    glPopMatrix();


    // ========================================================
    // BLADES
    // ========================================================

    glPushMatrix();

    glTranslatef(
        -1.8f,
        windmillTopY,
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
            0.75f,
            0.0f,
            0.18f,
            1.5f,
            0.12f
        );

        glPopMatrix();
    }

    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
}


// ============================================================
// OUTER WALLS
// ============================================================

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
        9.5f
    );

    // LOWER LEFT WALL
    drawWall(
        -6.3f,
        wallY,
        -7.5f,
        4.7f,
        wallH,
        wallD
    );

    // BOTTOM WALL
    drawWall(
        0.7f,
        wallY,
        -9.0f,
        9.3f,
        wallH,
        wallD
    );

    // BOTTOM RIGHT WALL
    drawWall(
        5.2f,
        wallY,
        -8.0f,
        wallD,
        wallH,
        2.0f
    );

    // SMALL BOTTOM LEFT VERTICAL WALL
    drawWall(
        -4.1f,
        wallY,
        -8.3f,
        wallD,
        wallH,
        1.8f
    );

    // RIGHT LOWER WALL
    drawWall(
        6.5f,
        wallY,
        -1.5f,
        wallD,
        wallH,
        11.0f
    );

    // RIGHT UPPER WALL
    drawWall(
        5.9f,
        wallY,
        -7.0f,
        1.5f,
        wallH,
        wallD
    );

    // RIGHT CENTER WALL
    drawWall(
        5.5f,
        wallY,
        4.0f,
        2.0f,
        wallH,
        wallD
    );

    // TOP RIGHT
    drawWall(
        4.5f,
        wallY,
        6.4f,
        wallD,
        wallH,
        5.1f
    );

    // TOP CENTER
    drawWall(
        1.0f,
        wallY,
        8.8f,
        7.0f,
        wallH,
        wallD
    );

    // TOP CENTER-LEFT
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
        -3.8f,
        wallY,
        7.0f,
        2.8f,
        wallH,
        1.0f
    );
}


// ============================================================
// INTERNAL WALLS
// ============================================================

void drawInternalWalls()
{
    float wallY = 0.9f;
    float wallH = 3.8f;
    float wallD = 0.35f;

    // LEFT UPPER ROOM
    drawWall(
        -5.0f,
        wallY,
        4.5f,
        0.35f,
        wallH,
        5.4f
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


    // RIGHT INTERNAL WALLS
    drawWall(
        4.2f,
        wallY,
        1.5f,
        0.35f,
        wallH,
        2.9f
    );

    drawWall(
        5.0f,
        wallY,
        3.0f,
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
        7.6f,
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
        10.0f,
        2.4f
    );

    // FB2
    drawBuilding(
        2.8f,
        0.9f,
        4.3f,
        1.1f,
        8.8f,
        2.1f
    );

    // MIDDLE RIGHT FB
    drawBuilding(
        2.8f,
        0.9f,
        -1.8f,
        1.8f,
        5.8f,
        2.5f
    );

    // BOTTOM FB
    drawBuilding(
        1.0f,
        0.8f,
        -6.8f,
        3.8f,
        5.6f,
        1.5f
    );

    // BOTTOM SB
    drawBuilding(
        3.7f,
        0.8f,
        -6.8f,
        1.4f,
        10.6f,
        1.5f
    );
}


// ============================================================
// DRAW MAP
// ============================================================

void drawMap()
{
    glPushMatrix();

    // 2x larger in X and Z
    // Keep Y unchanged
    glScalef(
        MAP_SCALE,
        1.0f,
        MAP_SCALE
    );

    drawFloor();
    drawOuterWalls();
    drawInternalWalls();
    drawBuildings();
    drawWindMill();

    glPopMatrix();
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
    // FIRST-PERSON CAMERA
    // ========================================================

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
        0.0f
    );

    drawMap();

    glutSwapBuffers();
}


// ============================================================
// WINDMILL ANIMATION
// ============================================================

void update(int value)
{
    updateCamera();

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

    glMatrixMode(
        GL_PROJECTION
    );

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

    glMatrixMode(
        GL_MODELVIEW
    );
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

    glEnable(
        GL_DEPTH_TEST
    );

    glDisable(
        GL_LIGHTING
    );

    glShadeModel(
        GL_FLAT
    );

    glEnable(
        GL_COLOR_MATERIAL
    );


    // ========================================================
    // TEXTURE
    // ========================================================

    glEnable(
        GL_TEXTURE_2D
    );

    loadwallTexture();
    loadFloorTexture();
    loadBuildTexture();
    loadBuild1Texture();
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
        1920,
        1080
    );

    glutCreateWindow(
        "Graphics and Animation - 3D Map"
    );

    init();


    // ========================================================
    // SETUP COLLISION FOR ALL STRUCTURES
    // ========================================================

    setupCollisions();


    // ========================================================
    // FPS MOUSE LOOK
    // ========================================================

    glutSetCursor(
        GLUT_CURSOR_NONE
    );

    centerMouse();

    glutPassiveMotionFunc(
        mouseMotion
    );

    glutDisplayFunc(
        display
    );

    glutReshapeFunc(
        resize
    );

    glutTimerFunc(
        30,
        update,
        0
    );

    glutMainLoop();

    return 0;
}
