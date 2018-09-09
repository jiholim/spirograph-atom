#include <cmath>
#include <iostream>

#ifdef _WIN32
#include "glut.h"
#define M_PI 3.14159
#elif __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#endif

using namespace std;

float alpha = 0.0, k=2;
float tx = 0.0, ty=0.0;

float rho = 0.5, speed=0.5;
float r = 1.0, g=1.0, b=1.0;

void drawHexagon()
{
    glBegin(GL_LINE_LOOP);
    
    for(int i = 0; i < 6; ++i) {
        glVertex2f(sin(i/6.0*2*M_PI), cos(i/6.0*2*M_PI));
    }
    
    glEnd();
}

void drawSquare(double x, double y, double side_length)
{
    double half_length = side_length / 2;
    
    glBegin(GL_LINE_LOOP);
    
    glVertex2d(x + half_length, y + half_length);
    glVertex2d(x + half_length, y - half_length);
    glVertex2d(x - half_length, y - half_length);
    glVertex2d(x - half_length, y + half_length);
    
    glEnd();
}

void drawSimpleSpirograph(int type)
{
    r = (rand() % 100 + 10)*0.01;
    g = (rand() % 100 + 10)*0.01;
    b = (rand() % 100 + 10)*0.01;
    
    glColor3f(r, g, b);
    
    for (int i = 0; i < 8; i++) {
        if (type == 1) {
            drawHexagon();
            glRotatef(5, 0, 0, 1);
        } else {
            drawSquare(0, 0, 1);
            glRotatef(20, 0, 0, 1);
        }
    }
}

void drawComplexSpirograph(float R, float rho)
{
    float t = 0.0;
    float x = 0.0, y = 0.0, x1 = 0.0, y1 = 0.0;
    
    glBegin(GL_LINES);
    
    r = (rand() % 100 + 10)*0.01;
    g = (rand() % 100 + 10)*0.01;
    b = (rand() % 100 + 10)*0.01;
    
    x1 = (R - r)*cos(t) + rho*cos(((R - r) / r)*t);
    y1 = (R - r)*sin(t) + rho*sin(((R - r) / r)*t);
    
    for (int i = 0; i < 1000; i++) {
        t = i*speed; //angle of rotation
        
        //Using spirograph equation from wikipedia
        x = (R - r)*cos(t) + rho*cos(((R - r) / r)*t);
        y = (R - r)*sin(t) + rho*sin(((R - r) / r)*t);
        
        glColor3f(r, g, b);
        glVertex2f(x1, y1);
        glVertex2f(x , y);
        
        x1 = x;
        y1 = y;
    }
    
    glEnd();
}

void timer(int value)
{
    glRotatef(10, 0, 0, 1);
    glutTimerFunc(500, timer, 0);
    glutPostRedisplay();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glPushMatrix();
    
    //controls transformation
    glScalef(k, k, k);
    glTranslatef(tx, ty, 0);
    glRotatef(alpha, 0, 0, 1);
    
    //draw
    drawComplexSpirograph(2.5, rho);
    drawComplexSpirograph(3.5, rho);
    drawSimpleSpirograph(0);
    drawSimpleSpirograph(1);
    
    glPopMatrix();
    glFlush ();
}

void reshape (int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    
    glOrtho(-10, 10, -10, 10, -10, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void init(void)
{
    glClearColor (1.0, 1.0, 1.0, 1.0);
    glShadeModel (GL_SMOOTH);
}

void keyboard (unsigned char key, int x, int y)
{
    //keys to control scaling - k
    //keys to control rotation - alpha
    //keys to control translation - tx, ty
    switch (key) {
        case 'a':
            alpha+=10;
            glutPostRedisplay();
            break;
            
        case 'd':
            alpha-=10;
            glutPostRedisplay();
            break;
            
        case 'q':
            k+=0.1;
            glutPostRedisplay();
            break;
            
        case 'e':
            if(k>0.1)
                k-=0.1;
            glutPostRedisplay();
            break;
            
        case 'z':
            tx-=0.1;
            glutPostRedisplay();
            break;
            
        case 'c':
            tx+=0.1;
            glutPostRedisplay();
            break;
            
        case 's':
            ty-=0.1;
            glutPostRedisplay();
            break;
            
        case 'w':
            ty+=0.1;
            glutPostRedisplay();
            break;
            
        case 27:
            exit(0);
            break;
            
        default:
            break;
    }
}

int main(int argc, char **argv)
{
    cout<<"+++++CONTROL BUTTONS+++++++\n\n";
    cout<<"Scale Up/Down: Q/E\n";
    cout<<"Rotate Clockwise/Counter-clockwise: A/D\n";
    cout<<"Move Up/Down: W/S\n";
    cout<<"Move Left/Right: Z/C\n";
    cout <<"ESC: Quit\n";
    
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (600, 600);
    glutInitWindowPosition (50, 50);
    glutCreateWindow (argv[0]);
    init ();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    //glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    
    return 0;
}
