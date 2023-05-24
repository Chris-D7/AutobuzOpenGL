/*
 Eliminarea fetelor functie de orientare
 Se va afisa fara activarea eliminarii fetelor. Cu orientarea directa
 implicita se vor schimba fetele care se elimina -cele fata sau cele spate- 
 cu functia glCullFace.
 Se va activa sau se va dezactiva testul de adancime
 */
#include "glos.h"
#include "math.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

void myinit(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK display(void);

static GLfloat alpha = 0, beta=0, gamma=0;
static GLfloat viewdistance = 500, ya=0, za=0;
static GLfloat x = -25, y = 50, z = 90;
GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };//coeficient de reflexie ambientala
GLfloat mat_ambient_color[] = { 0.1, 0.1, 0.1, 1.0 };
GLfloat mat_diffuse[] = { 0.1, 0.1, 0.1, 1.0 };//coeficient de reflexie difuza
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };//coeficient de reflexie speculara
GLfloat no_shininess[] = { 0.0 };//exponentul de reflexie speculara
GLfloat low_shininess[] = { 5.0 };//exponentul de reflexie speculara
GLfloat high_shininess[] = { 100.0 };//exponentul de reflexie speculara
GLfloat mat_emission[] = { 0.3, 0.2, 0.2, 0.0 };//intensitatea luminii emise
static GLfloat pozSursa[] = { -400.0, 400.0, 200, 1.0 };


GLfloat vertices[][3] = {
    //pentru corpul principal
    {-100, -25, -40}, {-100, 75, -40}, {200, 75, -40}, {200, -25, -40},
    {-100, -25, 40}, {-100, 75, 40}, {200, 75, 40}, {200, -25, 40}, //8
    //pentru geamuri
    {-115, 25, 40}, {-100, 75, 40}, {-100, 75, -40}, {-115, 25, -40},
    {-100, 25, 40}, {-100, 25, -40}, //14
    //pentru partea din fata/motor
    {-165, -25, -40}, {-165, 25, -40}, {-100, 25, -40}, {-100, -25, -40},
    {-165, -25, 40}, {-165, 25, 40}, {-100, 25, 40}, {-100, -25, 40}, //22
};
GLfloat ctrlpoints1[4][4][3] = {
    //pentru tavan
    {{-100, 75, -40}, {-100, 100, -30},
    {-100, 100, 30}, {-100, 75, 40}},
    {{-25, 75, -40},{-25, 100, -30},
    {-25, 100, 30}, {-25, 75, 40}},
    {{100, 75, -40},{100, 100, -30},
    {100, 100, 30}, {100, 75, 40}},
    {{200, 75, -40},{200, 100, -30},
    {200, 100, 30}, {200, 75, 40}}
};
GLfloat ctrlpoints2[4][4][3] = {
    //pentru motor
    {{-165, 25, -40}, {-165, 25, -30},
    {-165, 25, 30}, {-165, 25, 40}},
    {{-165, 25, -40},{-165, 35, -30},
    {-165, 35, 30}, {-165, 25, 40}},
    {{-155, 25, -40},{-155, 35, -40},
    {-155, 35, 40}, {-155, 25, 40}},
    {{-110, 25, -40},{-110, 35, -40},
    {-110, 35, 40}, {-110, 25, 40}}
};


void myinit(void)
{
    GLfloat mat_ambient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    // reflectanta speculara si exponentul de reflexie speculara nu sunt la valorile implicite (0.0)
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 128.0 };
    glMaterialfv(GL_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // valori implicite pentru intensitatea sursei LIGHT0
    GLfloat light_ambient[] = { 0.4, 0.4, 0.4, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glLightfv(GL_LIGHT0, GL_POSITION, pozSursa);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glClearColor(0.3, 0.4, 0.9, 1.0);
	glShadeModel(GL_SMOOTH);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_LINE_WIDTH);
    glEnable(GL_MAP2_VERTEX_3);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);

}

void CALLBACK Rotate(void) {
    alpha += 1;
    Sleep(6);
    display();
}

void CALLBACK alphaR(void) {
    alpha += 5;
}

void CALLBACK betaR(void) {
    beta += 5;
}

void CALLBACK gammaR(void) {
    gamma += 5;
}

void vertex(int i) {
    glVertex3f(vertices[i][0], vertices[i][1], vertices[i][2]);
}

