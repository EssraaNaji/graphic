#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

#define NUM_DROPS 1200
#define MAX_BG_BUILDINGS 30

typedef struct {
    float x, y, w, h;
    int r, g, b;
} Building;

typedef struct {
    float x, y, speed;
} RainDrop;

RainDrop drops[NUM_DROPS];
Building bgBuildings[MAX_BG_BUILDINGS];
int numBG = 0;

float cloud1 = 100, cloud2 = 600, cloud3 = 350, cloud4 = 800;
float cloudSpeed = 0.3f;
float rainSpeed = 7.0f;

int nightMode = 0;

void initRain() {
    for (int i = 0; i < NUM_DROPS; i++) {
        drops[i].x = rand() % 1000;
        drops[i].y = rand() % 600;
        drops[i].speed = 5.0f + (float)(rand() % 5);
    }
}

void initBackgroundBuildings() {
    numBG = MAX_BG_BUILDINGS;
    int ground = 450;
    for (int i = 0; i < numBG; i++) {
        bgBuildings[i].x = i * 60;
        bgBuildings[i].w = 40 + rand() % 50;
        bgBuildings[i].h = 100 + rand() % 200;
        bgBuildings[i].y = ground - bgBuildings[i].h;
        bgBuildings[i].r = 100 + rand() % 100;
        bgBuildings[i].g = 100 + rand() % 100;
        bgBuildings[i].b = 100 + rand() % 100;
    }
}
void updateRain() {
    // لو الوضع مش ليلي، مفيش داعي نحسب حركة المطر
    if (!nightMode) return;

    for (int i = 0; i < NUM_DROPS; i++) {
        drops[i].y += drops[i].speed + rainSpeed;
        if (drops[i].y > 600) {
            drops[i].y = 0;
            drops[i].x = rand() % 1000;
        }
    }
}

void updateClouds() {
    cloud1 += cloudSpeed; if (cloud1 > 1100) cloud1 = -150;
    cloud2 -= cloudSpeed; if (cloud2 < -150) cloud2 = 1100;
    cloud3 += cloudSpeed * 0.5; if (cloud3 > 1100) cloud3 = -150;
    cloud4 -= cloudSpeed * 0.7; if (cloud4 < -150) cloud4 = 1100;
    glutPostRedisplay();
}
void drawRain() {
    if (!nightMode) return;

   glColor4f(0.7f, 0.7f, 0.9f, 0.4f);

    glLineWidth(1.0);
    glBegin(GL_LINES);
    for (int i = 0; i < NUM_DROPS; i++) {
        glVertex2f(drops[i].x, drops[i].y);
        glVertex2f(drops[i].x, drops[i].y + 15);
    }
    glEnd();
}

void filledCircle(float r, float cx, float cy) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 40; i++) {
        float angle = 2.0f * 3.1416f * i / 40;
        float x = r * cosf(angle);
        float y = r * sinf(angle);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}

void drawSky() {
    glBegin(GL_QUADS);
    if (nightMode) {
        glColor3ub(5, 5, 20);
        glVertex2f(0, 0); glVertex2f(1000, 0);
        glColor3ub(25, 25, 80);
        glVertex2f(1000, 600); glVertex2f(0, 600);
    } else {
        glColor3ub(135, 206, 250);
        glVertex2f(0, 0); glVertex2f(1000, 0);
        glColor3ub(220, 240, 255);
        glVertex2f(1000, 600); glVertex2f(0, 600);
    }
    glEnd();
}

void drawCloud(float cx, float cy) {
    if (nightMode) glColor3ub(80, 80, 100);
    else glColor3ub(255, 255, 255);

    filledCircle(60, cx, cy);
    filledCircle(50, cx + 50, cy + 10);
    filledCircle(45, cx - 50, cy + 5);
    filledCircle(55, cx + 20, cy + 25);
}

