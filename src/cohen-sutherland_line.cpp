#include <GL/glut.h>
#include <iostream>

const int LEFT = 1, RIGHT = 2, BOTTOM = 4, TOP = 8;
float xmin = 50, ymin = 50, xmax = 100, ymax = 100;

int computeCode(float x, float y) {
    int code = 0;
    if (x < xmin) code |= LEFT;
    if (x > xmax) code |= RIGHT;
    if (y < ymin) code |= BOTTOM;
    if (y > ymax) code |= TOP;
    return code;
}

void cohenSutherlandClip(float x1, float y1, float x2, float y2) {
    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);
    bool accept = false;

    while (true) {
        if (!(code1 | code2)) {
            accept = true;
            break;
        } else if (code1 & code2) {
            break;
        } else {
            int outcode = code1 ? code1 : code2;
            float x, y;

            if (outcode & TOP) {
                x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
                y = ymax;
            } else if (outcode & BOTTOM) {
                x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
                y = ymin;
            } else if (outcode & RIGHT) {
                y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
                x = xmax;
            } else {
                y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
                x = xmin;
            }

            if (outcode == code1) {
                x1 = x; y1 = y; code1 = computeCode(x1, y1);
            } else {
                x2 = x; y2 = y; code2 = computeCode(x2, y2);
            }
        }
    }

    if (accept) {
        glColor3f(0, 1, 0);
        glBegin(GL_LINES);
        glVertex2f(x1, y1); glVertex2f(x2, y2);
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Draw clipping window
    glColor3f(1, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(xmin, ymin); glVertex2f(xmax, ymin);
    glVertex2f(xmax, ymax); glVertex2f(xmin, ymax);
    glEnd();

    // Line to be clipped
    glColor3f(0, 0, 1);
    glBegin(GL_LINES);
    glVertex2f(20, 80); glVertex2f(120, 80);
    glEnd();

    // Clipped line
    cohenSutherlandClip(20, 80, 120, 80);

    glFlush();
}

void init() {
    glClearColor(1, 1, 1, 1);
    gluOrtho2D(0, 200, 0, 200);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutCreateWindow("Cohen Sutherland");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