int unghi = 0;
int miscare = 0;
boolean enough = FALSE;
void CALLBACK moving(void) {
    if (miscare % 200 == 0) {
        enough = !enough;
    }
    if (enough) {
        miscare -= 2;
        unghi += 5;
    }
    else {
        miscare += 2;
        unghi -= 5;
    }
    Sleep(11);
    display();
}

void calcCoeficientiPlan(float P[3][3], float coef[4])
{
    float v1[3], v2[3];
    float length;
    static const int x = 0;
    static const int y = 1;
    static const int z = 2;
    //calculeazã doi vectori din trei puncte
    v1[x] = P[0][x] - P[1][x];
    v1[y] = P[0][y] - P[1][y];
    v1[z] = P[0][z] - P[1][z];

    v2[x] = P[1][x] - P[2][x];
    v2[y] = P[1][y] - P[2][y];
    v2[z] = P[1][z] - P[2][z];

    //se cacluleazã produsul vectorial al celor doi vectori
    // care reprezintã un al treilea vector perpendicular pe plan 
    // ale cãrui componente sunt chiar coeficienþii A, B, C din ecuaþia planului
    coef[x] = v1[y] * v2[z] - v1[z] * v2[y];
    coef[y] = v1[z] * v2[x] - v1[x] * v2[z];
    coef[z] = v1[x] * v2[y] - v1[y] * v2[x];
    //normalizeazã vectorul
    length = (float)sqrt((coef[x] * coef[x]) + (coef[y] * coef[y]) + (coef[z] * coef[z]));
    coef[x] /= length;
    coef[y] /= length;
    coef[z] /= length;
}

void MatriceUmbra(GLfloat puncte[3][3], GLfloat pozSursa[4], GLfloat mat[4][4])
{
    GLfloat coefPlan[4];
    GLfloat temp;
    //determinã coeficienþii planului
    calcCoeficientiPlan(puncte, coefPlan);
    //determinã si pe D
    coefPlan[3] = -(
        (coefPlan[0] * puncte[2][0]) +
        (coefPlan[1] * puncte[2][1]) +
        (coefPlan[2] * puncte[2][2]));
    //temp=A*xL+B*yL+C*zL+D*w
    temp = coefPlan[0] * pozSursa[0] +
        coefPlan[1] * pozSursa[1] +
        coefPlan[2] * pozSursa[2] +
        coefPlan[3] * pozSursa[3];
    //prima coloanã
    mat[0][0] = temp - pozSursa[0] * coefPlan[0];
    mat[1][0] = 0.0f - pozSursa[0] * coefPlan[1];
    mat[2][0] = 0.0f - pozSursa[0] * coefPlan[2];
    mat[3][0] = 0.0f - pozSursa[0] * coefPlan[3];
    //a doua coloana
    mat[0][1] = 0.0f - pozSursa[1] * coefPlan[0];
    mat[1][1] = temp - pozSursa[1] * coefPlan[1];
    mat[2][1] = 0.0f - pozSursa[1] * coefPlan[2];
    mat[3][1] = 0.0f - pozSursa[1] * coefPlan[3];
    //a treia coloana
    mat[0][2] = 0.0f - pozSursa[2] * coefPlan[0];
    mat[1][2] = 0.0f - pozSursa[2] * coefPlan[1];
    mat[2][2] = temp - pozSursa[2] * coefPlan[2];
    mat[3][2] = 0.0f - pozSursa[2] * coefPlan[3];
    //a patra coloana
    mat[0][3] = 0.0f - pozSursa[3] * coefPlan[0];
    mat[1][3] = 0.0f - pozSursa[3] * coefPlan[1];
    mat[2][3] = 0.0f - pozSursa[3] * coefPlan[2];
    mat[3][3] = temp - pozSursa[3] * coefPlan[3];
}