void drawBuilding(float x, float y, float w, float h, int r, int g, int b) {
    if (nightMode) glColor3ub(r / 4, g / 4, b / 4);
    else glColor3ub(r, g, b);

    glBegin(GL_QUADS);
    glVertex2f(x, y); glVertex2f(x + w, y);
    glVertex2f(x + w, y + h); glVertex2f(x, y + h);
    glEnd();

    glColor3ub(20, 20, 20);
    glLineWidth(1.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y); glVertex2f(x + w, y);
    glVertex2f(x + w, y + h); glVertex2f(x, y + h);
    glEnd();

    float windowW = 12, windowH = 18, spacingX = 20, spacingY = 28;
    for (float yy = y + 20; yy + windowH < y + h; yy += spacingY) {
        for (float xx = x + 10; xx + windowW < x + w; xx += spacingX) {
            if (nightMode) glColor3ub(255, 255, 100);
            else glColor3ub(180, 210, 230);

            glBegin(GL_QUADS);
            glVertex2f(xx, yy); glVertex2f(xx + windowW, yy);
            glVertex2f(xx + windowW, yy + windowH); glVertex2f(xx, yy + windowH);
            glEnd();
        }
    }
}

void drawTree(float x, float y) {
    glColor3ub(80, 50, 20);
    glBegin(GL_QUADS);
    glVertex2f(x - 6, y); glVertex2f(x + 6, y);
    glVertex2f(x + 6, y - 35); glVertex2f(x - 6, y - 35);
    glEnd();

    if (nightMode) glColor3ub(10, 50, 10);
    else glColor3ub(34, 139, 34);

    filledCircle(22, x, y - 45);
    filledCircle(18, x - 12, y - 35);
    filledCircle(18, x + 12, y - 35);
}

void drawStreet() {
    glBegin(GL_QUADS);
    if (nightMode) glColor3ub(80, 80, 80); else glColor3ub(192, 192, 192);
    glVertex2f(0, 530); glVertex2f(1000, 530);
    glVertex2f(1000, 550); glVertex2f(0, 550);

    if (nightMode) glColor3ub(20, 20, 25); else glColor3ub(50, 50, 60);
    glVertex2f(0, 550); glVertex2f(1000, 550);
    glVertex2f(1000, 600); glVertex2f(0, 600);
    glEnd();

    glColor3ub(255, 215, 0);
    glBegin(GL_LINES);
    for (int i = 20; i < 1000; i += 60) {
        glVertex2f(i, 575); glVertex2f(i + 30, 575);
    }
    glEnd();

    if (nightMode) {
        glColor3ub(255, 255, 180);
        glPointSize(4.0f);
        glBegin(GL_POINTS);
        for (int i = 10; i < 1000; i += 50) glVertex2f(i, 540);
        glEnd();
    }
}

void drawCity() {
    float ground = 450;

    // مباني الخلفية الثابتة
    for (int i = 0; i < numBG; i++) {
        drawBuilding(bgBuildings[i].x, bgBuildings[i].y, bgBuildings[i].w, bgBuildings[i].h,
                     bgBuildings[i].r, bgBuildings[i].g, bgBuildings[i].b);
    }

    // المباني الأمامية الكبيرة
    drawBuilding(360, ground - 450, 120, 450, 200, 80, 80);
    if (nightMode) glColor3ub(80, 20, 20); else glColor3ub(200, 80, 80);
    glBegin(GL_TRIANGLES);
    glVertex2f(360, ground - 450); glVertex2f(480, ground - 450); glVertex2f(420, ground - 520);
    glEnd();
    drawBuilding(500, ground - 300, 130, 300, 210, 180, 130);
    drawBuilding(650, ground - 400, 100, 400, 60, 120, 60);
    drawBuilding(770, ground - 280, 140, 280, 100, 140, 160);
    drawBuilding(930, ground - 350, 120, 350, 160, 100, 160);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawSky();
    drawCloud(cloud1, 100);
    drawCloud(cloud2, 150);
    drawCloud(cloud3, 80);
    drawCloud(cloud4, 130);

    if (nightMode) glColor3ub(0, 40, 0); else glColor3ub(124, 252, 0);
    glBegin(GL_QUADS); glVertex2f(0, 450); glVertex2f(1000, 450); glVertex2f(1000, 530); glVertex2f(0, 530); glEnd();

    drawCity();

    for (int i = 40; i < 1000; i += 80) drawTree(i, 480);

    drawStreet();
    drawRain();

    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        nightMode = !nightMode;
        glutPostRedisplay();
    }
}

void timer(int value) {
    updateRain();
    updateClouds();
    glutTimerFunc(16, timer, 0);
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1000, 600, 0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    initRain();
    initBackgroundBuildings();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1000, 600);
    glutCreateWindow("Night & Day City");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}
