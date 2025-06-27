#include <GL/glut.h>

// Function to draw label below each cube
void drawLabel(const char* label) {
    glPushMatrix();
        glDisable(GL_LIGHTING); // In case lighting is used later
        glRasterPos3f(-0.4f, -1.1f, 0.3f); // Lower and a bit in front of cube
        for (const char* c = label; *c != '\0'; c++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    glPopMatrix();
}

// Function to draw a single colored cube with label
void drawLabeledCube(const char* label) {
    glutWireCube(1.0);
    drawLabel(label);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(-8.0f, 0.0f, -20.0f); // Pull all objects into view

    // 1. Original
    glPushMatrix();
        glColor3f(0.1f, 0.1f, 0.1f);
        drawLabeledCube("Original");
    glPopMatrix();

    // 2. Translated
    glPushMatrix();
        glTranslatef(3.5f, 1.0f, 0.0f);
        glColor3f(0.2f, 0.6f, 1.0f);
        drawLabeledCube("Translated");
    glPopMatrix();

    // 3. Rotated
    glPushMatrix();
        glTranslatef(7.0f, 0.0f, 0.0f);
        glRotatef(45, 1, 1, 0);
        glColor3f(1.0f, 0.3f, 0.3f);
        drawLabeledCube("Rotated");
    glPopMatrix();

    // 4. Scaled
    glPushMatrix();
        glTranslatef(10.5f, 0.0f, 0.0f);
        glScalef(1.5f, 0.5f, 1.0f);
        glColor3f(0.3f, 1.0f, 0.3f);
        drawLabeledCube("Scaled");
    glPopMatrix();

    // 5. Sheared
    glPushMatrix();
        glTranslatef(14.0f, 0.0f, 0.0f);
        GLfloat shearMatrix[] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.5f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        glMultMatrixf(shearMatrix);
        glColor3f(1.0f, 0.8f, 0.0f);
        drawLabeledCube("Sheared");
    glPopMatrix();

    glutSwapBuffers();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    gluPerspective(60.0, 1.5, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(1100, 600);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("3D Transformations - With Clear Labels");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