void Autobuz(boolean color) {
    if(color)glEnable(GL_LIGHTING);
    //Corpul principal fara tavan-------------------------------
    if(color) glColor3f(1, 0.8, 0.0);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, -1);
    for (int i = 0; i < 4; i++) {
        vertex(i);
    }
    glNormal3f(0, 0, 1);
    for (int i = 7; i >= 4; i--) {
        vertex(i);
    }

    glNormal3f(1, 0, 0);
    vertex(3);
    vertex(2);
    vertex(6);
    vertex(7);

    glNormal3f(0, -1, 0);
    if (color) glColor3f(0.1, 0.1, 0.1);
    vertex(3);
    vertex(7);
    vertex(4);
    vertex(0);
    glEnd();

    //Geamuri din fata--------------------------------------------
    if (color) glColor3f(0.6, 0.6, 0.9);
    glBegin(GL_QUADS);
    glNormal3f(-1, 0.2, 0);
    for (int i = 8; i < 12; i++) {
        vertex(i);
    }
    glEnd();

    glBegin(GL_TRIANGLES);
    glNormal3f(0, 0, 1);
    vertex(12);
    vertex(9);
    vertex(8);

    glNormal3f(0, 0, -1);
    vertex(13);
    vertex(11);
    vertex(10);
    glEnd();

    if (color) glColor3f(0, 0, 0);
    glLineWidth(2);
    glBegin(GL_LINES);
    vertex(9);
    vertex(8);
    vertex(11);
    vertex(10);
    glEnd();

    //Tavan-----------------------------------------------------
    ////Suprafata Bezier
    glCullFace(GL_FRONT);
    if (color) glColor3f(1, 0.8, 0.0);
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
        0, 1, 12, 4, &ctrlpoints1[0][0][0]);
    glEvalMesh2(GL_FILL, 0, 20, 0, 20);
    glCullFace(GL_BACK);
    //legarea fata
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(-1, 0, 0);
    glVertex3f(-100, 75, 0);
    for (int i = 0; i <= 180; i++) {
        glVertex3f(-100, 75 + (19 * sin(i)), 40 * cos(i));
    }
    glEnd();
    //legarea in spate
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(1, 0, 0);
    glVertex3f(200, 75, 0);
    for (int i = 0; i <= 180; i++) {
        glVertex3f(200, 75 + (19 * cos(i)), 40 * sin(i));
    }
    glEnd();

    //Partea din fata---------------------------------------------
    glCullFace(GL_FRONT);
    if (color) glColor3f(1, 0.8, 0.0);
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
        0, 1, 12, 4, &ctrlpoints2[0][0][0]);
    glEvalMesh2(GL_FILL, 0, 20, 0, 20);
    glCullFace(GL_BACK);

    glBegin(GL_QUADS);
    glNormal3f(0, 0, -1);
    for (int i = 14; i < 18; i++) {
        vertex(i);
    }

    glNormal3f(0, 0, 1);
    for (int i = 21; i >= 18; i--) {
        vertex(i);
    }

    glNormal3f(-1, 0, 0);
    vertex(1 + 14);
    vertex(0 + 14);
    vertex(4 + 14);
    vertex(5 + 14);

    glNormal3f(0, -1, 0);
    if (color) glColor3f(0.1, 0.1, 0.1);
    vertex(3 + 14);
    vertex(7 + 14);
    vertex(4 + 14);
    vertex(0 + 14);

    glEnd();

    //Decor----------------------------------------------------
    //lowband
    glBegin(GL_QUADS);
    if (color) glColor3f(0.1, 0.1, 0.1);
    glNormal3f(-1, 0, 0);
    glVertex3f(-165, -25, 40);
    glVertex3f(-165.1, -15, 40.1);
    glVertex3f(-165.1, -15, -40.1);
    glVertex3f(-165, -25, -40);

    glNormal3f(0, 0, 1);
    glVertex3f(200, -25, 40);
    glVertex3f(200.1, -15, 40.1);
    glVertex3f(-165.1, -15, 40.1);
    glVertex3f(-165, -25, 40);

    glNormal3f(0, 0, -1);
    glVertex3f(-40, -25, -40);
    glVertex3f(-40, -15, -40.1);
    glVertex3f(200.1, -15, -40.1);
    glVertex3f(200.1, -25, -40);

    glVertex3f(-165, -25, -40);
    glVertex3f(-165.1, -15, -40.1);
    glVertex3f(-85.1, -15, -40.1);
    glVertex3f(-85, -25, -40);

    glNormal3f(1, 0, 0);
    glVertex3f(200, -25, -40);
    glVertex3f(200.1, -15, -40.1);
    glVertex3f(200.1, -15, 40.1);
    glVertex3f(200, -25, 40);

    glEnd();

    //rotile din fata
    
    glPushMatrix();
    if (color) glColor3f(0.1, 0.1, 0.1);
    glTranslatef(-130, -25, -40);
    glPushMatrix();
    glRotatef(unghi, 0, 0, 1);
    auxSolidTorus(10, 20);
    
    if (color) glColor3f(0.8, 0.8, 0.8);
    glDisable(GL_CULL_FACE);
    glBegin(GL_TRIANGLES);
    glNormal3f(0, 0, -1);
    for (int i = 0; i <= 36; i++) {
        if (i % 4 == 0) {
            glVertex3f(0, 0, -0.1);
            i = i * 10;
            glVertex3f(15 * cos(i), 15 * sin(i), -0.1);
            i = i / 10;
            i = i + 1;
            i = i * 10;
            glVertex3f(15 * cos(i), 15 * sin(i), -0.1);
            i = i / 10;
            i = i - 1;
        }
    }
    glEnd();
    glPopMatrix();
    glEnable(GL_CULL_FACE);
    if (color) glColor3f(0.1, 0.1, 0.1);
    glTranslatef(0, 0, +80);
    glPushMatrix();
    glRotatef(unghi, 0, 0, 1);
    auxSolidTorus(10, 20);
    
    if (color) glColor3f(0.8, 0.8, 0.8);
    glDisable(GL_CULL_FACE);
    glBegin(GL_TRIANGLES);
    glNormal3f(0, 0, 1);
    for (int i = 0; i <= 36; i++) {
        if (i % 4 == 0) {
            glVertex3f(0, 0, 0.1);
            i = i * 10;
            glVertex3f(15 * sin(i), 15 * cos(i), 0.1);
            i = i / 10;
            i = i + 1;
            i = i * 10;
            glVertex3f(15 * sin(i), 15 * cos(i), 0.1);
            i = i / 10;
            i = i - 1;
        }
    }
    glEnd();
    glPopMatrix();
    glEnable(GL_CULL_FACE);

    //rotile din spate
    if (color) glColor3f(0.1, 0.1, 0.1);
    glTranslatef(250, 0, 0);
    glPushMatrix();
    glRotatef(unghi, 0, 0, 1);
    auxSolidTorus(10, 20);
    
    if (color) glColor3f(0.8, 0.8, 0.8);
    glDisable(GL_CULL_FACE);
    glBegin(GL_TRIANGLES);
    glNormal3f(0, 0, 1);
    for (int i = 0; i <= 36; i++) {
        if (i % 4 == 0) {
            glVertex3f(0, 0, 0.1);
            i = i * 10;
            glVertex3f(15 * sin(i), 15 * cos(i), 0.1);
            i = i / 10;
            i = i + 1;
            i = i * 10;
            glVertex3f(15 * sin(i), 15 * cos(i), 0.1);
            i = i / 10;
            i = i - 1;
        }
    }
    glEnd();
    glPopMatrix();
    glEnable(GL_CULL_FACE);
    if (color) glColor3f(0.1, 0.1, 0.1);
    glTranslatef(0, 0, -80);
    glPushMatrix();
    glRotatef(unghi, 0, 0, 1);
    auxSolidTorus(10, 20);
    
    if (color) glColor3f(0.8, 0.8, 0.8);
    glDisable(GL_CULL_FACE);
    glBegin(GL_TRIANGLES);
    glNormal3f(0, 0, -1);
    for (int i = 0; i <= 36; i++) {
        if (i % 4 == 0) {
            glVertex3f(0, 0, -0.1);
            i = i * 10;
            glVertex3f(15 * cos(i), 15 * sin(i), -0.1);
            i = i / 10;
            i = i + 1;
            i = i * 10;
            glVertex3f(15 * cos(i), 15 * sin(i), -0.1);
            i = i / 10;
            i = i - 1;
        }
    }
    glEnd();
    glPopMatrix();
    glEnable(GL_CULL_FACE);
    glPopMatrix();

    //decor motor
    glBegin(GL_QUADS);
    if (color) glColor3f(0.12, 0.12, 0.12);
    glNormal3f(-1, 0, 0);
    glVertex3f(-165, -25, 30);
    glVertex3f(-165.1, 5, 30);
    glVertex3f(-165.1, 5, -30);
    glVertex3f(-165, -25, -30);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(-1, 0, 0);
    glVertex3f(-165.1, 5, 0);
    for (int i = 0; i <= 180; i++) {
        glVertex3f(-165.1, 5 + 10 * sin(i * 3.14 / 180), 30 * cos(i * 3.14 / 180));
    }
    glEnd();

    //decor spate
    glBegin(GL_QUADS);
    if (color) glColor3f(0.12, 0.12, 0.12);
    glNormal3f(1, 0, 0);
    glVertex3f(200, -25, -30);
    glVertex3f(200.1, 55, -30);
    glVertex3f(200.1, 55, 30);
    glVertex3f(200, -25, 30);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(1, 0, 0);
    glVertex3f(200.1, 45, 0);
    for (int i = 0; i <= 180; i++) {
        glVertex3f(200.1, 55 + 15 * cos(i), 30 * sin(i));
    }
    glEnd();

    //decor usa
    glBegin(GL_QUADS);
    if (color) glColor3f(0.12, 0.12, 0.12);
    glNormal3f(0, 0, -1);
    glVertex3f(-85, -25, -40);
    glVertex3f(-85, -20, -40.1);
    glVertex3f(-40, -20, -40.1);
    glVertex3f(-40, -25, -40);
    if (color) glColor3f(0.6, 0.6, 0.9);
    glNormal3f(0, 0, -1);
    glVertex3f(-85, -25, -40);
    glVertex3f(-85, 65, -40.1);
    glVertex3f(-40, 65, -40.1);
    glVertex3f(-40, -25, -40);
    glEnd();
    glBegin(GL_LINES);
    if (color) glColor3f(0.12, 0.12, 0.12);
    glNormal3f(0, 0, -1);
    glVertex3f(-85, -25, -40);
    glVertex3f(-85, 65, -40.1);
    glVertex3f(-85, 65, -40.1);
    glVertex3f(-40, 65, -40.1);
    glVertex3f(-40, 65, -40.1);
    glVertex3f(-40, -25, -40);
    glVertex3f(-62.5, -20, -40.1);
    glVertex3f(-62.5, 65, -40.1);
    glEnd();

    //decor geamuri
    glNormal3f(0, 0, -1);
    for (int i = 0; i < 200; i += 30) {
        glBegin(GL_QUADS);
        if (color) glColor3f(0.6, 0.6, 0.9);
        
        glVertex3f(-30 + i, 30, -40);
        glVertex3f(-30 + i, 65, -40.1);
        glVertex3f(-10 + i, 65, -40.1);
        glVertex3f(-10 + i, 30, -40);

        glEnd();
        glBegin(GL_LINES);
        if (color) glColor3f(0.12, 0.12, 0.12);
        
        glVertex3f(-30 + i, 30, -40);
        glVertex3f(-30 + i, 65, -40.1);
        glVertex3f(-30 + i, 65, -40.1);
        glVertex3f(-10 + i, 65, -40.1);
        glVertex3f(-10 + i, 65, -40.1);
        glVertex3f(-10 + i, 30, -40);
        glVertex3f(-30 + i, 30, -40.1);
        glVertex3f(-10 + i, 30, -40.1);

        glVertex3f(-30 + i, 47.5, -40.1);
        glVertex3f(-10 + i, 47.5, -40.1);
        glEnd();
    }
    glNormal3f(0, 0, 1);
    for (int i = 0; i < 270; i += 30) {
        glBegin(GL_QUADS);
        if (color) glColor3f(0.6, 0.6, 0.9);
        
        glVertex3f(-70 + i, 30, 40);
        glVertex3f(-70 + i, 65, 40.1);
        glVertex3f(-90 + i, 65, 40.1);
        glVertex3f(-90 + i, 30, 40);

        glEnd();
        glBegin(GL_LINES);
        if (color) glColor3f(0.12, 0.12, 0.12);
        
        glVertex3f(-90 + i, 30, 40);
        glVertex3f(-90 + i, 65, 40.1);
        glVertex3f(-90 + i, 65, 40.1);
        glVertex3f(-70 + i, 65, 40.1);
        glVertex3f(-70 + i, 65, 40.1);
        glVertex3f(-70 + i, 30, 40);
        glVertex3f(-90 + i, 30, 40.1);
        glVertex3f(-70 + i, 30, 40.1);

        glVertex3f(-90 + i, 47.5, 40.1);
        glVertex3f(-70 + i, 47.5, 40.1);
        glEnd();
    }

    //Reflectoare
    glPushMatrix();
    glTranslatef(-163, -10, 35);
    if (color) glColor3f(0.9, 0.9, 0.9);
    auxSolidSphere(5);
    glTranslatef(0, 10, 0);
    auxSolidSphere(5);
    auxSolidCylinder(5, 10);
    glTranslatef(0, -10, -70);
    auxSolidSphere(5);
    glTranslatef(0, 10, 0);
    auxSolidSphere(5);
    auxSolidCylinder(5, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-98, 80, 30);
    if (color) glColor3f(1, 0.3, 0.3);
    auxSolidSphere(5);
    glTranslatef(0, 0, -10);
    if (color) glColor3f(1, 0.4, 0);
    auxSolidSphere(5);
    glTranslatef(0, 0, -40);
    auxSolidSphere(5);
    glTranslatef(0, 0, -10);
    if (color) glColor3f(1, 0.3, 0.3);
    auxSolidSphere(5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(198, -10, 35);
    if (color) glColor3f(1, 0.3, 0.3);
    auxSolidSphere(5);
    glTranslatef(0, 10, 0);
    auxSolidSphere(5);
    auxSolidCylinder(5, 10);
    glTranslatef(0, -10, -70);
    auxSolidSphere(5);
    glTranslatef(0, 10, 0);
    auxSolidSphere(5);
    auxSolidCylinder(5, 10);
    glPopMatrix();
}

void plan(void) {
    glDisable(GL_CULL_FACE);
    glBegin(GL_QUADS);

    glColor3f(0.2, 1, 0.3);
     glVertex3f(500, -55.1, -500);
     glVertex3f(-500, -55.1, -500);
     glVertex3f(-500, -55.1, 500);
     glVertex3f(500, -55.1, 500);
    
    glEnd();
    glEnable(GL_CULL_FACE);
}

void CALLBACK display(void)
{
    GLfloat matUmbra[4][4];
    //oricare trei puncte din plan în sens CCW
    GLfloat puncte[3][3] = { {-50, -55, -50},
                            {-50, -55, 50},
                            {50, -55, 50} };
   
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    MatriceUmbra(puncte, pozSursa, matUmbra);
    glPushMatrix();
    glRotatef(+45, 0.5, 1, 0);
    glPushMatrix();
    glCullFace(GL_BACK);
    glLightfv(GL_LIGHT0, GL_POSITION, pozSursa);
    //gluLookAt(x, y, z, 0, 0, 0, 0, 1, 0);
	glRotatef(alpha, 1.0, 0.0, 0.0);
    glRotatef(beta, 0.0, 1.0, 0.0);
    glRotatef(gamma, 0.0, 0.0, 1.0);
    glTranslatef(miscare, 0, 0);
    Autobuz(GL_TRUE);

    glPopMatrix();

    glPushAttrib(GL_LIGHTING_BIT);
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glMultMatrixf((GLfloat*)matUmbra);
    glRotatef(alpha, 1.0, 0.0, 0.0);
    glRotatef(beta, 0.0, 1.0, 0.0);
    glRotatef(gamma, 0.0, 0.0, 1.0);
    glColor3f(0.4, 0.4, 0.4);
    glTranslatef(miscare, 0, 0);
    Autobuz(GL_FALSE);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(pozSursa[0], pozSursa[1], pozSursa[2]);
    glColor3f(1.0, 1.0, 0.0);
    auxSolidSphere(10);
    glPopMatrix();
    plan();
    //restaureazã starea variabilelor de iluminare
    glPopAttrib();
    glPopMatrix();

    glFlush();
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    if (!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-320, 320, -320 * (GLfloat)h / (GLfloat)w,
            320 * (GLfloat)h / (GLfloat)w, -800, 800);
    else
        glOrtho(-320 * (GLfloat)w / (GLfloat)h,
            320 * (GLfloat)w / (GLfloat)h, -320, 320, -800, 800);
    glMatrixMode(GL_MODELVIEW);
    /*glLoadIdentity();
    glRotatef(25, 0, 1.0, 0);*/
}

/*  Main Loop
 */
int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB | AUX_DEPTH);
    auxInitPosition (0, 0, 500, 400);
    auxInitWindow ("AUTOBUZ");
    myinit();
    auxReshapeFunc (myReshape);
    auxKeyFunc(AUX_LEFT, alphaR);
    auxKeyFunc(AUX_UP, betaR);
    auxKeyFunc(AUX_RIGHT, gammaR);
    //auxKeyFunc(AUX_2, zoomin);
    //auxKeyFunc(AUX_1, zoomout);
    auxIdleFunc(moving);
    auxMainLoop(display);
    return(0);
}